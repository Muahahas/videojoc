#include "Material.h"
#include "TemplatedMaterialParameter.h"
#include "XML\XML.h"
#include <assert.h>
#include "Utils\EnumToString.h"
#include "Math\Vector2.h"
#include "Math\Vector3.h"
#include "Math\Vector4.h"
#include "Math\Color.h"
#include "Utils\CheckedDelete.h"
#include "Graphics/Textures/TextureManager.h"
#include "Engine\Engine.h"
#include "Graphics/Effects\Technique.h"
#include "Graphics/Effects/TechniquePoolManager.h"
#include "Graphics/Buffers/ConstantBufferManager.h"

CMaterial::~CMaterial()
{
    /*
    for (std::vector< CMaterialParameter *>::iterator iMaterialParameter = mParameters.begin();
                iMaterialParameter != mParameters.end();
                ++iMaterialParameter)
    {}
    */
    base::utils::CheckedDelete(mParameters);
}

CMaterial::CMaterial(CXMLElement* aElement) : CName( aElement )
{
    CEngine &l_Engine = CEngine::GetInstance();
    CTechniquePoolManager &l_TechniquePoolManager = l_Engine.GetTechniquePoolManager();

    std::string lVertexType = aElement->GetAttribute<std::string>("vertex_type", "");
    std::string lEffect = aElement->GetAttribute<std::string>("effect", "");

    mTechnique = l_TechniquePoolManager(lVertexType);
    mTechnique = l_TechniquePoolManager(lEffect);


    mTechnique = strcmp(lEffect.c_str(), "") == 0 ? l_TechniquePoolManager(lVertexType) : l_TechniquePoolManager(lEffect);

    for (tinyxml2::XMLElement *iTextureOrParameter = aElement->FirstChildElement(); iTextureOrParameter != nullptr; iTextureOrParameter = iTextureOrParameter->NextSiblingElement())
    {
        if (strcmp(iTextureOrParameter->Name(), "texture") == 0)
        {
            //leemos textura
            std::string textureName = iTextureOrParameter->GetAttribute<std::string>("type", "");
            std::string textureFilename = iTextureOrParameter->GetAttribute<std::string>("filename", "");

            CTextureManager &lTextureManager = l_Engine.GetTextureManager();
            CTexture * lTexture = lTextureManager.GetTexture(textureFilename);
            mTextures.push_back(lTexture);
        }
        else if (strcmp(iTextureOrParameter->Name(), "parameter") == 0)
        {
            TParameterType lType;

            if (EnumString<TParameterType>::ToEnum(lType, iTextureOrParameter->GetAttribute<std::string>("type", "")))
            {
                CMaterialParameter* lParameter = nullptr;

#define CASE_CREATE_MATERIAL_PARAMETER( parameter_type, parameter_class_type, parameter_def_value ) \
case parameter_type: \
{ \
    lParameter = new CTemplatedMaterialParameter<parameter_class_type>(\
        iTextureOrParameter->GetAttribute<std::string>("name", ""), \
        iTextureOrParameter->GetAttribute<parameter_class_type>("value", parameter_def_value), \
        parameter_type);\
}\
break;\

                switch (lType)
                {
                    CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eFloat, float, 0.0f);
                    CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eFloat2, Vect2f, Vect2f(0.0f, 0.0f));
                    CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eFloat3, Vect3f, Vect3f(0.0f, 0.0f, 0.0f));
                    CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eFloat4, Vect4f, Vect4f(0.0f, 0.0f, 0.0f, 0.0f));
                    CASE_CREATE_MATERIAL_PARAMETER(CMaterial::eColor, CColor, CColor());
                default:
                    break;
                }

#undef CASE_CREATE_MATERIAL_PARAMETER

                if (lParameter)
                    mParameters.push_back(lParameter);
            }
        }
    }
}

void CMaterial::Apply()
{
    assert(mTechnique);
    ID3D11DeviceContext * lContext = CEngine::GetInstance().GetRenderManager().GetDeviceContext();

    // Bind the technique that will render this geometry
    mTechnique->Bind(lContext);

    for (size_t i = 0, lCount = mTextures.size(); i < lCount; ++i)
    {
        if (mTextures[i])
        {
            mTextures[i]->Bind(i, lContext);
        }
    }

    CConstantBufferManager& lCBM = CEngine::GetInstance().GetConstantBufferManager();
    for (size_t i = 0, lCount = mParameters.size(); i < lCount; ++i)
    {

        /*****************************************************************************
        SWITCH COLOCANDO PROPIEDADES DE MATERIALES EN EL CONSTANT BUFFER
        Cuidado con colocar dos propiedades que puedan coexistir en un mismo material
        en la misma casilla del vector!
        Posiciones ocupadas del vector:
        [0] -> COLOR DIFUSO
        [1] -> BUMP, PROPIEDADES DE EFECTO DE POSTPROCESADO
        [2] -> REBOTES DE LUZ ESPECULAR
        *****************************************************************************/

        switch (mParameters[i]->GetType())
        {
        case eFloat:
        {
            if (mParameters[i]->GetName() == "bump")
            {
                float *lBump = (float*)mParameters[i]->GetAddr(0);
                const Vect4f lBumpVector = Vect4f(*lBump, 0.0f, 0.0f, 0.0f);
                lCBM.mMaterialDesc.m_RawData[1] = lBumpVector;
            }
        }
        break;
        case eFloat2:
            if (mParameters[i]->GetName() == "fog_start_end")
            {
                float *lFogStart = (float*)mParameters[i]->GetAddr(0);
                float *lFogEnd = (float*)mParameters[i]->GetAddr(1);
                lCBM.mMaterialDesc.m_RawData[1] = Vect4f(*lFogStart, *lFogEnd, 1.0f, 1.0f);
            }
            else if (mParameters[i]->GetName() == "specular_exponent_contrib")
            {
                float *lSpecularExponent = (float*)mParameters[i]->GetAddr(0);
                float *lSpecularContrib = (float*)mParameters[i]->GetAddr(1);
                lCBM.mMaterialDesc.m_RawData[2] = Vect4f(*lSpecularExponent, *lSpecularContrib, 1.0f, 1.0f);
            }

            break;
        case eFloat3:
            break;
        case eFloat4:
            break;
        case eColor:
            float *r = (float*) mParameters[i]->GetAddr(0);
            float *g = (float*) mParameters[i]->GetAddr(1);
            float *b = (float*) mParameters[i]->GetAddr(2);

            const Vect4f lDiffuseColor = Vect4f(*r, *g, *b, 1.0f);
            lCBM.mMaterialDesc.m_RawData[0] = lDiffuseColor;
            break;
        }
    }

    lCBM.BindBuffer(lContext, CConstantBufferManager::CB_Material);
}

#pragma once
#ifndef _ENGINE_SHADER_20170104
#define _ENGINE_SHADER_20170104

#include "Utils\Name.h"
#include "Utils\Defines.h"
#include "Utils\EnumToString.h"
#include <d3d11.h>
#include <vector>

class CShader : public CName
{

public:
    enum EShaderStage
    {
        eVertexShader = 0,
        ePixelShader,
        eGeometryShader,
        EStageCount
    };

public:
    CShader(const std::string& aShaderCode, EShaderStage aType);
    CShader(const CXMLElement* aElement, const std::string aPath, EShaderStage aType);
    virtual ~CShader();
    virtual bool Load();
    virtual void Bind(ID3D11DeviceContext*) = 0;
    bool Reload();

    GET_SET(EShaderStage, Type)
    GET_SET_REF(std::string, Preprocessor)
    GET_SET_REF(std::string, EntryPoint)
    GET_SET_REF(std::string, Filename)
    GET_SET_REF(std::string, ShaderCode)
    GET_SET_PTR(ID3DBlob, Blob);

protected:
    DISALLOW_COPY_AND_ASSIGN(CShader);
    EShaderStage m_Type;
    std::string  m_Preprocessor;
    std::string  m_EntryPoint;
    std::string  m_Filename;
    std::string  m_ShaderCode;
    D3D_SHADER_MACRO *m_ShaderMacros;
    std::vector<std::string> m_PreprocessorMacros;

    ID3DBlob	*m_pBlob;

    void CreateShaderMacro();
    virtual std::string GetShaderModel() = 0;
    std::string CreateMacroHash() const;
    bool IsRecentlyUpdated();
};

//--------------------------------------------------------------------------------------------------------
Begin_Enum_String(CShader::EShaderStage)
{
    Enum_String_Id(CShader::eVertexShader, "vertex");
    Enum_String_Id(CShader::eGeometryShader, "geometry");
    Enum_String_Id(CShader::ePixelShader, "pixel");
}
End_Enum_String;

#endif //_ENGINE_SHADER_20170104

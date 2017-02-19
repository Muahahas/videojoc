#include "CinematicCameraPlayer.h"
#include "CinematicCameraKey.h"




CCinematicCameraPlayer::CCinematicCameraPlayer()
{


}

CCinematicCameraPlayer::~CCinematicCameraPlayer()
{

}

bool CCinematicCameraPlayer::Load(const CXMLElement *aElement)
{

    bool lOk = false;
    mName = aElement->GetAttribute<std::string>("name", "");
    for (tinyxml2::XMLElement const *iCamKey = aElement->FirstChildElement(); iCamKey != nullptr; iCamKey = iCamKey->NextSiblingElement())
    {
        if (strcmp(iCamKey->Name(), "key")==0)
        {
            CCinematicCameraKey *lCamKey = new CCinematicCameraKey();
            lOk = false || lCamKey->Load(iCamKey);
            Add(Convert(lCamKey->GetTime()),lCamKey);
			if (m_CurrentKey == m_NextKey)
			{
				m_CurrentKey = lCamKey;
			}
        }


	}	
	m_NextKey = m_CurrentKey;
	return lOk;


}

void CCinematicCameraPlayer::Apply(float aPercentage, CCinematicKey* A, CCinematicKey* B)
{
	CCameraController *cam = &(CEngine::GetInstance().GetCameraController());
	Vect3f lPos = cam->getPosition();
	lPos = A->GetPosition()*aPercentage + B->GetPosition()*(1 - aPercentage);
	cam->setPosition(lPos);



}
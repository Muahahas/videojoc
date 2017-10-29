#include "Event.h"

#include <string>
#include "XML/XML.h"
#include "Actor.h"
#include "Reactor.h"
#include "Engine/Engine.h"
#include "Events/EventManager.h"
#include "Utils/Logger.h"

#include "Events/DumbActor.h"
#include "Events/AudioTriggerActor.h"

#include "Events/DumbReactor.h"
#include "Events/OpenDoorReactor.h"
#include "Events/LoadSceneReactor.h"
#include "Events/ReloadSceneReactor.h"
#include "Events/TextGUIActor.h"

CEvent::CEvent() :
    m_Finished(false),
    m_HappeningRightFuckingNow(false),
    mAlreadyReacting(false) {}

CEvent::CEvent(CXMLElement* aEvent) :
    m_Finished(false),
    m_HappeningRightFuckingNow(false),
    mAlreadyReacting(false),
    CName(aEvent->GetAttribute<std::string>("name", ""))
{
    m_Active = aEvent->GetAttribute<bool>("active", false);

    for (tinyxml2::XMLElement *iElement = aEvent->FirstChildElement(); iElement != nullptr; iElement = iElement->NextSiblingElement())
    {
        if (strcmp(iElement->Name(), "actor") == 0)
        {

            std::string lActorName = iElement->GetAttribute<std::string>("name", "");
            CActor::EActorType lActorType;

            EnumString<CActor::EActorType>::ToEnum(lActorType, lActorName);

            switch (lActorType)
            {
            case CActor::eDumb:
                mActor = new CDumbActor();
                break;
            case CActor::eAudioTrigger:
                mActor = new CAudioTriggerActor();
                break;
            case CActor::eTextGUI:
                mActor = new CTextGUIActor();
                break;
            default:
                break;
            }

            mActor->Load(iElement);
        }
        else if (strcmp(iElement->Name(), "reactor") == 0)
        {

            std::string lReactorName = iElement->GetAttribute<std::string>("name", "");
            CReactor::EReactorType lReactorType;

            EnumString<CReactor::EReactorType>::ToEnum(lReactorType, lReactorName);

            switch (lReactorType)
            {
            case CReactor::eDumb:
                mReactor = new CDumbReactor();
                break;
            case CReactor::eOpenDoor:
                mReactor = new COpenDoorReactor();
                break;
            case CReactor::eReloadScene:
                mReactor = new CReloadSceneReactor();
                break;
            case CReactor::eLoadScene:
                mReactor = new CLoadSceneReactor();
                break;
            }

            mReactor->Load(iElement);
        }
    }
}

CEvent::~CEvent()
{
    __H_CHECKED_DELETE__(mReactor);
    __H_CHECKED_DELETE__(mActor);
}

void CEvent::Start()
{
    mActor->Act();
    m_HappeningRightFuckingNow = true;
}

void CEvent::Update(float elapsedTime)
{
    if (mActor->IsFinished() && !mAlreadyReacting)
    {
        mReactor->React();
        mAlreadyReacting = true;
    }

    if (!mActor->IsFinished())
    {
        mActor->Update(elapsedTime);
    }
    else if (mActor->IsFinished() && !mReactor->IsFinished())
    {
        mReactor->Update(elapsedTime);
    }
    else if (mActor->IsFinished() && mReactor->IsFinished())
    {
        m_Finished = true;
        m_HappeningRightFuckingNow = false;
    }
}
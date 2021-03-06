#pragma once

#ifndef _ENGINE_ACTOR_20170830_H
#define _ENGINE_ACTOR_20170830_H

#include "Utils/Name.h"
#include "Utils\EnumToString.h"

class CActor : public CName
{
public:

    enum EActorType
    {
        eDumb,
        eAudioTrigger,
        eTextGUIA,
        eCheckpoint,
        eCinematic,
        eNodeInteract,
        eGameComplete,
		 eWaitE,
        eAudioSwitch,
        eActorTypeCount
    };

    CActor() : m_Finished(false) {}
    ~CActor() {}

    virtual void Load(CXMLElement* aElement) = 0;
    virtual void Act() = 0;
    virtual void Update(float elapsedTime) = 0;

    GET_SET_BOOL(Finished);
    EActorType mActorType;

protected:
    bool m_Finished;
};

Begin_Enum_String(CActor::EActorType)
{
    Enum_String_Id(CActor::eDumb, "dumb");
    Enum_String_Id(CActor::eAudioTrigger, "audio_triggerA");
    Enum_String_Id(CActor::eTextGUIA, "textGUIA");
    Enum_String_Id(CActor::eCheckpoint, "checkpoint");
    Enum_String_Id(CActor::eCinematic, "Cinematica");
    Enum_String_Id(CActor::eNodeInteract, "node");
    Enum_String_Id(CActor::eGameComplete, "GameComplete");
    Enum_String_Id(CActor::eAudioSwitch, "audio_switch");
Enum_String_Id(CActor::eWaitE, "waitE");
}
End_Enum_String;

#endif //_ENGINE_ACTOR_20170215_H
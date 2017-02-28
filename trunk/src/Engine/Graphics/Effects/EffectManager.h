#pragma once
#ifndef _ENGINE_EFFECT_MANAGER_20170104
#define _ENGINE_EFFECT_MANAGER_20170104

#include "Utils\TemplatedMapVector.h"
#include "Effect.h"
#include "Utils/MemLeaks/MemLeaks.h"

class CEffectManager : public base::utils::CTemplatedMapVector<CEffect>
{
public:
    CEffectManager();
    virtual ~CEffectManager();
    bool Load(const std::string& aFilename);
    bool Reload();
    bool Refresh();

private:
    bool Load();
    DISALLOW_COPY_AND_ASSIGN(CEffectManager);
    std::string mFilename;
};

#endif //_ENGINE_EFFECT_MANAGER_20170104

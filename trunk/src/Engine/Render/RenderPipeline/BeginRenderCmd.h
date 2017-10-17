#pragma once
#ifndef _BEGINRENDERCMD_H_
#define _BEGINRENDERCMD_H_

#include "RenderCmd.h"
#include "Render\RenderManager.h"
#include "Utils\Name.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CBeginRenderCmd : public CRenderCmd
{
public:
    CBeginRenderCmd();
    virtual ~CBeginRenderCmd();
    bool Load(const CXMLElement* aElement);
    virtual void Execute(CRenderManager& lRM);
    virtual void DrawImgui();
private:
    DISALLOW_COPY_AND_ASSIGN(CBeginRenderCmd);
};

#endif
#pragma once
#ifndef _ENGINE_SCENEMESH_20170110_H
#define _ENGINE_SCENEMESH_20170110_H

#include "SceneNode.h"

class CMesh;
class CRenderManager;
class CSceneMesh : public CSceneNode
{
public:
    CSceneMesh(CXMLElement* aElement);
    CSceneMesh(CXMLElement* aElement, CMesh* aMesh);
    virtual ~CSceneMesh();
    virtual bool Render(CRenderManager& aRendermanager);
protected:
    CMesh* mMesh;
};

#endif //_ENGINE_SCENEMESH_20170110_H
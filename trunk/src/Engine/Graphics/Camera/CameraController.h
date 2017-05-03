#pragma once
#ifndef _ENGINE_CAMERACONTROLLER_CPB_20161126_H
#define _ENGINE_CAMERACONTROLLER_CPB_20161126_H

#include "Render\RenderManager.h"

#ifdef _DEBUG
#include "Utils/MemLeaks/MemLeaks.h"
#endif

class CCameraController
{
public:
    CCameraController() :
        m_Position(0, 0, 0),
        m_Front(0, 0, 1),
        m_Up(0, 1, 0),
        m_CameraInfo(0.1f, 1.0f, 0.2, 0) {}

    virtual ~CCameraController() {}
    virtual void Update(float ElapsedTime) = 0;

    void SetToRenderManager(CRenderManager &_RenderManager) const
    {
        _RenderManager.SetViewMatrix(m_Position, m_Position + m_Front, m_Up);
    }

    Vect3f getFront()
    {
        return m_Front;
    }

    Vect3f getUp()
    {
        return m_Up;
    }

    Vect3f getPosition()
    {
        return m_Position;
    }

    Vect4f getCameraInfo()
    {
        return m_CameraInfo;
    }

    void setFront(Vect3f front)
    {
        m_Front = front;
    }

    void setUp(Vect3f up)
    {
        m_Up = up;
    }

    void setPosition(Vect3f pos)
    {
        m_Position = pos;
    }

    void setCameraInfo(Vect4f info)
    {
        m_CameraInfo = info;
    }

protected:
    Vect3f 					m_Position;
    Vect3f 					m_Front;
    Vect3f 					m_Up;
    Vect4f 					m_CameraInfo;

    float clamp(float x, float upper, float lower)
    {
        return min(upper, max(x, lower));
    }
};

#endif //_ENGINE_CAMERACONTROLLER_CPB_20161126_H

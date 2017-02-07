#pragma once
#ifndef __CQUAD_HH__
#define __CQUAD_HH__

class CGeometry;
class CPixelShader;
class CEffect;

class CQuad
{
public:
    CQuad();
    virtual ~CQuad();

    bool Init( CPixelShader* aPS );
    bool Render();

private:
    CGeometry* mGeometry;
    CEffect * mEffect;
};

#endif

#pragma once
#include "EnumClass.h"
#include "ModelComponent.h"



class RenderContext {
public:
   
    void setDepthTest()
	{
        if (useDepth) glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);
    }

    void setCullMode()
	{
        switch (cullMode) {
        case CullMode::Back:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
        case CullMode::Front:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            break;
        case CullMode::None:
            glDisable(GL_CULL_FACE);
            break;
        }
    }

    void setRenderState(baseShader* shader)
    {
        bool _useDepth = shader->useDepth;
        CullMode _cullMode = shader->cullMode;
	    if (_useDepth!=useDepth)
	    {
            _useDepth = useDepth;
            setDepthTest();
	    }
        if (_cullMode!=cullMode)
        {
            cullMode = _cullMode;
            setCullMode();
        }
    }

private :
    bool useDepth = true;
    CullMode cullMode = CullMode::Back;
};

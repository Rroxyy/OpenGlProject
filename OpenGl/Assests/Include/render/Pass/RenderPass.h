#pragma once
#include <string>


class RenderContext;

class RenderPass {
public:
    RenderPass(){}
    virtual ~RenderPass() {}

    virtual void execute(RenderContext& context) = 0; // 每帧执行
    virtual void resize(int width, int height) {}    // 可选：窗口变更时重建RT
    virtual const std::string& getName() const = 0;  // 用于调试或排序
};

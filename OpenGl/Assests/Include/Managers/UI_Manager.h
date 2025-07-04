#pragma once
#include <functional>
#include <vector>


class UIManager {
public:
    using UIDrawFunc = std::function<void()>;

    void Register(const UIDrawFunc& func) {
        drawFuncs.push_back(func);
    }

    void DrawAllUI() {
        for (const auto& func : drawFuncs) {
            func();
        }
    }

    static UIManager& getInstance()
    {
        static UIManager instance;
        return instance;
    }

private:
    UIManager() = default;
    ~UIManager() = default;

    UIManager(const UIManager&) = delete;            // 拷贝构造
    UIManager& operator=(const UIManager&) = delete; // 拷贝赋值
    UIManager(UIManager&&) = delete;                 // 移动构造
    UIManager& operator=(UIManager&&) = delete;      // 移动赋值


    std::vector<UIDrawFunc> drawFuncs;
};

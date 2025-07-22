#pragma once
#include <functional>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>


class UIManager {
public:
    using UIDrawFunc = std::function<void()>;

    void Register(const UIDrawFunc& func) {
        drawFuncs.push_back(func);
    }

    

    void DrawAllUI() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuiIO& io = ImGui::GetIO();


        ImGui::Begin("Control Panel");
        ImGui::Spacing();

        showFPS();

        drawObjectsUI();

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void drawObjectsUI()
	{
        for (const auto& func : drawFuncs) 
        {
            func();
        }
	}

    void showFPS()
    {
        static float fps_accum = 0.0f;     // FPS 总和
        static int frame_count = 0;        // 帧计数器
        static float last_update = 0.0f;   // 上次更新时间
        static float avg_fps = 0.0f;       // 最终显示的平均 FPS

        float current_time = ImGui::GetTime();              // 当前时间（秒）
        fps_accum += ImGui::GetIO().Framerate;
        frame_count++;

        if (current_time - last_update >= 0.5f)             // 每隔 0.5 秒更新一次
        {
            avg_fps = fps_accum / frame_count;              // 平均 FPS
            fps_accum = 0.0f;
            frame_count = 0;
            last_update = current_time;
        }

        ImGui::Text("Avg FPS: %.1f", avg_fps);
        ImGui::Spacing();
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

//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include "Core.hpp"

#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <string>

struct ApplicationSpecification
{
    ApplicationSpecification(int argc, char** argv);
    std::string name = "MathProject";
    uint32_t width = 1600;
    uint32_t height = 900;
    int argc;
    char** argv;
};

class Application
{
public:
    Application(ApplicationSpecification appSpec);
    ~Application();

    void Run();

    inline uint32_t GetWidth() const { return m_AppSpec.width; }
    inline uint32_t GetHeight() const { return m_AppSpec.height; }
    inline float GetAspectRation() const { return static_cast<float>(m_AppSpec.width) / static_cast<float>(m_AppSpec.height); }

private:
	static void StaticRender(void* appPtr);
	void Render();

	static void StaticUpdate(int timerId, void *appPtr);
	void Update(int timerId);

	static void StaticMenu(int value, void* appPtr);
	void Menu(int value);

	void Initialize();
	void Exit();
	void CreateMenu();
private:
    ApplicationSpecification m_AppSpec;

};

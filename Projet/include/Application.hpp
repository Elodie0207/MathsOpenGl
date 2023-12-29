//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include "Core.hpp"

#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "Camera.hpp"
#include "Renderer/DrawObject.hpp"
#include "Renderer/Poly.hpp"
#include "Renderer/Quad.hpp"

enum class MouseButton
{
	Left = GLUT_LEFT_BUTTON,
	Middle = GLUT_MIDDLE_BUTTON,
	Right = GLUT_RIGHT_BUTTON,
};

enum class MousePressState
{
	Up = GLUT_UP,
	Down = GLUT_DOWN,
};

struct ApplicationSpecification
{
    ApplicationSpecification(int argc, char** argv);
    std::string name = "MathProject";
    uint32_t width = 1600;
    uint32_t height = 900;
    int argc;
    char** argv;
};

struct KeyState
{
	bool pressed = false;
	double timePressed = 0.0;
	double timeReleased = 0.0;
	inline double GetTimePressed() const { return timeReleased - timePressed; }
};

struct MouseState
{
	bool pressed = false;
	double timePressed = 0.0;
	double timeReleased = 0.0;
	Vec2Int positionPressed = Vec2Int(0);
	Vec2Int positionReleased = Vec2Int(0);

	inline Vec2Int GetTravelDistance() const { return positionReleased - positionPressed; }
	inline double GetTimePressed() const { return timeReleased - timePressed; }
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
	static void StaticUpdate(int timerId, void *appPtr);
	static void StaticMenu(int value, void* appPtr);
	static void StaticOnResize(int width, int height, void* appPtr);
	static void StaticOnKeyDown(unsigned char key, int mouseX, int mouseY, void* appPtr);
	static void StaticOnKeyUp(unsigned char key, int mouseX, int mouseY, void* appPtr);
	static void StaticOnMouseButton(int button, int state, int mouseX, int mouseY, void* appPtr);
	static void StaticOnMouseMotion(int mouseX, int mouseY, void* appPtr);

	void Menu(int value);
	void OnResize(int width, int height);
	void OnKeyDown(char key, int mouseX, int mouseY);
	void OnKeyUp(char key, int mouseX, int mouseY);
	void OnMouseButton(MouseButton button, MousePressState state, int mouseX, int mouseY);
	void OnMouseMotion(int mouseX, int mouseY);

	void Initialize();
	void CreateMenu();
	void Exit();
	void Redraw();

	void Render();
	void Update();

	void UpdateDeltaTime(double& Time, float& DeltaTime);
	double GetTime() const;
	bool GetKeyDown(char key) const;
	KeyState GetKeyState(char key) const;
	bool GetMouseDown(MouseButton key) const;
	MouseState GetMouseState(MouseButton key) const;
	Vec2Int GetMousePos() const;
	Vec2 GetInGameMousePos() const;

private:
    ApplicationSpecification m_AppSpec;
	Camera m_Camera;
	float m_CameraSpeed = 50.0f;
	double m_RenderTime = 0; float m_RenderDeltaTime = 0.0166f;
	double m_UpdateTime = 0; float m_UpdateDeltaTime = 0.0166f;

	std::unordered_map<char, KeyState> m_PressedKey;
	std::unordered_map<MouseButton, MouseState> m_PressedMouseButtons;
	Vec2Int m_MousePos;

	std::vector<std::function<void(char, int, int)>> m_OnKeyPressEvents;
	std::vector<std::function<void(char, int, int)>> m_OnKeyReleaseEvents;
	std::unordered_map<MouseButton, std::vector<std::function<void(const MouseState&)>>> m_OnMouseEvents;

	DrawObject drawObj = DrawObject({{0,0}, { 0, 512}, { 512, 512}, { 512, 0}});
	Poly poly = Poly({{0, 0}, {0, 512}, {512, 512}});
	Quad quad = Quad(Vec2(512, 512), Vec2(512, 512));
};

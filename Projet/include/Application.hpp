//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include "Core.hpp"

#include <glad/glad.h>

#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>

#include "Camera.hpp"
#include "Photoshoop/Tools.hpp"
#include "Renderer/Texture.hpp"

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE 1
#endif

enum GLFWKeyState : int {
	PRESS = GLFW_PRESS,
	RELEASE = GLFW_RELEASE,
	REPEAT = GLFW_REPEAT,
};

struct ApplicationSpecification
{
    ApplicationSpecification(int argc, char** argv);
    std::string name = "MathProject";
    uint32_t width = 1600;
    uint32_t height = 900;
	bool minified = false;
    int argc;
    char** argv;
};

struct PixelData
{
	inline PixelData(Vec2Int index, Vec2Int pixel) : Index(index), Pixel(pixel) {}
	inline PixelData() = default;
	inline ~PixelData() = default;
	Vec2Int Index;
	Vec2Int Pixel;
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

	inline const Camera& GetCamera() const { return m_Camera; }
	inline Camera& GetCamera() { return m_Camera; }

	/**
	 *
	 * @return
	 */
	[[nodiscard]] double GetTime() const;
	[[nodiscard]] bool GetKeyDown(Key key) const;
	[[nodiscard]] KeyState GetKeyState(Key key) const;
	[[nodiscard]] bool GetMouseDown(MouseButton key) const;
	[[nodiscard]] MouseState GetMouseState(MouseButton key) const;

	[[nodiscard]] Vec2Int GetMousePos() const;
	[[nodiscard]] Vec2 GetWorldMousePos() const;

	[[nodiscard]] Vec2 WorldToScreenPos(Vec2 pos) const;
	[[nodiscard]] Vec2 ScreenToWorldPos(Vec2 pos) const;

	void WriteWorldPixel(Vec2 worldPos, Vec4 color01);
	void WriteScreenPixel(Vec2Int screenPos, Vec4 color01);

	void WriteWorldPixel(Vec2 worldPos, Vec4 color01, int size);
	void WriteScreenPixel(Vec2Int screenPos, Vec4 color01, int size);

	Vec4 ReadWorldPixel(Vec2 worldPos);
	Vec4 ReadScreenPixel(Vec2Int screenPos);

	void WriteWorldLine(Vec2Int fromWorldPos, Vec2Int toWorldPos, Vec4 color01, int size = 1);
	void WriteScreenLine(Vec2Int fromWorldPos, Vec2Int toWorldPos, Vec4 color01, int size = 1);

	[[nodiscard]] bool HasTexture(Vec2 worldPos) const;
	[[nodiscard]] bool HasTexture(Vec2Int index) const;

	Ref<Texture> GetTexture(Vec2 worldPos);
	Ref<Texture> GetTexture(Vec2Int index);

	[[nodiscard]] Vec2Int GetTextureIndex(Vec2 worldPos) const;
	[[nodiscard]] PixelData GetTextureInfo(Vec2 worldPos) const;

	void DrawWorldPoint(Vec2 position, float duration = 1, Color color = Color(1,1,1,1));
	void DrawWorldLine(Vec2 from, Vec2 to, float duration = 1, Color color = Color(1,1,1,1));

	void Clear();
private:
	static void glfw_error_callback(int error, const char* description);
	/*
	static void StaticRender(void* appPtr);
	static void StaticUpdate(int timerId, void *appPtr);
	static void StaticMenu(int value, void* appPtr);
	static void StaticOnResize(int width, int height, void* appPtr);
	static void StaticOnKeyDown(unsigned char key, int mouseX, int mouseY, void* appPtr);
	static void StaticOnKeyUp(unsigned char key, int mouseX, int mouseY, void* appPtr);
	static void StaticOnMouseButton(int button, int state, int mouseX, int mouseY, void* appPtr);
	static void StaticOnMouseMotion(int mouseX, int mouseY, void* appPtr);
	static void StaticOnMouseWheel(int wheel, int direction, int mouseX, int mouseY, void* appPtr);

	void Menu(int value);
	 */
	void OnResize(int width, int height);
	void OnKeyDown(Key key, int mouseX, int mouseY);
	void OnKeyUp(Key key, int mouseX, int mouseY);
	void OnMouseButton(MouseButton button, PressState state, int mouseX, int mouseY);
	void OnMouseMotion(int mouseX, int mouseY);
	void OnMouseWheel(int wheel, int direction, int mouseX, int mouseY);

	void Initialize();

	void CreateMenu();

	void Exit();

	void Render();
	void Update();
	void Menu();


	void CreateTextures();
	Ref<Texture> CreateTexture(Vec2 worldPos);
	Ref<Texture> CreateTexture(Vec2Int index);
	Ref<Texture> CreateTexture();
	Ref<Texture> GetOrCreateTexture(Vec2 worldPos);
	Ref<Texture> GetOrCreateTexture(Vec2Int index);

	void OverwriteTextures();
private:
    ApplicationSpecification m_AppSpec;
	Camera m_Camera;
	double m_RenderTime = 0; float m_RenderDeltaTime = 0.0166f; // about a frame at 60fps
	double m_UpdateTime = 0; float m_UpdateDeltaTime = 0.0166f; // about a frame at 60fps

	std::unordered_map<Key, KeyState> m_PressedKey;
	std::unordered_map<MouseButton, MouseState> m_PressedMouseButtons;
	Vec2Int m_MousePos;

	std::vector<std::function<void(Key, int, int)>> m_OnKeyPressEvents;
	std::vector<std::function<void(Key, int, int)>> m_OnKeyReleaseEvents;
	std::unordered_map<MouseButton, std::vector<std::function<void(const MouseState&)>>> m_OnMouseEvents;

	ToolsHandler m_Tools;

	const int m_TexWidth, m_TexHeight;
	std::unordered_map<Vec2Int, Ref<Texture>> m_Textures;
//	std::unordered_map<Vec2Int, std::unordered_map<Vec2Int, Vec4>> m_PixelCache;
	std::vector<Quad> m_TextureObjects;
	std::vector<std::tuple<Poly, double>> m_Lines;
	GLFWwindow* m_Window = nullptr;
};

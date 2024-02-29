//
// Created by ianpo on 29/12/2023.
//

#define NOMINMAX

#include "Core.hpp"
#include "Application.hpp"
#include "Parameters.hpp"
#include <functional>

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE 1
#endif

#ifndef IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 1
#endif

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>


ApplicationSpecification::ApplicationSpecification(int argc, char **argv) : argc(argc), argv(argv) {}

Application::Application(ApplicationSpecification appSpec) :
		m_AppSpec(std::move(appSpec)),
		m_Camera(m_AppSpec.width, m_AppSpec.height),
		m_Tools(this),
		m_TexWidth(m_AppSpec.width),
		m_TexHeight(m_AppSpec.height)
{
	REI_ASSERT(m_AppSpec.width > 0 && m_AppSpec.height > 0, "The Aspect Ration ({0}/{1}) is no valid.", m_AppSpec.width, m_AppSpec.height);


	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		REI_ERROR("GLFW did not initialized.");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	// Create window with graphics context
	m_Window = glfwCreateWindow(static_cast<int>(m_AppSpec.width), static_cast<int>(m_AppSpec.height), m_AppSpec.name.c_str(), nullptr, nullptr);
	if (m_Window == nullptr) {
		REI_ERROR("GLFW did not initialized.");
		return;
	}

	glfwMakeContextCurrent(m_Window);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	printf("glad loading status: %d\n", status);

	REI_INFO("OpenGL Info:");
	REI_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
	REI_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
	REI_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

	glfwSwapInterval(1); // Enable vsync
	Initialize();
}

Application::~Application() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void Application::Initialize() {
	glEnable(GL_TEXTURE_2D);
	glLineWidth(5.0f);
	glPointSize(5.0f);

	glfwSetWindowUserPointer(m_Window, this);

	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
	{
		Application& data = *((Application*)glfwGetWindowUserPointer(window));
		REI_INFO("Resize to : ({0}, {1})", width, height);
		data.OnResize(width, height);
	});

	glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int iconify)
	{
		Application& data = *((Application*)glfwGetWindowUserPointer(window));
		data.m_AppSpec.minified = iconify == GLFW_TRUE;
		if(data.m_AppSpec.minified)
		{
			REI_INFO("Application is minified");}
		else
		{
			REI_INFO("Application is not minified");
		}
	});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int intkey, int scancode, int action, int mods)
	{
		const auto& io = ImGui::GetIO(); auto imguiUseInputs = io.WantCaptureMouse || io.WantCaptureKeyboard;
		Application& data = *((Application*)glfwGetWindowUserPointer(window));
		const auto& mousePos = data.m_MousePos;

		Key key = (Key)intkey;

		switch ((GLFWKeyState)action) {
			case GLFWKeyState::PRESS:
			{
				if(!imguiUseInputs) {
					REI_INFO("Pressing key {0}", GetKeyName(key));
					data.OnKeyDown(key, mousePos.x, mousePos.y);
				}
				break;
			}
			case GLFWKeyState::RELEASE:
			{
				REI_INFO("Release key {0}", GetKeyName(key));
				data.OnKeyUp(key, mousePos.x, mousePos.y);
				break;
			}
		}
	});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods){

		auto mouseButton = (MouseButton)button;
		auto mousePressState = (PressState)action;

		const auto& io = ImGui::GetIO(); auto imguiUseInputs = io.WantCaptureMouse || io.WantCaptureKeyboard;
		if(imguiUseInputs)
		{
			return;
		}
		Application& data = *((Application*)glfwGetWindowUserPointer(window));
		const auto& mousePos = data.m_MousePos;
		REI_INFO("Mouse Button : {0}; MouseState: {1}", button, action);
		data.OnMouseButton(mouseButton, mousePressState, mousePos.x, mousePos.y);
	});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset){
		const auto& io = ImGui::GetIO(); auto imguiUseInputs = io.WantCaptureMouse || io.WantCaptureKeyboard;
		if(imguiUseInputs) return;

		Application& data = *((Application*)glfwGetWindowUserPointer(window));
		const auto& mousePos = data.m_MousePos;
		REI_INFO("Scroll : ({0}, {1})", xoffset, yoffset);
		data.OnMouseWheel(0, yoffset, mousePos.x, mousePos.y);
	});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos){
		Application& data = *((Application*)glfwGetWindowUserPointer(window));
		REI_INFO("Mouse Move to : ({0}, {1})", xpos, ypos);
		data.OnMouseMotion(xpos, ypos);
	});


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	m_Tools.Initialize();
	CreateTextures();
}

void Application::Run() {

	// Our state
	bool show_demo_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGuiIO& io = ImGui::GetIO();

	while(!glfwWindowShouldClose(m_Window))
	{
		auto time = glfwGetTime();
		m_UpdateDeltaTime = static_cast<float>(time - m_UpdateTime);
		m_UpdateTime = time;
		m_RenderDeltaTime = static_cast<float>(time - m_RenderTime);
		m_RenderTime = time;

		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//		if (show_demo_window)
//			ImGui::ShowDemoWindow(&show_demo_window);


		Menu();

		Update();

		// Rendering
		ImGui::Render();

		int display_w, display_h;
		glfwGetFramebufferSize(m_Window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(m_Window);
	}
}

//void Application::CreateMenu() {
//    static int returnsubmenucolor4;
//
//
//    returnsubmenucolor4 = glutCreateMenuUcall(Application::StaticMenu,this);
//    glutAddMenuEntry("Red", (int)(Tools::RED));
//    glutAddMenuEntry("Yellow",(int)Tools::YELLOW);
//    glutAddMenuEntry("Blue",(int) Tools::BLUE);
//
//	glutCreateMenuUcall(Application::StaticMenu, this);
//	glutAddMenuEntry("move", (int)Tools::MOVE);
//    glutAddSubMenu("couleurs",returnsubmenucolor4); //;
//	glutAddMenuEntry("trace polygone", (int)Tools::DRAW_POLYGONE);
//	glutAddMenuEntry("trace fenetre", (int)Tools::DRAW_WINDOW);
//	glutAddMenuEntry("fenetrage", (int)Tools::WINDOWING);
//	glutAddMenuEntry("remplissage", (int)Tools::SWEEP_FILLING);
//	glutAddMenuEntry("Exit", -1);
//
//	glutAttachMenu(GLUT_RIGHT_BUTTON);
//}

void Application::Menu()
{
	ImGui::Begin("Tools");
	{
		if(ImGui::CollapsingHeader("Camera")) {
			if (ImGui::Button("Reset Position")) {
				m_Camera.SetPosition({0, 0});
			}

			Vec2 pos = m_Camera.GetPosition();
			if(ImGui::DragFloat2("Camera Position", &pos.x, 1))
			{
				m_Camera.SetPosition(pos);
			}

			float size = m_Camera.GetSize();
			if(ImGui::DragFloat("Camera Size", &size, 1, 1, FLT_MAX))
			{
				m_Camera.SetSize(size);
			}
		}
	}
	ImGui::End();

	m_Tools.OnImGui();
}

void Application::Exit()
{
	m_Tools.Destroy();
	glfwSetWindowShouldClose(m_Window, true);
}

void Application::OnResize(int width, int height)
{
	m_AppSpec.width = width > 0 ? width : 1;
	m_AppSpec.height = height > 0 ? height : 1;
	m_Camera.ChangeAspectRatio(m_AppSpec.width, m_AppSpec.height);
	glViewport(0,0,m_AppSpec.width, m_AppSpec.height);
//	Redraw();
}

void Application::OnKeyDown(Key key, int mouseX, int mouseY)
{
	if(key == Key::R)
	{
		m_Camera.SetPosition({0,0});
	}

	auto it = m_PressedKey.find(key);
	if(it == m_PressedKey.end())
	{
		REI_INFO("On First Key Down {0}", GetKeyName(key));
		m_PressedKey.insert({key, {true, GetTime(), 0.0}});
		for(auto& onKeyPress : m_OnKeyPressEvents)
		{
			onKeyPress.operator()(key, mouseX, mouseY);
		}
	}
	else if (!it->second.pressed)
	{
		REI_INFO("On Key Down {0}", GetKeyName(key));
		it->second.pressed = true;
		it->second.timePressed = GetTime();
		for(auto& onKeyPress : m_OnKeyPressEvents)
		{
			onKeyPress.operator()(key, mouseX, mouseY);
		}
	}
	else
	{
		//TODO: Handling OnKeyPressRepeat
	}
}

void Application::OnKeyUp(Key key, int mouseX, int mouseY)
{
	REI_INFO("OnKeyUp {0}", GetKeyName(key));
	auto it = m_PressedKey.find(key);
	if(it == m_PressedKey.end())
	{
		m_PressedKey.insert({key, {false, 0.0, GetTime()}});
	}
	else
	{
		it->second.pressed = false;
		it->second.timeReleased = GetTime();
	}

	for(auto& onKeyRelease : m_OnKeyReleaseEvents)
	{
		onKeyRelease.operator()(key, mouseX, mouseY);
	}
}

void Application::OnMouseButton(MouseButton button, PressState state, int mouseX, int mouseY)
{
	auto mousePos = Vec2Int(mouseX, mouseY);

	auto& mouseState = m_PressedMouseButtons[button];
	mouseState.pressed = state == PressState::Down;
	if(mouseState.pressed)
	{
		mouseState.timePressed = GetTime();
		mouseState.positionPressed = mousePos;
		m_Tools.OnMouseClick(button, mouseState);
	}
	else
	{
		mouseState.timeReleased = GetTime();
		mouseState.positionReleased = mousePos;
		m_Tools.OnMouseRelease(button, mouseState);
	}

	auto eventIt = m_OnMouseEvents.find(button);
	if(eventIt != m_OnMouseEvents.end())
	{
		for(auto& event : eventIt->second)
		{
			event.operator()(mouseState);
		}
	}
}

void Application::OnMouseMotion(int mouseX, int mouseY)
{
	m_MousePos = Vec2Int(mouseX, mouseY);
	m_Tools.OnMouseMove(m_MousePos);
}

void Application::OnMouseWheel(int wheel, int direction, int mouseX, int mouseY)
{
	float multiplier = 0.9f;
	if(direction < 0) multiplier = 1.0f/multiplier;
	float initialSize = m_Camera.GetSize();
	float size = initialSize * multiplier;
	m_Camera.SetSize(size);
}

double Application::GetTime() const {
	return glfwGetTime();
}

bool Application::GetKeyDown(Key key) const
{
	auto it = m_PressedKey.find(key);
	if(it == m_PressedKey.end()) return false;
	return it->second.pressed;
}

KeyState Application::GetKeyState(Key key) const
{
	auto it = m_PressedKey.find(key);
	if(it == m_PressedKey.end()) return { false, 0.0 };
	return it->second;
}

bool Application::GetMouseDown(MouseButton key) const
{
	auto it = m_PressedMouseButtons.find(key);
	if(it == m_PressedMouseButtons.end()) return false;
	return it->second.pressed;
}

MouseState Application::GetMouseState(MouseButton key) const
{
	auto it = m_PressedMouseButtons.find(key);
	if(it == m_PressedMouseButtons.end()) return { false, 0.0, 0.0, Vec2Int(0), Vec2Int(0) };
	return it->second;
}

void Application::Render() {
	OverwriteTextures();

	const Mat4& viewProj = m_Camera.GetViewProjMatrix();

	for(auto& quad : m_TextureObjects)
	{
		quad.Draw(viewProj);
	}

	m_Tools.Draw(viewProj);

	int toRemove = -1;
	auto time= GetTime();
	for (int i = 0; i < m_Lines.size(); ++i)
	{
		std::get<Poly>(m_Lines[i]).Draw(viewProj);


		auto lineTime = std::get<double>(m_Lines[i]);
		if(i > toRemove && lineTime <= time)
		{
			toRemove = i;
		}
	}

	if(toRemove > -1)
	{
		m_Lines.erase(m_Lines.begin(), m_Lines.begin() + toRemove);
	}

	glFlush();
}

void Application::Update()
{
	m_Tools.OnUpdate(m_UpdateDeltaTime);
}

Vec2Int Application::GetMousePos() const {
	return m_MousePos;
}

Vec2 Application::GetWorldMousePos() const
{
	return m_Camera.ScreenToGameSpace(m_MousePos);
}

Vec2 Application::WorldToScreenPos(Vec2 pos) const
{
	return m_Camera.GameToScreenSpace(pos);
}

Vec2 Application::ScreenToWorldPos(Vec2 pos) const
{
	return m_Camera.ScreenToGameSpace(pos);
}

void Application::WriteScreenPixel(Vec2Int screenPos, Vec4 color01)
{
	Vec2 worldPos = ScreenToWorldPos(screenPos);
	WriteWorldPixel(worldPos, color01);
}

void Application::WriteWorldPixel(Vec2 worldPos, Vec4 color01)
{
	auto info = GetTextureInfo(worldPos);
	auto& index = info.Index;
	auto& pixel = info.Pixel;

	GetOrCreateTexture(index)->SetPixel4(pixel, color01);

//	m_PixelCache[index][pixel] = color01;
}

void Application::WriteScreenPixel(Vec2Int screenPos, Vec4 color01, int size)
{
	Vec2 worldPos = ScreenToWorldPos(screenPos);
	WriteWorldPixel(worldPos, color01, size);
}

void Application::WriteWorldPixel(Vec2 pos, Vec4 color01, int size)
{
	float hSize = size / 2.0f;
	Vec2Int min = pos - hSize;
	Vec2Int max = pos + hSize;

	for (int x = min.x; x <= max.x; ++x) {
		for (int y = min.y; y <= max.y; ++y) {
			Vec2Int currentPx = {x,y};
			auto dst = Math::Distance(Vec2(pos), Vec2(currentPx));
			if(dst < std::max(hSize, 1.0f)) {
				WriteWorldPixel(currentPx, color01);
			}
		}
	}
}

void Application::WriteWorldLine(Vec2Int from, Vec2Int to, Vec4 color01, int size)
{
	// Bresenham's line algorithm
	int dx = std::abs(to.x - from.x);
	int dy = std::abs(to.y - from.y);

	int sx = (from.x < to.x) ? 1 : -1;
	int sy = (from.y < to.y) ? 1 : -1;

	int err = dx - dy;

	do
	{
		WriteWorldPixel({from.x, from.y}, color01, size);

		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			from.x += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			from.y += sy;
		}
	}
	while(from.x != to.x || from.y != to.y);
}
void Application::WriteScreenLine(Vec2Int fromScreenPos, Vec2Int toScreenPos, Vec4 color01, int size)
{
	Vec2Int from = ScreenToWorldPos(fromScreenPos);
	Vec2Int to = ScreenToWorldPos(toScreenPos);
	WriteWorldLine(from, to, color01);
}

Vec4 Application::ReadScreenPixel(Vec2Int screenPos)
{
	Vec2 worldPos = ScreenToWorldPos(screenPos);
	return ReadWorldPixel(worldPos);
}

Vec4 Application::ReadWorldPixel(Vec2 worldPos)
{
	auto info = GetTextureInfo(worldPos);
	auto& index = info.Index;
	auto& pixel = info.Pixel;

	Ref<Texture> texture = GetOrCreateTexture(index);
	return texture->GetPixel4(pixel);
}

void Application::CreateTextures()
{
//	CreateTexture(Vec2Int{-1,-1});
//	CreateTexture(Vec2Int{-1,+0});
//	CreateTexture(Vec2Int{-1,+1});
//	CreateTexture(Vec2Int{+0,-1});
	CreateTexture(Vec2Int{+0,+0});
//	CreateTexture(Vec2Int{+0,+1});
//	CreateTexture(Vec2Int{+1,-1});
//	CreateTexture(Vec2Int{+1,+0});
//	CreateTexture(Vec2Int{+1,+1});
}
Ref<Texture> Application::CreateTexture() {
//	Vec4 color = Parameters::GetClearColor();
	Vec4 color = {1,1,1,1};
	Texture2DSpecification spec;
	spec.filterMin = TextureFilter::Linear;
	spec.filterMag = TextureFilter::Linear;
	spec.wrapperS = TextureWrapper::ClampToEdge;
	spec.wrapperT = TextureWrapper::ClampToEdge;
	return Texture::Create(color, static_cast<int>(m_TexWidth), static_cast<int>(m_TexHeight));
}

void Application::OverwriteTextures()
{
	for (auto& texObj : m_TextureObjects)
	{
		if(texObj.m_Texture->IsDirty())
		{
			texObj.m_Texture->UpdateTexture();
		}
	}
}

Ref<Texture> Application::CreateTexture(Vec2 worldPos)
{
	return CreateTexture(GetTextureIndex(worldPos));
}

Ref<Texture> Application::CreateTexture(Vec2Int index)
{
	auto tex = CreateTexture();
	m_Textures[index] = tex;
	m_TextureObjects.emplace_back(Vec2((index.x * m_TexWidth) + (m_TexWidth/2), (index.y * m_TexHeight) + (m_TexHeight/2)), Vec2(m_TexWidth, m_TexHeight), tex);
	return tex;
}

Ref<Texture> Application::GetTexture(Vec2 worldPos)
{
	Vec2Int index = GetTextureIndex(worldPos);
	return GetTexture(index);
}

Ref<Texture> Application::GetTexture(Vec2Int index)
{
	auto it = m_Textures.find(index);
	if(it == m_Textures.end()) return nullptr;
	return it->second;
}

Vec2Int Application::GetTextureIndex(Vec2 worldPos) const
{
	Vec2Int index = Vec2Int(Math::Floor(worldPos.x / (float)m_TexWidth),
							Math::Floor( worldPos.y / (float)m_TexHeight));
	return index;
}

PixelData Application::GetTextureInfo(Vec2 worldPos) const
{
	auto index = GetTextureIndex(worldPos);
	auto pixel = Vec2Int(
			Math::Floor((int)Math::Floor(worldPos.x) % ((int)m_TexWidth)),
			Math::Floor((int)Math::Floor(worldPos.y) % ((int)m_TexHeight)));
	if(pixel.x < 0) pixel.x += static_cast<int>(m_TexWidth);
	if(pixel.y < 0) pixel.y += static_cast<int>(m_TexHeight);
	return {index, pixel};
}

bool Application::HasTexture(Vec2 worldPos) const
{
	auto index = GetTextureIndex(worldPos);
	return HasTexture(index);
}

bool Application::HasTexture(Vec2Int index) const
{
	auto it = m_Textures.find(index);
	return it != m_Textures.end();
}

Ref<Texture> Application::GetOrCreateTexture(Vec2 worldPos)
{
	return GetOrCreateTexture(GetTextureIndex(worldPos));
}

Ref<Texture> Application::GetOrCreateTexture(Vec2Int index)
{
	Ref<Texture> texture;
	auto it = m_Textures.find(index);
	if (it == m_Textures.end())
	{
		texture = CreateTexture(index);
	}
	else
	{
		texture = it->second;
	}

	return texture;
}

void Application::DrawWorldPoint(Vec2 position, float duration, Color color) {

	auto tuple = m_Lines.emplace_back(Poly({position}), GetTime() + duration);
	std::get<Poly>(tuple).m_Color = color;
}

void Application::DrawWorldLine(Vec2 from, Vec2 to, float duration, Color color) {

	auto tuple = m_Lines.emplace_back(Poly({from, to}), GetTime() + duration);
	std::get<Poly>(tuple).m_Color = color;
}

void Application::glfw_error_callback(int error, const char *description)
{
	REI_ERROR("GLFW Error {0}: {1}", error, description);
}

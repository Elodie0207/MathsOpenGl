//
// Created by ianpo on 29/12/2023.
//

#include "Core.hpp"
#include "Application.hpp"
#include "Parameters.hpp"
#include <functional>

#define CallNextUpdate(appPtr) glutTimerFuncUcall(1000 / 60, &Application::StaticUpdate, 0, appPtr);

ApplicationSpecification::ApplicationSpecification(int argc, char **argv) : argc(argc), argv(argv) {}

void Application::StaticRender(void* appPtr)
{
	REI_ASSERT(static_cast<Application*>(appPtr) != nullptr, "The pointer {0} is not a valid Application Pointer.", appPtr);
//	REI_INFO("New Rendering");
	Application* app = static_cast<Application *>(appPtr);
	app->UpdateDeltaTime(app->m_RenderTime, app->m_RenderDeltaTime);
	app->Render();
}

void Application::StaticUpdate(int timerId, void* appPtr)
{
	REI_ASSERT(static_cast<Application*>(appPtr) != nullptr, "The pointer {0} is not a valid Application Pointer.", appPtr);
	Application* app = static_cast<Application *>(appPtr);
	app->UpdateDeltaTime(app->m_UpdateTime, app->m_UpdateDeltaTime);
	app->Update();
	CallNextUpdate(appPtr);
}

void Application::StaticMenu(int value, void* appPtr)
{
	REI_ASSERT(static_cast<Application*>(appPtr) != nullptr, "The pointer {0} is not a valid Application Pointer.", appPtr);
	static_cast<Application*>(appPtr)->Menu(value);
}

void Application::StaticOnResize(int width, int height, void *appPtr)
{
	REI_ASSERT(static_cast<Application*>(appPtr) != nullptr, "The pointer {0} is not a valid Application Pointer.", appPtr);
	static_cast<Application*>(appPtr)->OnResize(width, height);
}

void Application::StaticOnKeyDown(unsigned char key, int mouseX, int mouseY, void* appPtr)
{
	REI_ASSERT(static_cast<Application*>(appPtr) != nullptr, "The pointer {0} is not a valid Application Pointer.", appPtr);
	static_cast<Application*>(appPtr)->OnKeyDown((char)key, mouseY, mouseY);
}

void Application::StaticOnKeyUp(unsigned char key, int mouseX, int mouseY, void* appPtr)
{
	REI_ASSERT(static_cast<Application*>(appPtr) != nullptr, "The pointer {0} is not a valid Application Pointer.", appPtr);
	static_cast<Application*>(appPtr)->OnKeyUp((char)key, mouseY, mouseY);
}

void Application::StaticOnMouseButton(int button, int state, int mouseX, int mouseY, void *appPtr)
{
	REI_ASSERT(static_cast<Application*>(appPtr) != nullptr, "The pointer {0} is not a valid Application Pointer.", appPtr);
	static_cast<Application *>(appPtr)->OnMouseButton(static_cast<MouseButton>(button),
													  static_cast<MousePressState>(state), mouseX, mouseY);
}

void Application::StaticOnMouseMotion(int mouseX, int mouseY, void* appPtr)
{
	REI_ASSERT(static_cast<Application*>(appPtr) != nullptr, "The pointer {0} is not a valid Application Pointer.", appPtr);
	static_cast<Application *>(appPtr)->OnMouseMotion(mouseX, mouseY);
}

Application::Application(ApplicationSpecification appSpec) : m_AppSpec(std::move(appSpec)), m_Camera(m_AppSpec.width, m_AppSpec.height), m_Tools(this)
{
	REI_ASSERT(m_AppSpec.width > 0 && m_AppSpec.height > 0, "The Aspect Ration ({0}/{1}) is no valid.", m_AppSpec.width, m_AppSpec.height);
	glutInit(&m_AppSpec.argc, m_AppSpec.argv);
	glutInitWindowSize(m_AppSpec.width, m_AppSpec.height);
	glutCreateWindow(m_AppSpec.name.c_str());

	REI_INFO("OpenGL Info:");
	REI_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
	REI_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
	REI_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

	Initialize();
}

Application::~Application() {}

void Application::Initialize() {
	glEnable(GL_TEXTURE_2D);
	glLineWidth(5.0f);

	m_Tools.Initialize();

	glutDisplayFuncUcall(&Application::StaticRender, this);
	CallNextUpdate(this);
	glutReshapeFuncUcall(&Application::StaticOnResize, this);
	glutKeyboardFuncUcall(&Application::StaticOnKeyDown, this);
	glutKeyboardUpFuncUcall(&Application::StaticOnKeyUp, this);
	glutMouseFuncUcall(&Application::StaticOnMouseButton, this);
	glutMotionFuncUcall(&Application::StaticOnMouseMotion, this);
	glutPassiveMotionFuncUcall(&Application::StaticOnMouseMotion, this);
	CreateMenu();
}

void Application::Run() {
	glutMainLoop();
}

void Application::CreateMenu() {
	glutCreateMenuUcall(Application::StaticMenu, this);

	// Ajout des entrées dans le menu
	glutAddMenuEntry("move", (int)Tools::MOVE);
	glutAddMenuEntry("couleurs", (int)Tools::CHANGE_COLOR);
	glutAddMenuEntry("trace polygone", (int)Tools::DRAW_POLYGONE);
	glutAddMenuEntry("trace fenetre", (int)Tools::DRAW_WINDOW);
	glutAddMenuEntry("fenetrage", (int)Tools::WINDOWING);
	glutAddMenuEntry("remplissage", (int)Tools::FILLING);
	glutAddMenuEntry("Exit", -1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void Application::Menu(int value)
{
	if(value == -1) {
		REI_INFO("Press 'Tools::EXIT'.");
		Exit();
		return;
	}

	auto tool = (Tools)value;

	switch (tool) {
		case Tools::CHANGE_COLOR:
			//TODO: Handling the change of color
			REI_INFO("Should Change Color.");
//			auto previous = m_Tools.GetSelectedTool();
			m_Tools.SetTool(Tools::NONE);
			//TODO: set back to the previous tools once the color has been selected/changed.
			break;
		default:
			m_Tools.SetTool(tool);
			break;
	}

	Redraw();
}

void Application::Exit()
{
	m_Tools.Destroy();
	glutExit();
}

void Application::OnResize(int width, int height)
{
	m_AppSpec.width = width > 0 ? width : 1;
	m_AppSpec.height = height > 0 ? height : 1;
	m_Camera.ChangeAspectRatio(m_AppSpec.width, m_AppSpec.height);
}

void Application::UpdateDeltaTime(double& Time, float& DeltaTime)
{
	int timeInMs = glutGet(GLUT_ELAPSED_TIME);
	double timeInS = timeInMs / 1000.0;
	DeltaTime = static_cast<float>(timeInS - Time);
	Time = timeInS;
}

void Application::OnKeyDown(char key, int mouseX, int mouseY)
{
	auto it = m_PressedKey.find(key);
	if(it == m_PressedKey.end())
	{
		REI_INFO("On First Key Down {0}", key);
		m_PressedKey.insert({key, {true, GetTime(), 0.0}});
		for(auto& onKeyPress : m_OnKeyPressEvents)
		{
			onKeyPress.operator()(key, mouseX, mouseY);
		}
	}
	else if (!it->second.pressed)
	{
		REI_INFO("On Key Down {0}", key);
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

void Application::OnKeyUp(char key, int mouseX, int mouseY)
{
	REI_INFO("OnKeyUp {0}", key);
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

void Application::OnMouseButton(MouseButton button, MousePressState state, int mouseX, int mouseY)
{
	auto mousePos = Vec2Int(mouseX, mouseY);

	auto& mouseState = m_PressedMouseButtons[button];
	mouseState.pressed = state == MousePressState::Down;
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

double Application::GetTime() const {
	return glutGet(GLUT_ELAPSED_TIME);
}

bool Application::GetKeyDown(char key) const
{
	auto it = m_PressedKey.find(key);
	if(it == m_PressedKey.end()) return false;
	return it->second.pressed;
}

KeyState Application::GetKeyState(char key) const
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
	auto clearColor = Parameters::GetClearColor();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const Mat4& viewProj = m_Camera.GetViewProjMatrix();

	m_Tools.Draw(viewProj);

	glFlush();
}

void Application::Update()
{
	if(m_Tools.OnUpdate(m_UpdateDeltaTime))
	{
		Redraw();
	}
}

void Application::Redraw()
{
	glutPostRedisplay();
}

Vec2Int Application::GetMousePos() const {
	return m_MousePos;
}

Vec2 Application::GetInGameMousePos() const
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

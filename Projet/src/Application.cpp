//
// Created by ianpo on 29/12/2023.
//

#include "Core.hpp"
#include "Application.hpp"
#include "Parameters.hpp"
#include <functional>

#define CallNextUpdate(appPtr) glutTimerFuncUcall(1000 / 60, &Application::StaticUpdate, 0, appPtr);
static Application* s_MainApp = nullptr;

enum {
	MENU_OPTION_1,
	MENU_OPTION_2,
	MENU_OPTION_3,
	MENU_OPTION_4,
	MENU_OPTION_5,
	MENU_OPTION_EXIT
};

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

Application::Application(ApplicationSpecification appSpec) : m_AppSpec(std::move(appSpec)), m_Camera(m_AppSpec.width, m_AppSpec.height)
{
	drawObj.m_Color = Vec4(0.8,0.2,0.3,1);
	poly.m_Color = Vec4(0.2,0.3,0.8,1);

	REI_ASSERT(m_AppSpec.width > 0 && m_AppSpec.height > 0, "The Aspect Ration ({0}/{1}) is no valid.", m_AppSpec.width, m_AppSpec.height);
	glutInit(&m_AppSpec.argc, m_AppSpec.argv);
	glutInitWindowSize(m_AppSpec.width, m_AppSpec.height);
	glutCreateWindow(m_AppSpec.name.c_str());

	s_MainApp = this;

	REI_INFO("OpenGL Info:");
	REI_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
	REI_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
	REI_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

	Initialize();
}

Application::~Application()
{
	s_MainApp = nullptr;
}

void Application::Initialize() {
	glEnable(GL_TEXTURE_2D);
	glLineWidth(5.0f);

	quad.Texture = Texture::Create(Vec4(0.1,0.2,0.9, 1.0), 128, 128);

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
	glutAddMenuEntry("couleurs", MENU_OPTION_1);
	glutAddMenuEntry("trace polygone", MENU_OPTION_2);
	glutAddMenuEntry("trace fenetre", MENU_OPTION_3);
	glutAddMenuEntry("fenetrage", MENU_OPTION_4);
	glutAddMenuEntry("remplissage", MENU_OPTION_5);
	glutAddMenuEntry("Exit", MENU_OPTION_EXIT);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void Application::Menu(int value)
{
	switch (value) {
		case MENU_OPTION_1:
			// a faire
			break;
		case MENU_OPTION_2:
			// a faire
			break;
		case MENU_OPTION_3:
			// a faire
			break;
		case MENU_OPTION_4:
			// a faire
			break;
		case MENU_OPTION_5:
			// a faire
			break;
		case MENU_OPTION_EXIT:
			Exit();
			return;
	}

	Redraw();
}

void Application::Exit()
{
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
	}
	else
	{
		mouseState.timeReleased = GetTime();
		mouseState.positionReleased = mousePos;
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

	drawObj.Draw(viewProj);
	quad.Draw(viewProj);
	poly.Draw(viewProj);

	glFlush();
}

void Application::Update()
{
	// Camera Movement
	Vec2 movement(0.0f);
	float frameMovement = m_CameraSpeed * m_UpdateDeltaTime;
	if(GetKeyDown('z')) // up
	{
		movement += Vec2(0, +1);
	}

	if(GetKeyDown('s')) // down
	{
		movement += Vec2(0, -1);
	}

	if(GetKeyDown('d')) // right
	{
		movement += Vec2(+1, 0);
	}

	if(GetKeyDown('q')) // down
	{
		movement += Vec2(-1, 0);
	}

	if(movement != Vec2(0.0))
	{
		movement = Math::Normalize(movement) * frameMovement;
//		REI_INFO("Move of {0} units", Math::ToString(movement));
		m_Camera.MovePosition(movement);
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

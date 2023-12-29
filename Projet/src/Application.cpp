//
// Created by ianpo on 29/12/2023.
//

#include "Core.hpp"
#include "Application.hpp"
#include <functional>

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
	static_cast<Application *>(appPtr)->Render();
}

void Application::StaticUpdate(int timerId, void* appPtr)
{
	static_cast<Application *>(appPtr)->Update(timerId);
}
void Application::StaticMenu(int value, void* appPtr)
{
	static_cast<Application*>(appPtr)->Menu(value);
}

Application::Application(ApplicationSpecification appSpec) : m_AppSpec(std::move(appSpec))
{
	glutInit(&m_AppSpec.argc, m_AppSpec.argv);
	glutInitWindowSize(m_AppSpec.width, m_AppSpec.height);
	glutCreateWindow(m_AppSpec.name.c_str());

	s_MainApp = this;

	Initialize();
}

Application::~Application()
{
	s_MainApp = nullptr;
}

void Application::Initialize() {
	glutDisplayFuncUcall(&Application::StaticRender, this);
	glutTimerFuncUcall(16, &Application::StaticUpdate, 0, this);
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

void Application::Render() {
	glClearColor(0.1, 0.5, 0.4, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Rendering

	glFlush();
}

void Application::Update(int timerId)
{
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
	glutPostRedisplay();// Demande à GLUT de redessiner la fenêtre
}

void Application::Exit()
{
	glutExit();
}

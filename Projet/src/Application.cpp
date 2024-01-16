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
void Application::StaticOnMouseWheel(int wheel, int direction, int mouseX, int mouseY, void* appPtr)
{
	REI_ASSERT(static_cast<Application*>(appPtr) != nullptr, "The pointer {0} is not a valid Application Pointer.", appPtr);
	static_cast<Application *>(appPtr)->OnMouseWheel(wheel, direction, mouseX, mouseY);
}

Application::Application(ApplicationSpecification appSpec) :
	m_AppSpec(std::move(appSpec)),
	m_Camera(m_AppSpec.width, m_AppSpec.height),
	m_Tools(this),
	m_TexWidth(m_AppSpec.width),
	m_TexHeight(m_AppSpec.height)
{
	REI_ASSERT(m_AppSpec.width > 0 && m_AppSpec.height > 0, "The Aspect Ration ({0}/{1}) is no valid.", m_AppSpec.width, m_AppSpec.height);
	glutInit(&m_AppSpec.argc, m_AppSpec.argv);
	glutInitWindowSize(static_cast<int>(m_AppSpec.width), static_cast<int>(m_AppSpec.height));
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
	glPointSize(5.0f);

	m_Tools.Initialize();

	glutDisplayFuncUcall(&Application::StaticRender, this);
	glutTimerFuncUcall(1000 / 60, &Application::StaticUpdate, 0, this);;
	glutReshapeFuncUcall(&Application::StaticOnResize, this);
	glutKeyboardFuncUcall(&Application::StaticOnKeyDown, this);
	glutKeyboardUpFuncUcall(&Application::StaticOnKeyUp, this);
	glutMouseFuncUcall(&Application::StaticOnMouseButton, this);
	glutMotionFuncUcall(&Application::StaticOnMouseMotion, this);
	glutPassiveMotionFuncUcall(&Application::StaticOnMouseMotion, this);
	glutMouseWheelFuncUcall(&Application::StaticOnMouseWheel, this);
	CreateMenu();

	CreateTextures();
}

void Application::Run() {
	glutMainLoop();
}

void Application::CreateMenu() {
    static int returnsubmenucolor4;


    returnsubmenucolor4 = glutCreateMenuUcall(Application::StaticMenu,this);
    glutAddMenuEntry("Red", (int)(Tools::RED));
    glutAddMenuEntry("Yellow",(int)Tools::YELLOW);
    glutAddMenuEntry("Blue",(int) Tools::BLUE);

	glutCreateMenuUcall(Application::StaticMenu, this);
	glutAddMenuEntry("move", (int)Tools::MOVE);
    glutAddSubMenu("couleurs",returnsubmenucolor4); //;
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
		case Tools::RED:

            m_Tools.CHANGE_COLOR(Tools::RED);
			m_Tools.SetTool(Tools::NONE);
			break;
        case Tools::BLUE:
            m_Tools.CHANGE_COLOR(Tools::BLUE);
            m_Tools.SetTool(Tools::NONE);
            break;
        case Tools::YELLOW:
            m_Tools.CHANGE_COLOR(Tools::YELLOW);
            m_Tools.SetTool(Tools::NONE);
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
	glViewport(0,0,m_AppSpec.width, m_AppSpec.height);
	Redraw();
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
		if(m_Tools.OnMouseClick(button, mouseState))
		{
			Redraw();
		}
	}
	else
	{
		mouseState.timeReleased = GetTime();
		mouseState.positionReleased = mousePos;
		if(m_Tools.OnMouseRelease(button, mouseState))
		{
			Redraw();
		}
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
	if(m_Tools.OnMouseMove(m_MousePos))
	{
		Redraw();
	}
}

void Application::OnMouseWheel(int wheel, int direction, int mouseX, int mouseY)
{
	float multiplier = 0.9f;
	if(direction < 0) multiplier = 1.0f/multiplier;
	float initialSize = m_Camera.GetSize();
	float size = initialSize * multiplier;
	m_Camera.SetSize(size);
	Redraw();
}

double Application::GetTime() const {
	auto glutTime = glutGet(GLUT_ELAPSED_TIME);
	return ((double)glutTime) / 1000.0;
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
	if(m_Tools.OnUpdate(m_UpdateDeltaTime) || !m_Lines.empty())
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
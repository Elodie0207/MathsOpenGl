//
// Created by ianpo on 29/12/2023.
//

#include "Photoshoop/Tools.hpp"
#include "Application.hpp"

void ToolsHandler::SetTool(Tools tool)
{
	m_Tool = tool;
}

ToolsHandler::ToolsHandler(Application * app) : m_App(app)
{
	REI_ASSERT(m_App != nullptr, "The application is not valid.");
	drawObj = DrawObject({{0,0}, { 0, 512}, { 512, 512}, { 512, 0}});
	poly = Poly({{0, 0}, {0, 512}, {512, 512}});
	quad = Quad(Vec2(512, 512), Vec2(512, 512));
}

ToolsHandler::~ToolsHandler()
{
	m_App = nullptr;
}

void ToolsHandler::Initialize() {
	drawObj.m_Color = Vec4(0.8,0.2,0.3,1);
	poly.m_Color = Vec4(0.2,0.3,0.8,1);
	quad.Texture = Texture::Create(Vec4(0.1,0.2,0.9, 1.0), 128, 128);
}

void ToolsHandler::Destroy() {

}

bool ToolsHandler::OnUpdate(float deltaTime)
{
	if(m_Tool == Tools::MOVE && !m_App->GetMouseDown(MouseButton::Left))
	{    // Camera Movement
		Vec2 movement(0.0f);
		float frameMovement = m_CameraSpeed * deltaTime;

		if (m_App->GetKeyDown('z')) // up
		{
			movement += Vec2(0, +1);
		}

		if (m_App->GetKeyDown('s')) // down
		{
			movement += Vec2(0, -1);
		}

		if (m_App->GetKeyDown('d')) // right
		{
			movement += Vec2(+1, 0);
		}

		if (m_App->GetKeyDown('q')) // down
		{
			movement += Vec2(-1, 0);
		}

		if (movement != Vec2(0.0)) {
			movement = Math::Normalize(movement) * frameMovement;
			m_App->GetCamera().MovePosition(movement);
			return true;
		}
	}
	return false;
}

void ToolsHandler::Draw(const Mat4 &ViewProjMatrix) {
	drawObj.Draw(ViewProjMatrix);
	quad.Draw(ViewProjMatrix);
	poly.Draw(ViewProjMatrix);
}

void ToolsHandler::OnMouseClick(MouseButton mouse, const MouseState& state)
{

}

void ToolsHandler::OnMouseRelease(MouseButton mouse, const MouseState& state)
{

}

void ToolsHandler::OnMouseMove(Vec2Int mousePos) {

}
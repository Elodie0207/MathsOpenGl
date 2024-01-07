//
// Created by ianpo on 29/12/2023.
//

#include "Photoshoop/Tools.hpp"
#include "Application.hpp"

// ===================== COMMON =====================
void ToolsHandler::SetTool(Tools tool)
{
	m_Tool = tool;
	switch (m_Tool)
	{
		case Tools::DRAW_POLYGONE:
			ClearDrawnPoly();
			break;
	}
}

ToolsHandler::ToolsHandler(Application * app) : m_App(app)
{
	REI_ASSERT(m_App != nullptr, "The application is not valid.");
	drawObj = DrawObject({{0,0}, { 0, 512}, { 512, 512}, { 512, 0}});
	polyDrawn = Poly({{0, 0}, {0, 512}, {512, 512}});
	quad = Quad(Vec2(512, 512), Vec2(512, 512));
}

ToolsHandler::~ToolsHandler()
{
	m_App = nullptr;
}

void ToolsHandler::Initialize() {
	drawObj.m_Color = Vec4(0.8,0.2,0.3,1);
	polyDrawn.m_Color = Vec4(0.2, 0.3, 0.8, 1);
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
	polyDrawn.Draw(ViewProjMatrix);
}

bool ToolsHandler::OnMouseClick(MouseButton mouse, const MouseState& state)
{
	switch (m_Tool)
	{
		case Tools::DRAW_POLYGONE:
			if(mouse == MouseButton::Left) {
				AddPointToPoly(state.positionPressed);
				return true;
			} else if (mouse == MouseButton::Middle) {
				StopDrawingPoly();
				return true;
			}
			break;
	}
	return false;
}

bool ToolsHandler::OnMouseRelease(MouseButton mouse, const MouseState& state)
{
	return false;
}

bool ToolsHandler::OnMouseMove(Vec2Int mousePos) {
	switch (m_Tool) {
		case Tools::DRAW_POLYGONE:
		{
			if(!drawing) break;
			polyDrawn.m_Points[polyDrawn.GetPointCount() - 1] = m_App->GetWorldMousePos();
			return true;
		}
			break;
	}
	return false;
}

// ===================== DRAW POLY =====================
void ToolsHandler::ClearDrawnPoly()
{
	polyDrawn.m_Points.clear();
	polyDrawn.m_Points.push_back(m_App->GetWorldMousePos());
	drawing = true;
}

void ToolsHandler::StopDrawingPoly()
{
	if(polyDrawn.GetPointCount() > 3) {
		polyDrawn.m_Points.pop_back();
		drawing = false;
	}
}

void ToolsHandler::AddPointToPoly(Vec2Int screenPos)
{
	if(drawing)
	{
		auto worldPos = m_App->ScreenToWorldPos(screenPos);
		Vec2Int firstWorldPos = polyDrawn.m_Points[0];
		float distanceToLast = Math::Distance<Vec2>(firstWorldPos, worldPos);
		REI_INFO("Distance To First {0}", distanceToLast);
		if(distanceToLast < 10) // Same place
		{
			if(polyDrawn.GetPointCount() > 3)
			{
				StopDrawingPoly();
			}
			else if(polyDrawn.GetPointCount() == 1)
			{
				polyDrawn.m_Points.push_back(worldPos);
			}
		}
		else
		{
			polyDrawn.m_Points.push_back(worldPos);
		}
	}
	else
	{
		ClearDrawnPoly();
	}
}

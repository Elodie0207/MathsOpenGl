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



	quad.m_Texture = Texture::Create(Vec4(0.1, 0.2, 0.9, 1.0), 128, 128);
}

void ToolsHandler::Destroy() {

}

bool ToolsHandler::OnUpdate(float deltaTime)
{
	if(m_Tool == Tools::MOVE && !m_App->GetMouseDown(MouseButton::Left))
	{    // Camera Movement
		Vec2 movement(0.0f);
		float frameMovement = m_CameraSpeed * deltaTime * m_App->GetCamera().GetSize();

		if (m_App->GetKeyDown('z')) // up
		{
			movement += Vec2(0, +frameMovement);
		}
		else if (m_App->GetKeyDown('Z')) // up
		{
			movement += Vec2(0, +(frameMovement * 2));
		}

		if (m_App->GetKeyDown('s')) // down
		{
			movement += Vec2(0, -frameMovement);
		}
		else if (m_App->GetKeyDown('S')) // down
		{
			movement += Vec2(0, -(frameMovement * 2));
		}

		if (m_App->GetKeyDown('d')) // right
		{
			movement += Vec2(+frameMovement, 0);
		}
		else if (m_App->GetKeyDown('D')) // right
		{
			movement += Vec2(+(frameMovement * 2), 0);
		}

		if (m_App->GetKeyDown('q')) // down
		{
			movement += Vec2(-frameMovement, 0);
		}
		else if (m_App->GetKeyDown('Q')) // down
		{
			movement += Vec2(-(frameMovement * 2), 0);
		}

		if (movement != Vec2(0.0)) {
			m_App->GetCamera().MovePosition(movement);
			return true;
		}
	}
	return false;
}

void ToolsHandler::Draw(const Mat4 &ViewProjMatrix) {
	drawObj.Draw(ViewProjMatrix);
	quad.Draw(ViewProjMatrix);
    if(color==0) {

        polyDrawn.m_Color = Vec4(0.2, 0.3, 0.8, 1);
    }
    else if(color==1) {
        polyDrawn.m_Color = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
    }
    else if(color==2){

        polyDrawn.m_Color= Vec4(1.0f, 0.0f, 0.0f, 1.0f);

    }
	polyDrawn.Draw(ViewProjMatrix);
}
static Vec2Int MousePosePressDraw;
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
		case Tools::FILLING:
		{
			if(mouse == MouseButton::Left) {
				MousePosePressDraw = state.positionPressed;
				REI_INFO("============== Mouse Click And Draw ==============");
				REI_INFO("Mouse Screen Pos : {0}", Math::ToString(state.positionPressed));
				REI_INFO("Mouse World Pos : {0}", Math::ToString(m_App->ScreenToWorldPos(state.positionPressed)));
				auto info = m_App->GetTextureInfo(m_App->ScreenToWorldPos(state.positionPressed));
				REI_INFO("Texture Index : {0}", Math::ToString(info.Index));
				REI_INFO("Pixel Coords : {0}", Math::ToString(info.Pixel));
				REI_INFO("==================================================");
				//m_App->WriteScreenPixel(state.positionPressed, Vec4(.8, .1, .2, 1));
			}
		}


	}
	return false;
}

bool ToolsHandler::OnMouseRelease(MouseButton mouse, const MouseState& state)
{
	switch (m_Tool)
	{
		case Tools::FILLING:
		{
			if (mouse == MouseButton::Left) {
				Vec2Int pressed = m_App->ScreenToWorldPos(state.positionPressed);
				Vec2Int released = m_App->ScreenToWorldPos(state.positionReleased);
				auto begin = Math::Min(pressed, released);
				auto end = Math::Max(pressed, released);
				auto color = Vec4(.8, .1, .2, 1);

				for (int x = begin.x; x <= end.x; x++)
				{
					for (int y = begin.y; y <= end.y; y++)
					{
						m_App->WriteWorldPixel({ x,y }, color);
					}
				}
				return true;

			}
		}
	}
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

void ToolsHandler::CHANGE_COLOR() {;

    int userchoice;
    REI_INFO("0 for blue, 1 for green, 2 for red");
    std::cin >> userchoice;
    REI_INFO(userchoice);
    color=userchoice;
}



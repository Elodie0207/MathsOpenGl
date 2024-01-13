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
		case Tools::DRAW_WINDOW:
			ClearDrawnWindow();
			break;
	}
}

ToolsHandler::ToolsHandler(Application * app) : m_App(app)
{
	REI_ASSERT(m_App != nullptr, "The application is not valid.");
//	drawObj = DrawObject({{0,0}, { 0, 512}, { 512, 512}, { 512, 0}});
	polyDrawn = Poly({{128, 128}, {128, 640}, {640, 640}});
	windowDrawn = Poly({{0, 0}, {0, 512}, {512, 512}, {512, 0}});
//	quad = Quad(Vec2(512, 512), Vec2(512, 512));
}

ToolsHandler::~ToolsHandler()
{
	m_App = nullptr;
}

void ToolsHandler::Initialize() {
//	drawObj.m_Color = Vec4(0.8,0.2,0.3,1);
//	quad.m_Texture = Texture::Create(Vec4(0.1, 0.2, 0.9, 1.0), 128, 128);
	polyDrawn.m_Color = Vec4(0.2, 0.3, 0.8, 1);
	windowDrawn.m_Color = Vec4(0.8, 0.3, 0.2, 1);
	windowedPoly.m_Color = Vec4(0.2, 0.8, 0.3, 1);
}

void ToolsHandler::Destroy()
{
}

bool ToolsHandler::OnUpdate(float deltaTime)
{
	if(!mouseMoving && m_Tool == Tools::MOVE && !m_App->GetMouseDown(MouseButton::Left))
	{    // Camera Movement
		Vec2 movement(0.0f);
		float frameMovement = m_CameraSpeed * deltaTime * m_App->GetCamera().GetSize();

		if (m_App->GetKeyDown('z')) // up
		{
			movement += Vec2(0, +frameMovement);
		}

		if (m_App->GetKeyDown('s')) // down
		{
			movement += Vec2(0, -frameMovement);
		}

		if (m_App->GetKeyDown('d')) // right
		{
			movement += Vec2(+frameMovement, 0);
		}

		if (m_App->GetKeyDown('q')) // down
		{
			movement += Vec2(-frameMovement, 0);
		}

		if (movement != Vec2(0.0)) {
			m_App->GetCamera().MovePosition(movement);
			return true;
		}
	}
	else if(m_Tool == Tools::DRAW_POLYGONE && drawingPoly && m_App->GetKeyDown(27))
	{
		StopDrawingPoly();
		return true;
	}
	else if(m_Tool == Tools::DRAW_WINDOW && drawingWindow && m_App->GetKeyDown(27))
	{
		StopDrawingWindow();
		return true;
	}
	return false;
}

void ToolsHandler::Draw(const Mat4 &viewProjMatrix) {
//	drawObj.Draw(viewProjMatrix);
//	quad.Draw(viewProjMatrix);
    if(color==0) {
        polyDrawn.m_Color = Vec4(0.2, 0.3, 0.8, 1);
    }
    else if(color==1) {
        polyDrawn.m_Color = Vec4(0.2f, 0.8f, 0.3f, 1.0f);
    }
    else if(color==2){
        polyDrawn.m_Color= Vec4(0.8f, 0.3f, 0.2f, 1.0f);
    }
	polyDrawn.Draw(viewProjMatrix);
	windowDrawn.Draw(viewProjMatrix);
	windowedPoly.Draw(viewProjMatrix);
}

static Vec2Int MousePosePressDraw;
bool ToolsHandler::OnMouseClick(MouseButton mouse, const MouseState& state)
{
	switch (m_Tool)
	{
		case Tools::MOVE:
		{
			if(mouse== MouseButton::Left)
			{
				mouseMoving = true;
				mousePosPressed = state.positionPressed;
				camPosePressed = m_App->GetCamera().GetPosition();
			}
			else if(mouse == MouseButton::Right)
			{
				mouseMoving = false;
			}
		}
		break;
		case Tools::DRAW_POLYGONE:
		{
			if (mouse == MouseButton::Left) {
				AddPointToPoly(state.positionPressed);
				return true;
			} else {
				StopDrawingPoly();
				return true;
			}
		}
		break;
		case Tools::DRAW_WINDOW:
		{
			if (mouse == MouseButton::Left) {
				AddPointToWindow(state.positionPressed);
				return true;
			} else {
				StopDrawingWindow();
				return true;
			}
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
		break;
		case Tools::WINDOWING:
		{
			if(polyDrawn.GetPointCount() > 2 && windowDrawn.GetPointCount() > 2)
			{
				auto points = Math::sutherlandHodgman(polyDrawn.GetLoopPoints(), windowDrawn.GetLoopPoints());
				windowedPoly.m_Points.clear();
				for(const auto& p : points)
				{
					windowedPoly.m_Points.emplace_back(p.x, p.y);
				}
			}
		}
		break;
	}
	return false;
}

bool ToolsHandler::OnMouseRelease(MouseButton mouse, const MouseState& state)
{
	switch (m_Tool)
	{
		case Tools::MOVE:
		{
			if(mouse== MouseButton::Left)
			{
				mouseMoving = false;
				auto mousePosReleased = state.positionReleased;

				auto gamePosPressed = m_App->ScreenToWorldPos(mousePosPressed);
				auto gamePosReleased = m_App->ScreenToWorldPos(mousePosReleased);
				auto movement = gamePosPressed - gamePosReleased;
				if(movement != Vec2(0,0)) {
					m_App->GetCamera().SetPosition(camPosePressed + movement);
					return true;
				}
			}
		}
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
		case Tools::MOVE:
		{
			if(mouseMoving)
			{
				auto gamePosPressed = m_App->ScreenToWorldPos(mousePosPressed);
				auto gamePos = m_App->ScreenToWorldPos(mousePos);

				auto movement = gamePosPressed - gamePos;
				if(movement != Vec2(0,0)) {
					m_App->GetCamera().SetPosition(camPosePressed + movement);
					return true;
				}
			}
		}
		case Tools::DRAW_POLYGONE:
		{
			if(!drawingPoly) break;
			polyDrawn.m_Points[polyDrawn.GetPointCount() - 1] = m_App->GetWorldMousePos();
			return true;
		}
		case Tools::DRAW_WINDOW:
		{
			if(!drawingWindow) break;
			windowDrawn.m_Points[windowDrawn.GetPointCount() - 1] = m_App->GetWorldMousePos();
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
	polyDrawn.m_Points.emplace_back(m_App->GetWorldMousePos());
	drawingPoly = true;
}

void ToolsHandler::StopDrawingPoly()
{
	if(drawingPoly && polyDrawn.GetPointCount() > 3) {
		polyDrawn.m_Points.pop_back();
		drawingPoly = false;
	}
}

void ToolsHandler::AddPointToPoly(Vec2Int screenPos)
{
	if(drawingPoly)
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
				polyDrawn.m_Points.emplace_back(worldPos);
			}
		}
		else
		{
			polyDrawn.m_Points.emplace_back(worldPos);
		}
	}
	else
	{
		ClearDrawnPoly();
	}
}

// ===================== DRAW WINDOW =====================
void ToolsHandler::ClearDrawnWindow()
{
	windowDrawn.m_Points.clear();
	windowDrawn.m_Points.emplace_back(m_App->GetWorldMousePos());
	drawingWindow = true;
}

void ToolsHandler::StopDrawingWindow()
{
	if(drawingWindow && windowDrawn.GetPointCount() > 3) {
		windowDrawn.m_Points.pop_back();
		drawingWindow = false;
	}
}

void ToolsHandler::AddPointToWindow(Vec2Int screenPos)
{
	if(drawingWindow)
	{
		auto worldPos = m_App->ScreenToWorldPos(screenPos);
		Vec2Int firstWorldPos = windowDrawn.m_Points[0];
		float distanceToLast = Math::Distance<Vec2>(firstWorldPos, worldPos);
		REI_INFO("Distance To First {0}", distanceToLast);
		if(distanceToLast < 10) // Same place
		{
			if(windowDrawn.GetPointCount() > 3)
			{
				StopDrawingPoly();
			}
			else if(windowDrawn.GetPointCount() == 1)
			{
				windowDrawn.m_Points.emplace_back(worldPos);
			}
		}
		else
		{
			windowDrawn.m_Points.emplace_back(worldPos);
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



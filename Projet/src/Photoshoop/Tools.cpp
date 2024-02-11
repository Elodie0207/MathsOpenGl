//
// Created by ianpo on 29/12/2023.
//

#include "Photoshoop/Tools.hpp"
#include "Application.hpp"
#include "Parameters.hpp"

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
	m_Polygons.emplace_back(Poly({{128, 128}, {128, 640}, {640, 640}}), false);
	m_Polygons.emplace_back(Poly({{0, 0}, {0, 512}, {512, 512}, {512, 0}}), true);

	polyDrawn = {};
	windowDrawn = {};

	//	drawObj = DrawObject({{0,0}, { 0, 512}, { 512, 512}, { 512, 0}});
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

		if (m_App->GetKeyDown(Key::Z)) // up
		{
			movement += Vec2(0, +frameMovement);
		}

		if (m_App->GetKeyDown(Key::S)) // down
		{
			movement += Vec2(0, -frameMovement);
		}

		if (m_App->GetKeyDown(Key::D)) // right
		{
			movement += Vec2(+frameMovement, 0);
		}

		if (m_App->GetKeyDown(Key::Q)) // down
		{
			movement += Vec2(-frameMovement, 0);
		}

		if (movement != Vec2(0.0)) {
			m_App->GetCamera().MovePosition(movement);
			return true;
		}
	}
	else if(m_Tool == Tools::DRAW_POLYGONE && drawingPoly && m_App->GetKeyDown(Key::Escape))
	{
		StopDrawingPoly();
		return true;
	}
	else if(m_Tool == Tools::DRAW_WINDOW && drawingWindow && m_App->GetKeyDown(Key::Escape))
	{
		StopDrawingWindow();
		return true;
	}
	else if(m_Tool == Tools::AREA_FILLING)
	{
		m_BorderPoly.m_Points.clear();
		auto mousePos = m_App->GetWorldMousePos();
		auto halfFillSize = m_FillSize * 0.5f;
		m_BorderPoly.m_Color = m_FillColor;
		m_BorderPoly.m_Color.a *= 0.25;
		m_BorderPoly.m_Points.emplace_back(mousePos.x - halfFillSize.x, mousePos.y - halfFillSize.y);
		m_BorderPoly.m_Points.emplace_back(mousePos.x + halfFillSize.x, mousePos.y - halfFillSize.y);
		m_BorderPoly.m_Points.emplace_back(mousePos.x + halfFillSize.x, mousePos.y + halfFillSize.y);
		m_BorderPoly.m_Points.emplace_back(mousePos.x - halfFillSize.x, mousePos.y + halfFillSize.y);
	}
	return false;
}

void ToolsHandler::Draw(const Mat4 &viewProjMatrix) {

	for (const auto& render : m_Polygons)
	{
		render.Polygon.Draw(viewProjMatrix);
	}
	if(m_Tool == Tools::DRAW_POLYGONE) polyDrawn.Draw(viewProjMatrix);
	if(m_Tool == Tools::DRAW_WINDOW) windowDrawn.Draw(viewProjMatrix);
	if(m_Tool == Tools::AREA_FILLING) m_BorderPoly.Draw(viewProjMatrix);
}

static Vec2 MousePosePressDraw;

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
//		case Tools::SWEEP_FILLING:
//		{
//			if(mouse == MouseButton::Left) {
//				MousePosePressDraw = state.positionPressed;
//				Math::polygon_fill(polyDrawn.GetPoints(), *m_App, Parameters::GetColor());
//
//				// TODO: get from the window the min and max (i.e. bounding box)
//				// TODO: launch using the 'MousePosePressDraw' (x/y) and min max with the 'Math::fillRegionConnexity4'.
//			}
//		}
//		break;
		case Tools::AREA_FILLING:
		{
			if(mouse == MouseButton::Left) {
				MousePosePressDraw = m_App->ScreenToWorldPos(state.positionPressed);
				auto halfSize = m_FillSize * 0.5f;
				auto min = MousePosePressDraw - halfSize;
				auto max = MousePosePressDraw + halfSize;
				Math::fillRegionConnexity4(MousePosePressDraw.x, MousePosePressDraw.y, min, max, *m_App, m_BorderColor, m_FillColor);

				// TODO: get from the window the min and max (i.e. bounding box)
				// TODO: launch using the 'MousePosePressDraw' (x/y) and min max with the 'Math::fillRegionConnexity4'.
			}
		}
		break;
//        case Tools::WINDOWING: {
//            if (polyDrawn.GetPointCount() > 2 && windowDrawn.GetPointCount() > 2) {
//                auto poly = polyDrawn.GetPoints();
//                auto window = windowDrawn.GetLoopPoints();
//                auto clippedPoints = Math::sutherlandHodgman(poly, window, *m_App);
//
//                windowedPoly.m_Points.clear();
//
//                std::cout << "Original Polygon Points:" << std::endl;
//                for (const auto &p: poly) {
//                    std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
//                }
//
//                std::cout << "Window Points:" << std::endl;
//                for (const auto &p: window) {
//                    std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
//                }
//
//                std::cout << "Intermediate Clipped Polygon Points:" << std::endl;
//                for (const auto &p: clippedPoints) {
//                    std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
//                }
//
//                std::cout << "Clipped Polygon Points:" << std::endl;
//                if (!clippedPoints.empty()) {
//                    for (const auto &p: clippedPoints) {
//                        windowedPoly.m_Points.emplace_back(p.x, p.y);
//                        std::cout << "\033[32m(" << p.x << ", " << p.y << ")\033[0m" << std::endl;
//                    }
//                } else {
//                    std::cout << "No clipped points (Polygon outside the window)" << std::endl;
//                }
//            }
//        }
//            break;
		case Tools::DRAWING:
		{
			if(mouse == MouseButton::Left)
			{
				m_IsDrawing = true;
				m_LastDrawingPos = m_App->ScreenToWorldPos(state.positionPressed);
				DrawWorldPos(m_LastDrawingPos, m_DrawingSize);
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
			if(mouse== MouseButton::Left && mouseMoving)
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
		break;
		case Tools::DRAWING:
		{
			if(mouse== MouseButton::Left)
			{
				m_IsDrawing = false;
			}
		}
			break;
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
		break;
		case Tools::DRAW_POLYGONE:
		{
			if(!drawingPoly) break;
			polyDrawn.m_Points[polyDrawn.GetPointCount() - 1] = m_App->GetWorldMousePos();
			return true;
		}
			break;
		case Tools::DRAW_WINDOW:
		{
			if(!drawingWindow) break;
			windowDrawn.m_Points[windowDrawn.GetPointCount() - 1] = m_App->GetWorldMousePos();
			return true;
		}
			break;
		case Tools::DRAWING:
		{
			if(m_IsDrawing) {
				// Bresenham's line algorithm
				Vec2Int target = m_App->ScreenToWorldPos(mousePos);
				int dx = std::abs(target.x - m_LastDrawingPos.x);
				int dy = std::abs(target.y - m_LastDrawingPos.y);

				int sx = (m_LastDrawingPos.x < target.x) ? 1 : -1;
				int sy = (m_LastDrawingPos.y < target.y) ? 1 : -1;

				int err = dx - dy;

				do
				{
					DrawWorldPos({m_LastDrawingPos.x, m_LastDrawingPos.y}, m_DrawingSize);

					int e2 = 2 * err;
					if (e2 > -dy)
					{
						err -= dy;
						m_LastDrawingPos.x += sx;
					}
					if (e2 < dx)
					{
						err += dx;
						m_LastDrawingPos.y += sy;
					}
				}
				while(m_LastDrawingPos.x != target.x && m_LastDrawingPos.y != target.y);
			}
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
		m_Polygons.push_back({polyDrawn, false});
		polyDrawn.m_Points.clear();
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
		m_Polygons.push_back({windowDrawn, true});
		windowDrawn.m_Points.clear();
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

void ToolsHandler::OnImGui()
{
	const std::vector<std::string> tools = { "NONE",
											 "MOVE",
											 "DRAW_POLYGONE",
											 "DRAW_WINDOW",
											 "WINDOWING",
											 "SWEEP_FILLING",
											 "AREA_FILLING",
											 "DRAWING",
	};
	ImGui::Begin("Tools Handler");
	{
		{
			auto tool = m_Tool;
			auto toolIndex = (int) tool;
			const std::string &currentTargetImage = tools[toolIndex];
			if (ImGui::BeginCombo("Tool", currentTargetImage.c_str())) {
				for (int i = 0; i < tools.size(); i++) {
					const bool is_selected = (toolIndex == i);
					const std::string &iImage = tools[i];
					if (ImGui::Selectable(iImage.c_str(), is_selected)) {
						toolIndex = i;
						SetTool((Tools) i);
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected) ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}
		{
			if (m_Tool == Tools::DRAW_POLYGONE) ImGui::ColorEdit4("Poly Drawn", &polyDrawn.m_Color.x);
			if (m_Tool == Tools::DRAW_WINDOW) ImGui::ColorEdit4("Window Drawn", &windowDrawn.m_Color.x);
//		ImGui::ColorEdit4("Windowed Poly", &windowedPoly.m_Color.x);

			ImGui::Spacing();
			if (m_Tool == Tools::DRAWING) {
				ImGui::ColorEdit4("Drawing Color", glm::value_ptr(m_BorderColor));
				ImGui::DragInt("Drawing Size", &m_DrawingSize, 1, 1, INT_MAX);
			}

			if(m_Tool == Tools::AREA_FILLING)
			{
				ImGui::ColorEdit4("Border Color", glm::value_ptr(m_BorderColor));
				ImGui::ColorEdit4("Fill Color", glm::value_ptr(m_FillColor));
				ImGui::DragFloat2("Fill Size", glm::value_ptr(m_FillSize), 1);
			}
		}
		ImGui::End();
	}

	ImGui::Begin("Polygon");
	{
		for (size_t polygonIndex = 0; polygonIndex < m_Polygons.size(); ++polygonIndex)
		{

#define poly m_Polygons[polygonIndex]

			std::string imguiId = std::to_string(poly.Id);
			ImGui::PushID(imguiId.c_str());
			{
				std::string name = poly.IsWindow ? "Window_" : "Polygon_";
				name += std::to_string(polygonIndex) + "##" + imguiId;
				if (ImGui::CollapsingHeader(name.c_str())) {

					ImGui::Checkbox("Is Window", &poly.IsWindow);
					ImGui::ColorEdit4("Color", glm::value_ptr(poly.Polygon.m_Color));

					if (ImGui::Button("Clear Points"))
					{
						poly.Polygon.m_Points.clear();
					}


					if (ImGui::CollapsingHeader("Points")) {
						for (int64_t i = poly.Polygon.m_Points.size() - 1; i >= 0; --i) {
							std::string pointName = "Point_"+Math::ToString(i);
							ImGui::PushID(pointName.c_str());
							{
								if (ImGui::Button("Erase")) {
									poly.Polygon.m_Points.erase(poly.Polygon.m_Points.begin() + i);
								} else {
									ImGui::SameLine();
									ImGui::DragInt2("Coordinates", glm::value_ptr(poly.Polygon.m_Points[i]));
								}
							}
							ImGui::PopID();
						}
					}

					if (ImGui::Button("LCA Fill"))
					{
						Math::polygon_fill(poly.Polygon.GetPoints(), *m_App, poly.Polygon.m_Color);
					}

					if(poly.IsWindow && ImGui::Button("Windowing"))
					{
						auto aabb = poly.Polygon.GetBoundBox();
						std::vector<RenderObject> toAdd;
						for (const auto& polToWindow : m_Polygons)
						{
							if (!polToWindow.IsWindow && &polToWindow != &poly)
							{
								auto otherAabb = polToWindow.Polygon.GetBoundBox();
								if(!aabb.Intersects(otherAabb))
								{
									// only slice the intersecting polygons to reduce interferences.
									continue;
								}

								if (polToWindow.Polygon.GetPointCount() > 2 && poly.Polygon.GetPointCount() > 2) {
									auto polyPoints = polToWindow.Polygon.GetPoints();
									auto windowPoints = poly.Polygon.GetLoopPoints();
									auto clippedPoints = Math::sutherlandHodgman(polyPoints, windowPoints, *m_App);

									toAdd.emplace_back();

									std::cout << "Original Polygon Points:" << std::endl;
									for (const auto &p: polyPoints) {
										std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
									}

									std::cout << "Window Points:" << std::endl;
									for (const auto &p: windowPoints) {
										std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
									}

									std::cout << "Intermediate Clipped Polygon Points:" << std::endl;
									for (const auto &p: clippedPoints) {
										std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
									}

									std::cout << "Clipped Polygon Points:" << std::endl;
									if (!clippedPoints.empty()) {
										for (const auto &p: clippedPoints) {
											toAdd.back().Polygon.m_Points.emplace_back(p.x, p.y);
											std::cout << Math::ToString(p) << std::endl;
										}
									} else {
										std::cout << "No clipped points (Polygon outside the window)" << std::endl;
									}
								}
							}
						}
						m_Polygons.insert(m_Polygons.end(), toAdd.begin(), toAdd.end());
					}

					if(ImGui::Button("Delete"))
					{
						m_Polygons.erase(m_Polygons.begin() + polygonIndex);
					}
				}
			}
			ImGui::PopID();
		}
		ImGui::End();
	}
}

void ToolsHandler::DrawWorldPos(Vec2Int pos, int size)
{
	float hSize = size / 2.0f;
	Vec2Int min = pos - (int)hSize;
	Vec2Int max = pos + (int)hSize;

	for (int x = min.x; x <= max.x; ++x) {
		for (int y = min.y; y <= max.y; ++y) {
			Vec2Int currentPx = {x,y};
			auto dst = Math::Distance(Vec2(pos), Vec2(currentPx));
			if(dst < std::max(hSize, 1.0f)) {
				m_App->WriteWorldPixel(currentPx, m_BorderColor);
			}
		}
	}
}






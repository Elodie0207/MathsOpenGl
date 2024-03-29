//
// Created by ianpo on 29/12/2023.
//

#include "Photoshoop/Tools.hpp"
#include "Application.hpp"
#include "Parameters.hpp"

int current_item = 0; // Start at 'Heart'
Vec2 ToolsHandler::s_MousePosePressDraw = {0, 0};
float ToolsHandler::s_CircleFillingRadius = 100;

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
		case Tools::AREA_FILLING:
			if(m_AreaFillingType == AREA_FILLING_CUSTOM)
			{
				m_Circle.Center = m_App->GetWorldMousePos();
				m_Circle.Radius = s_CircleFillingRadius;
				m_Circle.Color = m_FillColor;
			}
			break;
	}
}

ToolsHandler::ToolsHandler(Application * app) : m_App(app), m_Circle(s_MousePosePressDraw, s_CircleFillingRadius, m_FillColor)
{
	REI_ASSERT(m_App != nullptr, "The application is not valid.");
	m_Polygons.emplace_back(Poly(std::vector<Vec2>{{128, 128}, {128, 640}, {640, 640}}), false);
	m_Polygons.emplace_back(Poly(std::vector<Vec2>{{0, 0}, {0, 512}, {512, 512}, {512, 0}}), true);

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
	if(m_Tool == Tools::AREA_FILLING) {
		if (m_AreaFillingType == AREA_FILLING_CUSTOM) m_Circle.Draw(viewProjMatrix);
		else m_BorderPoly.Draw(viewProjMatrix);
	}
}

bool ToolsHandler::CheckCircle(int x, int y)
{
    // Calcul de la distance au carré entre le point (x, y) et le centre du cercle (s_MousePosePressDraw)
    float dx = x - s_MousePosePressDraw.x;
    float dy = y - s_MousePosePressDraw.y;
    float distanceSquared = dx * dx + dy * dy;

    // Vérification si la distance au carré est inférieure au carré du rayon
    return distanceSquared < (s_CircleFillingRadius * s_CircleFillingRadius);
}


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
		case Tools::AREA_FILLING:
		{
			if(mouse == MouseButton::Left) {
				s_MousePosePressDraw = m_App->ScreenToWorldPos(state.positionPressed);
				auto halfSize = m_FillSize * 0.5f;
				auto min = s_MousePosePressDraw - halfSize;
				auto max = s_MousePosePressDraw + halfSize;

				switch (m_AreaFillingType) {

					case AREA_FILLING_RECURSE:
						Math::fillRegionConnexity4Recursive(s_MousePosePressDraw.x, s_MousePosePressDraw.y, min, max, *m_App, m_BorderColor, m_FillColor);
						break;
					case AREA_FILLING_STACK:
						Math::fillRegionConnexity4(s_MousePosePressDraw.x, s_MousePosePressDraw.y, min, max, *m_App, m_BorderColor, m_FillColor);
						break;
					case AREA_FILLING_LINE_BY_LINE:
						Math::fillRegionLineByLine(s_MousePosePressDraw.x, s_MousePosePressDraw.y, min, max, *m_App, m_BorderColor, m_FillColor);
						break;
					case AREA_FILLING_CUSTOM:
						Math::fillRegion(s_MousePosePressDraw.x, s_MousePosePressDraw.y, &CheckCircle, *m_App, m_BorderColor, m_FillColor);
						break;
				}
                //pour la recusive
                //pour le ligne par leigne

				// TODO: get from the window the min and max (i.e. bounding box)
				// TODO: launch using the 's_MousePosePressDraw' (x/y) and min max with the 'Math::fillRegionConnexity4'.
			}
		}
		break;
		case Tools::DRAWING:
		{
			if(mouse == MouseButton::Left)
			{
				m_IsDrawing = true;
				m_LastDrawingPos = m_App->ScreenToWorldPos(state.positionPressed);
				m_App->WriteWorldPixel(m_LastDrawingPos, m_BorderColor, m_DrawingSize);
			}
		}
			break;
		case Tools::FORME:
		{
			if(mouse == MouseButton::Left)
			{
				auto pos = m_App->ScreenToWorldPos(state.positionPressed);
				switch (m_Forme) {
					case FORME_HEART:
					{
						auto poly = Figures::CreateHeart(heartSize);
						poly.Move(pos);
						m_Polygons.emplace_back(poly);
					}
						break;
					case FORME_STAR:
					{
						auto poly = Figures::CreateStar(armLength, numArms);
						poly.Move(pos);
						m_Polygons.emplace_back(poly);
					}
						break;
					case FORME_POLYGONE:
					{
						auto poly = Figures::CreatePolygone(armSize, numSides);
						poly.Move(pos);
						m_Polygons.emplace_back(poly);
					}
						break;
					case FORME_ELLIPSE:
					{
						auto poly = Figures::CreateEllipse(numSegments, radiusX, radiusY);
						poly.Move(pos);
						m_Polygons.emplace_back(poly);
					}
						break;
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
				auto target = m_App->ScreenToWorldPos(mousePos);
				m_App->WriteWorldLine(m_LastDrawingPos, target, m_BorderColor, m_DrawingSize);
				m_LastDrawingPos = target;
			}
		}
		break;
		case Tools::AREA_FILLING:
		{
			if(m_AreaFillingType == AREA_FILLING_CUSTOM)
			{
				m_Circle.Center = m_App->ScreenToWorldPos(mousePos);
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
/*void ToolsHandler::AddHeartAtPosition(Vec2Int screenPos) {
    Vec3 worldPos = S(screenPos);
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glPopMatrix();
}*/
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
											 "AREA_FILLING",
											 "DRAWING",
											 "FORME",

	};

	std::vector<std::string> sweep_filling_type_names = {
			"AREA_FILLING_RECURSE",
			"AREA_FILLING_STACK",
			"SWEEP_FILLING_LINE_BY_LINE",
			"AREA_FILLING_CUSTOM"
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
			//m_Circle
			switch (m_Tool) {
				case Tools::MOVE:
					break;
				case Tools::DRAW_POLYGONE:
					ImGui::ColorEdit4("Poly Drawn", &polyDrawn.m_Color.x);
					break;
				case Tools::DRAW_WINDOW:
					ImGui::ColorEdit4("Window Drawn", &windowDrawn.m_Color.x);
					break;
				case Tools::WINDOWING:
					break;
				case Tools::AREA_FILLING:
					ImGui::ColorEdit4("Border Color", glm::value_ptr(m_BorderColor));
					if(ImGui::ColorEdit4("Fill Color", glm::value_ptr(m_FillColor)))
					{
						m_Circle.Color = m_FillColor;
					}
					if(m_AreaFillingType == AREA_FILLING_TYPE::AREA_FILLING_CUSTOM)
					{
						if (ImGui::DragFloat("Circle Radius", &s_CircleFillingRadius)) {
							m_Circle.Radius = s_CircleFillingRadius;
						}
					}
					else
					{
						ImGui::DragFloat2("Fill Size", glm::value_ptr(m_FillSize), 1);
					}

					{
						std::string combo_preview_value = sweep_filling_type_names[m_AreaFillingType];
						if (ImGui::BeginCombo("Sweep Filling Type",
											  combo_preview_value.c_str())) // The second parameter is the label previewed before opening the combo.
						{
							for (int n = 0; n < sweep_filling_type_names.size(); n++) {
								bool is_selected = (m_AreaFillingType == n);
								if (ImGui::Selectable(sweep_filling_type_names[n].c_str(), is_selected)) {
									m_AreaFillingType = (AREA_FILLING_TYPE) n;
									if (m_AreaFillingType == AREA_FILLING_CUSTOM) {
										m_Circle.Center = m_App->GetWorldMousePos();
										m_Circle.Radius = s_CircleFillingRadius;
										m_Circle.Color = m_FillColor;
									}
								}

								if (is_selected) {
									ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
								}
							}
							ImGui::EndCombo();
						}

						if (m_AreaFillingType == AREA_FILLING_TYPE::AREA_FILLING_CUSTOM) {

						}
					}
					break;
				case Tools::DRAWING:
					ImGui::ColorEdit4("Drawing Color", glm::value_ptr(m_BorderColor));
					ImGui::DragInt("Drawing Size", &m_DrawingSize, 1, 1, INT_MAX);
					break;
			}

			if(m_Tool == Tools::FORME)
			{
				const char* items[] = { "Heart", "Star", "Polygone", "Ellipse" };
				ImGui::Combo("Formes", reinterpret_cast<int*>(&m_Forme), items, IM_ARRAYSIZE(items));

				switch (m_Forme)
				{
					case FORME_HEART:
					{
						ImGui::DragFloat("Heart Size",&heartSize);
					}
						break;
					case FORME_STAR:
					{
						ImGui::DragFloat("Arm Length",&armLength);
						ImGui::DragInt("Num Arms",&numArms);
					}
						break;
					case FORME_POLYGONE:
					{
						ImGui::DragFloat("Arm Size",&armSize);
						ImGui::DragInt("Num Sides",&numSides);
					}
						break;
					case FORME_ELLIPSE:
					{
						ImGui::DragInt("Num Segments",&numSegments);
						ImGui::DragFloat("Radius X",&radiusX);
						ImGui::DragFloat("Radius Y",&radiusY);
					}
						break;
				}
			}
		}
		ImGui::End();
	}

	ImGui::Begin("Polygon");
	{
		for (size_t polygonIndex = 0; polygonIndex < m_Polygons.size(); ++polygonIndex)
		{

			auto& poly = m_Polygons[polygonIndex];

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
									ImGui::DragFloat2("Coordinates", glm::value_ptr(poly.Polygon.m_Points[i]));
								}
							}
							ImGui::PopID();
						}
					}

					if (ImGui::Button("LCA Fill"))
					{
						Math::polygon_fill(poly.Polygon.GetPoints(), *m_App, poly.Polygon.m_Color);
					}

					if(ImGui::Button("Write Polygon To Texture"))
					{
						auto pts = poly.Polygon.GetLoopPoints();
						for (uint64_t i = 1; i < pts.size(); ++i)
						{
							const auto& pt1 = pts[i - 1];
							const auto& pt2 = pts[i - 0];

							m_App->WriteWorldLine(pt1, pt2, poly.Polygon.m_Color, 3);
						}
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



void ToolsHandler::Clear()
{
	m_Polygons.clear();
}





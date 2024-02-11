//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include "Core.hpp"

#include "Renderer/DrawObject.hpp"
#include "Renderer/Poly.hpp"
#include "Renderer/Quad.hpp"

class Application;

struct RenderObject
{
public:
	RenderObject() = default;
	~RenderObject() = default;
	inline RenderObject(Poly polygon, bool isWindow = false) : Polygon(polygon), IsWindow(isWindow), Id() {}

	Poly Polygon = Poly();
	UUID Id = UUID();
	bool IsWindow = false;
};

enum class Tools
{
	NONE = 0,
	MOVE,
	DRAW_POLYGONE,
	DRAW_WINDOW,
	WINDOWING,
	SWEEP_FILLING,
	AREA_FILLING,
	DRAWING,
};

class ToolsHandler {
public:

    ToolsHandler(Application *);

	~ToolsHandler();

	inline Tools GetSelectedTool() const { return m_Tool; }

	/**
	 * Function called each time the tools changes.
	 * The tools may be the same as the previous one.
	 * @param tool
	 */
	void SetTool(Tools tool);

	/**
	 * Function called once when the application is initializing.
	 */
	void Initialize();

	/**
	 * Function called once when the application is quitting.
	 */
	void Destroy();

	/**
	 * Function called each time a redraw is issue.
	 * @param ViewProjMatrix The View Projection Matrix of the camera.
	 */
	void Draw(const Mat4 &ViewProjMatrix);

	/**
	 * Function called every frames (~16ms)
	 * @param deltaTime
	 * @return Whether we should render this frame.
	 */
	bool OnUpdate(float deltaTime);

	/**
	 * Function called when the mouse has clicked on the window.
	 * @param mouse The button that clicked
	 * @param state The state of the pressed mouse button.
	 * @return Whether we should rerender.
	 */
	bool OnMouseClick(MouseButton mouse, const MouseState &state);

	/**
	 * Function called when the mouse has clicked on the window.
	 * @param mouse The button that clicked
	 * @param state The state of the released mouse button.
	 * @return Whether we should rerender.
	 */
	bool OnMouseRelease(MouseButton mouse, const MouseState &state);

	/**
	 * Function called each time the mouse move.
	 * @param mousePos the new position of the mouse.
	 * @return Whether we should rerender.
	 */
	bool OnMouseMove(Vec2Int mousePos);

//    /**
//	 * Function called each time the user wants to change the color.
//	 */
//    void CHANGE_COLOR(Tools color);

	/**
	 * Called when the ImGui interface is being rendered.
	 *
	 */
	void OnImGui();
private:
	/// === MOVEMENT ===

	bool mouseMoving = false;
	Vec2Int mousePosPressed = {0,0};
	Vec2 camPosePressed = {0,0};

	/// === DRAW POLY ===
	void ClearDrawnPoly();
	void AddPointToPoly(Vec2Int screenPos);
	void StopDrawingPoly();
	Poly polyDrawn;
	bool drawingPoly = false;

	/// === DRAW WINDOW ===
	void ClearDrawnWindow();
	void AddPointToWindow(Vec2Int screenPos);
	void StopDrawingWindow();
	Poly windowDrawn;
	bool drawingWindow = false;

	//TODO: Add function & variables to draw the window here.

	/// === WINDOWING ===

	//TODO: Add function & variables to make the windowing here.

	/// === SWEEP_FILLING ===

	//TODO: Add function & variables to make the filling here.

	/// === AREA_FILLING ===

	Color m_FillColor = {.8, .3, .2, 1.};
	Vec2 m_FillSize = {1000, 1000};
	Poly m_BorderPoly;

	//TODO: Add function & variables to make the filling here.
	/// === DRAWING ===
	Color m_BorderColor = Color(0);
	int m_DrawingSize = 1;
	bool m_IsDrawing = false;
	Vec2Int m_LastDrawingPos;

	void DrawWorldPos(Vec2Int pos, int size);

private:
	std::vector<RenderObject> m_Polygons;

	Tools m_Tool = Tools::MOVE;
	Application* m_App;

//	DrawObject drawObj;
//	Quad quad;
	float m_CameraSpeed = 0.6f;
//    int color=0;


};
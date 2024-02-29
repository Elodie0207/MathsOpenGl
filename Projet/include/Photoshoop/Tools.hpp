//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include "Core.hpp"

#include "Renderer/DrawObject.hpp"
#include "Renderer/Poly.hpp"
#include "Renderer/Quad.hpp"
#include "Renderer/Figures.h"
#include "Renderer/Circle.hpp"

class Application;

enum FORME_ENUM {
	FORME_HEART,
	FORME_STAR,
	FORME_POLYGONE,
	FORME_ELLIPSE,
};

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
	AREA_FILLING,
	DRAWING,
	FORME,
};

enum AREA_FILLING_TYPE
{
	AREA_FILLING_RECURSE,
	AREA_FILLING_STACK,
	AREA_FILLING_LINE_BY_LINE,
	AREA_FILLING_CUSTOM,
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
    static void AddHeartAtPosition(Vec2Int screenPos);
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

	void Clear();
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

	Circle m_Circle;
	static Vec2 s_MousePosePressDraw;// = {0,0};
	static float s_CircleFillingRadius;// = 100;
	static bool CheckCircle(int x, int y);

	//TODO: Add function & variables to make the filling here.
	/// === DRAWING ===
	Color m_BorderColor = Color(0);
	int m_DrawingSize = 1;
	bool m_IsDrawing = false;
	Vec2Int m_LastDrawingPos;

private:
	std::vector<RenderObject> m_Polygons;

	Tools m_Tool = Tools::MOVE;
	AREA_FILLING_TYPE m_AreaFillingType = AREA_FILLING_LINE_BY_LINE;
	Application* m_App;

//	DrawObject drawObj;
//	Quad quad;
	float m_CameraSpeed = 0.6f;
//    int color=0;

	FORME_ENUM m_Forme = FORME_HEART;
	float heartSize = 100.f;
	float armLength = 100.f; int numArms = 5;
	float armSize = 100; int numSides = 5;
	int numSegments = 100; float radiusX = 100.f; float radiusY = 5.f;

};
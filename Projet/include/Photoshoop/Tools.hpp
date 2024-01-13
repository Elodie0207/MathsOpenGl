//
// Created by ianpo on 29/12/2023.
//

#pragma once

#include "Core.hpp"

#include "Renderer/DrawObject.hpp"
#include "Renderer/Poly.hpp"
#include "Renderer/Quad.hpp"

class Application;

enum class Tools
{
	NONE,
	MOVE,
	CHANGE_COLOR,
	DRAW_POLYGONE,
	DRAW_WINDOW,
	WINDOWING,
	FILLING,
    RED,
    BLUE,
    GREEN
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
    /**
	 * Function called each time the user wants to change the color.
	 */
    void CHANGE_COLOR();
private:
	/// === MOVEMENT ===

	bool mouseMoving = false;
	Vec2Int mousePosPressed;
	Vec2 camPosePressed;

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

	Poly windowedPoly;

	//TODO: Add function & variables to make the windowing here.

	/// === FILLING ===

	//TODO: Add function & variables to make the filling here.

private:
	Tools m_Tool = Tools::MOVE;
	Application* m_App;

//	DrawObject drawObj;
//	Quad quad;
	float m_CameraSpeed = 0.6f;
    int color=0;


};
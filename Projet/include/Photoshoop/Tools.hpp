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
	 */
	void OnMouseClick(MouseButton mouse, const MouseState &state);

	/**
	 * Function called when the mouse has clicked on the window.
	 * @param mouse The button that clicked
	 * @param state The state of the released mouse button.
	 */
	void OnMouseRelease(MouseButton mouse, const MouseState &state);

	/**
	 * Function called each time the mouse move.
	 * @param mousePos the new position of the mouse.
	 */
	void OnMouseMove(Vec2Int mousePos);

private:
	Tools m_Tool = Tools::MOVE;
	Application *m_App;

	DrawObject drawObj;
	Poly poly;
	Quad quad;
	float m_CameraSpeed = 50.0f;
};
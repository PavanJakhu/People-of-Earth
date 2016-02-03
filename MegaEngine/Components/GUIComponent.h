// ***********************************************************************
// Author           : Pavan Jakhu
// Created          : 01-28-2016
//
// Last Modified By : Pavan Jakhu
// Last Modified On : 02-02-2016
// ***********************************************************************
// <copyright file="GUIComponent.h" company="Team MegaFox">
//     Copyright (c) Team MegaFox. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#pragma once
#include <CEGUI\CEGUI.h>
#include "../Core/CoreEngine.h"
#include "../Core/GameObject.h"
#include "../GUI/GUIEngine.h"

/// <summary>
/// A GUI Component class that attaches to GameObjects to have GUI funcationality.
/// </summary>
/// <example>
///   <code>
/// class MeshRenderer : public GUIComponent { ... }
/// </code>
/// </example>
class GUIComponent
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="GUIComponent"/> class.
	/// </summary>
	/// <param name="destRectPerc">The size of the widget relative the parent widget.</param>
	/// <param name="destRectPix">The size of the widget in pixels.</param>
	GUIComponent(const glm::vec4& destRectPerc, const glm::vec4& destRectPix) :
		m_parent(nullptr), m_widget(nullptr), m_destRectPerc(destRectPerc), m_destRectPix(destRectPix) { }

	/// <summary>
	/// Finalizes an instance of the <see cref="GUIComponent"/> class.
	/// </summary>
	virtual ~GUIComponent() { }


	/// <summary>
	/// Virtual function for custom input processing.
	/// </summary>
	/// <param name="input">The input manager.</param>
	/// <param name="delta">The frame time delta.</param>
	virtual void processInput(const InputManager& input, float delta) { }

	/// <summary>
	/// Virtual function for custom update functionality.
	/// </summary>
	/// <param name="delta">The frame time delta.</param>
	virtual void update(float delta) { }


	/// <summary>
	/// Adds to Core Engine.
	/// </summary>
	/// <param name="engine">The Core Engine.</param>
	virtual void addToEngine(CoreEngine* engine) { }


	/// <summary>
	/// Sets the parent GameObject.
	/// </summary>
	/// <param name="parent">The GameObject to be attached to.</param>
	virtual void setParent(GameObject* parent) { m_parent = parent; }

protected:

	/// <summary>
	/// Gets the core engine.
	/// </summary>
	/// <returns>CoreEngine *.</returns>
	CoreEngine* getCoreEngine() { return m_parent->getCoreEngine(); }


	/// <summary>
	/// Sets the widget.
	/// </summary>
	/// <param name="widget">The widget.</param>
	void setWidget(CEGUI::Window* widget) { m_widget = widget; }


	/// <summary>
	/// Gets the widget.
	/// </summary>
	/// <returns>CEGUI.Window *.</returns>
	CEGUI::Window* getWidget() { return m_widget; }


	/// <summary>
	/// Creates a widget.
	/// </summary>
	/// <param name="widgetType">Type of the widget.</param>
	/// <returns>CEGUI.Window *.</returns>
	CEGUI::Window* createWidget(const std::string& widgetType)
	{
		m_widget = m_parent->getCoreEngine()->getGUIEngine()->addWidget(widgetType, m_destRectPerc, m_destRectPix, m_parent->getName());
		return m_widget;
	}

private:
	/// <summary>
	/// The parent GameObject.
	/// </summary>
	GameObject* m_parent;
	
	/// <summary>
	/// The CEGUI widget.
	/// </summary>
	CEGUI::Window* m_widget;

	/// <summary>
	/// The m_dest rect perc
	/// </summary>
	glm::vec4 m_destRectPerc;

	/// <summary>
	/// The m_dest rect pix
	/// </summary>
	glm::vec4 m_destRectPix;

	/// <summary>
	/// Initializes a new instance of the <see cref="GameComponent" /> class.
	/// </summary>
	/// <param name="other">The other.</param>
	GUIComponent(const GUIComponent& other) {}

	/// <summary>
	/// Operator=s the specified other.
	/// </summary>
	/// <param name="other">The other.</param>
	void operator=(const GUIComponent& other) {}
};
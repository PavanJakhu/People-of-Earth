// ***********************************************************************
// Author           : Pavan Jakhu
// Created          : 02-03-2016
//
// Last Modified By : Jesse Derochie
// Last Modified On : 03-01-2016
// ***********************************************************************
// <copyright file="GUIContainer.h" company="Team MegaFox">
//     Copyright (c) Team MegaFox. All rights reserved.
// </copyright>
// <summary></summary>
// ***********************************************************************
#pragma once
#include "GUIComponent.h"

/// <summary>
/// The GUIContainer class is a GUI widget that has a background to it and
/// can have children that are only inside this widget.
/// </summary>
/// <seealso cref="GUIComponent" />
class GUIContainer : public GUIComponent
{
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="GUIContainer"/> class.
	/// </summary>
	/// <param name="destRectPerc">The dest rect perc.</param>
	/// <param name="destRectPix">The dest rect pix.</param>
	GUIContainer(const PxVec4& destRectPerc, const PxVec4& destRectPix, const std::string& title = "") :
		GUIComponent(destRectPerc, destRectPix), m_title(title) { }
	/// <summary>
	/// Finalizes an instance of the <see cref="GUIContainer"/> class.
	/// </summary>
	~GUIContainer() { }

	/// <summary>
	/// Adds to the Core Engine.
	/// </summary>
	/// <param name="engine">The Core Engine.</param>
	virtual void addToEngine(CoreEngine* engine)
	{
		GUIComponent::addToEngine(engine);
		CEGUI::FrameWindow* wi = static_cast<CEGUI::FrameWindow*>(createWidget(engine->getGUIEngine()->getSchemeStyle() + "/FrameWindow"));
		wi->setTitleBarEnabled(true);
		wi->getTitlebar()->setText(m_title);
		wi->getTitlebar()->disable();
		wi->setCloseButtonEnabled(false);
		wi->setSizingEnabled(false);
	}

private:
	std::string m_title;

};
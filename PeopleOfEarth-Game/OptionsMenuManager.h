#pragma once
#include <MegaEngine.h>
#include "TextLerpAlpha.h"

class OptionsMenuManager : public GameComponent
{
public:
	OptionsMenuManager(GameObject* effectsSlider = nullptr, GameObject* effectsBox = nullptr, 
		GameObject* bgmSlider = nullptr, GameObject* bgmBox = nullptr, GameObject* backButton = nullptr, GameObject* controlsButton = nullptr)
	{
		m_effectsSlider = effectsSlider->getGUIComponent<GUISlider>();
		m_effectsBox = effectsBox->getGUIComponent<GUILabel>();
		m_bgmSlider = bgmSlider->getGUIComponent<GUISlider>();
		m_bgmBox = bgmBox->getGUIComponent<GUILabel>();

		m_buttons.push_back(backButton->getGUIComponent<GUIButton>());
		m_buttons.push_back(controlsButton->getGUIComponent<GUIButton>());
		m_focusButton = 0;
		if (!m_usingMouse)
		{
			m_buttons[m_focusButton]->getParent()->addGameComponent(new TextLerpAlpha, true);
		}
	}
	~OptionsMenuManager() { }

	virtual void onStart() override
	{
		m_effectsSlider->setThumbPosition(getCoreEngine()->getAudioEngine()->getSoundVolume());
		m_effectsBox->setText(std::to_string((int)(m_effectsSlider->getThumbPosition() * 100.0f)));
		m_bgmSlider->setThumbPosition(getCoreEngine()->getAudioEngine()->getStreamVolume());
		m_bgmBox->setText(std::to_string((int)(m_bgmSlider->getThumbPosition() * 100.0f)));

		m_moveSound = getGameObjectByName("Moved button sound")->getGameComponent<AudioSource>();
		m_selectedSound = getGameObjectByName("Selected button sound")->getGameComponent<AudioSource>();
	}

	/// <summary>
	/// Virtual function for custom input processing.
	/// </summary>
	/// <param name="input">The input manager.</param>
	/// <param name="delta">The frame time delta.</param>
	virtual void processInput(const InputManager& input, float delta) override
	{
		if (!m_usingMouse && input.MouseButtonPress(SDL_BUTTON_LEFT))
		{
			m_usingMouse = true;
			m_buttons[m_focusButton]->getParent()->removeGameComponent(m_buttons[m_focusButton]->getParent()->getGameComponent<TextLerpAlpha>());
		}
		else if (m_usingMouse && (input.KeyPress(SDLK_a) || input.KeyPress(SDLK_d) || input.PadButtonPress(SDL_CONTROLLER_BUTTON_A) || input.PadButtonPress(SDL_CONTROLLER_BUTTON_DPAD_LEFT) || input.PadButtonPress(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) || input.GetThumbLPosition().x < -0.1f || input.GetThumbLPosition().x > 0.1f))
		{
			m_usingMouse = false;
			m_buttons[m_focusButton]->getParent()->addGameComponent(new TextLerpAlpha, true);
		}

		//Check if the player is using a controller
		if (!m_usingController && (input.PadButtonPress(SDL_CONTROLLER_BUTTON_A) || input.PadButtonPress(SDL_CONTROLLER_BUTTON_DPAD_UP) || input.PadButtonPress(SDL_CONTROLLER_BUTTON_DPAD_DOWN) || input.PadButtonPress(SDL_CONTROLLER_BUTTON_START) || input.ThumbLMoved()))
		{
			m_usingController = true;
		}
		else if (m_usingController && (input.KeyPress(SDLK_w) || input.KeyPress(SDLK_a) || input.KeyPress(SDLK_s) || input.KeyPress(SDLK_d) || input.KeyPress(SDLK_SPACE) || input.KeyPress(SDLK_RETURN)))
		{
			m_usingController = false;
		}

		//Dectecting if the player put the controller to rest
		if (m_usingController && input.PadButtonUp(SDL_CONTROLLER_BUTTON_DPAD_DOWN) && input.PadButtonUp(SDL_CONTROLLER_BUTTON_DPAD_UP) &&
			input.GetThumbLPosition().x < 0.1f && input.GetThumbLPosition().x > -0.1f && input.GetThumbLPosition().y < 0.1f && input.GetThumbLPosition().y > -0.1f)
		{
			m_ableToPlayMoveSound = true;
		}

		if (input.KeyPress(SDLK_a) || (input.GetThumbLPosition().x < -0.2f && m_ableToPlayMoveSound) || input.PadButtonPress(SDL_CONTROLLER_BUTTON_DPAD_LEFT))
		{
			if (m_focusButton > 0)
			{
				m_buttons[m_focusButton]->getParent()->removeGameComponent(m_buttons[m_focusButton]->getParent()->getGameComponent<TextLerpAlpha>());

				m_focusButton--;

				m_buttons[m_focusButton]->getParent()->addGameComponent(new TextLerpAlpha, true);

				m_moveSound->play();
			}
			else m_focusButton = 0;

			m_ableToPlayMoveSound = false;
		}

		if (input.KeyPress(SDLK_d) || (input.GetThumbLPosition().x > 0.2f && m_ableToPlayMoveSound) || input.PadButtonPress(SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
		{
			if (m_focusButton < m_buttons.size() - 1)
			{
				m_buttons[m_focusButton]->getParent()->removeGameComponent(m_buttons[m_focusButton]->getParent()->getGameComponent<TextLerpAlpha>());

				m_focusButton++;

				m_buttons[m_focusButton]->getParent()->addGameComponent(new TextLerpAlpha, true);

				m_moveSound->play();
			}
			else m_focusButton = m_buttons.size() - 1;

			m_ableToPlayMoveSound = false;
		}

		if (input.KeyPress(SDLK_RETURN) || input.PadButtonPress(SDL_CONTROLLER_BUTTON_A))
		{
			m_selectedSound->play();

			m_buttons[m_focusButton]->click();
		}

		if (input.PadButtonPress(SDL_CONTROLLER_BUTTON_B))
		{
			m_selectedSound->play();

			getCoreEngine()->getSceneManager()->pop();
		}
	}

	virtual void update(float delta) override
	{
		if (m_effectsSlider->valueChanged())
		{
			getCoreEngine()->getAudioEngine()->setSoundVolume(m_effectsSlider->getThumbPosition());
			m_effectsBox->setText(std::to_string((int)(m_effectsSlider->getThumbPosition() * 100.0f)));
		}

		if (m_bgmSlider->valueChanged())
		{
			getCoreEngine()->getAudioEngine()->setStreamVolume(m_bgmSlider->getThumbPosition());
			m_bgmBox->setText(std::to_string((int)(m_bgmSlider->getThumbPosition() * 100.0f)));
		}
	}

private:
	bool m_ableToPlayMoveSound = true;

	bool m_usingController = false;

	GUISlider* m_effectsSlider;
	GUILabel* m_effectsBox;
	GUISlider* m_bgmSlider;
	GUILabel* m_bgmBox;

	bool m_usingMouse = false;
	std::vector<GUIButton*> m_buttons;
	size_t m_focusButton;

	//Sounds
	AudioSource* m_moveSound;
	AudioSource* m_selectedSound;

};

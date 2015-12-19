#include <iostream>
#include <CoreEngine.h>
#include <Viewport.h>
#include <SceneManager.h>
#include <RenderingEngine.h>
#include <GUIEngine.h>
#include <AudioEngine.h>
#include "TestScene.h"

int main(int argc, char** argv)
{
	Viewport window("Project Management Demo", 1440, 900, FULLSCREEN);
	GUIEngine guiEngine;
	RenderingEngine renderingEngine(window, guiEngine);
	SceneManager sceneManager;
	TestScene testscene;
	AudioEngine audioEngine;

	sceneManager.addScene(&testscene);
	sceneManager.switchScene(testscene.getName());

	CoreEngine core(60.0, &window, &renderingEngine, nullptr, nullptr, &guiEngine, &sceneManager);

	core.start();

	return 0;

}
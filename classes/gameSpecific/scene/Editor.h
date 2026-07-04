#pragma once
#include "CSVMainMenu.h"
#include "scene/scene.h"
#include "singleton/dataHolder.h"

struct Editor : BackSceneStrict
{
    Editor(BackSceneStrict* scene) : BackSceneStrict(scene){};

    // editing ints
    int elemSelected = 0;
    int layerSelected = 0;

    // ui
    UIBase ui;
    std::string upperLeftStr = "Back";
    std::string upperCenterStr = "Layer Not Loaded";
    std::string upperRightStr = "Next";

    // rendering
    unsigned int shaderSimpleRef;
    unsigned int uiTextureRef;
    std::vector<float> batch;

    // data
    unsigned int xSize = 0;
    unsigned int ySize = 0;
    unsigned int tileTypes = 0;
    unsigned int layers = 0;
    float ratio = 0.0f;

    //scene methods
    void onLoad() override;
    void handle(float time) override;
    void render(float time, bool updateDisplay) override;
    void processInput(float time, GLFWwindow *ww) override;
    void aspectChange() override;

    //Editor
    void buttonPress(unsigned int x);
};

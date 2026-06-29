#pragma once

#include "scene/scene.h"
#include "uiHelper.h"

struct CSVMainMenu : Scene
{
    /// vars
    // ui
    UIBase ui;
    bool alreadyLoaded = false; // used to not load ui twice
    std::string upperLeftStr = "Quit";
    std::string upperCenterStr = "Welcome to CSV Writer!";
    std::string upperRightStr = "Next";
    std::string rowsTitle = "Y Size";
    std::string rowsValue1 = "0";
    std::string rowsValue2 = "0";
    std::string rowsValue3 = "0";
    std::string rowsValue4 = "0";
    std::string columnTitle = "X Size";
    std::string columnValue1 = "0";
    std::string columnValue2 = "0";
    std::string columnValue3 = "0";
    std::string columnValue4 = "0";
    std::string tileTitle = "Entity Types";
    std::string tileValue = "0";
    std::string layerTitle = "Layers";
    std::string layerValue = "0";

    // rendering
    unsigned int shaderSimpleRef;
    unsigned int uiTextureRef;
    std::vector<float> batch;

    // data
    unsigned int xSize = 10;
    unsigned int ySize = 10;
    unsigned int tileTypes = 2;
    unsigned int layers = 1;

    //scene methods
    void onLoad() override;
    void handle(float time) override;
    void render(float time, bool updateDisplay) override;
    void processInput(float time, GLFWwindow *ww) override;
    void aspectChange() override;

    //CSVMainMethods
    void buttonPress(unsigned int x);
};


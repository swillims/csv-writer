#pragma once
#include "uiHelper.h"
#include "scene/scene.h"

struct Export : BackSceneStrict
{
    // ui
    UIBase ui;
    std::string filePath;
    std::string fileName;
    std::string fileExtension;

    // rendering
    unsigned int shaderSimpleRef;
    unsigned int uiTextureRef;
    std::vector<float> batch;

    //scene methods
    void onLoad() override;
    void handle(float time) override;
    void render(float time, bool updateDisplay) override;
    void processInput(float time, GLFWwindow *ww) override;
    void aspectChange() override;

    //Editor
    void buttonPress(unsigned int x);
};

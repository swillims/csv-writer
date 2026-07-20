#pragma once
#include "uiHelper.h"
#include "scene/scene.h"


struct Export : BackSceneStrict
{
    Export(BackSceneStrict* scene) : BackSceneStrict(scene){};

    // ui
    UIBase ui;
    std::string filePath;
    std::string filePathString;
    float filePathFloat;
    // stringToFloatConstant is a guestimate. It is based on font and guessed average size+distance of chars.
    // - more professional answer would be to create a separate channel textChannel for each UIXRef and calculate based on width.
    // -- This is for personal use and that is 2/10 difficult task, so I'm not doing it.
    float stringToFloatConstant = 0.6f;

    bool hangingDelimEndOfLine = false;
    std::string endOfLineString;

    bool hangingDelimEndOfLayer = false;
    std::string endOfLayerString;

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
    void save();
};

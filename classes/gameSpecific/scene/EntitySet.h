#pragma once
#include "uiHelper.h"
#include "scene/scene.h"

struct EntitySet : BackSceneStrict
{
    EntitySet(BackSceneStrict* scene) : BackSceneStrict(scene){};

    // ui
    UIBase ui;
    bool alreadyLoaded = false;
    std::string upperLeftStr = "Back";
    std::string upperCenterStr = "Layer";
    std::string upperRightStr = "Next";
    // stringToFloatConstant is a guestimate. It is based on font and guessed average size+distance of chars.
    // - more professional answer would be to create a separate channel textChannel for each UIXRef and calculate based on width.
    // -- This is for personal use and that is 2/10 difficult task, so I'm not doing it.
    float stringToFloatConstant = 0.6f;
    std::vector<float> stringRatio;
    std::vector<std::string> elemStrings;

    // data
    int layerSelected = 0;

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

    //DelimSet Specific
    void buttonPress(unsigned int x);
};

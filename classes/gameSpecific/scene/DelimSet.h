#pragma once
#include <string>

#include "scene/scene.h"
#include "uiHelper.h"
#include "singleton/dataHolder.h"

struct  DelimSet : BackSceneStrict
{
    DelimSet(BackSceneStrict* scene) : BackSceneStrict(scene){};

    // ui
    UIBase ui;
    bool alreadyLoaded = false;
    std::string upperLeftStr = "Back";
    std::string upperCenterStr = "Update Delimiters";
    std::string upperRightStr = "Next";
    std::string entityString = "Entity: ";
    std::string lineString = "End of Line: ";
    std::string layerString = "Layer: ";
    std::string resetString = "Reset";
    std::string setString = "Set";
    float entityFloat = 1.0f;
    float lineFloat = 1.0f;
    float layerFloat = 1.0f;
    // this number is a guestimate. It is based on font and guessed average size+distance of chars.
    // - more professional answer would be to create a separate channel textChannel for each UIXRef and calculate based on width.
    // -- This is for personal use and that is 2/10 difficult task, so I'm not doing it.
    float stringToFloatConstant = 0.6f;

    // data
    // handled externally directly in dataholder

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

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


    // data
    std::string entityDelim = ",";
    std::string lineDelim = "\\n";
    std::string layerDelim = "|";

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

    //Delim Specific
    void buttonPress(unsigned int x);
};

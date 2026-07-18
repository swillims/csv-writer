#pragma once
#include <string>

#include "scene/scene.h"
#include "uiHelper.h"

struct StringEditor : BackSceneStrict
{
    StringEditor(std::string& source, BackSceneStrict* scene) : BackSceneStrict(scene), source{source} {}

    // ui
    UIBase ui;
    std::string actualString;
    std::string displayString;
    float editStringSize = 1.0f;
    // stringToFloatConstant is a guestimate. It is based on font and guessed average size+distance of chars.
    // - more professional answer would be to create a separate channel textChannel for each UIXRef and calculate based on width.
    // -- This is for personal use and that is 2/10 difficult task, so I'm not doing it.
    float stringToFloatConstant = 0.3f;

    // data
    std::string& source;

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
    void clean() override;

    //StringEditor Specific
    void buttonPress(unsigned int x);
    void keyAppend(unsigned int key, bool shift);
};

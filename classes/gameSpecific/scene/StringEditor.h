#pragma once
#include <string>

#include "scene/scene.h"

struct StringEditor : BackSceneStrict
{
    std::string actualString;
    std::string displayString;
    std::string& source;
    StringEditor(std::string& source, BackSceneStrict* scene) : BackSceneStrict(scene), source{source} {}

    //scene methods
    void onLoad() override;
    void handle(float time) override;
    void render(float time, bool updateDisplay) override;
    void processInput(float time, GLFWwindow *ww) override;
    void aspectChange() override;

    //StringEditor Specific
    void buttonPress(unsigned int x);
};

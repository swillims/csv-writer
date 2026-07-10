#pragma once
#include "CSVMainMenu.h"
#include "scene/scene.h"
#include "singleton/dataHolder.h"

struct UIDims {
    float xMin;
    float xSize;
    float yMin;
    float ySize;

    void loadUI(const UIElement& ui)
    {
        xMin = ui.xMin;
        xSize = ui.xSize;
        yMin = ui.yMin;
        ySize = ui.ySize;
    }
};

struct Editor : BackSceneStrict
{
    Editor(BackSceneStrict* scene) : BackSceneStrict(scene){};

    // editing ints
    int elemSelected = 1;
    int layerSelected = 0;
    double mouseX = 0;
    double mouseY = 0;

    // ui
    UIBase ui;
    std::string upperLeftStr = "Back";
    std::string upperCenterStr = "Layer Not Loaded";
    std::string upperRightStr = "Next";
    std::vector<std::string> elemStrings;
    UIElement* mapZone;
    UIElement* elemZone;
    UIDims mapDims;
    UIDims elemDims;

    // rendering
    unsigned int shaderSimpleRef;
    unsigned int uiTextureRef;
    std::vector<float> batch;
    std::vector<std::vector<unsigned int>> batchTextures;
    // layer // entity // floats
    std::vector<std::vector<std::vector<float>>> layerBatches;

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
    void mapPress();
    int getMapValue(int x, int y);
    int getMapValue(int layer, int x, int y);
    void updateElemBatch(int elem);
    void updateElemBatch(int layer, int elem);
    void updateMapBatches();
};
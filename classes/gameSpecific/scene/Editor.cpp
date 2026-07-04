#include "Editor.h"

#include "singleton/staticInput.h"

enum BUTTONS
{
    LEFTUPPERUI,
    RIGHTUPPERUI,

    LAYERLEFT,
    LAYERRIGHT,

    MAPCLICK,
    ELEMCLICK,
};

void Editor::onLoad()
{
    BackSceneStrict::onLoad();

    // simple shader declared on init
    shaderSimpleRef = StaticDraw::getShader("simple");

    // load ui texture
    if (!StaticDraw::imageFileRefs.contains("uiTexture"))
    {
        StaticDraw::loadImage("assets/gamespecific/png/ui_elems.png", "uiTexture", false);
    }
    uiTextureRef = StaticDraw::imageFileRefs["uiTexture"];

    // set up click listening
    StaticInput::MouseTrack(GLFW_MOUSE_BUTTON_LEFT);

    // load data from dataholder
    xSize = DataHolder::god.layerX;
    ySize = DataHolder::god.layerY;
    layers = DataHolder::god.layerZ;
    tileTypes = DataHolder::god.layerElems;

    ratio = static_cast<float>(xSize) / static_cast<float>(ySize);

    // choosing to clear old ui instead of guard clause because this section will have dynamic element loading for different amounts elements/layers
    ui.nodes.clear();

    // build ui
    ui.appendType<UIYSplits>(std::vector<float>{.1f,.9f})
        .appendType<UIXSplits>(std::vector<float>{.2f,.6f,.2f})
            .appendType<UIXRatio>(2,true)
                .appendType<UIStack>()
                    .appendType<TexUVNode>(0,.5f,.75,1.0f,LEFTUPPERUI).back()
                    .appendType<UITextOneLine>(DataHolder::EDITOR, upperLeftStr, .8 ).back()
                    .back()
                .back()
            .appendType<UIXRatio>(6,true)
                //.appendType<UITextOneLine>(DataHolder::EDITOR,upperCenterStr,.5f).back()
                .appendType<UIXSplits>(std::vector<float>{.1f,.8f,.1f})
                    .appendType<UIXRatio>(1,true)
                        .appendType<TexUVNode>(.75f, 1.0f, 0.0f,.25f,LAYERLEFT).back()
                        .back()
                    .appendType<UIXRatio>(8,true)
                        .appendType<UITextOneLine>(DataHolder::EDITOR,upperCenterStr,.8f).back()
                        .back()
                    .appendType<UIXRatio>(1,true)
                        .appendType<TexUVNode>(.25f,.5f,0.0f,.25f,LAYERRIGHT).back()
                        .back()
                    .back()
                .back()
            .appendType<UIXRatio>(2,true)
                .appendType<UIStack>()
                    .appendType<TexUVNode>(0,.5f,.75,1.0f,RIGHTUPPERUI).back()
                    .appendType<UITextOneLine>(DataHolder::EDITOR, upperRightStr, .8 ).back()
                    .back()
                .back()
            .back()
        .appendType<UIXSplits>(std::vector<float>{.7f,.3f})
            .appendType<UIBuffer>(.05f)
                .appendType<UIXRatio>(ratio,true) // remove ratio later if implementing zoom
                    .appendType<TexUVNode>(0,.001f,.999f,1.0f,MAPCLICK).back()
                    .back()
                .back()
            .appendType<UIYHolder>(ELEMCLICK)

    ;
    aspectChange();

}

void Editor::handle(float time)
{
    processInput(time, window);
}

void Editor::render(float time, bool updateDisplay)
{
    // clear
    StaticDraw::clear(.8,.8,.8);

    // switch back to default shader
    StaticDraw::useShader(shaderSimpleRef);

    // draw ui Elements from batch
    StaticDraw::multiDraw(uiTextureRef, batch);

    // write text
    StaticWrite::StartWrite();
    StaticWrite::DrawChannel(DataHolder::EDITOR, glm::vec3(0.0f, 0.0f, 0.0f));

    BackSceneStrict::render(time, updateDisplay);
}

void Editor::processInput(float time, GLFWwindow *ww)
{
    StaticInput::Tick();
    if (StaticInput::MouseClick(GLFW_MOUSE_BUTTON_LEFT))
    {
        double x, y;
        StaticInput::GetMouse(x,y);
        buttonPress(ui.findOneHover(x, y));
    }
}

void Editor::aspectChange()
{
    BackSceneStrict::aspectChange();

    // make new batches
    batch.clear();
    StaticWrite::SetUpChannel(DataHolder::EDITOR);
    StaticDraw::updateView();
    ui.adjustNodeDefault();
    ui.renderVerts(batch);
}

void Editor::buttonPress(unsigned int x)
{

}


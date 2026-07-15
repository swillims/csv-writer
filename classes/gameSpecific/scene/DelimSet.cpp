#include "DelimSet.h"

#include "singleton/staticInput.h"

enum BUTTONS
{
    LEFTUPPERUI,
    RIGHTUPPERUI,

    ENTITYDEFAULT,
    ENTITYEDIT,

    LINEDEFAULT,
    LINEEDIT,

    LAYERDEFAULT,
    LAYEREDIT,
};

void DelimSet::onLoad()
{
    BackSceneStrict::onLoad();

    entityDelim = DataHolder::god.entityDelim;
    entityString = "Entity: \"" + util::translateToHumanReadable(entityDelim) + "\"";
    lineDelim = DataHolder::god.lineDelim;
    lineString = "Line: \"" + util::translateToHumanReadable(lineDelim) + "\"";
    layerDelim = DataHolder::god.layerDelim;
    layerString = "Layer: \"" + util::translateToHumanReadable(layerDelim) + "\"";

    shaderSimpleRef = StaticDraw::getShader("simple");

    // load ui texture
    if (!StaticDraw::imageFileRefs.contains("uiTexture"))
    {
        StaticDraw::loadImage("assets/gamespecific/png/ui_elems.png", "uiTexture", false);
    }
    uiTextureRef = StaticDraw::imageFileRefs["uiTexture"];

    StaticInput::MouseTrack(GLFW_MOUSE_BUTTON_LEFT);

    if (!alreadyLoaded)
    {
        alreadyLoaded = true;
        ui.appendType<UIYSplits>(std::vector<float>{.1f,.9f})
            .appendType<UIXSplits>(std::vector<float>{.2f,.6f,.2f})
                .appendType<UIXRatio>(2,true)
                    .appendType<UIStack>()
                        .appendType<TexUVNode>(0,.5f,.75,1.0f,LEFTUPPERUI).back()
                        .appendType<UITextOneLine>(DataHolder::DELIMSET, upperLeftStr, .8 ).back()
                        .back()
                    .back()
                .appendType<UIXRatio>(6,true)
                    .appendType<UITextOneLine>(DataHolder::DELIMSET,upperCenterStr,.8f).back()
                    .back()
                .appendType<UIXRatio>(2,true)
                    .appendType<UIStack>()
                        .appendType<TexUVNode>(0,.5f,.75,1.0f,RIGHTUPPERUI).back()
                        .appendType<UITextOneLine>(DataHolder::DELIMSET, upperRightStr, .8f).back()
                        .back()
                    .back()
                .back()
            .appendType<UIYHolder>()
                .appendType<UIXHolder>()
                    .appendType<UITextOneLine>(DataHolder::DELIMSET,entityString,.2f).back()
                    .appendType<UIXHolder>()
                        .appendType<UIXRatio>(2,true)
                            .appendType<UIStack>()
                                .appendType<TexUVNode>(0,.5f,.75,1.0f,ENTITYDEFAULT).back()
                                .appendType<UITextOneLine>(DataHolder::DELIMSET, resetString, .6f).back()
                                .back()
                            .back()
                        .appendType<UIXRatio>(2,true)
                            .appendType<UIStack>()
                                .appendType<TexUVNode>(0,.5f,.75,1.0f,ENTITYEDIT).back()
                                .appendType<UITextOneLine>(DataHolder::DELIMSET, setString, .6f).back()
                                .back()
                            .back()
                        .back()
                    .back()
                .appendType<UIXHolder>()
                    .appendType<UIXHolder>()
                        .appendType<UITextOneLine>(DataHolder::DELIMSET,lineString,.2f).back()
                        .appendType<UIXHolder>()
                            .appendType<UIXRatio>(2,true)
                                .appendType<UIStack>()
                                    .appendType<TexUVNode>(0,.5f,.75,1.0f,LINEDEFAULT).back()
                                    .appendType<UITextOneLine>(DataHolder::DELIMSET, resetString, .6f).back()
                                    .back()
                                .back()
                            .appendType<UIXRatio>(2,true)
                                .appendType<UIStack>()
                                    .appendType<TexUVNode>(0,.5f,.75,1.0f,LINEEDIT).back()
                                    .appendType<UITextOneLine>(DataHolder::DELIMSET, setString, .6f).back()
                                    .back()
                                .back()
                            .back()
                        .back()
                    .back()
                .appendType<UIXHolder>()
                    .appendType<UIXHolder>()
                        .appendType<UITextOneLine>(DataHolder::DELIMSET,layerString,.2f).back()
                        .appendType<UIXHolder>()
                            .appendType<UIXRatio>(2,true)
                                .appendType<UIStack>()
                                    .appendType<TexUVNode>(0,.5f,.75,1.0f,LAYERDEFAULT).back()
                                    .appendType<UITextOneLine>(DataHolder::DELIMSET, resetString, .6f).back()
                                    .back()
                                .back()
                            .appendType<UIXRatio>(2,true)
                                .appendType<UIStack>()
                                    .appendType<TexUVNode>(0,.5f,.75,1.0f,LAYEREDIT).back()
                                    .appendType<UITextOneLine>(DataHolder::DELIMSET, setString, .6f).back()
                                    .back()
                                .back()
                            .back()
                        .back()
                    .back()
        ;
    }

    aspectChange();
}

void DelimSet::handle(float time)
{
    processInput(time, window);
}

void DelimSet::render(float time, bool updateDisplay)
{
    // clear
    StaticDraw::clear(.8,.8,.8);

    // switch back to default shader
    StaticDraw::useShader(shaderSimpleRef);

    // draw ui Elements from batch
    StaticDraw::multiDraw(uiTextureRef, batch);

    // write text
    StaticWrite::StartWrite();
    StaticWrite::DrawChannel(DataHolder::DELIMSET, glm::vec3(0.0f, 0.0f, 0.0f));

    BackSceneStrict::render(time, updateDisplay);
}

void DelimSet::processInput(float time, GLFWwindow *ww)
{
    StaticInput::Tick();
    if (StaticInput::MouseClick(GLFW_MOUSE_BUTTON_LEFT))
    {
        double x,y;
        StaticInput::GetMouse(x,y);
        buttonPress(ui.findOneHover(x, y));
    }
}

void DelimSet::aspectChange()
{
    BackSceneStrict::aspectChange();

    // make new batches
    batch.clear();
    StaticWrite::SetUpChannel(DataHolder::DELIMSET);
    StaticDraw::updateView();
    ui.adjustNodeDefault();
    ui.renderVerts(batch);
}

void DelimSet::buttonPress(unsigned int x)
{
    switch (x)
    {
        case LEFTUPPERUI:
        {
            DataHolder::SceneQueue(previous, true);
            break;
        }
        case RIGHTUPPERUI:
        {
            // add code later
            break;
        }
    }
}



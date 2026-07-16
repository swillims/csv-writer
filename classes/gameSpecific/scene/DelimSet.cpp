#include "DelimSet.h"

#include "StringEditor.h"
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

    entityString = "Entity: \"" + util::translateToHumanReadable(DataHolder::god.entityDelim) + "\"";
    lineString = "Line: \"" + util::translateToHumanReadable(DataHolder::god.lineDelim) + "\"";
    layerString = "Layer: \"" + util::translateToHumanReadable(DataHolder::god.layerDelim) + "\"";

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
                .appendType<UIXRatio>(2)
                    .appendType<UIStack>()
                        .appendType<TexUVNode>(0,.5f,.75,1.0f,LEFTUPPERUI).back()
                        .appendType<UITextOneLine>(DataHolder::DELIMSET, upperLeftStr, .8 ).back()
                        .back()
                    .back()
                .appendType<UIXRatio>(6)
                    .appendType<UITextOneLine>(DataHolder::DELIMSET,upperCenterStr,.8f).back()
                    .back()
                .appendType<UIXRatio>(2)
                    .appendType<UIStack>()
                        .appendType<TexUVNode>(0,.5f,.75,1.0f,RIGHTUPPERUI).back()
                        .appendType<UITextOneLine>(DataHolder::DELIMSET, upperRightStr, .8f).back()
                        .back()
                    .back()
                .back()
            .appendType<UIYHolder>()
                .appendType<UIXHolder>()
                    .appendType<UIXRefRatio>(entityFloat)
                        .appendType<UITextOneLine>(DataHolder::DELIMSET,entityString,1.0f).back()
                        .back()
                    .appendType<UIXHolder>()
                        .appendType<UIXRatio>(2)
                            .appendType<UIStack>()
                                .appendType<TexUVNode>(0,.5f,.75,1.0f,ENTITYDEFAULT).back()
                                .appendType<UITextOneLine>(DataHolder::DELIMSET, resetString, .6f).back()
                                .back()
                            .back()
                        .appendType<UIXRatio>(2)
                            .appendType<UIStack>()
                                .appendType<TexUVNode>(0,.5f,.75,1.0f,ENTITYEDIT).back()
                                .appendType<UITextOneLine>(DataHolder::DELIMSET, setString, .6f).back()
                                .back()
                            .back()
                        .back()
                    .back()
                .appendType<UIXHolder>()
                    .appendType<UIXHolder>()
                        .appendType<UIXRefRatio>(lineFloat)
                            .appendType<UITextOneLine>(DataHolder::DELIMSET,lineString,1.0f).back()
                            .back()
                        .appendType<UIXHolder>()
                            .appendType<UIXRatio>(2)
                                .appendType<UIStack>()
                                    .appendType<TexUVNode>(0,.5f,.75,1.0f,LINEDEFAULT).back()
                                    .appendType<UITextOneLine>(DataHolder::DELIMSET, resetString, .6f).back()
                                    .back()
                                .back()
                            .appendType<UIXRatio>(2)
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
                        .appendType<UIXRefRatio>(layerFloat)
                            .appendType<UITextOneLine>(DataHolder::DELIMSET,layerString,1.0f).back()
                            .back()
                        .appendType<UIXHolder>()
                            .appendType<UIXRatio>(2)
                                .appendType<UIStack>()
                                    .appendType<TexUVNode>(0,.5f,.75,1.0f,LAYERDEFAULT).back()
                                    .appendType<UITextOneLine>(DataHolder::DELIMSET, resetString, .6f).back()
                                    .back()
                                .back()
                            .appendType<UIXRatio>(2)
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
    entityFloat = stringToFloatConstant * static_cast<float>(entityString.size());
    lineFloat = stringToFloatConstant * static_cast<float>(lineString.size());
    layerFloat = stringToFloatConstant * static_cast<float>(layerString.size());


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
        case ENTITYEDIT:
        {
            StringEditor* strEdit = new StringEditor(DataHolder::god.entityDelim, this);
            DataHolder::SceneQueue(strEdit, false);
            break;
        }
        case LINEEDIT:
        {
            StringEditor* strEdit = new StringEditor(DataHolder::god.lineDelim, this);
            DataHolder::SceneQueue(strEdit, false);
            break;
        }
        case LAYEREDIT:
        {
            StringEditor* strEdit = new StringEditor(DataHolder::god.layerDelim, this);
            DataHolder::SceneQueue(strEdit, false);
            break;
        }
        case ENTITYDEFAULT:
        {
            DataHolder::god.entityDelim = DataHolder::god.entityDelimDefault;
            entityString = "Entity: \"" + util::translateToHumanReadable(DataHolder::god.entityDelim) + "\"";
            aspectChange();
            break;
        }
        case LINEDEFAULT:
        {
            DataHolder::god.lineDelim = DataHolder::god.lineDelimDefault;
            lineString = "Line: \"" + util::translateToHumanReadable(DataHolder::god.lineDelim) + "\"";
            aspectChange();
            break;
        }
        case LAYERDEFAULT:
        {
            DataHolder::god.layerDelim = DataHolder::god.layerDelimDefault;
            layerString = "Layer: \"" + util::translateToHumanReadable(DataHolder::god.layerDelim) + "\"";
            aspectChange();
            break;
        }
    }
}



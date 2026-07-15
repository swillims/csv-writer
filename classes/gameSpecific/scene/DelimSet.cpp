#include "DelimSet.h"

#include "singleton/staticInput.h"

enum BUTTONS
{
    LEFTUPPERUI,
    RIGHTUPPERUI,

    ENTITYDELIMDEFAULT,
    ENTITYDELIMEDIT,

    LINEDELIMDEFAULT,
    LINEDELIMEDIT,

    LAYERDELIMDEFAULT,
    LAYERDELIMEDIT,
};

void DelimSet::onLoad()
{
    BackSceneStrict::onLoad();

    entityDelim = DataHolder::god.entityDelim;
    lineDelim = DataHolder::god.lineDelim;
    layerDelim = DataHolder::god.layerDelim;

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
                        .appendType<UITextOneLine>(DataHolder::DELIMSET, upperRightStr, .8 ).back()
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
    StaticWrite::SetUpChannel(DataHolder::EDITOR);
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



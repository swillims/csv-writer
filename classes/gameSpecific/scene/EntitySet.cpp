#include "EntitySet.h"

#include "singleton/dataHolder.h"
#include "singleton/staticInput.h"

enum
{
    LEFTUPPERUI,
    RIGHTUPPERUI,

    LAYERLEFT,
    LAYERRIGHT,

    UILAST
};

void EntitySet::onLoad()
{
    BackSceneStrict::onLoad();

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
        // ui.append...

        // add for each element
    }

    aspectChange();
}

void EntitySet::handle(float time)
{
    BackSceneStrict::handle(time);
}

void EntitySet::render(float time, bool updateDisplay)
{
    // clear
    StaticDraw::clear(.8,.8,.8);

    // switch back to default shader
    StaticDraw::useShader(shaderSimpleRef);

    // draw ui Elements from batch
    StaticDraw::multiDraw(uiTextureRef, batch);

    // write text
    StaticWrite::StartWrite();
    StaticWrite::DrawChannel(DataHolder::ENTITYEDIT, glm::vec3(0.0f, 0.0f, 0.0f));

    BackSceneStrict::render(time, updateDisplay);
}

void EntitySet::processInput(float time, GLFWwindow *ww)
{
    StaticInput::Tick();
    if (StaticInput::MouseClick(GLFW_MOUSE_BUTTON_LEFT))
    {
        double x,y;
        StaticInput::GetMouse(x,y);
        buttonPress(ui.findOneHover(x, y));
    }
}

void EntitySet::aspectChange()
{
    BackSceneStrict::aspectChange();

    // make new batches
    batch.clear();
    StaticWrite::SetUpChannel(DataHolder::ENTITYEDIT);
    StaticDraw::updateView();
    ui.adjustNodeDefault();
    ui.renderVerts(batch);
}

void EntitySet::buttonPress(unsigned int x)
{

}

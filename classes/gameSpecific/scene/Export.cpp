#include "Export.h"

#include "singleton/dataHolder.h"
#include "singleton/staticInput.h"

enum BUTTONS
{
    LEFTUPPERUI,
    RIGHTUPPERUI,

    FIlEFOLDER,
    FILENAME,

    HANGINGENTITYDELIM,
    HANGINGLINEDELIM,

    UILAST
};

void Export::onLoad()
{
    BackSceneStrict::onLoad();

    // load ui texture
    if (!StaticDraw::imageFileRefs.contains("uiTexture"))
    {
        StaticDraw::loadImage("assets/gamespecific/png/ui_elems.png", "uiTexture", false);
    }
    uiTextureRef = StaticDraw::imageFileRefs["uiTexture"];

    StaticInput::MouseTrack(GLFW_MOUSE_BUTTON_LEFT);

    // shader
    shaderSimpleRef = StaticDraw::getShader("simple");

    ui.appendType<UIYSplits>(std::vector<float>{.1f,.9f})
        .appendType<UIXSplits>(std::vector<float>{.2f,.6f,.2f})
            .appendType<UIXRatio>(2)
                .appendType<UIStack>()
                    .appendType<TexUVNode>(0,.5f,.75,1.0f,LEFTUPPERUI).back()
                    .appendType<UITextOneLineConst>(DataHolder::EXPORT, "Back", .8 ).back()
                    .back()
                .back()
            .appendType<UIXRatio>(6)
                .appendType<UITextOneLineConst>(DataHolder::EXPORT,"Export Data",.6f).back()
                .back()
            .appendType<UIXRatio>(2)
                .appendType<UIStack>()
                    .appendType<TexUVNode>(0,.5f,.75,1.0f,RIGHTUPPERUI).back()
                    .appendType<UITextOneLineConst>(DataHolder::EXPORT, "Export", .8 ).back()
                    .back()
                .back()
            .back()
        .appendType<UIYHolder>(UILAST)
    ;
    UIElement& holder = ui.findByKey(UILAST);
    holder.key = -1;

    // add code to append to Holder

    aspectChange();
}

void Export::handle(float time)
{
    processInput(time, window);
}

void Export::render(float time, bool updateDisplay)
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

void Export::processInput(float time, GLFWwindow *ww)
{
    StaticInput::Tick();
    if (StaticInput::MouseClick(GLFW_MOUSE_BUTTON_LEFT))
    {
        double x,y;
        StaticInput::GetMouse(x,y);
        buttonPress(ui.findOneHover(x, y));
    }
}

void Export::aspectChange()
{
    BackSceneStrict::aspectChange();

    // add code for setting strings


    // make new batches
    batch.clear();
    StaticWrite::SetUpChannel(DataHolder::ENTITYEDIT);
    StaticDraw::updateView();
    ui.adjustNodeDefault();
    ui.renderVerts(batch);
}

void Export::buttonPress(unsigned int x)
{

}

#include "EntitySet.h"

#include "StringEditor.h"
#include "singleton/dataHolder.h"
#include "singleton/staticInput.h"
#include "util.h"

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

    // shader
    shaderSimpleRef = StaticDraw::getShader("simple");

    if (!alreadyLoaded)
    {
        alreadyLoaded = true;
        ui.appendType<UIYSplits>(std::vector<float>{.1f,.9f})
            .appendType<UIXSplits>(std::vector<float>{.2f,.6f,.2f})
                .appendType<UIXRatio>(2)
                    .appendType<UIStack>()
                        .appendType<TexUVNode>(0,.5f,.75,1.0f,LEFTUPPERUI).back()
                        .appendType<UITextOneLine>(DataHolder::ENTITYEDIT, upperLeftStr, .8 ).back()
                        .back()
                    .back()
                .appendType<UIXRatio>(6)
                    .appendType<UIXSplits>(std::vector<float>{.1f,.8f,.1f})
                        .appendType<UIXRatio>(1)
                            .appendType<TexUVNode>(.75f, 1.0f, 0.0f,.25f,LAYERLEFT).back()
                            .back()
                        .appendType<UIXRatio>(8)
                            .appendType<UITextOneLine>(DataHolder::ENTITYEDIT,upperCenterStr,.6f).back()
                            .back()
                        .appendType<UIXRatio>(1)
                            .appendType<TexUVNode>(.25f,.5f,0.0f,.25f,LAYERRIGHT).back()
                            .back()
                        .back()
                    .back()
                .appendType<UIXRatio>(2)
                    .appendType<UIStack>()
                        .appendType<TexUVNode>(0,.5f,.75,1.0f,RIGHTUPPERUI).back()
                        .appendType<UITextOneLine>(DataHolder::ENTITYEDIT, upperRightStr, .8 ).back()
                        .back()
                    .back()
                .back()
            .appendType<UIYHolder>(UILAST)
        ;
        UIElement& holder = ui.findByKey(UILAST);
        holder.key = -1;

        // done in two loops to avoid shifting breaking references
        for (int i = 0; i < DataHolder::god.layerElems; i++)
        {
            stringRatio.push_back(1.0f);
            elemStrings.push_back(std::to_string(i));
        }
        for (int i = 0; i < DataHolder::god.layerElems; i++)
        {
            int last = UILAST + (i*2);
            holder.appendType<UIXHolder>()
                .appendType<UIXRefRatio>(stringRatio[i])
                    .appendType<UITextOneLine>(DataHolder::ENTITYEDIT,elemStrings[i],1.0f).back()
                    .back()
                .appendType<UIXHolder>()
                    .appendType<UIXRatio>(2)
                        .appendType<UIStack>()
                            .appendType<TexUVNode>(0,.5f,.75,1.0f,last+1).back()
                            .appendType<UITextOneLineConst>(DataHolder::ENTITYEDIT,"Reset",.6f).back()
                            .back()
                        .back()
                    .appendType<UIXRatio>(2)
                        .appendType<UIStack>()
                            .appendType<TexUVNode>(0,.5f,.75,1.0f,last+2).back()
                            .appendType<UITextOneLineConst>(DataHolder::ENTITYEDIT,"Set",.6f).back()
            ;
        }

    }

    aspectChange();
}

void EntitySet::handle(float time)
{
    processInput(time, window);
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

    for (int i = 0; i < stringRatio.size(); i++)
    {
        elemStrings[i] = util::translateToHumanReadable("Entity " + std::to_string(i) + ": " + DataHolder::god.entityStrings[i + (layerSelected*stringRatio.size())]);
        stringRatio[i] = static_cast<float>(elemStrings[i].size()) * stringToFloatConstant;
    }

    upperCenterStr = "Set Entity Values in Layer " + std::to_string(layerSelected);

    // make new batches
    batch.clear();
    StaticWrite::SetUpChannel(DataHolder::ENTITYEDIT);
    StaticDraw::updateView();
    ui.adjustNodeDefault();
    ui.renderVerts(batch);
}

void EntitySet::buttonPress(unsigned int x)
{
    if (x == -1)
    {
        return;
    }
    switch (x)
    {
        case(LEFTUPPERUI):
        {
            DataHolder::SceneQueue(previous,true);
            break;
        }
        case(RIGHTUPPERUI):
        {
            // add code later after making final scene
            break;
        }
            case(LAYERLEFT):
        {
            layerSelected--;
            if (layerSelected == -1){layerSelected=DataHolder::god.layerZ-1;}
            aspectChange();
            break;
        }
            case(LAYERRIGHT):
        {
            layerSelected++;
            if (layerSelected == DataHolder::god.layerZ){layerSelected=0;}
            aspectChange();
            break;
        }
    }
    if (x > UILAST)
    {
        int id = x - (UILAST + 1);

        int i = id / 2;
        bool isReset = (id % 2 == 0);

        if (i >= DataHolder::god.layerElems)
            return;

        int index = i + (layerSelected * DataHolder::god.layerZ);
        if (isReset)
        {
            DataHolder::god.entityStrings[index] = std::to_string(i);
            aspectChange();
        }
        else
        {
            StringEditor* edit = new StringEditor(DataHolder::god.entityStrings[index],this);
            DataHolder::SceneQueue(edit,false);
        }
    }
}

#include "Export.h"

#include "singleton/dataHolder.h"
#include "singleton/staticInput.h"

#include "singleton/staticFile.h"

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
                    .appendType<UITextOneLineConst>(DataHolder::EXPORT, "Export", .6 ).back()
                    .back()
                .back()
            .back()
        .appendType<UIYHolder>(UILAST)
    ;
    // using multiple statement +UILAST pattern instead of html-like because I thought I would need dynamically loaded UI elements and it is too much work to refactor
    UIElement& holder = ui.findByKey(UILAST);
    holder.key = -1;

    holder.appendType<UIXSplits>(std::vector<float>{.7f,.3f})
        .appendType<UIXRefRatio>(filePathFloat)
            .appendType<UITextOneLine>(DataHolder::EXPORT, filePathString, 1.0f ).back()
            .back()
        .appendType<UIXRatio>(2)
            .appendType<UIStack>()
                .appendType<TexUVNode>(0,.5f,.75,1.0f,FIlEFOLDER).back()
                .appendType<UITextOneLineConst>(DataHolder::EXPORT, "Set", .6 )
    ;
    holder.appendType<UIXSplits>(std::vector<float>{.7f,.3f})
        .appendType<UIBuffer>(.05)
            .appendType<UIXRatio>(7)
                .appendType<UITextOneLineConst>(DataHolder::EXPORT, "Include trailing entity delimiter on each line?", .3, XLEFT).back()
                .back()
            .back()
        .appendType<UIXRatio>(2)
            .appendType<UIStack>()
                .appendType<TexUVNode>(0,.5f,.75,1.0f,HANGINGENTITYDELIM).back()
                .appendType<UITextOneLine>(DataHolder::EXPORT, endOfLineString, .6 )
    ;
    holder.appendType<UIXSplits>(std::vector<float>{.7f,.3f})
        .appendType<UIBuffer>(.05)
            .appendType<UIXRatio>(7)
                .appendType<UITextOneLineConst>(DataHolder::EXPORT, "Include trailing line delimiter at the end of each layer?", .3, XLEFT).back()
                .back()
            .back()
        .appendType<UIXRatio>(2)
            .appendType<UIStack>()
                .appendType<TexUVNode>(0,.5f,.75,1.0f,HANGINGLINEDELIM).back()
                .appendType<UITextOneLine>(DataHolder::EXPORT, endOfLayerString, .6 ).back()
    ;
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
    StaticWrite::DrawChannel(DataHolder::EXPORT, glm::vec3(0.0f, 0.0f, 0.0f));

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

    filePathString = "File path: " + filePath;

    filePathFloat = static_cast<float>(filePathString.length()) * stringToFloatConstant;

    endOfLineString = hangingDelimEndOfLine ? "True" : "False";
    endOfLayerString = hangingDelimEndOfLayer ? "True" : "False";

    // make new batches
    batch.clear();
    StaticWrite::SetUpChannel(DataHolder::EXPORT);
    StaticDraw::updateView();
    ui.adjustNodeDefault();
    ui.renderVerts(batch);
}

void Export::buttonPress(unsigned int x)
{
    if (x==-1){return;}
    switch (x)
    {
        case LEFTUPPERUI:
        {
            DataHolder::SceneQueue(previous,true);
            break;
        }
        case RIGHTUPPERUI:
        {
            save();
            break;
        }
        case FIlEFOLDER:
        {
            std::string file = StaticFile::NavigateSaveFile();
            if (file.length() > 0)
            {
                filePath = file;
            }
            aspectChange();
            break;
        }
        case HANGINGENTITYDELIM:
        {
            hangingDelimEndOfLine = !hangingDelimEndOfLine;
            aspectChange();
            break;
        }
        case HANGINGLINEDELIM:
        {
            hangingDelimEndOfLayer = !hangingDelimEndOfLayer;
            aspectChange();
            break;
        }
    }
}

void Export::save()
{
    std::string data;
    const DataHolder& g = DataHolder::god;

    for (int z = 0; z < g.layerZ; z++)
    {
        for (int y = 0; y < g.layerY; y++)
        {
            for (int x = 0; x < g.layerX; x++)
            {
                int index = y * g.layerX + x;
                int entity = g.layers[z][index];

                data += DataHolder::god.entityStrings[(z*g.layerElems)+entity];

                // add delimiter unless this is the last entity on the line
                if (x < g.layerX - 1 || hangingDelimEndOfLine){data += g.entityDelim;}
            }

            // add delim after line
            if (y < g.layerY - 1 || hangingDelimEndOfLayer) {data += g.lineDelim;}
        }

        // add delim after layers
        if (z < g.layerZ - 1){data += g.layerDelim;}
    }

    if (filePath.length() > 0)
    {
        StaticFile::Write(filePath, data);
        filePath = "";
        aspectChange();
    }
}

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

    // load data from dataholder
    xSize = DataHolder::god.layerX;
    ySize = DataHolder::god.layerY;
    layers = DataHolder::god.layerZ;
    tileTypes = DataHolder::god.layerElems;

    std::vector<unsigned> vi;
    std::vector<std::vector<float>> vvf;
    std::vector<float> vf;
    // create batches
    for (int i = 0; i < tileTypes; i++)
    {
        vvf.push_back(vf);
    }
    for (int i = 0; i < layers; i++)
    {
        layerBatches.push_back(vvf);
        batchTextures.push_back(vi);
    }

    // shaders
    // simple shader declared on init
    shaderSimpleRef = StaticDraw::getShader("simple");
    // load shaders and assign empty batches
    for (int i = 0; i < layers; i++)
    {
        for (int j = 0; j < tileTypes; j++)
        {
            std::string uiName = std::to_string(i) + "_" + std::to_string(j);
            if (!StaticDraw::imageFileRefs.contains(uiName))
            {
                StaticDraw::compileShader("assets/shaders/simple.vs", "assets/shaders/color.fs", uiName);
            }
            batchTextures[i].push_back(StaticDraw::getShader(uiName));
            StaticDraw::useShader(batchTextures[i][j]);
            GLint colorLoc = glGetUniformLocation(batchTextures[i][j], "color");
            // change this to random later
            glm::vec4 color = DataHolder::god.colorList[i*layers+j];
            glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
            //glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
    //StaticDraw::useShader(batchTextures[0][0]);
    //GLint colorLoc = glGetUniformLocation(batchTextures[0][0], "color");
    //glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 0.5f);

    // load ui texture
    if (!StaticDraw::imageFileRefs.contains("uiTexture"))
    {
        StaticDraw::loadImage("assets/gamespecific/png/ui_elems.png", "uiTexture", false);
    }
    uiTextureRef = StaticDraw::imageFileRefs["uiTexture"];



    // set up click listening
    StaticInput::MouseTrack(GLFW_MOUSE_BUTTON_LEFT);
    // set up key listening
    for (int i = GLFW_KEY_0; i < GLFW_KEY_0 + tileTypes; i++)
    {
        StaticInput::KeyTrack(i);
    }

    ratio = static_cast<float>(xSize) / static_cast<float>(ySize);

    // choosing to clear old ui instead of guard clause because this section will have dynamic element loading for different amounts elements/layers
    ui.nodes.clear();
    elemStrings.clear();

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
            .appendType<UIYHolder>(ELEMCLICK).appendType<UIYHolder>()
    ;

    mapZone = &ui.findByKey(MAPCLICK);
    elemZone = &ui.findByKey(ELEMCLICK);

    aspectChange();
    // Done in a separate loop to avoid a bug with elements shifting. It looks weird because it is.
    for (int i = 0; i < tileTypes; i++)
    {
        glm::vec4 color = DataHolder::god.colorList[i];
        elemColorSubValues.push_back(color.r);
        elemColorSubValues.push_back(color.g);
        elemColorSubValues.push_back(color.b);
        elemColorSubValues.push_back(color.a);
    }
    for (int i = 0; i < tileTypes; i++)
    {
        ui.findByKey(ELEMCLICK)[0].appendType<UIXRatio>(4,true)
            .appendType<UIXHolder>()
                .appendType<UIStack>()
                    .appendType<TexUVNode>(.5f,1.0f,.75f,1.0f).back()
                    .appendType<UIXHolder>()
                        .appendType<UIYShifter>(elemColorSubValues[i*4 + 0], elemColorSelecterWidth, i*4 + 0)
                            .appendType<TexUVNode>(0,.001f,.999f,1.0f).back()
                            .back()
                        .appendType<UIYShifter>(elemColorSubValues[i*4 + 1], elemColorSelecterWidth, i*4 + 1)
                            .appendType<TexUVNode>(0,.001f,.999f,1.0f).back()
                            .back()
                        .appendType<UIYShifter>(elemColorSubValues[i*4 + 2], elemColorSelecterWidth, i*4 + 2)
                            .appendType<TexUVNode>(0,.001f,.999f,1.0f).back()
                            .back()
                        .appendType<UIYShifter>(elemColorSubValues[i*4 + 3], elemColorSelecterWidth, i*4 + 3)
                            .appendType<TexUVNode>(0,.001f,.999f,1.0f).back()
                            .back()
                        .back()
                    .back()
                .appendType<UITextOneLineConst>(DataHolder::EDITOR, "Entity " + std::to_string(i), .35f, XLEFT)
        ;
    };

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

    // draw map
    for (int i = 0; i < layers; i++)
    {
        for (int j = 0; j < tileTypes; j++)
        {
            StaticDraw::useShader(batchTextures[i][j]);
            StaticDraw::multiDraw(0,layerBatches[i][j]);
        }
    }

    // write text
    StaticWrite::StartWrite();
    StaticWrite::DrawChannel(DataHolder::EDITOR, glm::vec3(0.0f, 0.0f, 0.0f));

    BackSceneStrict::render(time, updateDisplay);
}

void Editor::processInput(float time, GLFWwindow *ww)
{
    StaticInput::Tick();
    if (StaticInput::MouseHeld(GLFW_MOUSE_BUTTON_LEFT))
    {
        StaticInput::GetMouse(mouseX,mouseY);
        clickHeld(ui.findOneHover(mouseX, mouseY));
    }
    if (StaticInput::MouseClick(GLFW_MOUSE_BUTTON_LEFT))
    {
        // redundant but left in as a comment to avoid thinking I forgot to include it
        //StaticInput::GetMouse(mouseX,mouseY);
        buttonPress(ui.findOneHover(mouseX, mouseY));
    }
    for (int i = 0; i < tileTypes; i++)
    {
        if (StaticInput::KeyClick(GLFW_KEY_0+i))
        {
            elemSelected = i;
        }
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
    mapDims.loadUI(*mapZone);
    elemDims.loadUI(*elemZone);

    updateMapBatches();
}

void Editor::buttonPress(unsigned int x)
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
            break;
        }
        case LAYERLEFT:
        {
            break;
        }
        case LAYERRIGHT:
        {
            break;
        }
    }
}

void Editor::clickHeld(unsigned int x)
{
    switch (x)
    {
        case MAPCLICK:
        {
            mapPress();
            break;
        }
        case ELEMCLICK:
        {
            elemPress();
            break;
        }
    }
}

void Editor::mapPress()
{
    float fx = (mouseX - mapDims.xMin) / mapDims.xSize;
    float fy = (mouseY - mapDims.yMin) / mapDims.ySize;

    int ix = xSize * fx;
    int iy = ySize * (1.0f - fy);

    int previousEntity = getMapValue(layerSelected,ix,iy);
    DataHolder::god.layers[layerSelected][ix + (iy * xSize)] = elemSelected;
    updateElemBatch(layerSelected,elemSelected);
    updateElemBatch(layerSelected, previousEntity);
}

void Editor::elemPress()
{
    int elemPicked = (*elemZone)[0].findOneHover(mouseX, mouseY);
    if (elemPicked != -1)
    {
        UIElement& bar = (*elemZone)[0].findByKey(elemPicked);
        float fy = (mouseY - bar.yMin) / bar.ySize;
        elemColorSubValues[elemPicked] = fy;

        int i = elemPicked % 4;
        // add math for layers here?
        int j = elemPicked / 4;
        elemSelected = j;

        glm::vec4& color = DataHolder::god.colorList[j]; // add layers later
        StaticDraw::useShader(batchTextures[layerSelected][j]);
        GLint colorLoc = glGetUniformLocation(batchTextures[layerSelected][j], "color");
        color[i] = fy;
        glUniform4f(colorLoc, color.r, color.g, color.b, color.a);

        aspectChange();
    }
}

int Editor::getMapValue(int x, int y){return getMapValue(layerSelected, x, y);}
int Editor::getMapValue(int layer, int x, int y)
{
    if (x<0 || y<0 || x>=xSize || y>=ySize) {return -1;}
    return DataHolder::god.layers[layer][x + (y * xSize)];
}

void Editor::updateElemBatch(int elem){updateElemBatch(layerSelected, elem);}
void Editor::updateElemBatch(int layer, int elem)
{
    if (elem < 0)
    {
        return;
    }
    float xWidth = mapDims.xSize / xSize;
    float yWidth = mapDims.ySize / ySize;
    layerBatches[layer][elem].clear();
    for (int y = 0; y < ySize; y++)
    {
        for (int x = 0; x < xSize; x++)
        {
            if (DataHolder::god.layers[layer][x + (y * xSize)] == elem)
            {
                float xMin = mapDims.xMin + (x * xWidth);
                float xMax = xMin + xWidth;

                float yMin = mapDims.yMin + mapDims.ySize - ((y + 1) * yWidth); // this is backwards and that is ok for now easy fix later
                float yMax = yMin + yWidth;


                layerBatches[layer][elem].insert(
                    layerBatches[layer][elem].end(),
                    {
                        xMax, yMax, 1, 1,
                        xMax, yMin, 1, 0,
                        xMin, yMin, 0, 0,

                        xMax, yMax, 1, 1,
                        xMin, yMin, 0, 0,
                        xMin, yMax, 0, 1
                    });
            }
        }
    }
}

void Editor::updateMapBatches()
{
    for (int i = 0; i < layers; i++)
    {
        for (int j = 0; j < layerBatches[i].size(); j++)
        {
            updateElemBatch(i,j);
        }
    }
}


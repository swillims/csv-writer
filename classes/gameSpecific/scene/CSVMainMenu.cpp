#include "CSVMainMenu.h"

#include "singleton/dataHolder.h"
#include "singleton/staticInput.h"

enum MAINMENUBUTTONS
{
    LEFTUPPERUI,
    RIGHTUPPERUI,

    COLLUMNONEUP,
    COLLUMNONEDOWN,
    COLLUMNTWOUP,
    COLLUMNTWODOWN,
    COLLUMNTHREEUP,
    COLLUMNTHREEDOWN,
    COLLUMNFOURUP,
    COLLUMNFOURDOWN,

    ROWONEUP,
    ROWONEDOWN,
    ROWTWOUP,
    ROWTWODOWN,
    ROWTHREEUP,
    ROWTHREEDOWN,
    ROWFOURUP,
    ROWFOURDOWN,

    TILECOUNTUP,
    TILECOUNTDOWN,

    LAYERCOUNTUP,
    LAYERCOUNTDOWN,
};

void CSVMainMenu::onLoad()
{
    Scene::onLoad();

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

    if (!alreadyLoaded)
    {
        std::vector<float> uiDividerRatio = {.4f,.6f};

        ui.appendType<UIYSplits>(std::vector<float>{.1f,.9f})
            .appendType<UIXSplits>(std::vector<float>{.2f,.6f,.2f})
                .appendType<UIXRatio>(2,true)
                    .appendType<UIStack>()
                        .appendType<TexUVNode>(0,.5f,.75,1.0f,LEFTUPPERUI).back()
                        .appendType<UITextOneLine>(DataHolder::MEANMENU, upperLeftStr, .8 ).back()
                        .back()
                    .back()
                .appendType<UIXRatio>(6,true)
                    .appendType<UITextOneLine>(DataHolder::MEANMENU,upperCenterStr,.5f).back()
                    .back()
                .appendType<UIXRatio>(2,true)
                    .appendType<UIStack>()
                        .appendType<TexUVNode>(0,.5f,.75,1.0f,RIGHTUPPERUI).back()
                        .appendType<UITextOneLine>(DataHolder::MEANMENU, upperRightStr, .8 ).back()
                        .back()
                    .back()
                .back()
            .appendType<UIYHolder>()
                .appendType<UIXSplits>(uiDividerRatio)
                    .appendType<UIXRatio>(4,true)
                        .appendType<UITextOneLine>(DataHolder::MEANMENU, columnTitle, .5f ).back()
                        .back()
                    .appendType<UIXHolder>()
                        .appendType<UIYHolder>()
                            .appendType<TexUVNode>(.5f,.75f,0.0f,.25f,COLLUMNONEUP).back()
                            .appendType<UITextOneLine>(DataHolder::MEANMENU, columnValue1, .8 ).back()
                            .appendType<TexUVNode>(0.0f,.25f,0.0f,0.25f,COLLUMNONEDOWN).back()
                            .back()
                        .appendType<UIYHolder>()
                            .appendType<TexUVNode>(.5f,.75f,0.0f,.25f,COLLUMNTWOUP).back()
                            .appendType<UITextOneLine>(DataHolder::MEANMENU, columnValue2, .8 ).back()
                            .appendType<TexUVNode>(0.0f,.25f,0.0f,0.25f,COLLUMNTWODOWN).back()
                            .back()
                        .appendType<UIYHolder>()
                            .appendType<TexUVNode>(.5f,.75f,0.0f,.25f,COLLUMNTHREEUP).back()
                            .appendType<UITextOneLine>(DataHolder::MEANMENU, columnValue3, .8 ).back()
                            .appendType<TexUVNode>(0.0f,.25f,0.0f,0.25f,COLLUMNTHREEDOWN).back()
                            .back()
                        .appendType<UIYHolder>()
                            .appendType<TexUVNode>(.5f,.75f,0.0f,.25f,COLLUMNFOURUP).back()
                            .appendType<UITextOneLine>(DataHolder::MEANMENU, columnValue4, .8 ).back()
                            .appendType<TexUVNode>(0.0f,.25f,0.0f,0.25f,COLLUMNFOURDOWN).back()
                            .back()
                        .back()
                    .back()
                .appendType<UIXSplits>(uiDividerRatio)
                    .appendType<UIXRatio>(4,true)
                        .appendType<UITextOneLine>(DataHolder::MEANMENU, rowsTitle, .5f ).back()
                        .back()
                    .appendType<UIXHolder>()
                        .appendType<UIYHolder>()
                            .appendType<TexUVNode>(.5f,.75f,0.0f,.25f,ROWONEUP).back()
                            .appendType<UITextOneLine>(DataHolder::MEANMENU, rowsValue1, .8 ).back()
                            .appendType<TexUVNode>(0.0f,.25f,0.0f,0.25f,ROWONEDOWN).back()
                            .back()
                        .appendType<UIYHolder>()
                            .appendType<TexUVNode>(.5f,.75f,0.0f,.25f,ROWTWOUP).back()
                            .appendType<UITextOneLine>(DataHolder::MEANMENU, rowsValue2, .8 ).back()
                            .appendType<TexUVNode>(0.0f,.25f,0.0f,0.25f,ROWTWODOWN).back()
                            .back()
                        .appendType<UIYHolder>()
                            .appendType<TexUVNode>(.5f,.75f,0.0f,.25f,ROWTHREEUP).back()
                            .appendType<UITextOneLine>(DataHolder::MEANMENU, rowsValue3, .8 ).back()
                            .appendType<TexUVNode>(0.0f,.25f,0.0f,0.25f,ROWTHREEDOWN).back()
                            .back()
                        .appendType<UIYHolder>()
                            .appendType<TexUVNode>(.5f,.75f,0.0f,.25f,ROWFOURUP).back()
                            .appendType<UITextOneLine>(DataHolder::MEANMENU, rowsValue4, .8 ).back()
                            .appendType<TexUVNode>(0.0f,.25f,0.0f,0.25f,ROWFOURDOWN).back()
                            .back()
                        .back()
                    .back()
                .appendType<UIXSplits>(uiDividerRatio)
                    .appendType<UIXRatio>(4,true)
                        .appendType<UITextOneLine>(DataHolder::MEANMENU, tileTitle, .5f ).back()
                        .back()
                    .appendType<UIXHolder>()
                        .appendType<TexUVNode>(.75f, 1.0f, 0.0f,.25f,TILECOUNTDOWN).back()
                        .appendType<UITextOneLine>(DataHolder::MEANMENU, tileValue, .8 ).back()
                        .appendType<TexUVNode>(.25f,.5f,0.0f,.25f,TILECOUNTUP).back()
                        .back()
                    .back()
                .appendType<UIXSplits>(uiDividerRatio)
                    .appendType<UIXRatio>(4,true)
                        .appendType<UITextOneLine>(DataHolder::MEANMENU, layerTitle, .5f ).back()
                        .back()
                    .appendType<UIXHolder>()
                        .appendType<TexUVNode>(.75f, 1.0f, 0.0f,.25f,LAYERCOUNTDOWN).back()
                        .appendType<UITextOneLine>(DataHolder::MEANMENU, layerValue, .8 ).back()
                        .appendType<TexUVNode>(.25f,.5f,0.0f,.25f,LAYERCOUNTUP).back()
                        .back()
        ;


    }

    aspectChange();
}

void CSVMainMenu::handle(float time)
{
    processInput(time, window);
}

void CSVMainMenu::render(float time = 0, bool updateDisplay = true)
{
    // clear
    StaticDraw::clear(.8,.8,.8);

    // switch back to default shader
    StaticDraw::useShader(shaderSimpleRef);

    // draw ui Elements from batch
    StaticDraw::multiDraw(uiTextureRef, batch);

    // write text
    StaticWrite::StartWrite();
    StaticWrite::DrawChannel(DataHolder::MEANMENU, glm::vec3(0.0f, 0.0f, 0.0f));

    Scene::render(time, updateDisplay);
}

void CSVMainMenu::buttonPress(unsigned int x)
{
    switch (x)
    {
        case LEFTUPPERUI :
            glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
            break;

        case RIGHTUPPERUI :
            ; // add code for changing scenes
            break;

        case COLLUMNONEUP:
            xSize += 1000;
            if (xSize > 9999){xSize = 9999;}
            aspectChange();
            break;
        case COLLUMNTWOUP :
            xSize += 100;
            if (xSize > 9999){xSize = 9999;}
            aspectChange();
            break;
        case COLLUMNTHREEUP:
            xSize += 10;
            if (xSize > 9999){xSize = 9999;}
            aspectChange();
            break;
        case COLLUMNFOURUP :
            xSize += 1;
            if (xSize > 9999){xSize = 9999;}
            aspectChange();
            break;

        case COLLUMNONEDOWN:
            xSize -= 1000;
            if (xSize > 9999 || xSize == 0){xSize = 1;} // 9999 checks for underflow
            aspectChange();
            break;
        case COLLUMNTWODOWN :
            xSize -= 100;
            if (xSize > 9999 || xSize == 0){xSize = 1;} // 9999 checks for underflow
            aspectChange();
            break;
        case COLLUMNTHREEDOWN:
            xSize -= 10;
            if (xSize > 9999 || xSize == 0){xSize = 1;} // 9999 checks for underflow
            aspectChange();
            break;
        case COLLUMNFOURDOWN :
            xSize -= 1;
            if (xSize > 9999 || xSize == 0){xSize = 1;} // 9999 checks for underflow
            aspectChange();
            break;

        case ROWONEUP:
            ySize += 1000;
            if (ySize > 9999){ySize = 9999;}
            aspectChange();
            break;
        case ROWTWOUP:
            ySize += 100;
            if (ySize > 9999){ySize = 9999;}
            aspectChange();
            break;
        case ROWTHREEUP:
            ySize += 10;
            if (ySize > 9999){ySize = 9999;}
            aspectChange();
            break;
        case ROWFOURUP:
            ySize += 1;
            if (ySize > 9999){ySize = 9999;}
            aspectChange();
            break;

        case ROWONEDOWN:
            ySize -= 1000;
            if (ySize > 9999 || ySize == 0){ySize = 1;}
            aspectChange();
            break;
        case ROWTWODOWN:
            ySize -= 100;
            if (ySize > 9999 || ySize == 0){ySize = 1;}
            aspectChange();
            break;
        case ROWTHREEDOWN:
            ySize -= 10;
            if (ySize > 9999 || ySize == 0){ySize = 1;}
            aspectChange();
            break;
        case ROWFOURDOWN:
            ySize -= 1;
            if (ySize > 9999 || ySize == 0){ySize = 1;}
            aspectChange();
            break;

        case TILECOUNTUP:
            tileTypes += 1;
            aspectChange();
            break;
        case TILECOUNTDOWN:
            tileTypes -= 1;
            if (tileTypes < 1){tileTypes = 1;}
            aspectChange();
            break;

        case LAYERCOUNTUP:
            layers += 1;
            aspectChange();
            break;
        case LAYERCOUNTDOWN:
            layers -= 1;
            if (layers < 1){layers = 1;}
            aspectChange();
            break;
        default: ; // adding default makes a warning go away.
    }
    if (x==LEFTUPPERUI)
    {
        glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
    }
    else if (x==RIGHTUPPERUI)
    {
        // code for next scene
    }
}

void CSVMainMenu::processInput(float time, GLFWwindow *ww)
{
    StaticInput::Tick();
    if (StaticInput::MouseClick(GLFW_MOUSE_BUTTON_LEFT))
    {
        double x, y;
        StaticInput::GetMouse(x,y);
        buttonPress(ui.findOneHover(x, y));
    }
}

void CSVMainMenu::aspectChange()
{
    // calculate strings
    columnValue1 = std::to_string((xSize/1000)%10);
    columnValue2 = std::to_string((xSize/100)%10);
    columnValue3 = std::to_string((xSize/10)%10);
    columnValue4 = std::to_string((xSize/1)%10);

    rowsValue1 = std::to_string((ySize/1000)%10);
    rowsValue2 = std::to_string((ySize/100)%10);
    rowsValue3 = std::to_string((ySize/10)%10);
    rowsValue4 = std::to_string((ySize/1)%10);

    tileValue = std::to_string(tileTypes);
    layerValue = std::to_string(layers);

    // make new batches
    batch.clear();
    StaticWrite::SetUpChannel(DataHolder::MEANMENU);
    StaticDraw::updateView();
    ui.adjustNodeDefault();
    ui.renderVerts(batch);
}

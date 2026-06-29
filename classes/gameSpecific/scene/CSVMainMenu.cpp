#include "CSVMainMenu.h"

#include "singleton/dataHolder.h"

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

    // declared on StaticDraw Init
    shaderSimpleRef = StaticDraw::getShader("simple");

    //if (!StaticDraw::imageFileRefs.contains("mainMenuBackground"))
    //{
    //    StaticDraw::loadImage("assets/core/background.png", "mainMenuBackground", false);
    //}
    //backgroundRef = StaticDraw::imageFileRefs["mainMenuBackground"];

    if (!StaticDraw::imageFileRefs.contains("uiTexture"))
    {
        StaticDraw::loadImage("assets/gamespecific/png/ui_elems.png", "uiTexture", false);
    }
    uiTextureRef = StaticDraw::imageFileRefs["uiTexture"];

    if (!alreadyLoaded)
    {
        std::vector<float> uiDividerRatio = {.4f,.6f};

        ui.appendType<UIYSplits>(std::vector<float>{.1f,.9f})
            .appendType<UIXSplits>(std::vector<float>{.2f,.6f,.2f})
                .appendType<UIXRatio>(2,true)
                    .appendType<UIStack>(LEFTUPPERUI)
                        .appendType<TexUVNode>(0,.5f,.75,1.0f).back()
                        .appendType<UITextOneLine>(DataHolder::MEANMENU, upperLeftStr, .8 ).back()
                        .back()
                    .back()
                .appendType<UIXRatio>(6,true)
                    .appendType<UITextOneLine>(DataHolder::MEANMENU,upperCenterStr,.5f).back()
                    .back()
                .appendType<UIXRatio>(2,true)
                    .appendType<UIStack>(RIGHTUPPERUI)
                        .appendType<TexUVNode>(0,.5f,.75,1.0f).back()
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
                            .appendType<TexUVNode>(0.0f,.25f,0.0f,0.25f,ROWONEUP).back()
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

void CSVMainMenu::processInput(float time, GLFWwindow *ww)
{

}

void CSVMainMenu::aspectChange()
{
    std::cout << "aspectChange\n";
    batch.clear();
    StaticWrite::SetUpChannel(DataHolder::MEANMENU);
    StaticDraw::updateView();
    ui.adjustNodeDefault();
    ui.renderVerts(batch);

    std::cout << batch.size() << "\n";
    // figure out text
}

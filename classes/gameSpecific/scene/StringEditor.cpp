#include "StringEditor.h"

#include "singleton/dataHolder.h"
#include "singleton/staticDraw.h"
#include "singleton/staticInput.h"

enum Button
{
    CANCEL,
    UPDATE,

    WINR,
    NEWLINE,
    TAB,
};

void StringEditor::onLoad()
{
    BackSceneStrict::onLoad();

    actualString = source;
    displayString = util::translateToHumanReadable(actualString);

    shaderSimpleRef = StaticDraw::getShader("simple");

    // load ui texture
    if (!StaticDraw::imageFileRefs.contains("uiTexture"))
    {
        StaticDraw::loadImage("assets/gamespecific/png/ui_elems.png", "uiTexture", false);
    }
    uiTextureRef = StaticDraw::imageFileRefs["uiTexture"];

    StaticInput::MouseTrack(GLFW_MOUSE_BUTTON_LEFT);

    for (int i=GLFW_KEY_A; i<=GLFW_KEY_Z; i++)
    {
        StaticInput::KeyTrack(i);
    }
    for (int i=GLFW_KEY_0; i<=GLFW_KEY_9; i++)
    {
        StaticInput::KeyTrack(i);
    }
    StaticInput::KeyTrack(GLFW_KEY_TAB);
    StaticInput::KeyTrack(GLFW_KEY_BACKSPACE);
    StaticInput::KeyTrack(GLFW_KEY_ENTER);
    StaticInput::KeyTrack(GLFW_KEY_LEFT_SHIFT);
    StaticInput::KeyTrack(GLFW_KEY_RIGHT_SHIFT);
    StaticInput::KeyTrack(GLFW_KEY_SPACE);
    StaticInput::KeyTrack(GLFW_KEY_MINUS);
    StaticInput::KeyTrack(GLFW_KEY_EQUAL);
    StaticInput::KeyTrack(GLFW_KEY_COMMA);
    StaticInput::KeyTrack(GLFW_KEY_PERIOD);
    StaticInput::KeyTrack(GLFW_KEY_SLASH);
    StaticInput::KeyTrack(GLFW_KEY_LEFT_BRACKET);
    StaticInput::KeyTrack(GLFW_KEY_RIGHT_BRACKET);
    StaticInput::KeyTrack(GLFW_KEY_BACKSLASH);
    StaticInput::KeyTrack(GLFW_KEY_SEMICOLON);
    StaticInput::KeyTrack(GLFW_KEY_APOSTROPHE);
    StaticInput::KeyTrack(GLFW_KEY_GRAVE_ACCENT);

    ui.appendType<UIXRatio>(2)
        .appendType<UIYHolder>()
            .appendType<UIXRefRatio>(editStringSize)
                .appendType<UITextOneLine>(DataHolder::STRINGEDIT, displayString, .5).back()
                .back()
            .appendType<UIXHolder>()
                .appendType<UIXRatio>(2)
                    .appendType<UIStack>()
                        .appendType<TexUVNode>(0,.5f,.75,1.0f,NEWLINE).back()
                        .appendType<UITextOneLineConst>(DataHolder::STRINGEDIT, "Add \\n", .5).back()
                        .back()
                    .back()
                .appendType<UIXRatio>(2)
                    .appendType<UIStack>()
                        .appendType<TexUVNode>(0,.5f,.75,1.0f,TAB).back()
                        .appendType<UITextOneLineConst>(DataHolder::STRINGEDIT, "Add \\t", .5).back()
                        .back()
                    .back()
                .appendType<UIXRatio>(2)
                    .appendType<UIStack>()
                        .appendType<TexUVNode>(0,.5f,.75,1.0f,WINR).back()
                        .appendType<UITextOneLineConst>(DataHolder::STRINGEDIT, "Add \\r", .5).back()
                        .back()
                    .back()
                .back()
            .appendType<UIXHolder>()
                .appendType<UIXRatio>(2)
                    .appendType<UIStack>()
                        .appendType<TexUVNode>(0,.5f,.75,1.0f,CANCEL).back()
                        .appendType<UITextOneLineConst>(DataHolder::STRINGEDIT, "Cancel", .5).back()
                        .back()
                    .back()
                .appendType<UIXRatio>(2)
                    .appendType<UIStack>()
                        .appendType<TexUVNode>(0,.5f,.75,1.0f,UPDATE).back()
                        .appendType<UITextOneLineConst>(DataHolder::STRINGEDIT, "Update", .5).back()
    ;

    aspectChange();
}

void StringEditor::handle(float time)
{
    processInput(time, window);
}

void StringEditor::render(float time, bool updateDisplay)
{
    // clear
    StaticDraw::clear(.8,.8,.8);

    // it is possible to render previous layer but not worth it
    // after rendering previous layer use a color shader set to like .8,.8,.8,.5 to make a slightly greyed out background

    // switch back to default shader
    StaticDraw::useShader(shaderSimpleRef);

    // draw ui Elements from batch
    StaticDraw::multiDraw(uiTextureRef, batch);

    // write text
    StaticWrite::StartWrite();
    StaticWrite::DrawChannel(DataHolder::STRINGEDIT, glm::vec3(0.0f, 0.0f, 0.0f));

    BackSceneStrict::render(time, updateDisplay);
}

void StringEditor::processInput(float time, GLFWwindow *ww)
{
    StaticInput::Tick();
    if (StaticInput::MouseClick(GLFW_MOUSE_BUTTON_LEFT))
    {
        double x,y;
        StaticInput::GetMouse(x,y);
        buttonPress(ui.findOneHover(x, y));
    }
    bool shift = StaticInput::KeyHeld(GLFW_KEY_LEFT_SHIFT) || StaticInput::KeyHeld(GLFW_KEY_RIGHT_SHIFT);
    bool change = false;
    for (int i=0; i<GLFW_KEY_LAST; i++)
    {
        if (StaticInput::KeyClick(i))
        {
            keyAppend(i,shift);
            change = true;
        }
    }
    if (change){aspectChange();}
}

void StringEditor::aspectChange()
{
    displayString = util::translateToHumanReadable(actualString);
    editStringSize = stringToFloatConstant * static_cast<float>(displayString.size());

    BackSceneStrict::aspectChange();

    // make new batches
    batch.clear();
    StaticWrite::SetUpChannel(DataHolder::STRINGEDIT);
    StaticDraw::updateView();
    ui.adjustNodeDefault();
    ui.renderVerts(batch);
}

void StringEditor::clean()
{
    StaticInput::KeyTrackSetAll(false); // too many loose ends
}

void StringEditor::buttonPress(unsigned int x)
{
    switch (x)
    {
        case CANCEL:
        {
            DataHolder::SceneQueue(previous,true);
            break;
        }
        case UPDATE:
        {
            source = actualString;
            DataHolder::SceneQueue(previous,true);
            break;
        }
        case TAB:
        {
            actualString += "\t";
            aspectChange();
            break;
        }
        case NEWLINE:
        {
            actualString += "\n";
            aspectChange();
            break;
        }
        case WINR:
        {
            actualString += "\r";
            aspectChange();
            break;
        }
    }
}

void StringEditor::keyAppend(unsigned int key, bool shift)
{
    if (key == GLFW_KEY_BACKSPACE)
    {
        if (!actualString.empty())
        {
            actualString.pop_back();
        }
        return;
    }

    char a = StaticInput::keyToChar(key, shift);

    if (a != '\0')
    {
        actualString += a;
    }
}

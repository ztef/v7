
#include <vsg/all.h>
#include "InputKeyboardValues.h"


class InputHandler : public vsg::Inherit<vsg::Visitor, InputHandler>
{
public:
    vsg::ref_ptr<vsg::Text> keyboard_text;
    vsg::ref_ptr<vsg::Text> pointer_text;
    vsg::ref_ptr<vsg::Text> scroll_text;
    vsg::ref_ptr<vsg::Text> window_text;
    vsg::ref_ptr<vsg::Text> frame_text;
    InputKeyboardValues* keyb;

    InputHandler(vsg::ref_ptr<vsg::Text> in_keyboard_text, vsg::ref_ptr<vsg::Text> in_pointer_text, InputKeyboardValues* in_keyb) :
        keyboard_text(in_keyboard_text),
        pointer_text(in_pointer_text),
        keyb(in_keyb)
    {
    }

    void assign(vsg::Text& text, const std::string& str)
    {
        auto text_string = text.text.cast<vsg::stringValue>();

        if (text_string)
        {
            text_string->value() = str;
        }
        else
        {
            text.text = text_string = vsg::stringValue::create(str);
        }

        text.setup();
    }

    void apply(vsg::KeyPressEvent& keyPress) override
    {
        assign(*keyboard_text, vsg::make_string(keyPress.className(), ", keyBase=", keyPress.keyBase, " ", char(keyPress.keyBase), ", keyModified=", keyPress.keyModified, " ", char(keyPress.keyModified)));

        keyb->lastkey = char(keyPress.keyModified);
    }

    void apply(vsg::KeyReleaseEvent& keyRelease) override
    {
        assign(*keyboard_text, vsg::make_string(keyRelease.className(), ", keyBase=", keyRelease.keyBase, " ", char(keyRelease.keyBase), ", keyModified=", keyRelease.keyModified, " ", char(keyRelease.keyModified)));
    }

    void apply(vsg::MoveEvent& moveEvent) override
    {
        // note under Unix the mask value includes the keyboard modifiers in mask as the native mask is used
        assign(*pointer_text, vsg::make_string(moveEvent.className(), ", x=", moveEvent.x, ", y= ", moveEvent.y, ", mask=", moveEvent.mask, " [",
                                               ((moveEvent.mask & vsg::BUTTON_MASK_1) != 0), ((moveEvent.mask & vsg::BUTTON_MASK_2) != 0), ((moveEvent.mask & vsg::BUTTON_MASK_3) != 0), "]"));
    }

    void apply(vsg::ButtonPressEvent& buttonPress) override
    {
        // note under Unix the mask value includes the keyboard modifiers in mask as the native mask is used
        assign(*pointer_text, vsg::make_string(buttonPress.className(), ", x=", buttonPress.x, ", y=", buttonPress.y, ", mask=", buttonPress.mask, " [",
                                               ((buttonPress.mask & vsg::BUTTON_MASK_1) != 0), ((buttonPress.mask & vsg::BUTTON_MASK_2) != 0), ((buttonPress.mask & vsg::BUTTON_MASK_3) != 0), "], button=", buttonPress.button));
    }

    void apply(vsg::ButtonReleaseEvent& buttonRelease) override
    {
        // note under Unix the mask value includes the keyboard modifiers in mask as the native mask is used
        assign(*pointer_text, vsg::make_string(buttonRelease.className(), ", x=", buttonRelease.x, ", y=", buttonRelease.y, ", mask=", buttonRelease.mask, " [",
                                               ((buttonRelease.mask & vsg::BUTTON_MASK_1) != 0), ((buttonRelease.mask & vsg::BUTTON_MASK_2) != 0), ((buttonRelease.mask & vsg::BUTTON_MASK_3) != 0), "], button=", buttonRelease.button));
    }

   

protected:
};

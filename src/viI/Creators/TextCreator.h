#include <vsg/all.h>



class TextCreator {

public:


static vsg::ref_ptr<vsg::Text> create(std::string texto, float x, float y, float z,  float sz, vsg::ref_ptr<vsg::Font> font, vsg::ref_ptr<vsg::Options> options ){

            auto layout = vsg::StandardLayout::create();
                        layout->glyphLayout = vsg::StandardLayout::LEFT_TO_RIGHT_LAYOUT;
                        layout->position = vsg::vec3(x, y, z);
                        layout->horizontal = vsg::vec3(sz, 0.0, 0.0);
                        layout->vertical = vsg::vec3(0.0, 0.0, sz);
                        layout->color = vsg::vec4(0.0, 1.0, 0.0, 1.0);

                        

                        auto text = vsg::Text::create();
                        text->text = vsg::stringValue::create(texto);
                        text->font = font;

                        text->layout = layout;

                        text->setup(0, options);

                        return text;

}

};
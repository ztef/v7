
#include <stdio.h>
#include <iostream>

#include <vsgImGui/RenderImGui.h>
#include <vsgImGui/SendEventsToImGui.h>
#include <vsgImGui/Texture.h>
#include <vsgImGui/imgui.h>
#include <vsgImGui/implot.h>
#include "../Interaction/Handlers/InputKeyboardValues.h"
#include "../Interaction/Handlers/SceneController.h"

#include <vsg/all.h>

struct Params : public vsg::Inherit<vsg::Object, Params>
{
    bool showGui = true; // you can toggle this with your own EventHandler and key
    bool showDemoWindow = false;
    bool showSecondWindow = false;
    bool showImPlotDemoWindow = false;
    bool showLogoWindow = true;
    bool showImagesWindow = false;
    float clearColor[3]{0.2f, 0.2f, 0.4f}; // Unfortunately, this doesn't change dynamically in vsg
    uint32_t counter = 0;
    float dist = 0.f;
};




class Main_menu : public vsg::Inherit<vsg::Command, Main_menu>
{
public:
    vsg::ref_ptr<vsg::SetLineWidth> setLineWidth;
    InputKeyboardValues * keyb; 
    SceneController * scenecontroller;
    void load();

    Main_menu(vsg::ref_ptr<vsg::SetLineWidth> in_setLineWidth,  InputKeyboardValues * in_keyb, SceneController * in_sc) :
        setLineWidth(in_setLineWidth),
        keyb(in_keyb),
        scenecontroller(in_sc)
    {
    }

    // Example here taken from the Dear imgui comments (mostly)
    void record(vsg::CommandBuffer&) const override
    {

        ImGui::SetNextWindowSize(ImVec2(400, 300)); 
        ImGui::Begin("viExplorer");
        ImGui::Text(keyb->lastkey.c_str()); 
        // Array of items for the listbox
            static const char* items[] = { "Escenario XYZ", "Escenario Radial", "Escenario Elíptico", "Escenario Geográfico" };

            // Index of the currently selected item
            static int currentItemIndex = 0;

            // Listbox control
            if (ImGui::ListBox("Escenarios", &currentItemIndex, items, IM_ARRAYSIZE(items)))
            {
                // Handle selection change
                // currentItemIndex contains the index of the selected item

                scenecontroller->setScene(currentItemIndex);


            }
        
        ImGui::SliderFloat("Line Width", &(setLineWidth->lineWidth), 1, 5);
        ImGui::End();
    }
};








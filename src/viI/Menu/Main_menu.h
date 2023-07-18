
#include <stdio.h>
#include <iostream>

#include <vsgImGui/RenderImGui.h>
#include <vsgImGui/SendEventsToImGui.h>
#include <vsgImGui/Texture.h>
#include <vsgImGui/imgui.h>
#include <vsgImGui/implot.h>

#include "../Interaction/VIConsole.h"

#include "../Interaction/Handlers/InputKeyboardValues.h"
#include "../Interaction/Handlers/SceneController.h"
#include "../Command/CommandProcessor.h"

#include <vsg/all.h>

struct Params : public vsg::Inherit<vsg::Object, Params>
{
    bool showGui = true; // you can toggle this with your own EventHandler and key
    bool showDemoWindow = false;
    bool showSecondWindow = false;
    bool showImPlotDemoWindow = false;
    bool showLogoWindow = true;
    bool showAboutWindow = false;
    bool showConsoleWindow = false;
    bool showImagesWindow = false;
    float clearColor[3]{0.2f, 0.2f, 0.4f}; // Unfortunately, this doesn't change dynamically in vsg
    uint32_t counter = 0;
    float dist = 0.f;
};



class Main_menu : public vsg::Inherit<vsg::Command, Main_menu>
{
public:
    vsg::ref_ptr<vsgImGui::Texture> logo;
    vsg::ref_ptr<vsgImGui::Texture> about;
    vsg::ref_ptr<vsg::SetLineWidth> setLineWidth;
    vsg::ref_ptr<Params> params;
    InputKeyboardValues * keyb; 
    SceneController * scenecontroller;
    CommandProcessor * commandProcessor;
    //static ExampleAppConsole console;

    


    void load();

    Main_menu(vsg::ref_ptr<Params> in_params, vsg::ref_ptr<vsg::Options> options, vsg::ref_ptr<vsg::SetLineWidth> in_setLineWidth,  InputKeyboardValues * in_keyb, SceneController * in_sc, CommandProcessor * in_cp) :
        params(in_params),
        setLineWidth(in_setLineWidth),
        keyb(in_keyb),
        scenecontroller(in_sc),
        commandProcessor(in_cp)
    {


       
        auto texData = vsg::read_cast<vsg::Data>("./textures/vilogow.png", options);
       
       logo = vsgImGui::Texture::create_if(texData, texData);

       auto texData1 = vsg::read_cast<vsg::Data>("./textures/ojo.jpeg", options);
       about = vsgImGui::Texture::create_if(texData1, texData1); 
      
    }

    void compile(vsg::Context& context) override
    {
       if (logo) logo->compile(context);
       if (about) about->compile(context);
    }



    // Example here taken from the Dear imgui comments (mostly)
    void record(vsg::CommandBuffer& cb) const override
    {

        ImGui::SetNextWindowSize(ImVec2(400, 300)); 
        ImGui::Begin("viExplorer");
        ImGui::Text(keyb->lastkey.c_str()); 
        // Array of items for the listbox
            static const char* items[] = { "XYZ World", "Radial World", "Radial Depth World","Spherical World", "Cylindrical world" ,"Geographic World" };

            // Index of the currently selected item
            static int currentItemIndex = 0;

            // Listbox control
            if (ImGui::ListBox("SCENE TYPES", &currentItemIndex, items, IM_ARRAYSIZE(items)))
            {
                // Handle selection change
                // currentItemIndex contains the index of the selected item

                scenecontroller->setScene(currentItemIndex);


            }
        
        ImGui::SliderFloat("Line Width", &(setLineWidth->lineWidth), 1, 5);
        ImGui::Checkbox("Console", &params->showConsoleWindow);
        ImGui::Checkbox("About", &params->showAboutWindow);
        ImGui::End();

        ImVec2 squareUV(static_cast<float>(logo->height) / logo->width, 1.0f);
         // Copied from imgui_demo.cpp simple overlay
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
                const float PAD = 10.0f;
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
                ImVec2 work_size = viewport->WorkSize;
                ImVec2 window_pos, window_pos_pivot;
                window_pos.x = work_pos.x + PAD;
                window_pos.y = work_pos.y + work_size.y - PAD;
                window_pos_pivot.x = 0.0f;
                window_pos_pivot.y = 1.0f;
                ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
                window_flags |= ImGuiWindowFlags_NoMove;
                ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                ImGui::Begin("vsgCS UI", nullptr, window_flags);

                // Display a square from the VSG logo
                const float size = 128.0f;
                ImGui::Image(logo->id(cb.deviceID), ImVec2(size, size), ImVec2(0.0f, 0.0f), squareUV);

                ImGui::End();
                ImGui::PopStyleVar();


        if(params->showAboutWindow){

              ImGui::SetNextWindowSize(ImVec2(500, 550)); 
              ImGui::Begin("About", &params->showAboutWindow);
             
             
                    ImGui::Image(about->id(cb.deviceID), ImVec2(about->width / 2.0f, about->height / 2.0f));
                    ImGui::Text("Visual Interactor v0.0.1");
                    const float size = 90.0f;
                    ImGui::Image(logo->id(cb.deviceID), ImVec2(size, size), ImVec2(0.0f, 0.0f));
                    ImGui::Text("Visual Interaction Systems Corp. ");

                
              ImGui::End();
        }
         if(params->showConsoleWindow){

              
             
             
               static VIConsole console(commandProcessor);
               console.Draw("viSQL Console",&params->showConsoleWindow);

                
             
        }


    }
};








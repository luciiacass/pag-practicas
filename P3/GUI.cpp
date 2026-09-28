//
// Created by luuca on 26/09/2026.
//

#include "GUI.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace PAG {
    GUI* GUI::instancia = nullptr;

    GUI::GUI () { }

    GUI::~GUI () { }

    GUI& GUI::getInstancia () {
        if ( !instancia ) {
            instancia = new GUI ();
        }
        return *instancia;
    }

    void GUI::init ( GLFWwindow* window ) {
        IMGUI_CHECKVERSION ();
        ImGui::CreateContext ();
        ImGuiIO& io = ImGui::GetIO ();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplGlfw_InitForOpenGL ( window, true );
        ImGui_ImplOpenGL3_Init ();
    }

    void GUI::shutdown () {
        ImGui_ImplOpenGL3_Shutdown ();
        ImGui_ImplGlfw_Shutdown ();
        ImGui::DestroyContext ();
    }

    void GUI::newFrame () {
        ImGui_ImplOpenGL3_NewFrame ();
        ImGui_ImplGlfw_NewFrame ();
        ImGui::NewFrame ();
    }

    void GUI::draw () {
        messagesWindow ();
        backgroundWindow ();
    }

    void GUI::render () {
        ImGui::Render ();
        ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData () );
    }

    // - Ventana donde se muestran los mensajes de la aplicación
    void GUI::messagesWindow () {

        ImGui::SetNextWindowPos ( ImVec2 ( 10, 10 ), ImGuiCond_Once );
        ImGui::SetNextWindowSize ( ImVec2 ( 400, 250 ), ImGuiCond_Once );


        if ( ImGui::Begin ( "Mensajes" ) ) {
            ImGui::SetWindowFontScale ( 1.0f );

            for ( unsigned int i = 0; i < messages.size (); i++ ) {
                ImGui::TextUnformatted ( messages[i].c_str () );
            }

            if ( ImGui::GetScrollY () >= ImGui::GetScrollMaxY () ) {
                ImGui::SetScrollHereY ( 1.0f );
            }
        }
        ImGui::End ();
    }

    // - Ventana con el selector del color de fondo
    void GUI::backgroundWindow () {
        ImGui::SetNextWindowPos ( ImVec2 ( 430, 10 ), ImGuiCond_Once );
        if ( ImGui::Begin ( "Color de fondo", nullptr, ImGuiWindowFlags_AlwaysAutoResize ) ) {
            ImGui::SetWindowFontScale ( 1.0f );

            // - ColorPicker3 devuelve true cuando el usuario cambia el color
            if ( ImGui::ColorPicker3 ( "Fondo", bgColor, ImGuiColorEditFlags_PickerHueWheel  ) ) {
                colorChanged = true;
            }
        }
        ImGui::End ();
    }


    void GUI::addMessage ( const std::string& message ) {
        messages.push_back ( message );
    }

    // - Traslada a Dear ImGui los eventos de ratón que recibe GLFW
    void GUI::mouseButtonEvent ( int button, bool pressed ) {
        ImGuiIO& io = ImGui::GetIO ();
        io.AddMouseButtonEvent ( button, pressed );
    }

    // - Devuelve si el usuario ha cambiado el color y reinicia el aviso
    bool GUI::hasColorChanged () {
        bool aux = colorChanged;
        colorChanged = false;
        return aux;
    }

    const float* GUI::getBgColor () {
        return bgColor;
    }

    // - Para que el control refleje los cambios hechos con la rueda del ratón
    void GUI::setBgColor ( const float* color ) {
        for ( int i = 0; i < 3; i++ ) {
            bgColor[i] = color[i];
        }
    }



}
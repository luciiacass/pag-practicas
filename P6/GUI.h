//
// Created by luuca on 26/09/2026.
//

#ifndef P2_GUI_H
#define P2_GUI_H

#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "imfilebrowser.h"


namespace PAG {
    class GUI {
    private:
        static GUI* instancia;
        GUI ();

        std::vector<std::string> messages;        ///< Mensajes de la aplicación
        float bgColor[3] = { 0.6f, 0.6f, 0.6f };  ///< Color elegido en el control
        bool colorChanged = false;                ///< true si el usuario lo ha cambiado

        //atributos shader
        std::string shaderName = "shaders/pag03";
        bool loadShaderPressed = false;

        //atributos camara
        int camMov = 0;
        float camAngle = 5.0;
        float camDist = 0.1;
        float latitude = 5.0;
        float longitude = 5.0;

        ImGui::FileBrowser file;

        // Control de modelos
        std::string rutaModelo;
        bool cargarModelo = false;
        bool eliminarModelo = false;
        int modeloSeleccionado = -1;
        std::string archivoSeleccionado;

        void messagesWindow ();
        void backgroundWindow ();
        void shaderWindow();
        void cameraWindow();
        void modelsWindow();

    public:
        virtual ~GUI ();
        static GUI& getInstancia ();

        void init ( GLFWwindow* window );
        void shutdown ();

        void newFrame ();
        void draw ();
        void render ();

        void addMessage ( const std::string& message );
        void mouseButtonEvent ( int button, bool pressed );

        bool hasColorChanged ();
        const float* getBgColor ();
        void setBgColor ( const float* color );

        bool hasLoadShaderPressed();
        const std::string& getShaderName() const;

        //valores elegidos en la ventana por el usuario
        float getCameraAngle();
        float getCameraDistance();
        float getOrbitLatitude();
        float getOrbitLongitude();
        int getCamMov();
        bool movementMouse();

        bool hasLoadModelPressed();
        std::string getModelPath();
        bool hasDeleteModelPressed();
        int getSelectedModel();



    };
}

#endif //P2_GUI_H
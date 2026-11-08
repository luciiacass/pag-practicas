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

        float siguienteY = 10.0f;
        const float separacionVentanas = 5.0f;

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

        ImGui::FileBrowser file; //no se usa finalmente

        // Control de modelos
        std::string rutaModelo;
        bool cargarModelo = false;
        bool eliminarModelo = false;
        int modeloSeleccionado = -1;
        std::string archivoSeleccionado;
        //transformacion de los modelos
        float traslacion[3] = {0.0f, 0.0f, 0.0f};
        float rotacion[3] = {0.0f, 0.0f, 0.0f};
        float escala[3] = {1.0f, 1.0f, 1.0f};
        bool transformacion = false;

        //control material
        float colorAmbiente[3] = {0.2f, 0.2f, 0.2f};
        float colorDifuso[3] = {0.2f, 0.2f, 0.2f};
        float colorEspecular[3] = {0.2f, 0.2f, 0.2f};
        float exponente = 32.0f;

        bool materialChanged = false;

        //visualizacion (solido o alambre)
        int modo_visualizacion = 0; //solido (1: alambre)
        bool visualizacion_changed = false;

        void messagesWindow ();
        void backgroundWindow ();
        void shaderWindow();
        void cameraWindow();
        void modelsWindow();
        void transformWindow();
        void materialWindow();
        void visualizacionWindow();

    public:
        virtual ~GUI ();
        static GUI& getInstancia ();

        void posicionarVentana();

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

        bool hasTransformPressed();
        float* getTraslacion();
        float* getRotacion();
        float* getEscala();

        bool hasMaterialChanged();
        float* getColorAmbiente();
        float* getColorDifuso();
        float* getColorEspecular();
        float getExponente();

        bool hasModoVisualizacionChanged();
        int getModoVisualizacion();


    };
}

#endif //P2_GUI_H
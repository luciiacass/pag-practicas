//
// Created by luuca on 26/09/2026.
//
#include "glad/glad.h"

#include "GUI.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>
#include <filesystem>
#include <string>

namespace PAG {
    GUI* GUI::instancia = nullptr;

    GUI::GUI () {
        file.SetTitle("Seleccionar modelo OBJ");
        file.SetTypeFilters({".obj"});
    }

    GUI::~GUI () { }

    GUI& GUI::getInstancia () {
        if ( !instancia ) {
            instancia = new GUI ();
        }
        return *instancia;
    }

    void GUI::posicionarVentana() {
        ImGui::SetNextWindowPos(
            ImVec2(10.0f, siguienteY),
            ImGuiCond_Always
        );

        ImGui::SetNextWindowCollapsed(
            true,
            ImGuiCond_FirstUseEver
        );
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
        siguienteY = 10.0f;

        messagesWindow ();
        backgroundWindow ();
        shaderWindow ();
        cameraWindow();
        modelsWindow();
        transformWindow();
        materialWindow();
        visualizacionWindow();
    }

    void GUI::render () {
        ImGui::Render ();
        ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData () );
    }

    // - Ventana donde se muestran los mensajes de la aplicación
    void GUI::messagesWindow () {

        posicionarVentana();

        if ( ImGui::Begin ( "Mensajes" ) ) {
            ImGui::SetWindowFontScale ( 1.0f );

            for ( unsigned int i = 0; i < messages.size (); i++ ) {
                ImGui::TextUnformatted ( messages[i].c_str () );
            }

            if ( ImGui::GetScrollY () >= ImGui::GetScrollMaxY () ) {
                ImGui::SetScrollHereY ( 1.0f );
            }
        }

        siguienteY += ImGui::GetWindowHeight()
            + separacionVentanas;

        ImGui::End ();
    }

    // - Ventana con el selector del color de fondo
    void GUI::backgroundWindow () {
        posicionarVentana();

        if ( ImGui::Begin ( "Color de fondo", nullptr, ImGuiWindowFlags_AlwaysAutoResize ) ) {
            ImGui::SetWindowFontScale ( 1.0f );

            // - ColorPicker3 devuelve true cuando el usuario cambia el color
            if ( ImGui::ColorPicker3 ( "Fondo", bgColor, ImGuiColorEditFlags_PickerHueWheel  ) ) {
                colorChanged = true;
            }
        }

        siguienteY += ImGui::GetWindowHeight()
              + separacionVentanas;

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

    void GUI::shaderWindow() {

        posicionarVentana();

        if (ImGui::Begin(
                "Shader Program",
                nullptr,
                ImGuiWindowFlags_AlwaysAutoResize
            )) {

            ImGui::Text("Nombre base:");

            ImGui::InputText(
                "##shaderName",
                &shaderName,
                ImGuiInputTextFlags_AutoSelectAll
            );

            loadShaderPressed =
                ImGui::Button("Load");
            }

        siguienteY += ImGui::GetWindowHeight()
            + separacionVentanas;

        ImGui::End();
    }

    bool GUI::hasLoadShaderPressed() {

        bool aux = loadShaderPressed;
        loadShaderPressed = false;

        return aux;
    }

    const std::string& GUI::getShaderName() const {
        return shaderName;
    }


    void GUI::cameraWindow() {

        posicionarVentana();
        if (ImGui::Begin("Camara virtual")) {

            // opciones del desplegable
            const char* movements[] = {
                "Pan",
                "Tilt",
                "Dolly",
                "Crane",
                "Orbit",
                "Zoom"
            };

            ImGui::Combo(
                "Movimiento",
                &camMov,
                movements,
                IM_ARRAYSIZE(movements)
            );

            ImGui::Separator();

            switch (camMov) {

                case 0: // PAN
                    ImGui::SliderFloat(
                        "Angulo", &camAngle, 1.0f, 30.0f
                    );

                    if (ImGui::Button("Izquierda")) {
                        Renderer::getInstancia().panCam(-camAngle);
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Derecha")) {
                        Renderer::getInstancia().panCam(camAngle);
                    }
                    break;

                case 1: // TILT
                    ImGui::SliderFloat(
                        "Angulo", &camAngle, 1.0f, 30.0f
                    );

                    if (ImGui::Button("Arriba")) {
                        Renderer::getInstancia().tiltCam(camAngle);
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Abajo")) {
                        Renderer::getInstancia().tiltCam(-camAngle);
                    }
                    break;

                case 2: // DOLLY
                ImGui::SliderFloat("Distancia", &camDist, 0.05f, 1.0f);

                    if (ImGui::Button("Izquierda##Dolly")) {
                        Renderer::getInstancia().dollyCam(-camDist, 0.0f);
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Derecha##Dolly")) {
                        Renderer::getInstancia().dollyCam(camDist, 0.0f);
                    }

                    if (ImGui::Button("Adelante")) {
                        Renderer::getInstancia().dollyCam(0.0f, -camDist);
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Atras")) {
                        Renderer::getInstancia().dollyCam(0.0f, camDist);
                    }
                    break;

                case 3: // CRANE
                    ImGui::SliderFloat("Distancia", &camDist, 0.05f, 1.0f);

                    if (ImGui::Button("Subir")) {
                        Renderer::getInstancia().craneCam(camDist);
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Bajar")) {
                        Renderer::getInstancia().craneCam(-camDist);
                    }
                    break;

                case 4: // ORBIT
                    ImGui::SliderFloat("Latitud", &latitude, 1.0f, 30.0f);
                    ImGui::SliderFloat("Longitud", &longitude, 1.0f, 30.0f);

                    if (ImGui::Button("Izquierda##Orbit")) {
                        Renderer::getInstancia().orbitCam(0.0f, -longitude);
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Derecha##Orbit")) {
                        Renderer::getInstancia().orbitCam(0.0f, longitude);
                    }

                    if (ImGui::Button("Arriba##Orbit")) {
                        Renderer::getInstancia().orbitCam(latitude, 0.0f);
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Abajo##Orbit")) {
                        Renderer::getInstancia().orbitCam(-latitude, 0.0f);
                    }
                    break;

                case 5: // ZOOM
                    ImGui::SliderFloat("Angulo", &camAngle, 1.0f, 20.0f);

                    if (ImGui::Button("Zoom +")) {
                        Renderer::getInstancia().zoomCam(-camAngle);
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Zoom -")) {
                        Renderer::getInstancia().zoomCam(camAngle);
                    }
                    break;

                default:
                    ImGui::Text("Controles pendientes");
                    break;
            }
        }

        siguienteY += ImGui::GetWindowHeight()
            + separacionVentanas;
        ImGui::End();
    }



void GUI::modelsWindow() {

    posicionarVentana();

    if (ImGui::Begin("Gestion de modelos")) {

        // Mostrar archivos OBJ disponibles
        if (ImGui::BeginCombo(
            "##archivos",
            archivoSeleccionado.empty()
                ? "Seleccionar OBJ"
                : archivoSeleccionado.c_str()
        )) {

            std::filesystem::path carpeta = "objs";

            if (std::filesystem::exists(carpeta)) {
                for (const auto& archivo :
                     std::filesystem::directory_iterator(carpeta)) {

                    if (!archivo.is_regular_file())
                        continue;

                    if (archivo.path().extension() != ".obj")
                        continue;

                    std::string nombre =
                        archivo.path().filename().string();

                    if (ImGui::Selectable(nombre.c_str())) {
                        archivoSeleccionado = nombre;
                    }
                }
            }

            ImGui::EndCombo();
        }

        // Cargar el archivo elegido
        if (ImGui::Button("Cargar modelo OBJ") &&
            !archivoSeleccionado.empty()) {

            rutaModelo =
                (std::filesystem::path("objs") /
                 archivoSeleccionado).string();

            cargarModelo = true;
        }

        ImGui::Separator();

        int numModelos =
            Renderer::getInstancia().getNumModelos();

        ImGui::Text("Modelos cargados: %d", numModelos);

        if (modeloSeleccionado >= numModelos)
            modeloSeleccionado = numModelos - 1;

        for (int i = 0; i < numModelos; i++) {

            std::string nombre =
                "Modelo " + std::to_string(i + 1) +
                " - " +
                Renderer::getInstancia().getNombreModelo(i);


            if (ImGui::Selectable(
                nombre.c_str(), modeloSeleccionado == i)) {

                modeloSeleccionado = i;

                Material& mat = Renderer::getInstancia().getMaterial(i);

                for (int j = 0; j < 3; j++) {

                    colorAmbiente[j] = mat.getAmbiente()[j];
                    colorDifuso[j] = mat.getDifuso()[j];
                    colorEspecular[j] = mat.getEspecular()[j];
                }

                exponente = mat.getExponent();

                materialChanged = false;
                }
        }

        if (modeloSeleccionado >= 0) {
            if (ImGui::Button("Eliminar modelo")) {
                eliminarModelo = true;
            }
        }
    }

        siguienteY += ImGui::GetWindowHeight()
            + separacionVentanas;

    ImGui::End();
}


    void GUI::transformWindow() {

        posicionarVentana();

        if (ImGui::Begin("Transformaciones")) {

            if (modeloSeleccionado == -1) {

                ImGui::Text("Selecciona un modelo");

            } else {


                ImGui::Text("Traslacion");
                ImGui::SameLine(100);
                ImGui::SetNextItemWidth(185);
                ImGui::DragFloat3("##traslacion", traslacion, 0.1f);

                ImGui::Text("Rotacion");
                ImGui::SameLine(100);
                ImGui::SetNextItemWidth(185);
                ImGui::DragFloat3("##rotacion", rotacion, 1.0f);

                ImGui::Text("Escalado");
                ImGui::SameLine(100);
                ImGui::SetNextItemWidth(185);
                ImGui::DragFloat3("##escala", escala, 0.01f, 0.01f, 10.0f);

                ImGui::Separator();

                if (ImGui::Button("Aplicar transformacion")) {
                    transformacion = true;
                }

            }
        }

        siguienteY += ImGui::GetWindowHeight()
      + separacionVentanas;

        ImGui::End();
    }


    float GUI::getCameraAngle()  {
        return camAngle;
    }

    float GUI::getCameraDistance()  {
        return camDist;
    }

    float GUI::getOrbitLatitude()  {
        return latitude;
    }

    float GUI::getOrbitLongitude()  {
        return longitude;
    }

    int GUI::getCamMov() {
        return camMov;
    }

    bool GUI::movementMouse() {
        return ImGui::GetIO().WantCaptureMouse;
    }

    bool GUI::hasLoadModelPressed() {
        bool aux = cargarModelo;
        cargarModelo = false;
        return aux;
    }

    std::string GUI::getModelPath() {
        return rutaModelo;
    }

    bool GUI::hasDeleteModelPressed() {
        bool aux = eliminarModelo;
        eliminarModelo = false;
        return aux;
    }

    int GUI::getSelectedModel()  {
        return modeloSeleccionado;
    }

    bool GUI::hasTransformPressed() {

        bool aux = transformacion;
        transformacion = false;

        return aux;
    }

     float* GUI::getTraslacion() {
        return traslacion;
    }

     float* GUI::getRotacion() {
        return rotacion;
    }

     float* GUI::getEscala() {
        return escala;
    }


    void GUI::materialWindow() {

        posicionarVentana();

        if (ImGui::Begin(
            "Propiedades del material",
            nullptr,
            ImGuiWindowFlags_AlwaysAutoResize
        )) {

            if (modeloSeleccionado == -1) {

                ImGui::Text("Selecciona un modelo");

            } else {
                if (ImGui::ColorEdit3("Ambiente", colorAmbiente)) {
                    materialChanged = true;
                }

                if (ImGui::ColorEdit3("Difuso", colorDifuso)) {
                    materialChanged = true;
                }

                if (ImGui::ColorEdit3("Especular", colorEspecular)) {
                    materialChanged = true;
                }

                if (ImGui::SliderFloat(
                    "Exponente",
                    &exponente,
                    1.0f,
                    256.0f
                )) {
                    materialChanged = true;
                }
            }
        }

        siguienteY += ImGui::GetWindowHeight()
      + separacionVentanas;

        ImGui::End();
    }


    bool GUI::hasMaterialChanged() {
        bool aux = materialChanged;
        materialChanged = false;
        return aux;
    }

    float* GUI::getColorAmbiente() {
        return colorAmbiente;
    }

    float* GUI::getColorDifuso() {
        return colorDifuso;
    }

    float* GUI::getColorEspecular() {
        return colorEspecular;
    }

    float GUI::getExponente() {
        return exponente;
    }


    void GUI::visualizacionWindow() {

        posicionarVentana();

        if (ImGui::Begin("Modo de visualizacion")) {

            const char* modos[] = {
                "Solido",
                "Alambre"
            };

            if (ImGui::Combo(
                "Modo",
                &modo_visualizacion,
                modos,
                IM_ARRAYSIZE(modos)
            )) {
                visualizacion_changed = true;
            }
        }

        siguienteY += ImGui::GetWindowHeight() + separacionVentanas;

        ImGui::End();
    }

    bool GUI::hasModoVisualizacionChanged() {
        bool aux = visualizacion_changed;
        visualizacion_changed = false;
        return aux;
    }

    int GUI::getModoVisualizacion() {
        return modo_visualizacion;
    }






}
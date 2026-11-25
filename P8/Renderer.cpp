//
// Created by luuca on 21/09/2026.
//
#include "glad/glad.h"
#include "Renderer.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <random>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "GUI.h"



namespace PAG {

    Renderer::Renderer (){ }

    Renderer* Renderer::instancia = nullptr;

    float Renderer::color_limit ( float value ) {
        if (value < 0.0) return 0.0;
        if (value > 1.0) return 1.0;
        return value;
    }

    Renderer::~Renderer () {
    }

    Renderer& Renderer::getInstancia (){
        if ( !instancia ) {
            instancia = new Renderer ();
        }
        return *instancia;
    }

    void Renderer::refresh (){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (shader.getId() == 0) {
            return;
        }
        //shader
        glUseProgram ( shader.getId() );

        glm::mat4 V = camera.getViewMatrix();
        glm::mat4 P = camera.getProjectionMatrix();


        //visualizacion (solido o alambrado)
        if (modo_visualizacion == Visualizacion::solido) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            //si no hay luces
            if (luces.empty()) {

                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                selecSubrutina("material");
                dibujarModelos(V, P);

                return;
            }

            //rendering multipasada para todas las luces
            for (int i = 0; i < luces.size(); i++) {

                if (i == 0) {
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                } else {
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                }

                aplicarLuz(luces[i], V);
                dibujarModelos(V, P);
            }
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            selecSubrutina("alambre");
            dibujarModelos(V, P);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            return;
        }

    }

    //Actualiza tanto el viewport de OpenGL como las dimensiones de la camara
    void Renderer::reframe(int width, int height ) {
        glViewport ( 0, 0, width, height );
        camera.setViewport(width, height);
    }

    void Renderer::scroll (double xoffset, double yoffset ){

        // - Establecemos nuevos valores que no pasen el limite de [0,1]
        for (int i = 0; i<3; i++) {

            //los colores incrementan a ritmos distintos
            GLfloat increment = (GLfloat) yoffset * differentIncrement[i] ;
            GLfloat new_value;

            new_value = screenColor[i] + increment;

            screenColor[i] = color_limit ( new_value );
        }

        // - Aplicamos el nuevo color de fondo
        glClearColor ( screenColor[0], screenColor[1], screenColor[2], screenColor[3] );
        // - Forzamos el redibujado inmediatamente para ver el cambio
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    }

    void Renderer::info() {
        std::cout << glGetString ( GL_RENDERER ) << std::endl
                  << glGetString ( GL_VENDOR ) << std::endl
                  << glGetString ( GL_VERSION ) << std::endl
                  << glGetString ( GL_SHADING_LANGUAGE_VERSION ) << std::endl;
    }

    void Renderer::depth() {
        glEnable(GL_BLEND); // Activación del modo mezcla
        glEnable(GL_DEPTH_TEST); // Activación del Z-Buffer
        glDepthFunc(GL_LEQUAL); // Uso de la función “menor o igual”
    }


    const float* Renderer::getScreenColor () {
        return screenColor;
    }

    void Renderer::setScreenColor ( const float* color ) {
        for ( int i = 0; i < 3; i++ ) {
            screenColor[i] = color_limit ( color[i] );
        }
        glClearColor ( screenColor[0], screenColor[1], screenColor[2], screenColor[3] );
    }

    void Renderer::loadShaderProgram(const std::string& name) {
        shader.create(name);
    }



    /**
    * Método para inicializar los parámetros globales de OpenGL
    */
    void PAG::Renderer::inicializaOpenGL ( ){
        glClearColor ( screenColor[0], screenColor[1], screenColor[2], screenColor[3] );
        glEnable ( GL_DEPTH_TEST );
        glEnable ( GL_MULTISAMPLE );
    }


    void Renderer::panCam(float angle) {
        camera.pan(angle);
    }

    void Renderer::tiltCam(float angle) {
        camera.tilt(angle);
    }

    void Renderer::dollyCam(float dx, float dz) {
        camera.dolly(dx, dz);
    }

    void Renderer::craneCam(float dy) {
        camera.crane(dy);
    }

    void Renderer::orbitCam(float latitude, float longitude) {
        camera.orbit(latitude, longitude);
    }

    void Renderer::zoomCam(float angle) {
        camera.zoom(angle);
    }

    void Renderer::moveCam(int mov, float dx, float dy) {
        const float pixels = 100.0f;

        float horizontal = dx / pixels;
        float vertical = dy / pixels;

        switch (mov) {

            case 0: // PAN
                panCam(horizontal * GUI::getInstancia().getCameraAngle());
                break;

            case 1: // TILT
                tiltCam(-vertical * GUI::getInstancia().getCameraAngle());
                break;

            case 2: // DOLLY
                dollyCam(horizontal * GUI::getInstancia().getCameraDistance(), vertical * GUI::getInstancia().getCameraDistance());
                break;

            case 3: // CRANE
                craneCam(-vertical * GUI::getInstancia().getCameraDistance());
                break;

            case 4: // ORBIT
                orbitCam(vertical * GUI::getInstancia().getOrbitLatitude(), horizontal * GUI::getInstancia().getOrbitLongitude());
                break;

            case 5: // ZOOM
                zoomCam(vertical * GUI::getInstancia().getCameraAngle());
                break;

            default:
                break;
        }
    }

    //void Renderer::inicializaModelo() {
    //    modelo.create();
    //}

    void Renderer::inicializaModelos() { //probar dos triangulos en la interfaz
        //creacion de modelos
        Modelo* m1 = new Modelo();
        m1->create();

        Modelo* m2 = new Modelo();
        m2->create();

        //matrices de transformacion
        glm::mat4 mat1 = glm::mat4(1);
        mat1 = glm::translate (mat1, glm::vec3(0, 1, -5));

        glm::mat4 mat2 = glm::mat4(1);
        mat2 = glm::translate (mat2, glm::vec3(2, -1, -5));

        //adjuntar matrices al modelo
        m1->setMatriz(mat1);
        m2->setMatriz(mat2);

        //añadir modelos al vector
        modelos.push_back(m1);
        modelos.push_back(m2);
    }

    void Renderer::finalizaModelos() {
        for (Modelo* modelo : modelos) {
            delete modelo;
        }

        modelos.clear();
    }

    void Renderer::loadOBJ(std::string path) {

        Modelo* nuevoModelo = new Modelo();
        try {
            nuevoModelo->loadobj(path);

            //matrices de la camara
            glm::mat4 V = camera.getViewMatrix();
            glm::mat4 P = camera.getProjectionMatrix();
            float distancia = 5.0f;

            //limites en funcion de las coordenadas visibles de la camara
            float maxX = distancia / P[0][0];
            float minX = -maxX;

            float maxY = distancia / P[1][1];
            float minY = -maxY;

            //pos aleatoria dentro de las coordenadas anteriores
            static std::mt19937 generador(std::random_device{}());

            std::uniform_real_distribution<float> randomX(minX, maxX);
            std::uniform_real_distribution<float> randomY(minY, maxY);

            //centro de la caja limitada de los modelos
            glm::vec3 centro = (nuevoModelo->getmin() + nuevoModelo->getmax()) / 2.0f;
            //coordenadas -> 3D
            glm::mat4 inversaV = glm::inverse(V);

            bool encontrado = false;

            for (int intento = 0; intento < 100; intento++) {

                float x = randomX(generador);
                float y = randomY(generador);

                glm::vec4 puntoCamara(x, y, -distancia, 1.0f);

                glm::vec3 puntoMundo = glm::vec3(inversaV * puntoCamara);

                glm::vec3 traslacion = puntoMundo - centro;

                glm::mat4 M(1.0f);

                M = glm::translate(M, traslacion);

                nuevoModelo->setMatriz(M);

                if (!solapamiento(nuevoModelo)) {
                    encontrado = true;
                    break;
                }
            }

            if (!encontrado) {
                throw std::runtime_error("No se ha encontrado una posicion libre");
            }

            modelos.push_back(nuevoModelo);

        }
        catch (...) {
            delete nuevoModelo;
            throw;
        }
    }

    void Renderer::eliminaModelo(int indice) {

        if (indice < 0 || indice >= modelos.size()) {
            throw std::runtime_error("Indice de modelo no valido");
        }

        delete modelos[indice];
        modelos.erase(modelos.begin() + indice);
    }

    int Renderer::getNumModelos()  {
        return int(modelos.size());
    }

    std::string Renderer::getNombreModelo(int indice) {

        if (indice < 0 || indice >= modelos.size()) {
            return "";
        }

        return modelos[indice]->getNombre();
    }

    bool Renderer::solapamiento(Modelo* nuevomodelo) {

        if (modelos.empty()) {
            return false;
        }
        //pos de la matriz de modelado
        glm::vec3 pos1 = glm::vec3(nuevomodelo->getMatriz()[3]);
        //pos caja nuevo modelo
        glm::vec3 min1 = nuevomodelo->getmin() + pos1;
        glm::vec3 max1 = nuevomodelo->getmax() + pos1;

        for (Modelo* modelo : modelos) {
            glm::vec3 pos2 = glm::vec3(modelo->getMatriz()[3]);
            glm::vec3 min2 = modelo->getmin() + pos2;
            glm::vec3 max2 = modelo->getmax() + pos2;

            float margen = 0.2f;

            //solapamiento en cada eje
            bool solapaX = min1.x < max2.x + margen &&
                           max1.x + margen > min2.x;

            bool solapaY = min1.y < max2.y + margen &&
                           max1.y + margen > min2.y;

            bool solapaZ = min1.z < max2.z + margen &&
                           max1.z + margen > min2.z;

            if (solapaX && solapaY && solapaZ) {
                return true;
            }
        }

        return false;
    }


    void Renderer::transformarModelo(int indice, float* traslacion, float* rotacion, float* escala) {

        if (indice < 0 || indice >= modelos.size()) {
            return;
        }

        Modelo* modelo = modelos[indice];

        glm::mat4 M = modelo->getMatriz();

        // traslaciones
        glm::mat4 T = glm::translate(glm::mat4(1.0f),glm::vec3(traslacion[0], traslacion[1], traslacion[2]));

        M = T * M;

        // rotaciones
        M = glm::rotate(M, glm::radians(rotacion[0]), glm::vec3(1.0f, 0.0f, 0.0f));

        M = glm::rotate(M, glm::radians(rotacion[1]), glm::vec3(0.0f, 1.0f, 0.0f));

        M = glm::rotate(M, glm::radians(rotacion[2]), glm::vec3(0.0f, 0.0f, 1.0f));

        // escalado
        M = glm::scale( M, glm::vec3(escala[0], escala[1], escala[2]));

        modelo->setMatriz(M);
    }

    Material& Renderer::getMaterial(int indice) {
        if (indice < 0 || indice >= modelos.size()) {
            throw std::runtime_error(
                "Indice de modelo no valido"
            );
        }

        return modelos[indice]->getMaterial();
    }


    void Renderer::cambiarMaterialModelos(int indice,float* ambiente,float* difuso,float* especular,float exponente) {
            Material& mat = getMaterial(indice);

            mat.setAmbiente(ambiente);
            mat.setDifuso(difuso);
            mat.setEspecular(especular);
            mat.setExponente(exponente);
    }

    void Renderer::setVisualizacion(Visualizacion visualizacion) {
        modo_visualizacion = visualizacion;
    }

    void Renderer::inicializarLuces() {
        luces.clear();

        //ambiente (iluminacion general)
        Luz ambiente;
        ambiente.setTipo(tipoLuz::ambiente);
        ambiente.setIa(glm::vec3(0.3f, 0.3f, 0.3f));
        luces.push_back(ambiente);

        //puntual (iluminacion a la izq)
        Luz puntual;
        puntual.setTipo(tipoLuz::puntual);

        puntual.setPosicion(glm::vec3(-3.0f, 2.0f, 2.0f));
        puntual.setId(glm::vec3(1.0f, 0.0f, 0.0f));
        puntual.setIs(glm::vec3(1.0f, 1.0f, 1.0f));
        luces.push_back(puntual);

        //direccional (iluminación arriba a la derecha)
        Luz direccional;
        direccional.setTipo(tipoLuz::direccional);

        direccional.setDireccion(glm::vec3(-1.0f, -1.0f, -1.0f));
        direccional.setId(glm::vec3(0.0f, 0.0f, 1.0f));
        direccional.setIs(glm::vec3(1.0f, 1.0f, 1.0f));
        luces.push_back(direccional);

        //foco (dirigida al modelo)
        Luz foco;
        foco.setTipo(tipoLuz::foco);

        foco.setPosicion(glm::vec3(0.0f, 3.0f, 2.0f));
        foco.setDireccion(glm::vec3(0.0f, -3.0f, -4.0f));

        foco.setId(glm::vec3(0.0f, 1.0f, 0.0f));
        foco.setIs(glm::vec3(1.0f, 1.0f, 1.0f));

        foco.setGamma(30.0f);
        foco.setS(2.0f);

        luces.push_back(foco);

    }

    void Renderer::selecSubrutina(const char* nombre) {
        GLuint indice = glGetSubroutineIndex(
                shader.getId(),
                GL_FRAGMENT_SHADER,
                nombre
            );

        if (indice == GL_INVALID_INDEX) {
            throw std::runtime_error(
                std::string("No se encuentra la subrutina: ") + nombre
            );
        }

        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &indice);
    }

    void Renderer::aplicarLuz(Luz& luz, glm::mat4& V) {
        GLuint id = shader.getId();

        //subrutina
        switch (luz.getTipo()) {

            case tipoLuz::ambiente:
                selecSubrutina("ambiente");
                break;

            case tipoLuz::puntual:
                selecSubrutina("puntual");
                break;

            case tipoLuz::direccional:
                selecSubrutina("direccional");
                break;

            case tipoLuz::foco:
                selecSubrutina("foco");
                break;
        }

        //luz
        glm::vec3 Ia = luz.getIa();
        glm::vec3 Id = luz.getId();
        glm::vec3 Is = luz.getIs();

        glUniform3fv(glGetUniformLocation(id, "Ia"), 1, &Ia[0]);
        glUniform3fv(glGetUniformLocation(id, "Id"), 1, &Id[0]);
        glUniform3fv(glGetUniformLocation(id, "Is"), 1, &Is[0]);

        //posicion
        glm::vec3 posicionVision = glm::vec3(V * glm::vec4(luz.getPosicion(), 1.0f));
        glUniform3fv(glGetUniformLocation(id, "posicionLuz"),1, &posicionVision[0]);

        //direccion
        glm::vec3 direccionVision = glm::vec3(V * glm::vec4(luz.getDireccion(), 0.0f));
        glUniform3fv(glGetUniformLocation(id, "direccionLuz"),1, &direccionVision[0]);

        //foco
        glUniform1f(glGetUniformLocation(id, "gamma"),luz.getGamma());
        glUniform1f(glGetUniformLocation(id, "s"),luz.getS());
    }

    void Renderer::dibujarModelos( glm::mat4& V, glm::mat4& P) {
        GLuint id = shader.getId();

        for (Modelo* modelo : modelos) {

            // matrices
            glm::mat4 M = modelo->getMatriz();
            glm::mat4 MV = V * M;
            glm::mat4 MVP = P * MV;

            // matriz para transformar las normales
            glm::mat3 matrizNormal = glm::transpose(glm::inverse(glm::mat3(MV)));

            // Envio de matrices al shader
            glUniformMatrix4fv(glGetUniformLocation(id, "MVP"),1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix4fv(glGetUniformLocation(id, "MV"),1, GL_FALSE, &MV[0][0]);
            glUniformMatrix3fv(glGetUniformLocation(id, "mNormal"),1, GL_FALSE, &matrizNormal[0][0]);

            Material& mat = modelo->getMaterial();

            //ambiente
            glUniform3fv(
                glGetUniformLocation(id, "materialAmbiente"),
                1, mat.getAmbiente()
            );

            //difusa
            glUniform3fv(
                glGetUniformLocation(id, "materialDifuso"),
                1, mat.getDifuso()
            );

            //especular
            glUniform3fv(
                glGetUniformLocation(id, "materialEspecular"),
                1, mat.getEspecular()
            );

            // exponente
            glUniform1f(
                glGetUniformLocation(id, "exponente"),
                mat.getExponent()
            );

            modelo->draw();
        }
    }




} // PAG
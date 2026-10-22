//
// Created by luuca on 21/09/2026.
//

#ifndef P2_RENDERER_H
#define P2_RENDERER_H

#include "Shader.h"
#include "Camera.h"
#include "Modelo.h"


#include <string>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

namespace PAG {
    class Renderer {
    private:
        static Renderer* instancia;
        Renderer ();

        float screenColor[4] = { 0.6, 0.6, 0.6, 1.0 };
        float differentIncrement[3] = { 0.06, 0.03, 0.02 };

        float color_limit ( float value );


        Shader shader;
        Camera camera;
        //Modelo modelo;
        std::vector<Modelo*> modelos;

    public:
        virtual ~Renderer ();
        static Renderer& getInstancia ();



        //callbacks
        void refresh ();
        void reframe(int width, int height );
        void scroll (double xoffset, double yoffset );
        void info();
        void depth();

        const float* getScreenColor ();
        void setScreenColor ( const float* screenColor );

        void loadShaderProgram(const std::string& name);

        void inicializaOpenGL();

        void panCam(float angle);
        void tiltCam(float angle);
        void dollyCam(float dx, float dz);
        void craneCam(float dy);
        void orbitCam(float latitude, float longitude);
        void zoomCam(float angle);

        void moveCam(int mov, float dx, float dy);

        //void inicializaModelo();
        void inicializaModelos();
        void finalizaModelos();
        void loadOBJ(std::string path);


    };
} // PAG

#endif //P2_RENDERER_H
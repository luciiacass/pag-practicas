//
// Created by luuca on 21/09/2026.
//

#ifndef P2_RENDERER_H
#define P2_RENDERER_H
#include <string>
#include <GLFW/glfw3.h>

namespace PAG {
    class Renderer {
    private:
        static Renderer* instancia;
        Renderer ();

        float screenColor[4] = { 0.6, 0.6, 0.6, 1.0 };
        float differentIncrement[3] = { 0.06, 0.03, 0.02 };

        float color_limit ( float value );

        GLuint idVS = 0; // Identificador del vertex shader
        GLuint idFS = 0; // Identificador del fragment shader
        GLuint idSP = 0; // Identificador del shader program
        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBO = 0; // Identificador del vertex buffer object
        GLuint idIBO = 0; // Identificador del index buffer object
        GLuint idVBOColores = 0;

        std::string readFile ( std::string filename );

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

        void creaShaderProgram(const std::string& name);
        void creaModelo();
        void inicializaOpenGL();


    };
} // PAG

#endif //P2_RENDERER_H
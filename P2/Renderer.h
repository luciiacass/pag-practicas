//
// Created by luuca on 21/09/2026.
//

#ifndef P2_RENDERER_H
#define P2_RENDERER_H
#include <GLFW/glfw3.h>

namespace PAG {
    class Renderer {
    private:
        static Renderer* instancia;
        Renderer ();

        float screenColor[4] = { 0.6, 0.6, 0.6, 1.0 };
        float differentIncrement[3] = { 0.06, 0.03, 0.02 };

        float color_limit ( float value );
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


    };
} // PAG

#endif //P2_RENDERER_H
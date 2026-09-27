//
// Created by luuca on 26/09/2026.
//

#ifndef P2_GUI_H
#define P2_GUI_H

#include <string>
#include <vector>
#include <GLFW/glfw3.h>

namespace PAG {
    class GUI {
    private:
        static GUI* instancia;
        GUI ();

        std::vector<std::string> messages;        ///< Mensajes de la aplicación
        float bgColor[3] = { 0.6f, 0.6f, 0.6f };  ///< Color elegido en el control
        bool colorChanged = false;                ///< true si el usuario lo ha cambiado

        void messagesWindow ();
        void backgroundWindow ();

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

    };
}

#endif //P2_GUI_H
//
// Created by luuca on 21/09/2026.
//

#include "Renderer.h"

#include <iostream>
#include <string>

#include "glad/glad.h"

namespace PAG {

    Renderer* Renderer::instancia = nullptr;

    Renderer::Renderer (){ }

    float Renderer::color_limit ( float value ) {
        if (value < 0.0) return 0.0;
        if (value > 1.0) return 1.0;
        return value;
    }

    Renderer::~Renderer (){ }


    Renderer& Renderer::getInstancia (){
        if ( !instancia ) {
            instancia = new Renderer ();
        }
        return *instancia;
    }

    void Renderer::refresh (){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::reframe(int width, int height ) {
        glViewport ( 0, 0, width, height );
    }

    void Renderer::scroll (double xoffset, double yoffset ){
        std::cout << "Movida la rueda del ratón " << xoffset
                << " Unidades en horizontal y " << yoffset
                << " unidades en vertical" << std::endl;

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
        glEnable ( GL_DEPTH_TEST );
    }







} // PAG
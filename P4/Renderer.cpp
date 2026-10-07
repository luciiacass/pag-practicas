//
// Created by luuca on 21/09/2026.
//
#include "glad/glad.h"
#include "Renderer.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>



namespace PAG {

    Renderer::Renderer (){ }

    Renderer* Renderer::instancia = nullptr;

    float Renderer::color_limit ( float value ) {
        if (value < 0.0) return 0.0;
        if (value > 1.0) return 1.0;
        return value;
    }

    Renderer::~Renderer () {

        if ( idVBO != 0 )
        { glDeleteBuffers ( 1, &idVBO );
        }
        if ( idIBO != 0 )
        { glDeleteBuffers ( 1, &idIBO );
        }
        if ( idVAO != 0 )
        { glDeleteVertexArrays ( 1, &idVAO );
        }
        if ( idVBOColores != 0 )
        { glDeleteBuffers ( 1, &idVBOColores );
        }

    }

    Renderer& Renderer::getInstancia (){
        if ( !instancia ) {
            instancia = new Renderer ();
        }
        return *instancia;
    }

    void Renderer::refresh (){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
        glUseProgram ( shader.getId() );
        glBindVertexArray ( idVAO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
        glDrawElements ( GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr );

    }

    void Renderer::reframe(int width, int height ) {
        glViewport ( 0, 0, width, height );
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
        glEnable ( GL_DEPTH_TEST );
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
    * Método para crear el VAO para el modelo a renderizar
    * @note No se incluye ninguna comprobación de errores
    */
    void PAG::Renderer::creaModelo ( )
    {
        GLuint indices[] = { 0, 1, 2 };

        glGenVertexArrays ( 1, &idVAO ); //1. Crea el VAO
        glBindVertexArray ( idVAO ); //2. Enlaza el VAO

        //NO ENTRELAZADO
        /*GLfloat vertices[] = { -.5, -.5, 0,
                            .5, -.5, 0,
                            .0, .5, 0 };

        GLfloat colores[]  = { 1.0, 0.0, 0.0,
                              0.0, 1.0, 0.0,
                              0.0, 0.0, 1.0 };

        glGenBuffers ( 1, &idVBO );
        glBindBuffer ( GL_ARRAY_BUFFER, idVBO );
        glBufferData ( GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW );
        glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), nullptr );
        glEnableVertexAttribArray ( 0 );

        glGenBuffers ( 1, &idVBOColores );
        glBindBuffer ( GL_ARRAY_BUFFER, idVBOColores );
        glBufferData ( GL_ARRAY_BUFFER, 9*sizeof(GLfloat), colores, GL_STATIC_DRAW );
        glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), nullptr );
        glEnableVertexAttribArray ( 1 );
*/
        //ENTRELAZADO

        //Atributos de cada vertice juntos
        GLfloat datos[] = { -.5, -.5, 0,   1.0, 0.0, 0.0,
                             .5, -.5, 0,   0.0, 1.0, 0.0,
                             .0,  .5, 0,   0.0, 0.0, 1.0 };

        glGenBuffers ( 1, &idVBO ); //3. Crea el VBO
        glBindBuffer ( GL_ARRAY_BUFFER, idVBO ); //4. Enlaza el VBO

        //5. Atributo 0 (posicion)
        glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), nullptr );
        glEnableVertexAttribArray ( 0 );
        //6. Atributo 1 (color)
        glVertexAttribPointer ( 1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat),
                                (GLubyte*) nullptr + 3*sizeof(GLfloat) );
        glEnableVertexAttribArray ( 1 );
        //7. Pasa el VBO
        glBufferData ( GL_ARRAY_BUFFER, 18*sizeof(GLfloat), datos, GL_STATIC_DRAW );



        glGenBuffers ( 1, &idIBO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
        glBufferData ( GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLuint), indices, GL_STATIC_DRAW );
    }

    /**
    * Método para inicializar los parámetros globales de OpenGL
    */
    void PAG::Renderer::inicializaOpenGL ( ){
        glClearColor ( screenColor[0], screenColor[1], screenColor[2], screenColor[3] );
        glEnable ( GL_DEPTH_TEST );
        glEnable ( GL_MULTISAMPLE );
    }










} // PAG
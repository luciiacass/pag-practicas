//
// Created by luuca on 21/10/2026.
//

#include "Modelo.h"

namespace PAG {
    Modelo::Modelo() {}

    Modelo::~Modelo() {
        if (idVBO != 0) {
            glDeleteBuffers(1, &idVBO);
        }

        if (idIBO != 0) {
            glDeleteBuffers(1, &idIBO);
        }

        if (idVAO != 0) {
            glDeleteVertexArrays(1, &idVAO);
        }
    }

    void Modelo::create() {
        GLuint indices[] = { 0, 1, 2 };

        glGenVertexArrays ( 1, &idVAO ); //1. Crea el VAO
        glBindVertexArray ( idVAO ); //2. Enlaza el VAO

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

    void Modelo::draw() {
        glBindVertexArray ( idVAO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
        glDrawElements ( GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr );

    }


} // PAG
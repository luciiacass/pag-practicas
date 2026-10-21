//
// Created by luuca on 21/10/2026.
//

#ifndef P6_MODELO_H
#define P6_MODELO_H
#include <string>

#include "glad/glad.h"

namespace PAG {
    class Modelo {
    private:
        GLuint idVAO = 0;
        GLuint idVBO = 0;
        GLuint idIBO = 0;

    public:
        Modelo();
        ~Modelo();

        void create();
        void draw();
    };
} // PAG

#endif //P6_MODELO_H
//
// Created by luuca on 21/10/2026.
//

#ifndef P6_MODELO_H
#define P6_MODELO_H
#include <assimp/Importer.hpp> //lee archivos
#include <assimp/scene.h> //representa los objetos
#include <assimp/postprocess.h> //representa las mallas

#include <vector>
#include <string>
#include <stdexcept>

#include "glad/glad.h"
#include <glm/glm.hpp>

namespace PAG {
    class Modelo {
    private:
        GLuint idVAO = 0;
        GLuint idVBO = 0;
        GLuint idIBO = 0;

        int indexCount = 0;

        glm::mat4 matModelo = glm::mat4(1.0);

        std::string nombre;

        //coordenadas caja limitadora
        glm::vec3 min;
        glm::vec3 max;

    public:
        Modelo();
        ~Modelo();

        void create(); //triangulo de prácticas anteriores
        void draw();
        void loadobj(std::string path);

        //metodos para que cada modelo pueda almacenar una matriz distinta
        void setMatriz(const glm::mat4& mat);
        const glm::mat4& getMatriz();
        const std::string& getNombre();

        const glm::vec3& getmin();
        const glm::vec3& getmax();


    };
} // PAG

#endif //P6_MODELO_H
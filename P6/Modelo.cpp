//
// Created by luuca on 21/10/2026.
//
#include <filesystem>
#include <limits>

#include "Modelo.h"

#include <iostream>

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
        glDrawElements ( GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr );

    }

    void Modelo::loadobj(std::string path) {
        //importar archiv
        Assimp::Importer imp;

        const aiScene* scene = imp.ReadFile(path,
            aiProcess_JoinIdenticalVertices |
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals);
        if (!scene || scene->mNumMeshes == 0) {
            throw std::runtime_error(
                "Error al cargar OBJ: " +
                std::string(imp.GetErrorString())
            );
        }

        // inicializacion caja limitadora
        min = glm::vec3(std::numeric_limits<float>::max());
        max = glm::vec3(std::numeric_limits<float>::lowest());

        nombre = std::filesystem::path(path).filename().string();

        //datos de las mallas (vertices e indices)
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;

        //1 obj tiene varias mallas
        for (int i = 0; i < scene->mNumMeshes; i++) {
            aiMesh* malla = scene->mMeshes[i]; //malla que estamos recorriendo
            //numero vertices almacenados para saber en que pos empezamos
            int offset = vertices.size() / 6; //3 de pos y 3 de color
            //vertices
            for ( int j = 0; j < malla->mNumVertices; j++) { //todos los vertices de esa malla

                aiVector3D posicion = malla->mVertices[j];

                // pos de cada vertice
                vertices.push_back(posicion.x);
                vertices.push_back(posicion.y);
                vertices.push_back(posicion.z);

                // color blanco
                //vertices.push_back(1.0f);
                //vertices.push_back(1.0f);
                //vertices.push_back(1.0f);

                //algo de sombras
                aiVector3D normal = malla->mNormals[j];

                glm::vec3 luz = glm::normalize(glm::vec3(0.5f, 1.0f, 0.8f));

                glm::vec3 N = glm::normalize(
                    glm::vec3(normal.x, normal.y, normal.z)
                );

                float intensidad = std::max(glm::dot(N, luz), 0.0f);

                float color = 0.3f + 0.7f * intensidad;

                vertices.push_back(color);
                vertices.push_back(color);
                vertices.push_back(color);


                // posicion caja limitadora
                min.x = std::min(min.x, posicion.x);
                min.y = std::min(min.y, posicion.y);
                min.z = std::min(min.z, posicion.z);

                max.x = std::max(max.x, posicion.x);
                max.y = std::max(max.y, posicion.y);
                max.z = std::max(max.z, posicion.z);

            }

            std::cout << "Bounding Box del modelo: " << path << std::endl;

            std::cout << "Min: "
                      << min.x << ", "
                      << min.y << ", "
                      << min.z << std::endl;

            std::cout << "Max: "
                      << max.x << ", "
                      << max.y << ", "
                      << max.z << std::endl;

            //caras de la malla en triangulos
            for (int j = 0; j < malla->mNumFaces; j++) {

                aiFace cara = malla->mFaces[j];

                for (int k = 0; k < cara.mNumIndices; k++) {
                    indices.push_back(offset + cara.mIndices[k]); //3 vertices de cada cara
                }
            }


        }

        //buffers de OpenGL
        //VAO
        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        //VBO
        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(GLfloat),
            vertices.data(),
            GL_STATIC_DRAW
        );

        glVertexAttribPointer( //POS
            0, 3, GL_FLOAT, GL_FALSE,
            6 * sizeof(GLfloat), nullptr
        );
        glEnableVertexAttribArray(0);

        glVertexAttribPointer( //COLOR
            1, 3, GL_FLOAT, GL_FALSE,
            6 * sizeof(GLfloat),
            reinterpret_cast<void*>(3 * sizeof(GLfloat))
        );
        glEnableVertexAttribArray(1);

        //IBO
        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(GLuint),
            indices.data(),
            GL_STATIC_DRAW
        );

        indexCount = int(indices.size());

        glBindVertexArray(0);
    }

    void Modelo::setMatriz(const glm::mat4& mat) {
        matModelo = mat;
    }

    const glm::mat4& Modelo::getMatriz() {
        return matModelo;
    }

    const std::string& Modelo::getNombre() {
        return nombre;
    }

    const glm::vec3& Modelo::getmin() {
        return min;
    }

    const glm::vec3& Modelo::getmax() {
        return max;
    }


} // PAG
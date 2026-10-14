//
// Created by luuca on 14/10/2026.
//

#ifndef P5_CAMERA_H
#define P5_CAMERA_H

#include <glm/glm.hpp>

namespace PAG {
    class Camera {
    private:
        // Atributos de pos
        glm::vec3 pos;
        glm::vec3 la;

        // Atributos de coordenadas
        glm::vec3 u;
        glm::vec3 v;
        glm::vec3 n;

        // Atributos de proyección
        float fovX;
        float zNear;
        float zFar;

        // Atributos aspect
        int width;
        int height;

        // Calcula los ejes locales de la cámara
        void userfriendlyAxes();

    public:
        Camera();

        // Matrices de la cámara (visión y proyección)
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;

        void setViewport(int width, int height);

    };
} // PAG

#endif //P5_CAMERA_H
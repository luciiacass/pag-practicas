//
// Created by luuca on 14/10/2026.
//


#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <stdexcept>

namespace PAG {

    Camera::Camera()
        : pos(0.0f, 0.0f, 3.0f),
          la(0.0f, 0.0f, 0.0f),
          fovX(45.0f),
          zNear(0.1f),
          zFar(100.0f),
          width(1024),
          height(576)
    {
        userfriendlyAxes();
    }

    void Camera::userfriendlyAxes() {
        // n = pos - la
        n = glm::normalize(pos - la);

        glm::vec3 Y(0.0f, 1.0f, 0.0f);

        // u = Y x n
        u = glm::normalize(glm::cross(Y, n));

        // v = n x u
        v = glm::normalize(glm::cross(n, u));
    }

    //Metodo para obtener la matriz de visión
    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(pos, la, v);
    }

    //Metodo para obtener la matriz de proyección
    glm::mat4 Camera::getProjectionMatrix() const {

        float aspect = float(width) / float(height);

        // Transformación de fovX a fovY
        float fovXRad = glm::radians(fovX); //pasamos a radianes
        //aplicamos la formula
        float fovY = 2.0f * std::atan( std::tan(fovXRad / 2.0f) / aspect);

        return glm::perspective(fovY, aspect, zNear, zFar);
    }

    void Camera::setViewport(int newWidth, int newHeight) {

        if (newWidth <= 0 || newHeight <= 0) {
            return;
        }

        width = newWidth;
        height = newHeight;
    }

}

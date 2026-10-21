//
// Created by luuca on 14/10/2026.
//


#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <stdexcept>

namespace PAG {

    Camera::Camera()
        : pos(0.0, 0.0, 3.0), //un poco desplazado en 'profundidad' para poder verlo
          la(0.0, 0.0, 0.0),
          fovX(45.0),
          zNear(0.1),
          zFar(100.0),
          width(1024),
          height(576)
    {
        userfriendlyAxes();
    }

    void Camera::userfriendlyAxes() {

        // n = pos - la
        n = glm::normalize(pos - la);

        //Evitamos que Y y n sean colineales
        glm::vec3 Y(0.0, 1.0, 0.0);
        glm::vec3 Z(0.0, 0.0, 1.0);

        if (glm::abs(glm::dot(n, Y)) > 0.999) {

            if (n.y > 0.0) {
                u = glm::normalize(glm::cross(Z, n));
            } else {
                u = glm::normalize(glm::cross(-Z, n));
            }

        } else {
            u = glm::normalize(glm::cross(Y, n));
        }

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
        float fovY = 2.0 * std::atan( std::tan(fovXRad / 2.0) / aspect);

        return glm::perspective(fovY, aspect, zNear, zFar);
    }

    void Camera::setViewport(int newWidth, int newHeight) {

        if (newWidth <= 0 || newHeight <= 0) {
            return;
        }

        width = newWidth;
        height = newHeight;
    }

    // Mp = T(p) x Rv x T(-p) -> Se aplica al punto 'la'
    void Camera::pan(float angle) {
        // angulo a radianes
        float angleRad = glm::radians(angle);

        glm::mat4 M(1.0);

        //transformacion
        M = glm::translate(M, pos);
        M = glm::rotate(M, angleRad, v);
        M = glm::translate(M, -pos);

        // aplicada a 'la'
        la = glm::vec3( M * glm::vec4(la, 1.0f));

        userfriendlyAxes();
    }

    // Mt = T(p) x Ru x T(-p) -> Se aplica al punto 'la'
    void Camera::tilt(float angle) {
        float rad = glm::radians(angle);

        glm::mat4 M(1.0);
        M = glm::translate(M, pos);
        M = glm::rotate(M, rad, u);
        M = glm::translate(M, -pos);

        la = glm::vec3(M * glm::vec4(la, 1.0));

        userfriendlyAxes();
    }

    // 'Camara en railes' -> Se aplica a la pos de la camara y 'la' (Y fija)
    void Camera::dolly(float dx, float dz) {

        glm::vec3 desplazamiento(dx, 0.0, dz);

        pos += desplazamiento;
        la += desplazamiento;
    }

    // 'Camara de arriba a abajo' -> -> Se aplica a la pos de la camara y 'la' (Z y X fijas)
    void Camera::crane(float dy) {

        glm::vec3 desplazamiento(0.0, dy, 0.0);

        pos += desplazamiento;
        la += desplazamiento;
    }

    // Mo = T(la) x Mlongitud x Mlatitud x T(-la)
    // Se mueve la pos de la cámara, con centro de rotación el punto al que mira.
    // Latitud: u fijo
    // Longitud: v fijo
    void Camera::orbit(float latitude, float longitude) {

        float latRad = glm::radians(latitude);
        float lonRad = glm::radians(longitude);

        glm::mat4 M(1.0);

        M = glm::translate(M, la);
        M = glm::rotate(M, lonRad, v);
        M = glm::rotate(M, latRad, u);
        M = glm::translate(M, -la);

        pos = glm::vec3(M * glm::vec4(pos, 1.0f));

        userfriendlyAxes();
    }

    // Cambia el ángulo de la camara
    void Camera::zoom(float angle) {

        fovX += angle;

        //evitamos que la camara mire 'hacia atrás'
        if (fovX < 1.0) {
            fovX = 1.0;
        }

        if (fovX > 179.0) {
            fovX = 179.0;
        }
    }


}

//
// Created by luuca on 25/11/2026.
//

#include "Luz.h"

namespace PAG {

    Luz::Luz(){}

    tipoLuz Luz::getTipo() {
        return tipo_luz;
    }
    void Luz::setTipo(tipoLuz luz) {
        tipo_luz = luz;
    }

    glm::vec3 Luz::getIa() {
        return Ia;
    }
    glm::vec3 Luz::getId() {
        return Id;
    }
    glm::vec3 Luz::getIs() {
        return Is;
    }

    glm::vec3 Luz::getPosicion() {
        return posicion;
    }
    glm::vec3 Luz::getDireccion() {
        return direccion;
    }

    float Luz::getGamma() {
        return gamma;
    }
    float Luz::getS() {
        return s;
    }

    void Luz::setIa(glm::vec3& color) {
        Ia = color;
    }
    void Luz::setId(glm::vec3& color) {
        Id = color;
    }
    void Luz::setIs(glm::vec3& color) {
        Is = color;
    }

    void Luz::setPosicion(glm::vec3& pos) {
        posicion = pos;
    }
    void Luz::setDireccion(glm::vec3& dir) {
        direccion = dir;
    }

    void Luz::setGamma(float angulo) {
        gamma = angulo;
    }
    void Luz::setS(float exponente) {
        s = exponente;
    }
} // PAG
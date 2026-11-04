//
// Created by luuca on 04/11/2026.
//

#include "Material.h"

#include <algorithm>

namespace PAG {

    float* Material::getAmbiente() {
        return ambiente;
    }

    float* Material::getDifuso() {
        return difuso;
    }

    float* Material::getEspecular() {
        return especular;
    }

    float Material::getExponent() {
        return exp;
    }

    void Material::setAmbiente( float* color) {
        for (int i = 0; i < 3; i++) {
            ambiente[i] = std::clamp(color[i], 0.0f, 1.0f);
        }
    }

    void Material::setDifuso( float* color) {
        for (int i = 0; i < 3; i++) {
            difuso[i] = std::clamp(color[i], 0.0f, 1.0f);
        }
    }

    void Material::setEspecular(float* color) {
        for (int i = 0; i < 3; i++) {
            especular[i] = std::clamp(color[i], 0.0f, 1.0f);
        }
    }

    void Material::setExponente(float valor) {
        exp = std::clamp(valor, 1.0f, 256.0f);
    }

} // PAG
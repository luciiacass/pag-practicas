//
// Created by luuca on 25/11/2026.
//

#ifndef P8_LUZ_H
#define P8_LUZ_H

#include <glm/glm.hpp>

namespace PAG {

    enum class tipoLuz {
        ambiente,
        puntual,
        direccional,
        foco
    };

    class Luz {
    private:
        tipoLuz tipo_luz = tipoLuz::ambiente;

        //colores
        glm::vec3 Ia = glm::vec3(1.0f);
        glm::vec3 Id = glm::vec3(1.0f);
        glm::vec3 Is = glm::vec3(1.0f);

        glm::vec3 posicion = glm::vec3(0.0f, 2.0f, 2.0f);
        glm::vec3 direccion = glm::vec3(0.0f, -1.0f, 0.0f);

        //foco
        float gamma = 30.0f;
        float s = 1.0f;

        bool active = true;

    public:
        Luz();

        tipoLuz getTipo();
        void setTipo(tipoLuz luz);

        glm::vec3 getIa();
        glm::vec3 getId();
        glm::vec3 getIs();

        glm::vec3 getPosicion();
        glm::vec3 getDireccion();

        float getGamma();
        float getS();

        void setIa(const glm::vec3& color);
        void setId(const glm::vec3& color);
        void setIs(const glm::vec3& color);

        void setPosicion(const glm::vec3& pos);
        void setDireccion(const glm::vec3& dir);

        void setGamma(float angulo);
        void setS(float exponente);

        bool getActiva();
        void setActiva(bool estado);
    };
} // PAG

#endif //P8_LUZ_H
//
// Created by luuca on 04/11/2026.
//

#ifndef P7_MATERIAL_H
#define P7_MATERIAL_H

namespace PAG {
    class Material {
    private:
        float ambiente[3] = {0.2, 0.2, 0.2}; //iluminacion ambiente
        float difuso[3] = {0.2, 0.2, 0.2}; //iluminacion del material
        float especular[3] = {0.2, 0.2, 0.2}; //iluminacion de los reflejos
        float exp = 32; //concentracion del brillo especular

    public:
        float* getAmbiente();
        float* getDifuso();
        float* getEspecular();
        float getExponent();

        void setAmbiente(float* a);
        void setDifuso(float* d);
        void setEspecular(float* e);
        void setExponente(float e);

    };
} // PAG

#endif //P7_MATERIAL_H
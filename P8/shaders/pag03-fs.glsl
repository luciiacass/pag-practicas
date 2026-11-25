#version 410
in vec3 colorGradiente;
in vec3 posicionVision;
in vec3 normalVision;

out vec4 colorFragmento;

uniform vec3 materialAmbiente;
uniform vec3 materialDifuso;
uniform vec3 materialEspecular;
uniform float exponente;

//propiedades luz y foco
uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;

uniform vec3 posicionLuz;
uniform vec3 direccionLuz;

uniform float gamma;
uniform float s;

//reflexion difusa y especular
vec3 calcularReflexion(vec3 L){
    vec3 N = normalize(normalVision);
    vec3 V = normalize(-posicionVision);

    //difuso
    float difuso = max(dot(N, L), 0.0);
    vec3 Cd = Id * materialDifuso * difuso;

    //especular
    vec3 Cs = vec3(0.0);

    if (difuso > 0.0) {

            vec3 R = reflect(-L, N);

            float especular = pow(
                max(dot(R, V), 0.0),
                exponente
            );

            Cs = Is * materialEspecular * especular;
    }
    return Cd + Cs;

}

//subrutinas
subroutine vec4 fCalcularColor();
subroutine uniform fCalcularColor uMetodo;

//alambre
subroutine (fCalcularColor)
vec4 alambre()
{
    return vec4(1.0, 0.0, 0.0, 1.0);
}

//solido
subroutine (fCalcularColor)
vec4 material()
{
    return vec4(materialDifuso, 1.0);
}

//luz ambiente Ca = Ia * Ka
subroutine (fCalcularColor)
vec4 ambiente(){
    vec3 color = Ia * materialAmbiente;
    return vec4(color, 1.0);
}

//luz puntual
subroutine (fCalcularColor)
vec4 puntual() {
    vec3 L = normalize(posicionLuz - posicionVision);
    vec3 color = calcularReflexion(L);

    return vec4(color, 1.0);
}

//luz direccional
subroutine (fCalcularColor)
vec4 direccional() {
    vec3 L = normalize(-direccionLuz);
    vec3 color = calcularReflexion(L);

    return vec4(color, 1.0);
}

//foco
subroutine (fCalcularColor)
vec4 foco() {
    vec3 L = normalize(posicionLuz - posicionVision);
    vec3 D = normalize(direccionLuz);

    float cosAngulo = dot(D, -L);
    float limite = cos(radians(gamma));

    //si esta dentro del cono recibe luz
    if (cosAngulo >= limite) {
        float factor = pow(max(cosAngulo, 0.0), s);
        vec3 color = calcularReflexion(L) * factor;

        return vec4(color, 1.0);
    }

    // si esta fuera del cono no recibe luz
    return vec4(0.0, 0.0, 0.0, 1.0);
}

void main ()
{
    colorFragmento = uMetodo();

}
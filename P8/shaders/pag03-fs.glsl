#version 410
in vec3 colorGradiente;

out vec4 colorFragmento;
uniform vec3 materialDifuso;

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

void main ()
{
    colorFragmento = uMetodo();

}
#version 410
in vec3 colorGradiente;

out vec4 colorFragmento;
uniform vec3 materialDifuso;

void main ()
{
    colorFragmento = vec4 ( materialDifuso, 1.0 );

}
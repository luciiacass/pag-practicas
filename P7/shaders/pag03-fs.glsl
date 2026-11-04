#version 410
in vec3 colorGradiente;
out vec4 colorFragmento;
void main ()
{ colorFragmento = vec4 ( colorGradiente, 1.0 );
}
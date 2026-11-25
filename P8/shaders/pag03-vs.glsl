#version 410
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 normal;

out vec3 colorGradiente;
out vec3 posicionVision;
out vec3 normalVision;


uniform mat4 MVP; //pos para el renderizado
uniform mat4 MV; //pos en el espacio de visión
uniform mat3 mNormal;

void main (){
    posicionVision = vec3(MV * vec4(posicion, 1.0));
    normalVision = normalize(mNormal * normal);

    gl_Position = MVP * vec4 ( posicion, 1 );
}
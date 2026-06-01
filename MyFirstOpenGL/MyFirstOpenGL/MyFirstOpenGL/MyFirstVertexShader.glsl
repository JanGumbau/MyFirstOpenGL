#version 440 core

layout(location = 0) in vec3 posicion;

uniform mat4 translationMatrix;
uniform mat4 rotationMatrix;
uniform mat4 scaleMatrix;
uniform float time;

void main() {

    float velocitat = 0.8; // radians per segon
    float angle = time * velocitat;
    float sinus = sin(angle);
    float cosinus = cos(angle);

 
    mat4 rotacioNoria = mat4(
        vec4( cosinus, sinus, 0.0, 0.0),
        vec4(-sinus, cosinus, 0.0, 0.0),
        vec4( 0.0,    0.0,    1.0, 0.0),
        vec4( 0.0,    0.0,    0.0, 1.0)
    );

    mat4 model = translationMatrix * rotacioNoria * scaleMatrix;
    gl_Position = model * vec4(posicion, 1.0);
}

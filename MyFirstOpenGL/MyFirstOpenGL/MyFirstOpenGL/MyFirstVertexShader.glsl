#version 440 core

layout(location = 0) in vec3 posicion;

uniform mat4 translationMatrix;
uniform mat4 rotationMatrix;
uniform mat4 scaleMatrix;

// Temps en segons des de l'inici del programa.
// L'utilitzem per fer girar la noria.
uniform float time;

void main() {

    // Velocitat de rotacio (radians per segon)
    float velocitat = 0.8;
    float angle = time * velocitat;

    // Matriu de rotacio al voltant de l'eix Z (la noria gira al pla XY)
    float c = cos(angle);
    float s = sin(angle);

    mat4 rotacioNoria = mat4(
        vec4( c,  s, 0.0, 0.0),
        vec4(-s,  c, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );

    // Apliquem la transformacio model: escalat -> rotacio noria -> translacio
    mat4 model = translationMatrix * rotacioNoria * scaleMatrix;

    gl_Position = model * vec4(posicion, 1.0);
}

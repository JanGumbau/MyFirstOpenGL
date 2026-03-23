#version 440 core

layout (location=0) in vec2 position;
layout (location=1) in vec2 random;

void main(){

	gl_Position = vec4(position.x + random.x, position.y + random.y, 0.0, 1.0);


}


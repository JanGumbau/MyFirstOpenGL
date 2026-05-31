#version 440 core

// Acceptem linies (perque dibuixem la noria amb GL_LINES)
layout (lines) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(){

	for(int i = 0; i < gl_in.length(); i++){
		gl_Position = projectionMatrix * viewMatrix * gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();
}

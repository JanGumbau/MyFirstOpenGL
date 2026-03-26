#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


void main(){
	
	for(int i=0; i < gl_in.length(); i++){
	
	gl_Position = gl_in[i].gl_Position + vec4(0,-1,0,0);
	EmitVertex();
	
	}

	EndPrimitive();
	

}
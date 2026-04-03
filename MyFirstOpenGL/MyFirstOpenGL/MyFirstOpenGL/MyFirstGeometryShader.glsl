#version 440 core

layout (triangles) in;

layout (triangle_strip, max_vertices = 6) out;

void main(){
	
	for(int i=0; i < gl_in.length(); i++){
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
	
	for(int i = gl_in.length() - 1; i >=0; i--){
		gl_Position = vec4(gl_in[i].gl_Position.x, -gl_in[i].gl_Position.y, gl_in[i].gl_Position.z, gl_in[i].gl_Position.w);
		EmitVertex();
	}
	EndPrimitive();
}
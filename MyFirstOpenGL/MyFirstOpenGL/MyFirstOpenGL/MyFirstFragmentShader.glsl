#version 440 core

out vec4 fragColor;

void main(){
	
	fragColor = vec4(1.0,0.0,0.0,1.0);

	if(gl_FragCoord.x > 320.0){
	
		fragColor = vec4(0.0,0.0,1.0,1.0);
		}else{
		fragColor = vec4(1.0,0.0,0.0,1.0);

	}
}
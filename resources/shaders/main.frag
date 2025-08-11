#version 460
in vec2 vUv;
in vec3 vertexOut;

out vec4 fragCol;

void main(){
	fragCol = vec4(vUv,0.,1.);
}
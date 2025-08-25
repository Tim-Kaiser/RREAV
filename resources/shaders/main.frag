#version 460

layout(std430, binding=0) buffer audioData
{
	float samples[];
};

in vec2 vUv;
in vec3 vertexOut;


out vec4 fragCol;

void main(){

	float wave = samples[int(vUv.x * samples.length())] * 0.9;

	float c = smoothstep(0.0, 0.2, abs(wave - vUv.y));

	fragCol = vec4(c, c, c, 1.);
}
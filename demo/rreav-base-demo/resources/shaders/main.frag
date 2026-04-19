#version 460

layout(std430, binding=0) buffer sampleData
{
	float samples[];
};

layout(std430, binding=1) buffer frequencyData
{
	float frequencies[];
};


in vec2 vUv;
in vec3 vertexOut;


out vec4 fragCol;

void main(){

	int idx = int(vUv.x * float(samples.length()));
	idx = clamp(idx, 0, int(samples.length()) - 1);
	float wave = samples[idx];

	int fftIdx = int(vUv.x * float(frequencies.length()));
	fftIdx = clamp(fftIdx, 0, int(frequencies.length()) - 1);
	float fft = clamp(frequencies[fftIdx], -1.0, 1.0);

	vec3 col = vec3( fft, 4.0*fft*(1.0-fft), 1.0-fft ) * fft;
	col += 1.0 - smoothstep(0.0, 0.15, abs(wave - vUv.y));

	fragCol = vec4(col, 1.);
}
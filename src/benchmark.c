#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char* seed;

char* make_string(const char* s){
	char* str = malloc(strlen(s) + 1);
	memcpy(str, s, strlen(s) + 1);
	return str;
}

void shiftString(char* string){
	int stringLength = strlen(string);
	for(int i = stringLength - 1; i >= 1; i--){
		string[i] = string[i - 1];
	}
	string[0] = '0';
}

char xor(char a, char b){
	if(a == b){
		return '0';
	}
	return '1';
}
char lfsr(){
	// Returns random number between 0 and 2^32
	char a = seed[strlen(seed) - 3];
	char b = seed[strlen(seed) - 11];
	char carryBit = xor(a, b);

	char outputBit = seed[strlen(seed) - 1];

	shiftString(seed);

	seed[0] = carryBit;

	return outputBit;
}

int rng(){
	// Create number by flipping bits
	int answer = 0;
	for(int i = 0; i < 24; i++){
		if(lfsr() == '1'){
			answer += pow(2, i);
		}
	}
	return answer;
}

void noise2D(float* seed_array, int width, float* noise_array, int octaves, float roughness){
	// Fill seed array with rng values
	for(int i = 0; i < width * width; i++){
		seed_array[i] = (rng() % 1000) / 999.0f;
	}
	// Fill noise array with zeros
	for(int i = 0; i < width * width; i++){
		noise_array[i] = 0.0f;
	}

	for(int x = 0; x < width; x++){
		for(int y = 0; y < width; y++){
			float noise = 0.0f;
			float scale = 1.0f;
			float maxScale = 0.0f;
			for(int o = 0; o < octaves; o++){
				int pitch = width >> o;
				int sampleX1 = (x / pitch) * pitch;
				int sampleY1 = (y / pitch) * pitch;
				int sampleX2 = (sampleX1 + pitch) % width;
				int sampleY2 = (sampleY1 + pitch) % width;
				float blendX = (x - sampleX1) / (float)pitch;
				float blendY = (y - sampleY1) / (float)pitch;

				float lerpT = (1.0f - blendX) * seed_array[sampleY1 * width + sampleX1] + blendX * seed_array[sampleY1 * width + sampleX2];
				float lerpB = (1.0f - blendX) * seed_array[sampleY2 * width + sampleX1] + blendX * seed_array[sampleY2 * width + sampleX2];
				
				noise += (blendY * (lerpB - lerpT) + lerpT) * scale;
				maxScale += scale;
				scale /= roughness;
			}
			noise_array[y * width + x] = noise / maxScale;
		}
	}
}

int main(){
	seed = make_string("00001101001110100100100100101001");
	
	// Perlin noise code
	const int WIDTH = 1024;
	float* seed_array = malloc(WIDTH * WIDTH * sizeof(float));
	float* noise_array = malloc(WIDTH * WIDTH * sizeof(float));
	float roughness = 1.6f;

	clock_t begin = clock();
	noise2D(seed_array, WIDTH, noise_array, 8, roughness);
	clock_t time = clock() - begin;
	
	float duration = (float)time/CLOCKS_PER_SEC;
	printf("Time: %f seconds\n", duration);

	FILE* file = fopen("output_c.json", "w");
	const char* line = "{\"array\": [";
	fwrite(line, 1, strlen(line), file);

	for(int i = 0; i < WIDTH * WIDTH - 1; i++){
		fprintf(file, "%f,", noise_array[i]);
	}
	fprintf(file, "%f", noise_array[WIDTH * WIDTH - 1]);

	fwrite("]}", 1, 2, file);
	fclose(file);

	return 0;
}
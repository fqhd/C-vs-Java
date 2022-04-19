#include <stdio.h>
#include <math.h>
#include <string.h>

int random(){
	static int seed = 1943;
	int squared_num = pow(seed, 2);
	char str[9];
	str[8] = '\0';

	// Initialize the str with 0s
	for(int i = 0; i < 8; i++){
		str[i] = '0';
	}

	// Print squared_num to str
	int multiple = 1;
	for(int i = 7; i >= 0; i--){
		int divided_num = squared_num / multiple;
		int digit = divided_num % 10;
		str[i] = 48 + digit;
		multiple *= 10;
	}

	// Get 4 middle numbers in str and store them in r->curr
	int answer = 0;
	int multiplier = 1000;
	for(int i = 2; i < 6; i++){
		char dig = str[i];
		int digit = dig - 48;
		answer += digit * multiplier;
		multiplier /= 10;
	}

	seed = answer;

	return answer;
}

// Perlin-Like noise implementation by javidx9 https://www.youtube.com/watch?v=6-0UaeJBumA&ab_channel=javidx9
void noise1D(float* seed_array, int length, float* noise_array, int octaves){
	for(int x = 0; x < length; x++){
		float noise = 0.0f;
		float scale = 1.0f;
		float maxScale = 0.0f;
		for(int o = 0; o < octaves; o++){
			int pitch = length >> o;
			int sample1 = (x / pitch) * pitch;
			int sample2 = (sample1 + pitch) % length;
			float blend = (x - sample1) / pitch;
			float lerp = (blend - 1.0f) * seed_array[sample1] + blend * seed_array[sample2];
			noise += lerp * scale;
			scale /= 2.0f;
			maxScale += scale;
		}
		noise_array[x] = noise / maxScale;
	}
}

void noise2D(float* seed_array, int width, float* noise_array, int octaves){
	// Fill seed array with random values
	for(int i = 0; i < width * width; i++){
		seed_array[i] = random() / 9999.0f;
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
				float blendY = (x - sampleY1) / (float)pitch;

				float lerpT = (1.0f - blendX) * seed_array[sampleY1 * width + sampleX1] + blendX * seed_array[sampleY1 * width + sampleX2];
				float lerpB = (1.0f - blendX) * seed_array[sampleY2 * width + sampleX1] + blendX * seed_array[sampleY2 * width + sampleX2];
				
				noise += (blendY * (lerpB - lerpT) + lerpT) * scale;
				scale /= 2.0f;
				maxScale += scale;
			}
			noise_array[y * width + x] = noise / maxScale;
		}
	}
}

int main(){
	const int WIDTH = 32;
	float seed_array[WIDTH * WIDTH];
	float noise_array[WIDTH * WIDTH];
	noise1D(seed_array, WIDTH, noise_array, 4);

	return 0;
}
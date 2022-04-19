#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

// int rng(){
// 	static int seed = 651321;
// 	int squared_num = pow(seed, 2);
// 	char str[8];

// 	// Initialize the str with 0s
// 	for(int i = 0; i < 8; i++){
// 		str[i] = '0';
// 	}

// 	// Print squared_num to str
	// int multiple = 1;
	// for(int i = 7; i >= 0; i--){
	// 	int divided_num = squared_num / multiple;
	// 	int digit = divided_num % 10;
	// 	str[i] = 48 + digit;
	// 	multiple *= 10;
	// }

	// // Get 4 middle numbers in str and store them in r->curr
	// int answer = 0;
	// int multiplier = 1000;
	// for(int i = 2; i < 6; i++){
	// 	char dig = str[i];
	// 	int digit = dig - 48;
	// 	answer += digit * multiplier;
	// 	multiplier /= 10;
	// }

// 	seed = answer;

// 	return answer;
// }

// int getPseudoRandomSeed(){
// 	static int seed = 7;
// 	const int MAGIC_NUMBER = 37;
	
// 	// Get new seed
// 	int b = abs(MAGIC_NUMBER - seed);
// 	seed = ((seed * b) % MAGIC_NUMBER) + 11;

// 	return seed;
// }

typedef struct {
	uint64_t a;
	uint64_t b;
} Seed;

uint8_t get_num_digits(uint64_t n){
	uint8_t num_digits = 0;

	uint64_t multiple = 1;
	while(n/multiple != 0){
		num_digits++;
		multiple *= 10;
	}

	return num_digits;
}

char* to_string(uint64_t n){
	// Get num digits
	uint64_t num_digits = get_num_digits(n);

	// Allocate enough space for string
	char* str = malloc(num_digits + 1);

	// Print number into string
	uint64_t multiple = 1;
	for(int i = num_digits - 1; i >= 0; i--){
		uint64_t divided_num = n / multiple;
		int digit = divided_num % 10;
		str[i] = 48 + digit;
		multiple *= 10;
	}
	str[num_digits] = '\0';

	// Return string
	return str;
}

uint64_t to_number(char* s){
	uint64_t number = 0;
	uint64_t multiple = 1;
	for(int i = strlen(s) - 1; i >= 0; i--){
		number += (s[i] - 48) * multiple;
		multiple *= 10;
	}
	return number;
}

char* mix_strings(char* a, char* b){
	int joined_lengths = strlen(a) + strlen(b);
	char* result = malloc(joined_lengths + 1);
	int aIndex = 0;
	int bIndex = 0;
	for(int i = 0; i < joined_lengths; i++){
		if(i % 2){
			result[i] = a[aIndex];
			aIndex++;
		}else{
			result[i] = b[bIndex];
			bIndex++;
		}
	}
	result[joined_lengths] = '\0';
	return result;
}

uint64_t getPRN(){
	static Seed s = {2512, 3684};

	char* firstNum = to_string(s.a);
	char* secondNum = to_string(s.b);
	char* mixed_strings = mix_strings(firstNum, secondNum);

	uint64_t bigNum = to_number(mixed_strings);
	bigNum = bigNum * bigNum;

	char* bigNumString = to_string(bigNum);

	int bigNumStringLength = strlen(bigNumString);
	char* firstOut = malloc(5);
	firstOut[4] = '\0';
	for(int i = 0; i < 4; i++){
		int digitInBigNum = bigNumString[bigNumStringLength - i - 1] - 48;
		firstOut[i] = bigNumString[digitInBigNum];
	}

	s.a = to_number(firstOut);
	s.b++;
	s.b = s.b % 10000; // Wrap around

	free(firstOut);
	free(bigNumString);
	free(secondNum);
	free(firstNum);

	return s.a + s.b;
}

// int rng2(){
// 	// Create number by flipping bits
// 	unsigned int answer = 0;
// 	for(int i = 0; i < 32; i++){
// 		if(getPseudoRandomSeed() % 2){
// 			answer |= 1 << i;
// 		}
// 	}
// 	return answer;
// }

// Perlin-Like noise implementation by javidx9 https://www.youtube.com/watch?v=6-0UaeJBumA&ab_channel=javidx9
void noise1D(float* seed_array, int length, float* noise_array, int octaves){
	// Fill seed array with rng values
	for(int i = 0; i < length; i++){
		seed_array[i] = getPRN() / 99999.0f;
	}
	// Fill noise array with zeros
	for(int i = 0; i < length; i++){
		noise_array[i] = 0.0f;
	}

	for(int x = 0; x < length; x++){
		float noise = 0.0f;
		float scale = 1.0f;
		float maxScale = 0.0f;
		for(int o = 0; o < octaves; o++){
			int pitch = length >> o;
			int sample1 = (x / pitch) * pitch;
			int sample2 = (sample1 + pitch) % length;
			float blend = (x - sample1) / pitch;
			float lerp = (1.0f - blend) * seed_array[sample1] + blend * seed_array[sample2];
			noise += lerp * scale;
			scale /= 2.0f;
			maxScale += scale;
		}
		noise_array[x] = noise / maxScale;
	}
}

void noise2D(float* seed_array, int width, float* noise_array, int octaves){
	// Fill seed array with rng values
	for(int i = 0; i < width * width; i++){
		seed_array[i] = rand() / 9999.0f;
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
				maxScale += scale;
				scale /= 2.0f;
			}
			noise_array[y * width + x] = noise / maxScale;
		}
	}
}

int main(){
	int numOdds = 0;
	for(int i = 0; i < 100000; i++){
		if((getPRN() % 10) == 0){
			numOdds++;
		}
	}
	float percentage = numOdds / 100000.0f;

	printf("Accuracy: %f\n", percentage);


	srand(4768);
	const int WIDTH = 128;
	float* seed_array = malloc(WIDTH * WIDTH * sizeof(float));
	float* noise_array = malloc(WIDTH * WIDTH * sizeof(float));
	noise2D(seed_array, WIDTH, noise_array, 10);

	FILE* file = fopen("output.json", "w");
	const char* line = "{\"array\": [";
	fwrite(line, 1, strlen(line), file);

	for(int i = 0; i < WIDTH * WIDTH - 1; i++){
		fprintf(file, "%f,", seed_array[i]);
	}
	fprintf(file, "%f", seed_array[WIDTH * WIDTH - 1]);

	fwrite("]}", 1, 2, file);
	fclose(file);

	return 0;
}
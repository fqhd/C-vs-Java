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
void fnoise(float* seed_array, int length, float* noise_array, int octaves){
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

int main(){
	const int LENGTH = 32;
	float seed_array[LENGTH];
	float noise_array[LENGTH];
	fnoise(seed_array, LENGTH, noise_array, 4);

	return 0;
}
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

int main(){
	printf("%d\n", random());

	return 0;
}
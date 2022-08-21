import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.Arrays;
import java.util.List;

public class Benchmark {

    public static void main(String[] args) {
        main();
    }

	static char[] seed = {'0', '0', '0', '0', '1', '1', '0', '1', '0', '0', '1', '1', '1', '0', '1', '0', '0', '1', '0', '0', '1', '0', '0', '1', '0', '0', '1', '0', '1', '0', '0', '1'};

	static void shiftString(char[] string){
		int stringLength = string.length;
		for(int i = stringLength - 1; i >= 1; i--){
			string[i] = string[i - 1];
		}
		string[0] = '0';
	}

	static char xor(char a, char b){
		if(a == b){
			return '0';
		}
		return '1';
	}

	static char lfsr(){
		// Returns random number between 0 and 2^32
		char a = seed[seed.length - 3];
		char b = seed[seed.length - 11];
		char carryBit = xor(a, b);

		char outputBit = seed[seed.length - 1];

		shiftString(seed);

		seed[0] = carryBit;

		return outputBit;
	}

	static int rng(){
		// Create number by flipping bits
		int answer = 0;
		for(int i = 0; i < 24; i++){
			if(lfsr() == '1'){
				answer += Math.pow(2, i);
			}
		}
		return answer;
	}

    private static int alfd = 0;

    static void noise2D(float[] seed_array, int width, float[] noise_array, int octaves, float roughness) {
        // Fill seed array with rng values
        for (int i = 0; i < width * width; i++) {
            seed_array[i] = (rng() % 1000) / 999.0f;
        }
        // Fill noise array with zeros
        for (int i = 0; i < width * width; i++) {
            noise_array[i] = 0.0f;
        }

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < width; y++) {
                int total = 0;
                for (int o = 0; o < octaves; o++) {
                    total += rng() % 255;
                }
                noise_array[y * width + x] = (float)total / (255 * octaves);
            }
        }
    }

    static int main() {
        int WIDTH = 1024;

        float[] seed_array = new float[WIDTH * WIDTH];
        float[] noise_array = new float[WIDTH * WIDTH];
		float roughness = 1.6f;

        long start = System.currentTimeMillis();
        noise2D(seed_array, WIDTH, noise_array, 8, roughness);
        System.out.println("Time: " + (System.currentTimeMillis() - start)/1000.0f + " seconds");

        try {
            Files.write(new File("output_java.json").toPath(), List.of(Arrays.toString(noise_array)), StandardCharsets.UTF_8);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return 0;
    }
}
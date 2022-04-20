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

    static void noise2D(float[] seed_array, int width, float[] noise_array, int octaves, float roughness) {
        // Fill seed array with rng values
        for (int i = 0; i < width * width; i++) {
            seed_array[i] = (float)(rng() % 1000) / 999.0f;
        }
        // Fill noise array with zeros
        for (int i = 0; i < width * width; i++) {
            noise_array[i] = 0.0f;
        }

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < width; y++) {
                float noise = 0.0f;
                float scale = 1.0f;
                float maxScale = 0.0f;
                for (int o = 0; o < octaves; o++) {
                    int pitch = width >> o;
                    int sampleX1 = (x / pitch) * pitch;
                    int sampleY1 = (y / pitch) * pitch;
                    int sampleX2 = (sampleX1 + pitch) % width;
                    int sampleY2 = (sampleY1 + pitch) % width;
                    float blendX = (x - sampleX1) / (float) pitch;
                    float blendY = (x - sampleY1) / (float) pitch;

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

    static int main() {
		System.out.println("random: " + rng());
		System.out.println("random: " + rng());
		System.out.println("random: " + rng());
		System.out.println("random: " + rng());

        int WIDTH = 1024;

        float[] seed_array = new float[WIDTH * WIDTH];
        float[] noise_array = new float[WIDTH * WIDTH];
		float roughness = 1.6f;

        long start = System.currentTimeMillis();
        noise2D(seed_array, WIDTH, noise_array, 8, roughness);
        System.out.println("End: " + (System.currentTimeMillis() - start));

        try {
            Files.write(new File("output_java.json").toPath(), List.of(Arrays.toString(noise_array)), StandardCharsets.UTF_8);
        } catch (Exception e) {
            e.printStackTrace();
        }

        System.out.println(noise_array[0]);

        return 0;
    }
}
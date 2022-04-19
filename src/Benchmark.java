import java.io.File;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.Arrays;
import java.util.List;

public class Benchmark {

    public static void main(String[] args) {
        long start = System.currentTimeMillis();
        main();
        System.out.println("End: " + (System.currentTimeMillis() - start));
    }
    
    static class Seed {
        long a;
        long b;

        public Seed(long a, long b) {
            this.a = a;
            this.b = b;
        }
    }

    static int get_num_digits(long n) {
        int num_digits = 0;

        long multiple = 1;
        while (n / multiple != 0) {
            num_digits++;
            multiple *= 10;
        }

        return num_digits;
    }

    static char[] to_string(long n) {
        // Get num digits
        int num_digits = get_num_digits(n);

        // Allocate enough space for string
        char[] str = new char[num_digits];

        // Print number into string
        long multiple = 1;
        for (int i = num_digits - 1; i >= 0; i--) {
            long divided_num = n / multiple;
            int digit = (int) (divided_num % 10);
            str[i] = (char) (48 + digit);
            multiple *= 10;
        }
        // Return string
        return str;
    }

    static long to_number(char[] s) {
        long number = 0;
        long multiple = 1;
        for (int i = s.length - 1; i >= 0; i--) {
            number += (s[i] - 48) * multiple;
            multiple *= 10;
        }
        return number;
    }

    static char[] mix_strings(char[] a, char[] b) {
        int joined_lengths = a.length + b.length;
        char[] result = new char[joined_lengths];
        int aIndex = 0;
        int bIndex = 0;
        for (int i = 0; i < joined_lengths; i++) {
            if (i % 2 == 1) {
                result[i] = a[aIndex];
                aIndex++;
            } else {
                result[i] = b[bIndex];
                bIndex++;
            }
        }
        return result;
    }

    static Seed s = new Seed(2512, 3684);

    static long getPRN() {
        if (s.b < 1000) {
            s.b = 1000;
        }
        if (s.a < 1000) {
            s.a = 1000;
        }

        char[] firstNum = to_string(s.a);
        char[] secondNum = to_string(s.b);
        char[] mixed_strings = mix_strings(firstNum, secondNum);

        long bigNum = to_number(mixed_strings);
        bigNum = bigNum * bigNum;

        char[] bigNumString = to_string(bigNum);

        int bigNumStringLength = bigNumString.length;
        char[] firstOut = new char[4];
        for (int i = 0; i < 4; i++) {
            int digitInBigNum = bigNumString[bigNumStringLength - i - 1] - 48;
            firstOut[i] = bigNumString[digitInBigNum];
        }

        s.a = to_number(firstOut);
        s.b++;
        s.b = s.b % 10000; // Wrap around

        return s.a + s.b;
    }

    static int rng2() {
        // Create number by flipping bits
        int answer = 0;
        for (int i = 0; i < 32; i++) {
            if (getPRN() % 2 == 1) {
                answer |= 1 << i;
            }
        }
        return answer;
    }

    // Perlin-Like noise implementation by javidx9 https://www.youtube.com/watch?v=6-0UaeJBumA&ab_channel=javidx9
    static void noise1D(float[] seed_array, int length, float[] noise_array, int octaves) {
        // Fill seed array with rng values
        double max_uint = Math.pow(2, 32);
        for (int i = 0; i < length; i++) {
            seed_array[i] = (float) (rng2() / max_uint);
        }
        // Fill noise array with zeros
        for (int i = 0; i < length; i++) {
            noise_array[i] = 0.0f;
        }

        for (int x = 0; x < length; x++) {
            float noise = 0.0f;
            float scale = 1.0f;
            float maxScale = 0.0f;
            for (int o = 0; o < octaves; o++) {
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

    static void noise2D(float[] seed_array, int width, float[] noise_array, int octaves) {
        // Fill seed array with rng values
        double max_uint = Math.pow(2, 32);
        for (int i = 0; i < width * width; i++) {
            seed_array[i] = rng2() % 1000 / 999.0f;
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
                    scale /= 2.0f;
                }
                noise_array[y * width + x] = noise / maxScale;
            }
        }
    }

    static int main() {
        int WIDTH = 512;

        float[] seed_array = new float[WIDTH * WIDTH];
        float[] noise_array = new float[WIDTH * WIDTH];

        noise2D(seed_array, WIDTH, noise_array, 10);

        try {
            Files.write(new File("out.json").toPath(), List.of(Arrays.toString(seed_array)), StandardCharsets.UTF_8);
        } catch (Exception e) {
            e.printStackTrace();
        }

        System.out.println(noise_array[0]);

        return 0;
    }
}
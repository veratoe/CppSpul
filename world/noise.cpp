#include "noise.h"
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <time.h>


unsigned int PRNG() {

    static unsigned int seed = 5323;
    seed = (8253729 * seed + 2396403);
    return (seed % 32767);
}

float smoothStep(float a, float b, float c) {
    return a + (b - a) * c * c * (3.0 - 2.0 * c);
}

std::vector<std::vector<float> > Noise::generate(int a, int b) {


    float max = 0.0f;
    float min = 1.0f;

    srand(time(0));

    std::vector<std::vector<float> > array(a, std::vector<float>(b, 0));
    //std::vector<std::vector<int> > array;
    std::vector< int > values(a * b);

    for (int q = 0; q < (a * b); q++) {
        values[q] = rand();
    }

    for (int j = 0; j < b; j++) {
        for (int i = 0; i < a; i++) {
        //std::vector<int> row;
            //row.push_back(values[ (i + j) % 256 ]);

            array[i][j] = 0.0f;

            int z = 64;
            float amp = 2;
            float sum_amp = 0;

            for (int o = 0; o < 7; o++) {

                array[i][j] += amp * smoothStep( 
                    smoothStep(values[i / z + (j / z * a)],  values[(i / z) + 1 + (j / z * a)], (float) (i % z) / z),
                    smoothStep(values[i / z + ((j / z + 1) * a)],  values[(i / z) + 1 + ((j / z + 1)* a)], (float) (i % z) / z),
                    (float) (j % z) / z) / RAND_MAX;
                

                sum_amp += amp;
                
                amp /= 2;
                z /= 2;
            }

            array[i][j] /= sum_amp;

            if (array[i][j] < min) min = array[i][j];
            if (array[i][j] > max) max = array[i][j];
        }

    }
    
    printf("%f, %f", min, max);
    printf("\n");

    for (int j = 0; j < b; j++) {
        for (int i = 0; i < a; i++) {
            array[i][j] -= min;
            array[i][j] /= (max - min);

            array[i][j] = (array[i][j] < 0.25 || array[i][j] > 0.75) ? 1.0 : 0.0;
        }
    }

    return array;

}







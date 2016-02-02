#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include "detect.h"

double *baseline;

// execute some tests
// * detect of template results in 1
// * detect of baseline results in 0
// * detect of near baseline results in 0
// * detect of near template results in 1
// * detect of near template results shifted results in 1
// * detect of random data results in 0
void verify_algorithm_integrity(double *baseline, double *template, int bins);


void test(int bins) {
    double *template = malloc(bins * sizeof(*template));
    verify_algorithm_integrity(baseline, template, bins);
    free(template);
}

// just create some random data
void init_random_data(double *data, int bins) {
    for (int i=0; i<bins; i++) {
        data[i] = rand();
    }
}

// create some data that is very similar to what is in source
void init_similar_data(double *data, double *source, int bins) {
    for (int i=0; i<bins; i++) {
        data[i] = source[i] + rand()/(RAND_MAX + 1.0);
    }
}

// shift data
void shift_data(double *data, int bins, int shift) {
    double *temp = malloc(bins * sizeof(*temp));
    for (int i=0; i<bins+shift; i++) {
        temp[i+shift] = data[i];
    }
    for (int i=shift; i<bins; i++) {
        data[i] = temp[i];
    }
}

void verify_algorithm_integrity(double *baseline, double *template, int bins) {
    init_random_data(template, bins);
    // ensure that data that perfectly matches the baseline does not cause a match
    if (match(baseline, template, bins, 0.9) == 0) {
        printf("Test 1 PASS\n");
    } else {
        printf("Test 1 FAIL\n");
    }
    
    // ensure that data that perfectly matches the template causes a match
    if (match(template, template, bins, 0.9) == 1) {
        printf("Test 2 PASS\n");
    } else {
        printf("Test 2 FAIL\n");
    }

    double *data = malloc(bins * sizeof(*data));
    init_similar_data(data, baseline, bins);
    // ensure that data very similar to the baseline does not cause a match
    if (match(data, template, bins, 0.9) == 0) {
        printf("Test 3 PASS\n");
    } else {
        printf("Test 3 FAIL\n");
    }

    init_similar_data(data, template, bins);
    // ensure that data that is very similar to the template causes a match
    if (match(data, template, bins, 0.9) == 1) {
        printf("Test 4 PASS\n");
    } else {
        printf("Test 4 FAIL\n");
    }

    init_similar_data(data, template, bins);
    shift_data(data, bins, 10);
    // ensure that data that is very similar to the template causes a match even when shifted
    if (match(data, template, bins, 0.9) == 1) {
        printf("Test 5 PASS\n");
    } else {
        printf("Test 5 FAIL\n");
    }

    init_random_data(data, bins);
    // ensure that random data does not cause a match.  In some unlikely circumstances
    // caused by RNG, this might fail.
    if (match(data, template, bins, 0.9) == 0) {
        printf("Test 6 PASS\n");
    } else {
        printf("Test 6 FAIL\n");
    }
    
    free(data);
}


int main(int argc, char **argv) {
    srand(time(NULL));
    int bins = 100;

    // setup the baseline data
    baseline = malloc(bins * sizeof(*baseline));
    init_random_data(baseline, bins);

    // run some tests to make sure things work correctly for sane data
    test(bins);
    return EXIT_SUCCESS;
}

#include "seatest.h"
#include "filter_function.h"
#include "pnm.h" // Ensure this contains the full definition of PNM
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Prototypes for test functions
void test_negative();
void test_monochrome();
void test_weird_named_function();
void test_black_and_white();
void test_suite();

PNM* test_image(FileType type, unsigned int width, unsigned int height, unsigned int max_value) {
    PNM* image = malloc(sizeof(PNM));
    if (!image) {
        fprintf(stderr, "Failed to allocate memory for PNM struct\n");
        exit(EXIT_FAILURE);
    }

    image->type = type;
    image->width = width;
    image->height = height;
    image->max_value = max_value;

    // Allocate memory for pixel values
    size_t pixel_count = width * height * 3; // Assuming 3 channels (RGB) for PPM
    image->pixel_values = malloc(pixel_count * sizeof(unsigned int));
    if (!image->pixel_values) {
        fprintf(stderr, "Failed to allocate memory for pixel values\n");
        free(image);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < pixel_count; i++) {
        image->pixel_values[i] = i % (max_value + 1);
    }

    return image;
}

void test_negative() {
    printf("Running test_negative...\n");
    PNM* image = test_image(PPM, 2, 2, 255);
    int result = negative(&image);
    fprintf(stderr, "image->pixel_values[0]: %d\n", image->pixel_values[0]);
    assert_int_equal(0, result);

    printf("Freeing image in test_negative...\n");
    free(image->pixel_values);
    free(image);
    printf("test_negative completed successfully.\n");
}

void test_monochrome() {
    PNM* image = test_image(PPM, 2, 2, 255);
    int result = monochrome(&image, "r");
    assert_int_equal(0, result);

    result = monochrome(&image, NULL);
    assert_int_equal(-3, result);

    free(image->pixel_values);
    free(image);
}

void test_weird_named_function() {
    PNM* image = test_image(PPM, 2, 2, 255);
    int result = weird_named_function(&image, "1");
    assert_int_equal(0, result);

    result = weird_named_function(&image, "invalid");
    assert_int_equal(-4, result);

    free(image->pixel_values);
    free(image);
}

void test_black_and_white() {
    PNM* image = test_image(PPM, 2, 2, 255);
    int result = black_and_white(&image, "128");
    assert_int_equal(0, result);

    result = black_and_white(&image, NULL);
    assert_int_equal(-3, result);

    free(image->pixel_values);
    free(image);
}

void test_suite() {
    test_fixture_start();
    run_test(test_negative);
    run_test(test_monochrome);
    run_test(test_weird_named_function);
    run_test(test_black_and_white);
    test_fixture_end();
}

int main() {
    return run_tests(test_suite);
}
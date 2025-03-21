#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>

#include "pnm.h" 


struct PNM_t {
    FileType type;
    char magic_c;
    unsigned int magic_n;
    unsigned int height;
    unsigned int width;
    unsigned int max_value;
    unsigned int *pixel_values;
 };


int load_pnm(PNM **image,char* filename );

int read_header(FILE *file, PNM *image);

int upside_down(PNM **image){
    unsigned image_size = (*image)->width * (*image)->height;

    for (size_t i = 0; i < image_size; ++i) {
       size_t j = image_size - 1 - i;
       if ((*image)->type == "ppm") {
          for (int a = 0; a < 3; ++a) {
             unsigned temp = (*image)->pixel_values[3 * i + a];
             (*image)->pixel_values[3 * i + a] = (*image)->pixel_values[3 * j + a];
             (*image)->pixel_values[3 * j + a] = temp;
          }
       } else {
          unsigned temp = (*image)->pixel_values[i];
          (*image)->pixel_values[i] = (*image)->pixel_values[j];
          (*image)->pixel_values[j] = temp;
       }
    }
    return 0;
 }




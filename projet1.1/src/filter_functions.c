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

   char *file_type_string(FileType type) {
   switch (type) {
       case PPM: 
         return "ppm";
       case PGM: 
         return "pgm";
       case PBM: 
         return "pbm";
       default: 
         return "type_error";
   }
}


int load_pnm(PNM **image,char* filename );

int read_header(FILE *file, PNM *image);

int upside_down(PNM **image){

   if ((*image)->pixel_values == NULL) return -1;

    unsigned image_size = (*image)->width * (*image)->height;

    for (unsigned long i = 0; i < image_size/2; ++i) {
      unsigned long j = image_size - 1 - i;
      if (strcmp("ppm", file_type_string((*image)->type)) == 0) {
          for (int a = 0; a < 3; ++a) {
             unsigned temp = (*image)->pixel_values[3 * i + a];
             (*image)->pixel_values[3 * i + a] = (*image)->pixel_values[3 * j + a];
             (*image)->pixel_values[3 * j + a] = temp;
          }
      } 
       else {
          unsigned temp = (*image)->pixel_values[i];
          (*image)->pixel_values[i] = (*image)->pixel_values[j];
          (*image)->pixel_values[j] = temp;
       }
    }
    return 0;
 }

 int monochrome(PNM **image, const char *parametre) {
   if (image == NULL) return -1;
   if (parametre == NULL) return -3; // invalid parameter name or no parameter given
   if (strcmp("ppm", file_type_string((*image)->type)) == 1) return -2 ;//wrong format

   unsigned int p;

   if (!strcmp(parametre, "r") || !strcmp(parametre, "R")) {
      p = 0;
   } else if (!strcmp(parametre, "v") || !strcmp(parametre, "V")) {
      p = 1;
   } else if (!strcmp(parametre, "b") || !strcmp(parametre, "B")) {
      p = 2;
   } else {
      return -3; // invalid parameter name or no parameter given
   }


   size_t image_size = (*image)->width * (*image)->height;

   for (size_t i = 0; i < image_size; ++i) {
      unsigned temp = (*image)->pixel_values[3 * i + p];
      for (int a = 0; a < 3; ++a) (*image)->pixel_values[3 * i + a] = 0;
      (*image)->pixel_values[3 * i + p] = temp;
   }
   return 0;
}

int negative(PNM **image) {
   if (image == NULL) return -1;
   if (strcmp((*image)->type, file_type_string((*image)->type)) == 0) return -2 ;//wrong format

   size_t image_size = get_width(image) * get_height(image) * 3;

   for (size_t i = 0; i < image_size; ++i) {
      (*image)->pixel_values[i] = (*image)->max_value - (*image)->pixel_values[i];
   }
   return 0;
}

int weird_named_function(PNM **image, const char *parametre) {
   if (image == NULL) return -1;
   if (parametre == NULL) return -3;// invalid parameter name or no parameter given
   if (strcmp((*image)->type,file_type_string((*image)->type)) == 0) return -2 ;//wrong format

   int grey_filter_mode;

   if (sscanf(parametre, "%d", &grey_filter_mode) != 1) return -3; // invalid parameter name or no parameter given
   if (grey_filter_mode != 1 && grey_filter_mode != 2) return -3; // //

   size_t image_size = (*image)->width * (*image)->height;

   unsigned *transformed_pixel_value = malloc(image_size * sizeof(unsigned));

   if (transformed_pixel_value == NULL) return -4; // memory allocation failed

   for (size_t i = 0; i < image_size; ++i) {
      int r = (*image)->pixel_values[3 * i];
      int g = (*image)->pixel_values[3 * i + 1];
      int b = (*image)->pixel_values[3 * i + 2];
      switch (grey_filter_mode) {
         case 1:
         transformed_pixel_value[i] = round((r + g + b) / 3);
            break;
         case 2:
         transformed_pixel_value[i] = round(0.299 * r + 0.587 * g + 0.114 * b);
            break;
      }
      transformed_pixel_value[i] *= 255 / (*image)->max_value; // 255 is the max pixel value for PGM format 
   }


   (*image)->type = "pgm";
   (*image)->max_value = 255;
   (*image)->pixel_values = transformed_pixel_value;

   free((*image)->pixel_values);

   return 0;
}

int black_and_white(PNM **image, char *parametre) {
   if (image == NULL) return -1;
   if (parametre == NULL) return -3; //invalid or no parameter given
   if (file_type_string((*image)->type) =="pbm" ) return -2;// wrong format ginven

   int limit;
   if (sscanf(parametre, "%d", &limit) != 1) return -3;//invalid or no parameter given
   
   if (limit < 0 || 255 < limit) return -3; //invalid or no parameter given

   if (file_type_string((*image)->type) == "ppm") {
      if (fifty_shades_of_grey(image, "2") != 0) return -4; //grey filter error
   }


   unsigned long data_size = (*image)->width * (*image)->height;

   for (unsigned long i = 0; i < data_size; ++i) {
      if ((*image)->pixel_values[i] >= limit) (*image)->pixel_values[i] = 1;
      else (*image)->pixel_values[i] = 0;
   }

   (*image)->type = PBM;
   (*image)->max_value = 1;

   return 0;

}
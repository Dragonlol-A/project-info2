#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>
#include <math.h>

#include "pnm.h" 




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

   if (image == NULL || *image == NULL || (*image)->pixel_values == NULL) return -1;

   unsigned image_size = (*image)->width * (*image)->height;
   fprintf(stderr, "image_size: %d\n", image_size);

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

 int monochrome(PNM **image,  char *parametre) {
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


   unsigned long image_size = (*image)->width * (*image)->height;

   for (unsigned long i = 0; i < image_size; ++i) {
      unsigned temp = (*image)->pixel_values[3 * i + p];
      for (int a = 0; a < 3; ++a) (*image)->pixel_values[3 * i + a] = 0;
      (*image)->pixel_values[3 * i + p] = temp;
   }
   return 0;
}

int negative(PNM **image) {
   if (image == NULL) return -1;
   if (strcmp("ppm", file_type_string((*image)->type)) == 1) return -2 ;//wrong format

   unsigned long image_size = (*image)->width * (*image)->height * 3;

   for (unsigned long i = 0; i < image_size; ++i) {
      (*image)->pixel_values[i] = (*image)->max_value - (*image)->pixel_values[i];
   }
   return 0;
}

int weird_named_function(PNM **image, char *parametre) {
   if (image == NULL) return -1;
   fprintf(stderr,"%s\n ", file_type_string((*image)->type));
   if (strcmp("ppm",file_type_string((*image)->type)) != 0) return -2 ;//wrong format

   unsigned long image_size = (*image)->width * (*image)->height;

   unsigned *transformed_pixel_value = malloc(image_size * sizeof(unsigned)*3);

   if (transformed_pixel_value == NULL) return -4; // memory allocation failed

   for (unsigned long i = 0; i < image_size; ++i) {
      int r = (*image)->pixel_values[3 * i];
      int g = (*image)->pixel_values[3 * i + 1];
      int b = (*image)->pixel_values[3 * i + 2];

 
      switch (*parametre) {
         case '1':
         transformed_pixel_value[i] = (unsigned)round((r + g + b) / 3);
         break;
         case '2':
         fprintf(stderr, "parametre: %s\n", parametre);
         transformed_pixel_value[i] = (unsigned)round(0.299 * r + 0.587 * g + 0.114 * b);
            break;
         default:
         fprintf(stderr, "Invalid parameter given for \n");

   }

   }
   (*image)->pixel_values = transformed_pixel_value;
   (*image)->type = PGM;
   (*image)->magic_n = 2;
   (*image)->max_value = 255;


   return 0;
}

int black_and_white(PNM **image, char *parametre) {
   unsigned exit = 0;

   if (image == NULL) return -1;
   if (parametre == NULL) return -3; //invalid or no parameter given
   if (file_type_string((*image)->type) =="pbm" ) return -2;// wrong format ginven

   int limit;
   if (sscanf(parametre, "%d", &limit) != 1) return -3;//invalid or no parameter given
   
   if (limit < 0 || 255 < limit) return -3; //invalid or no parameter given

   if (strcmp("ppm",file_type_string((*image)->type)) == 0) {
      fprintf(stderr, "parametre: %s\n", parametre);
      *parametre = '1';
      char parametre_local[] = "1";
      fprintf(stderr, "parametre: %s\n", parametre_local);
      if (weird_named_function(image, parametre_local) != 0) return -4;

      return 0;
   }


   unsigned long image_size = (*image)->width * (*image)->height;
   for (unsigned long i = 0; i < image_size*3; ++i) {
      if ((*image)->pixel_values[i] >= limit) (*image)->pixel_values[i] = 1;
      else (*image)->pixel_values[i] = 0;
   }

   (*image)->type = PBM;
   (*image)->magic_n = 1;
   (*image)->max_value = 1;
   return 0;

}
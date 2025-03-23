/**
 * pnm.c
 * 
 * Ce fichier contient les définitions de types et 
 * les fonctions de manipulation d'images PNM.
 * 
 * @author: Gendebien Alexandre s2404939
 * @date: 24/02/25
 * @projet: INFO0030 Projet 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "pnm.h"


/**
 * Définition du type opaque PNM
 *
 */
struct PNM_t {
   FileType type;
   char magic_c;
   unsigned int magic_n;
   unsigned int height;
   unsigned int width;
   unsigned int max_value;
   unsigned int *pixel_values;
};


// Proto


static void skip_comm(FILE *file);
static int read_data(FILE *file, PNM *image);
int read_header(FILE *file, PNM *image);


// Code


void freePNM(PNM *image) {
   if (!image) return;
   if (image->pixel_values)
      free(image->pixel_values);
   free(image);
}


void skip_comm(FILE *file) {
   char c;

   while ((c = fgetc(file)) != EOF) {
      if (isspace(c)) continue;
      if (c == '#') {
         while ((c = fgetc(file)) != '\n') {
         }
      } else {
         ungetc(c, file);
         return;
      }
   }
}

int read_header(FILE *file, PNM *image) {

   if (fscanf(file, "%c%u", &image->magic_c, &image->magic_n) != 2) return -2;
   if (image->magic_c != 'P') return -2;
   FileType format;
   switch (image->magic_n) {
      case 1:
         format = PBM;
         break;
      case 2:
         format = PGM;
         break;
      case 3:
         format = PPM;
         break;
      default:
         return -3;
   }
   image->type = format;

   skip_comm(file);

   if (fscanf(file, "%u %u", &(image->width), &(image->height)) != 2) return -2;

   skip_comm(file);

   if (format == PBM) {
      image->max_value = 1;
   } else {
      unsigned int max_value;
      if (fscanf(file, "%u", &max_value) != 1) return -2;

      switch (format) {
         case PGM:
            if (max_value > 255) return -2;
            break;
         case PPM:
            if (max_value > 65535) return -2;
            break;
         default: 
            return -2;
      }
      image->max_value = max_value;
   }
   return 0;
}

static int read_data(FILE *file, PNM *image) {
   FileType type = image->type;
   unsigned int height = image->height;
   unsigned int width = image->width;
   unsigned int max_value = image->max_value;
   unsigned int *p_v = image->pixel_values;

   unsigned int value_number = height * width;

   for (unsigned i = 0; i < value_number; i++) {
      skip_comm(file);
      if (type == PPM) {
         unsigned int v_1, v_2, v_3;
         if (fscanf(file, "%u %u %u", &v_1, &v_2, &v_3) != 3) return -1;
         if (v_1 > max_value || v_2 > max_value || v_3 > max_value) return -2;
         unsigned int j = i * 3;
         p_v[j] = v_1;
         p_v[j + 1] = v_2;
         p_v[j + 2] = v_3;
      } else {
         unsigned int v;
         if (fscanf(file, "%u", &v) != 1) return -2;
         if (v > max_value) return -2;
         p_v[i] = v;
      }
   }
   return 0;
}

int load_pnm(PNM **image, char *filename) {
   FILE *file = fopen(filename, "r");
   if (file == NULL){
      return -2;
   }

   *image = malloc(sizeof(PNM));
   if (*image == NULL) {
      fclose(file);
      return -1;
   }

   if (read_header(file, *image) != 0) {
      freePNM(*image);
      *image = NULL;
      fclose(file);
      return -2;
   }

   unsigned long size = (*image)->width * (*image)->height * sizeof(unsigned);
   if ((*image)->type == PPM) {
      size *= 3;
   }
   unsigned int *pixel_values = malloc(size);
   if (pixel_values == NULL) {
      freePNM(*image);
      *image = NULL;
      fclose(file);
      return -1;
   }
   (*image)->pixel_values = pixel_values;

   if (read_data(file, *image) != 0) {
      freePNM(*image);
      *image = NULL;
      fclose(file);
      return -2;
   }
   fclose(file);

   return 0;
}


int write_pnm(PNM *image, char* filename){
   FILE *file = fopen(filename, "w");
   unsigned int i = 0;
   
  
   fprintf(file, "%c%d\n", image->magic_c, image->magic_n);
   fprintf(file,"%u %u\n", image->width, image->height);
   if (image->type!=PBM)
      fprintf(file, "%u\n", image->max_value);
   
   while (i<(image->height*image->width*3))
   {  
      fprintf(file,"%u ", image->pixel_values[i]);
      ++i;    
   }
   

   return 0;
}

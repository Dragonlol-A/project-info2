/**
 * main.c
 * 
 * Ce fichier contient la fonction main() du programme de manipulation
 * de fichiers pnm.
 *
 * @author: Gendebien Alexandre s2404939
 * @date: 24/02/25
 * @projet: INFO0030 Projet 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>

#include "pnm.h"


int main(int argc, char *argv[]) {
   char *optstring = "i:o:f:";
   int option;
   char *input_filename = NULL;
   char *output_filename = NULL;
   char *format = NULL;
   PNM *image = NULL;

   while ((option = getopt(argc, argv, optstring)) != -1) {
      switch (option) { 
         case 'f':
            format = optarg;
            break;
         case 'i':
            input_filename = optarg;
            break;
         case 'o':
            output_filename = optarg;
            break;
         default:
            fprintf(stderr, "Given argument error: %s -i input_file -o output_file\n", argv[0]);
            exit(EXIT_FAILURE);
      }
   }

   if (input_filename == NULL || output_filename == NULL || format == NULL) {
      fprintf(stderr, "Files format, input and output files must be specified.\n");
      exit(EXIT_FAILURE);
   }

   if(format[0] != 'p' || (format[1] != 'g' && format[1] !='b' && format[1] != 'p') || format[2] != 'm' ){
      fprintf(stderr, "Selected format unknown try: pbm or pgm or ppm\n");
      exit(EXIT_FAILURE);
   }

   int in_filename_length = strlen(input_filename);
   int out_filename_length = strlen(output_filename);
   int IFL = in_filename_length; 
   int OFL = out_filename_length;
   

   if(input_filename[IFL-3] != output_filename[OFL-3] || input_filename[IFL-2] != output_filename[OFL-2] || input_filename[IFL] != output_filename[OFL]) {
      printf("error: input and output files extentions are not matching\n");
      exit(EXIT_FAILURE);
   }

   if(format[1] != input_filename[IFL-2]){
      fprintf(stderr,"error: format not matching with files format\n");
      exit(EXIT_FAILURE);
   }

   if (load_pnm(&image, input_filename) != 0) {
      fprintf(stderr, "Failed to load PNM image from %s\n", input_filename);
      freePNM(image);
      exit(EXIT_FAILURE);
   }

   if (write_pnm(image, output_filename) != 0) {
      fprintf(stderr, "Failed to write PNM image to %s\n", output_filename);
      freePNM(image);
      exit(EXIT_FAILURE);
   }
   
   return 0;
}
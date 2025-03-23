#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>

#include "filter_function.h"
#include "pnm.h"



int main(int argc, char *argv[]){
    char *optstring = "i:f:p:o:";
    char *filtre;
    char *input_filename;
    char *output_filename;
    char *parametre;
    int option;
    PNM *image = NULL;
    
    
    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) { 
            case 'f':
            filtre = optarg;
            break;
            case 'p':
            parametre = optarg;
            break;
        case 'i':
        input_filename = optarg;
        break;
        case 'o':
        output_filename = optarg;
        break;
        default:
        fprintf(stderr, "Given argument error: -i input_file -f filter -o output_file\n");
        freePNM(image);
        exit(EXIT_FAILURE);
    }
}

if (input_filename == NULL || filtre == NULL || output_filename == NULL) {
    fprintf(stderr, " Input file name, filter and output file name must be specified\n");
    freePNM(image);
    exit(EXIT_FAILURE);
}


if (load_pnm(&image, input_filename) == -1) {
    fprintf(stderr, "Failed to open input file  \n");
    freePNM(image);
    exit(EXIT_FAILURE);
}


if (!strcmp(filtre, "retournement" ) || !strcmp(filtre, "RETOURNEMENT")) {
    upside_down(&image);
} 
else if (!strcmp(filtre, "monochrome") || !strcmp(filtre, "MONOCHROME")) {
    if (parametre == NULL || parametre !='r' || parametre !='v' || parametre !='b' || parametre !='R' || parametre !='V' || parametre !='B') {
        fprintf(stderr, "unkown parameter given \n");
            freePNM(image);
            exit(EXIT_FAILURE);
        }
        monochrome(&image, parametre);
    } 
     else if (!strcmp(filtre, "negatif") || !strcmp(filtre, "NEGATIF")) {
        negative(&image);
    }
    else if (!strcmp(filtre, "gris") || !strcmp(filtre, "GRIS")) {
        weird_named_function(&image);
    } 
    else if (!strcmp(filtre, "nb") || !strcmp(filtre, "NB")) {
        fprintf(stderr, "black_and_white filter is not implemented yet\n");
        black_and_white(image, parametre);
    } 
    else {
        fprintf(stderr, "invalid filter name \n");
        freePNM(image);
        exit(EXIT_FAILURE);
     }


     

//after executing filter function
write_pnm(image, output_filename );

}




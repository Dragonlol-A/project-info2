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
    char *optstring = "hi:f:p:o:";
    char *filtre;
    char *input_filename;
    char *output_filename;
    char *parametre;
    char *help;
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
            case 'h':
            help = "t";
            fprintf(stderr, "the program must be used like:\n\ 
            -i input_file name -f filter_name -p parameter(*) -i output name file\n\  
            (*)filter is only used with the 'black and white' and 'monochrome' filters.\n To show the list of filters type: '-f ?'\n");
            exit(EXIT_SUCCESS);
            break;
            default:
                fprintf(stderr, "Given argument error: -i input_file -f filter -o output_file\n problem while using the program? Try using: -h  for help\n");
                freePNM(image);
                exit(EXIT_FAILURE);
    }
}



if ((input_filename == NULL || filtre == NULL || output_filename == NULL) && strcmp(help, "t")) {
    fprintf(stderr, " Input file name, filter and output file name must be specified\n need help? use : -h\n ");
    freePNM(image);
    exit(EXIT_FAILURE);
}

int in_filename_length = strlen(input_filename);
   int out_filename_length = strlen(output_filename);
   int IFL = in_filename_length; 
   int OFL = out_filename_length;
   
if(input_filename[IFL-3] != 'p' || (input_filename[IFL-2] != 'g' && input_filename[IFL-2] !='b' && input_filename[IFL-2] != 'p') || input_filename[IFL-1] != 'm' ){
fprintf(stderr, "Selected format unknown try: pbm or pgm or ppm\n");
exit(EXIT_FAILURE);
}


if (load_pnm(&image, input_filename) != 0) {
    fprintf(stderr, "Failed to open input file  \n");
    freePNM(image);
    exit(EXIT_FAILURE);
}

if (!strcmp(filtre, "?" )){
    fprintf(stderr,"the list is represented as 'filter name : how to write it as an argument'\n\
        upside-down : retournement\n\
        monochrome  : monochrome\n\
        negative    : negatif\n\
        grey shades : gris\n\
        black&white : nb\n\
        note: every filters can be written in capitals letters\n");
}

if (!strcmp(filtre, "retournement" ) || !strcmp(filtre, "RETOURNEMENT")) {
    if(input_filename[IFL-3] != output_filename[OFL-3] || input_filename[IFL-2] != output_filename[OFL-2] || input_filename[IFL] != output_filename[OFL]) {
        printf("error: input and output files extentions are not matching\n");
        exit(EXIT_FAILURE);
    } else upside_down(&image);
} 

else if (!strcmp(filtre, "monochrome") || !strcmp(filtre, "MONOCHROME")) {
    if (parametre == NULL || parametre !='r' || parametre !='v' || parametre !='b' || parametre !='R' || parametre !='V' || parametre !='B') {
        fprintf(stderr, "unkown parameter given \n");
            freePNM(image);
            exit(EXIT_FAILURE);
        } 
    if(input_filename[IFL-3] != output_filename[OFL-3] || input_filename[IFL-2] != output_filename[OFL-2] || input_filename[IFL] != output_filename[OFL]) {
        printf("error: input and output files extentions are not matching\n");
        exit(EXIT_FAILURE);
        } else monochrome(&image, parametre);
    } 

else if (!strcmp(filtre, "negatif") || !strcmp(filtre, "NEGATIF")) {
    if(input_filename[IFL-3] != output_filename[OFL-3] || input_filename[IFL-2] != output_filename[OFL-2] || input_filename[IFL] != output_filename[OFL]) {
        printf("error: input and output files extentions are not matching\n");
        exit(EXIT_FAILURE);
    } else negative(&image);

}

else if (!strcmp(filtre, "gris") || !strcmp(filtre, "GRIS")) {
    if(output_filename[OFL-3] != 'p' || (output_filename[OFL-2] != 'g') || ( output_filename[OFL-1] != 'm')){
        fprintf(stderr,"the output format must be pgm for this function\n");
    }else if (weird_named_function(&image, parametre)){
        fprintf(stderr, "error while executing the function 'gris'\n");
        exit(EXIT_FAILURE);
    }
} 

else if (!strcmp(filtre, "nb") || !strcmp(filtre, "NB")) {
    if (black_and_white(&image, parametre)!=0) {
        fprintf(stderr, "error while executing the function 'nb'\n");
        exit(EXIT_FAILURE);
    }
    
} 

else {
    fprintf(stderr, "invalid filter name \n");
    freePNM(image);
    exit(EXIT_FAILURE);
    }


     

//after executing filter function
write_pnm(image, output_filename );
exit(EXIT_SUCCESS);
}

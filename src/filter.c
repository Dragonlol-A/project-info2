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
    char *optstring = "i:f:o:";
    char filtre;
    char input_filename;
    char output_filename;
    char parametre;
    int option;

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
            fprintf(stderr, "Given argument error: %s -i input_file -o output_file\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
}


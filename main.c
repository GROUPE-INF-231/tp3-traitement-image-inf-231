#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm_io.h"
#include "operations.h"
#define MAX_ARGS 10
#define MAX_INPUT 100

void display_help() {
    printf("Liste des commandes disponibles :\n");
    printf("dom <R ou G ou B> <val> <fichier.ppm>\n");
    printf("\tFoncer ou eclaircir une couleur dominante\n\n");
    printf("gris <fichier.ppm>\n");
    printf("\tConvertir l image en niveaux de gris\n\n");
    printf("size <fichier.ppm>\n");
    printf("\tAfficher la taille de l image\n\n");
    printf("cut <fichier.ppm> l1 l2 c1 c2 <fichier_resultat.ppm>\n");
    printf("\tDecouper une partie de l image\n\n");
    printf("neg <fichier.ppm> <fichier_resultat.ppm>\n");
    printf("\tCreer le negatif de l image\n\n");
    printf("fil <fichier.ppm> <fichier_resultat.ppm>\n");
    printf("\tAppliquer le filtre median\n\n");
    printf("help\n");
    printf("\tAfficher cette liste de commandes\n\n");
    printf("quit\n");
    printf("\tQuitter le programme\n\n");
}

int main() {
    char input[MAX_INPUT];
    char* arg[MAX_ARGS];
    int argc;

    printf("\n***********************************************************\n");
    printf("Application de traitement d images PPM\n");
    printf("***********************************************************\n");

    while (1) {
        printf("ppmviewer> ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        argc = 0;
        char* token = strtok(input, " ");
        while (token != NULL && argc < MAX_ARGS) {
            arg[argc++] = token;
            token = strtok(NULL, " ");
        }
        if (argc == 0) continue;

        if (strcmp(arg[0], "quit") == 0) {
            return 0;
        }
        else if (strcmp(arg[0], "help") == 0) {
            display_help();
        }
        else if (strcmp(arg[0], "dom") == 0) {
            if (argc != 4) {
                printf("Usage: dom <R/G/B> <val> <fichier.ppm>\n");
            }
            else {
                char dominant = arg[1][0];
                int ajustement = atoi(arg[2]);
                char* filename = arg[3];

                printf("Reading image: %s\n", filename);
                PPMImage* img = read_ppm(filename);
                if (img == NULL) {
                    printf("fichier non trouve\n");
                }
                else {
                    printf("Image loaded: %dx%d\n", img->width, img->height);
                    PPMImage* result = dom_operation(img, dominant, ajustement);
                    if (result) {
                        // Create output filename
                        char output_filename[256];
                        strcpy(output_filename, filename);
                        char* dot = strrchr(output_filename, '.');
                        if (dot != NULL) {
                            *dot = '\0';
                        }
                        strcat(output_filename, "_dom.ppm");

                        printf("Writing result to: %s\n", output_filename);
                        write_ppm(result, output_filename);
                        free_ppm_img(result);
                        printf("operation effectuee ; %s cree\n", output_filename);
                    }
                    free_ppm_img(img);
                }
            }
        }
        else if (strcmp(arg[0], "gris") == 0) {
            if (argc != 2) {
                printf("Usage: gris <fichier.ppm>\n");
            }
            else {
                char* filename = arg[1];
                PPMImage* img = read_ppm(filename);
                if (img == NULL) {
                    printf("fichier non trouve\n");
                }
                else {
                    PPMImage* result = gris_operation(img);
                    if (result) {

                        char output_filename[100];
                        strcpy(output_filename, filename);
                        char* dot = strrchr(output_filename, '.');
                        if (dot != NULL) {
                            *dot = '\0';
                        }
                        strcat(output_filename, "_gris.ppm");

                        write_ppm(result, output_filename);
                        free_ppm_img(result);
                        printf("operation effectuee ; %s cree\n", output_filename);
                    }
                    free_ppm_img(img);
                }
            }
        }
        else if (strcmp(arg[0], "size") == 0) {
            if (argc != 2) {
                printf("Usage: size <fichier.ppm>\n");
            }
            else {
                size_operation(arg[1]);
            }
        }
        else if (strcmp(arg[0], "cut") == 0) {
            if (argc != 7) {
                printf("Usage: cut <fichier.ppm> l1 l2 c1 c2 <fichier_resultat.ppm>\n");
            }
            else {
                int l1 = atoi(arg[2]);
                int l2 = atoi(arg[3]);
                int c1 = atoi(arg[4]);
                int c2 = atoi(arg[5]);
                char* output_filename = arg[6];

                PPMImage* img = read_ppm(arg[1]);
                if (img == NULL) {
                    printf("fichier non trouve\n");
                }
                else {
                    PPMImage* result = cut_operation(img, l1, l2, c1, c2);
                    if (result) {
                        write_ppm(result, output_filename);
                        free_ppm_img(result);
                        printf("operation effectuee\n");
                    }
                    else {
                        printf("coordonnees invalides\n");
                    }
                    free_ppm_img(img);
                }
            }
        }
        else if (strcmp(arg[0], "neg") == 0) {
            if (argc != 3) {
                printf("Usage: neg <fichier.ppm> <fichier_resultat.ppm>\n");
            }
            else {
                PPMImage* img = read_ppm(arg[1]);
                if (img == NULL) {
                    printf("fichier non trouve\n");
                }
                else {
                    PPMImage* result = neg_operation(img);
                    if (result) {
                        write_ppm(result, arg[2]);
                        free_ppm_img(result);
                        printf("operation effectuee\n");
                    }
                    free_ppm_img(img);
                }
            }
        }
        else if (strcmp(arg[0], "fil") == 0) {
            if (argc != 3) {
                printf("Usage: fil <fichier.ppm> <fichier_resultat.ppm>\n");
            }
            else {
                PPMImage* img = read_ppm(arg[1]);
                if (img == NULL) {
                    printf("fichier non trouve\n");
                }
                else {
                    PPMImage* result = fil_operation(img);
                    if (result) {
                        write_ppm(result, arg[2]);
                        free_ppm_img(result);
                        printf("operation effectuee\n");
                    }
                    free_ppm_img(img);
                }
            }
        }
        else {
            printf("Commande inconnue. Tapez 'help' pour afficher la liste des commandes.\n");
        }
    }

    return 0;
}
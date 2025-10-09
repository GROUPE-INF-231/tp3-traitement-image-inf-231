#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm_io.h"

PPMImage* create_ppm_image(int width, int height, int max_color) {
    if (width <= 0 || height <= 0 || max_color < 0 || max_color > 255) {
        fprintf(stderr, "Error: Invalid image parameters (width=%d, height=%d, max_color=%d)\n",
            width, height, max_color);
        return NULL;
    }

    PPMImage* img = (PPMImage*)malloc(sizeof(PPMImage));
    if (img == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for PPMImage structure\n");
        return NULL;
    }

    strcpy(img->version, "P3");
    img->width = width;
    img->height = height;
    img->max_color = max_color;

    img->pixels = (Pixel**)malloc(height * sizeof(Pixel*));
    if (img->pixels == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for pixel rows\n");
        free(img);
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        img->pixels[i] = NULL;
    }

    for (int i = 0; i < height; i++) {
        img->pixels[i] = (Pixel*)malloc(width * sizeof(Pixel));
        if (img->pixels[i] == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for pixel row %d\n", i);

            for (int j = 0; j < i; j++) {
                free(img->pixels[j]);
            }
            free(img->pixels);
            free(img);
            return NULL;
        }

        for (int j = 0; j < width; j++) {
            img->pixels[i][j].r = 0;
            img->pixels[i][j].g = 0;
            img->pixels[i][j].b = 0;
        }
    }

    return img;
}

PPMImage* create_Image() {
    int width, height;
    printf("Entrez la largeur et la hauteur de l'image :\n");
    do {
        scanf("%d %d", &width, &height);
        if (width <= 0 || height <= 0) {
            printf("Erreur ! Entrez des valeurs correctes pour la largeur et la hauteur :\n");
        }
    } while (width <= 0 || height <= 0);

    int max_color;
    printf("Entrez le nombre maximum de couleur (max : 255) :\n");
    do {
        scanf("%d", &max_color);
        if (max_color < 0 || max_color > 255) {
            printf("Erreur ! Entrez une valeur de couleur comprise entre 0 et 255 :\n");
        }
    } while (max_color < 0 || max_color > 255);

    PPMImage* img = create_ppm_image(width, height, max_color);
    if (img == NULL) {
        fprintf(stderr, "Erreur : Echec de la creation de l'image\n");
        exit(EXIT_FAILURE);
    }

    Pixel p;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("Entrez les valeurs rouge, vert, bleu pour le pixel (%d,%d) :\n", i, j);
            do {
                scanf("%d %d %d", &p.r, &p.g, &p.b);
                if (p.r < 0 || p.r > max_color || p.g < 0 || p.g > max_color || p.b < 0 || p.b > max_color) {
                    printf("Erreur ! Entrez des valeurs r(rouge) g(vert) b(bleu) comprises entre 0 - %d :\n", max_color);
                }
            } while (p.r < 0 || p.r > max_color || p.g < 0 || p.g > max_color || p.b < 0 || p.b > max_color);

            img->pixels[i][j] = p;
        }
    }

    // Retirer le character nouvelle ligne creer automatiquement
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    return img;
}

void write_ppm(PPMImage* img, char* filename) {
    if (img == NULL) {
        return;
    }

    FILE* imagefile = fopen(filename, "w");
    if (imagefile == NULL) {
        fprintf(stderr, "Erreur ! Impossible d'ouvrir le fichier %s en mode ecriture\n", filename);
        return;
    }

    char description[100];
    printf("Entrez une courte description pour l'image :\n");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = '\0';

    fprintf(imagefile, "%s\n# %s\n%d %d\n%d\n", img->version, description, img->width, img->height, img->max_color);

    int i, j;
    for (i = 0; i < img->height; i++) {
        for (j = 0; j < img->width; j++) {
            fprintf(imagefile, "%d %d %d ", img->pixels[i][j].r, img->pixels[i][j].g, img->pixels[i][j].b);
        }

        fprintf(imagefile, "\n");
    }

    fclose(imagefile);
}

PPMImage* read_ppm(char* filename) {
    FILE* imagefile = fopen(filename, "r");
    if (imagefile == NULL) {
        fprintf(stderr, "Erreur ! Echec de l'ouverture de l'image %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    PPMImage* img = malloc(sizeof(PPMImage));
    if (img == NULL) {
        printf("Echec de l'allocation memoire !\n");
        exit(EXIT_FAILURE);
    }

    char line[500];

    fgets(img->version, sizeof(img->version), imagefile);

    do {
        fgets(line, sizeof(line), imagefile);
    } while (line[0] == '#');

    sscanf(line, "%d %d", &img->width, &img->height);

    do {
        fgets(line, sizeof(line), imagefile);
    } while (line[0] == '#');

    sscanf(line, "%d", &img->max_color);

    img->pixels = malloc(img->height * sizeof(Pixel*));
    if (img->pixels == NULL) {
        free(img);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < img->height; i++) {
        img->pixels[i] = malloc(img->width * sizeof(Pixel));
        if (!img->pixels[i]) {
            for (int k = 0; k < i; k++) free(img->pixels[k]);
            free(img->pixels);
            free(img);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            fscanf(imagefile, "%d %d %d",
                &img->pixels[i][j].r,
                &img->pixels[i][j].g,
                &img->pixels[i][j].b);
        }
    }

    fclose(imagefile);
    return img;
}

void free_ppm_img(PPMImage* img) {
    if (img == NULL)
        return;

    if (img->pixels != NULL) {
        for (int i = 0; i < img->height; i++) {
            free(img->pixels[i]);
        }
        free(img->pixels);
    }

    free(img);
}

int clamp(int value, int min, int max) {
    if (value < min) {
        return min;
    }
    else if (value > max) {
        return max;
    }
    else {
        return value;
    }
}

char get_dominant_color(Pixel p) {
    if (p.r >= p.g && p.r >= p.b) {
        return 'R';
    }
    else if (p.g >= p.r && p.g >= p.b) {
        return 'G';
    }
    else {
        return 'B';
    }
}

int are_valid_coordinates(PPMImage* img, int row_1, int row_2, int column_1, int column_2) {
    if (row_1 < 0 || column_1 < 0) {
        return 0;
    }
    if (row_2 > img->height || column_2 > img->width) {
        return 0;
    }
    if (row_1 >= row_2 || column_1 >= column_2) {
        return 0;
    }
    return 1;
}

PPMImage* copy_image(PPMImage* img) {
    if (img == NULL) {
        printf("Erreur ! Image inexistante\n");
        exit(EXIT_FAILURE);
    }

    PPMImage* copy = malloc(sizeof(PPMImage));
    if (copy == NULL) {
        printf("Echec de l'allocation memoire !\n");
        exit(EXIT_FAILURE);
    }

    strcpy(copy->version, img->version);
    copy->width = img->width;
    copy->height = img->height;
    copy->max_color = img->max_color;

    copy->pixels = malloc(copy->height * sizeof(Pixel*));
    if (copy->pixels == NULL) {
        free(copy);
        printf("Memory allocation failed for pixel rows.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < copy->height; i++) {
        copy->pixels[i] = malloc(copy->width * sizeof(Pixel));
        if (copy->pixels[i] == NULL) {
            for (int k = 0; k < i; k++) free(copy->pixels[k]);
            free(copy->pixels);
            free(copy);
            printf("Memory allocation failed for pixel columns.\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < copy->height; i++) {
        for (int j = 0; j < copy->width; j++) {
            copy->pixels[i][j] = img->pixels[i][j];
        }
    }

    return copy;
}
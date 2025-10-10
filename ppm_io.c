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

void write_ppm(PPMImage* img, char* filename) {
    if (img == NULL) {
        fprintf(stderr, "Error: Cannot write NULL image\n");
        return;
    }

    FILE* imagefile = fopen(filename, "w");
    if (imagefile == NULL) {
        fprintf(stderr, "Error: Cannot open file for writing: %s\n", filename);
        return;
    }

    // Write header without asking for description
    fprintf(imagefile, "%s\n", img->version);
    fprintf(imagefile, "%d %d\n", img->width, img->height);
    fprintf(imagefile, "%d\n", img->max_color);

    // Write pixel data
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            fprintf(imagefile, "%d %d %d ",
                img->pixels[i][j].r,
                img->pixels[i][j].g,
                img->pixels[i][j].b);
        }
        fprintf(imagefile, "\n");
    }

    fclose(imagefile);
}

PPMImage* read_ppm(char* filename) {
    FILE* imagefile = fopen(filename, "r");
    if (imagefile == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return NULL;
    }

    PPMImage* img = malloc(sizeof(PPMImage));
    if (img == NULL) {
        fclose(imagefile);
        return NULL;
    }

    // Read version
    if (fscanf(imagefile, "%2s", img->version) != 1) {
        fprintf(stderr, "Error reading PPM version\n");
        free(img);
        fclose(imagefile);
        return NULL;
    }

    // Skip comments and read dimensions
    int ch;
    while ((ch = fgetc(imagefile)) == ' ' || ch == '\t' || ch == '\n' || ch == '#') {
        if (ch == '#') {
            while ((ch = fgetc(imagefile)) != '\n' && ch != EOF);
        }
    }
    ungetc(ch, imagefile);

    if (fscanf(imagefile, "%d %d", &img->width, &img->height) != 2) {
        fprintf(stderr, "Error reading image dimensions\n");
        free(img);
        fclose(imagefile);
        return NULL;
    }

    // Read max color
    while ((ch = fgetc(imagefile)) == ' ' || ch == '\t' || ch == '\n' || ch == '#') {
        if (ch == '#') {
            while ((ch = fgetc(imagefile)) != '\n' && ch != EOF);
        }
    }
    ungetc(ch, imagefile);

    if (fscanf(imagefile, "%d", &img->max_color) != 1) {
        fprintf(stderr, "Error reading max color value\n");
        free(img);
        fclose(imagefile);
        return NULL;
    }

    // Allocate pixel array
    img->pixels = malloc(img->height * sizeof(Pixel*));
    if (img->pixels == NULL) {
        fprintf(stderr, "Memory allocation failed for rows\n");
        free(img);
        fclose(imagefile);
        return NULL;
    }

    for (int i = 0; i < img->height; i++) {
        img->pixels[i] = malloc(img->width * sizeof(Pixel));
        if (img->pixels[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for row %d\n", i);
            // Free previously allocated rows
            for (int j = 0; j < i; j++) {
                free(img->pixels[j]);
            }
            free(img->pixels);
            free(img);
            fclose(imagefile);
            return NULL;
        }
    }

    // Read pixel data
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            if (fscanf(imagefile, "%d %d %d",
                &img->pixels[i][j].r,
                &img->pixels[i][j].g,
                &img->pixels[i][j].b) != 3) {
                fprintf(stderr, "Error reading pixel data at (%d,%d)\n", i, j);
                // Cleanup and return
                for (int k = 0; k < img->height; k++) {
                    free(img->pixels[k]);
                }
                free(img->pixels);
                free(img);
                fclose(imagefile);
                return NULL;
            }
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
    if (img == NULL) return 0;

    if (row_1 < 0 || row_2 <= row_1 || row_2 > img->height) {
        return 0;
    }
    if (column_1 < 0 || column_2 <= column_1 || column_2 > img->width) {
        return 0;
    }
    return 1;
}

PPMImage* copy_image(PPMImage* img) {
    if (img == NULL) {
        fprintf(stderr, "Error: Cannot copy NULL image\n");
        return NULL;
    }

    PPMImage* copy = malloc(sizeof(PPMImage));
    if (copy == NULL) {
        fprintf(stderr, "Memory allocation failed for PPMImage copy\n");
        return NULL;
    }

    // Copy basic info
    strcpy(copy->version, img->version);
    copy->width = img->width;
    copy->height = img->height;
    copy->max_color = img->max_color;

    // Allocate pixel array
    copy->pixels = malloc(copy->height * sizeof(Pixel*));
    if (copy->pixels == NULL) {
        fprintf(stderr, "Memory allocation failed for pixel rows in copy\n");
        free(copy);
        return NULL;
    }

    for (int i = 0; i < copy->height; i++) {
        copy->pixels[i] = malloc(copy->width * sizeof(Pixel));
        if (copy->pixels[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for row %d in copy\n", i);
            // Free previously allocated rows
            for (int j = 0; j < i; j++) {
                free(copy->pixels[j]);
            }
            free(copy->pixels);
            free(copy);
            return NULL;
        }

        // Copy pixel data
        for (int j = 0; j < copy->width; j++) {
            copy->pixels[i][j] = img->pixels[i][j];
        }
    }

    return copy;
}
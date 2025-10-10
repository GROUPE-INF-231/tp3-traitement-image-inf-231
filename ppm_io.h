#ifndef PPMVIEWER_H
#define PPMVIEWER_H

typedef struct {
    int r;
    int g;
    int b;
} Pixel;

typedef struct {
    char version[3];
    int width;
    int height;
    int max_color;
    Pixel** pixels;
} PPMImage;

PPMImage* create_ppm_image(int width, int height, int max_color);
void write_ppm(PPMImage* img, char* filename);
PPMImage* read_ppm(char* filename);
void free_ppm_img(PPMImage* img);
int clamp(int value, int min, int max);
int valmedian(int* tab, int n);
char get_dominant_color(Pixel p);
int are_valid_coordinates(PPMImage* img, int row_1, int row_2, int column_1, int column_2);
PPMImage* copy_image(PPMImage* img);

#endif
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
PPMImage* create_Image();
void write_ppm(PPMImage* img, char* filename);
PPMImage* read_ppm(char* filename);
void write_ppm(PPMImage* img, char* filename);


PPMImage* read_ppm(char* filename);
#endif
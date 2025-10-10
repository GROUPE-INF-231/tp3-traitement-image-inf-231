#ifndef TAILLE_IMAGE_H
#define TAILLE_IMAGE_H

typedef struct {
    int r, g, b;
} Pixel;

typedef struct {
    char version[4];
    int largeur;
    int hauteur;
    int max_val;
    Pixel *pixels;
} Image;

// Fonctions disponibles
Image* lirePPM(const char *nomFichier);
void afficherTaille(const Image *img);
void libererImage(Image *img);

#endif

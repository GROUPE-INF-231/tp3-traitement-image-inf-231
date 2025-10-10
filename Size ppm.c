#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// ==== Partie venant de taille_image.h (interface) ====

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

Image* lirePPM(const char *nomFichier);  // Prototype
void afficherTaille(const Image *img);  // Prototype
void libererImage(Image *img);          // Prototype

/// ==== Partie venant de taille_image.c (implémentation) ====

// Fonction interne pour lire un entier en ignorant les commentaires
int lireEntier(FILE *f, int *val) {
    char c;
    do {
        c = fgetc(f);
        if (c == '#') {
            while (fgetc(f) != '\n');
        }
    } while (c == ' ' || c == '\n' || c == '\t');
    ungetc(c, f);
    return fscanf(f, "%d", val) == 1;
}

// Fonction principale pour lire une image PPM
Image* lirePPM(const char *nomFichier) {
    FILE *f = fopen(nomFichier, "r");
    if (!f) {
        printf("❌ Erreur : impossible d’ouvrir le fichier %s\n", nomFichier);
        return NULL;
    }

    Image *img = (Image*)malloc(sizeof(Image));
    if (!img) {
        printf("❌ Erreur d’allocation mémoire.\n");
        fclose(f);
        return NULL;
    }

    fscanf(f, "%2s", img->version);
    if (strcmp(img->version, "P3") != 0) {
        printf("❌ Format non supporté. Seuls les fichiers P3 sont acceptés.\n");
        fclose(f);
        free(img);
        return NULL;
    }

    if (!lireEntier(f, &img->largeur) || !lireEntier(f, &img->hauteur) || !lireEntier(f, &img->max_val)) {
        printf("❌ Erreur de lecture de l’en-tête.\n");
        fclose(f);
        free(img);
        return NULL;
    }

    int total_pixels = img->largeur * img->hauteur;
    img->pixels = (Pixel*)malloc(total_pixels * sizeof(Pixel));
    if (!img->pixels) {
        printf("❌ Erreur d’allocation pour les pixels.\n");
        fclose(f);
        free(img);
        return NULL;
    }

    for (int i = 0; i < total_pixels; i++) {
        fscanf(f, "%d %d %d", &img->pixels[i].r, &img->pixels[i].g, &img->pixels[i].b);
    }

    fclose(f);
    return img;
}

void afficherTaille(const Image *img) {
    if (!img) {
        printf("Aucune image chargée.\n");
        return;
    }
    printf(" Taille de l’image : %d x %d pixels\n", img->largeur, img->hauteur);
}

void libererImage(Image *img) {
    if (img) {
        free(img->pixels);
        free(img);
    }
}

/// ==== Partie du main.c ====
int main() {
    char nomFichier[100];

    printf("=== Programme : Taille d’une image PPM ===\n");
    printf("Nom du fichier PPM : ");
    scanf("%s", nomFichier);

    Image *img = lirePPM(nomFichier);
    if (img) {
        afficherTaille(img);
        libererImage(img);
    } else {
        printf("❌ Impossible de charger l’image.\n");
    }

    return 0;
}

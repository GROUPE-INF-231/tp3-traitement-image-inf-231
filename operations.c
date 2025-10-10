#include "operations.h"
#include "ppm_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PPMImage* dom_operation(PPMImage* img, char dominant_cible, int ajustement) {
    if (img == NULL) {
        fprintf(stderr, "Erreur: Image source inexistante.\n");
        return NULL;
    }

    PPMImage* new_img = copy_image(img);
    if (new_img == NULL) {
        return NULL;
    }

    char target_dom = dominant_cible;
    if (target_dom >= 'a' && target_dom <= 'z') {
        target_dom = target_dom - 'a' + 'A';
    }

    if (target_dom != 'R' && target_dom != 'G' && target_dom != 'B') {
        fprintf(stderr, "Erreur: Dominante cible invalide. Utilisez R, G, ou B.\n");
        free_ppm_img(new_img);
        return NULL;
    }

    for (int i = 0; i < new_img->height; i++) {
        for (int j = 0; j < new_img->width; j++) {
            Pixel* p = &new_img->pixels[i][j];
            char dom = get_dominant_color(*p);

            if (dom == target_dom) {
                p->r = clamp(p->r + ajustement, 0, new_img->max_color);
                p->g = clamp(p->g + ajustement, 0, new_img->max_color);
                p->b = clamp(p->b + ajustement, 0, new_img->max_color);
            }
        }
    }
    return new_img;
}

PPMImage* gris_operation(PPMImage* img) {
    if (img == NULL) {
        fprintf(stderr, "Erreur: Image source inexistante.\n");
        return NULL;
    }

    PPMImage* new_img = copy_image(img);
    if (new_img == NULL) {
        return NULL;
    }

    for (int i = 0; i < new_img->height; i++) {
        for (int j = 0; j < new_img->width; j++) {
            Pixel* p = &new_img->pixels[i][j];
            int avg = (p->r + p->g + p->b) / 3;
            p->r = avg;
            p->g = avg;
            p->b = avg;
        }
    }
    return new_img;
}

PPMImage* neg_operation(PPMImage* img) {
    if (img == NULL) {
        fprintf(stderr, "Erreur: Image source inexistante.\n");
        return NULL;
    }

    PPMImage* new_img = copy_image(img);
    if (new_img == NULL) {
        return NULL;
    }

    int max = new_img->max_color;

    for (int i = 0; i < new_img->height; i++) {
        for (int j = 0; j < new_img->width; j++) {
            Pixel* p = &new_img->pixels[i][j];
            p->r = max - p->r;
            p->g = max - p->g;
            p->b = max - p->b;
        }
    }
    return new_img;
}

void size_operation(const char* filename) {
    PPMImage* img = read_ppm((char*)filename);
    if (img == NULL) {
        printf("Erreur : impossible d ouvrir le fichier %s\n", filename);
        return;
    }

    printf("Dimension de l image (largeur x hauteur): %d x %d\n", img->width, img->height);
    free_ppm_img(img);
}

PPMImage* cut_operation(PPMImage* img, int l1, int l2, int c1, int c2) {
    if (img == NULL) {
        fprintf(stderr, "Erreur: Image source inexistante.\n");
        return NULL;
    }

    if (!are_valid_coordinates(img, l1, l2, c1, c2)) {
        fprintf(stderr, "Erreur: Coordonnees de decoupe invalides.\n");
        fprintf(stderr, "Rappel: 0 <= l1 < l2 <= %d et 0 <= c1 < c2 <= %d\n", img->height, img->width);
        return NULL;
    }

    int new_height = l2 - l1;
    int new_width = c2 - c1;

    PPMImage* new_img = create_ppm_image(new_width, new_height, img->max_color);
    if (new_img == NULL) {
        return NULL;
    }

    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j++) {
            new_img->pixels[i][j] = img->pixels[l1 + i][c1 + j];
        }
    }

    strcpy(new_img->version, img->version);
    return new_img;
}

int valmedian(int* tab, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (tab[j] < tab[i]) {
                int temp = tab[i];
                tab[i] = tab[j];
                tab[j] = temp;
            }
        }
    }
    return tab[n / 2];
}

PPMImage* fil_operation(PPMImage* img) {
    if (img == NULL) {
        fprintf(stderr, "Erreur : Image inexistante.\n");
        return NULL;
    }

    PPMImage* filtre = copy_image(img);
    int x[] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
    int y[] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int rval[9], gval[9], bval[9];
            int count = 0;
            for (int k = 0; k < 9; k++) {
                int new_i = i + x[k];
                int new_j = j + y[k];

                if (new_i >= 0 && new_i < img->height && new_j >= 0 && new_j < img->width) {
                    rval[count] = img->pixels[new_i][new_j].r;
                    gval[count] = img->pixels[new_i][new_j].g;
                    bval[count] = img->pixels[new_i][new_j].b;
                    count++;
                }
            }
            filtre->pixels[i][j].r = valmedian(rval, count);
            filtre->pixels[i][j].g = valmedian(gval, count);
            filtre->pixels[i][j].b = valmedian(bval, count);
        }
    }
    return filtre;
}
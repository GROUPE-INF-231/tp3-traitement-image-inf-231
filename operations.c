#include "operations.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
// --- Opération 1: Foncer ou Éclaircir les Pixels Dominants ---

PPMImage* dom_operation(PPMImage* img, char dominant_cible, int ajustement) {
    if (img == NULL) {
        fprintf(stderr, "Erreur: Image source inexistante.\n");
        return NULL;
    }

    // Créer une copie pour la modification
    PPMImage* new_img = copy_image(img);
    if (new_img == NULL) {
        return NULL;
    }

    // Assurer que la dominante est R, G ou B
    char target_dom = toupper(dominant_cible);
    if (target_dom != 'R' && target_dom != 'G' && target_dom != 'B') {
        fprintf(stderr, "Erreur: Dominante cible invalide. Utilisez R, G, ou B.\n");
        free_ppm_img(new_img);
        return NULL;
    }

    for (int i = 0; i < new_img->height; i++) {
        for (int j = 0; j < new_img->width; j++) {
            Pixel *p = &new_img->pixels[i][j];
            char dom = get_dominant_color(*p);

            // Si le pixel a la couleur dominante cible, on ajuste
            if (dom == target_dom) {
                // Appliquer l'ajustement à CHAQUE composante
                p->r = clamp(p->r + ajustement, 0, new_img->max_color);
                p->g = clamp(p->g + ajustement, 0, new_img->max_color);
                p->b = clamp(p->b + ajustement, 0, new_img->max_color);
            }
        }
    }
    return new_img;
}

// --- Opération 2: Passer en Niveaux de Gris ---

PPMImage* gris_operation(PPMImage* img) {
    if (img == NULL) {
        fprintf(stderr, "Erreur: Image source inexistante.\n");
        return NULL;
    }

    // Créer une copie pour la modification
    PPMImage* new_img = copy_image(img);
    if (new_img == NULL) {
        return NULL;
    }

    for (int i = 0; i < new_img->height; i++) {
        for (int j = 0; j < new_img->width; j++) {
            Pixel *p = &new_img->pixels[i][j];
            // Calculer la valeur moyenne (niveau de gris)
            int avg = (p->r + p->g + p->b) / 3;

            // Appliquer la moyenne aux trois composantes
            p->r = avg;
            p->g = avg;
            p->b = avg;
        }
    }
    return new_img;
}

// --- Opération 3: Créer le Négatif ---

PPMImage* neg_operation(PPMImage* img) {
    if (img == NULL) {
        fprintf(stderr, "Erreur: Image source inexistante.\n");
        return NULL;
    }

    // Créer une copie pour la modification
    PPMImage* new_img = copy_image(img);
    if (new_img == NULL) {
        return NULL;
    }

    int max = new_img->max_color;

    for (int i = 0; i < new_img->height; i++) {
        for (int j = 0; j < new_img->width; j++) {
            Pixel *p = &new_img->pixels[i][j];
            // Changer chaque couleur par son complémentaire (max_color - valeur)
            p->r = max - p->r;
            p->g = max - p->g;
            p->b = max - p->b;
        }
    }
    return new_img;
}

// --- Opération 5: Découper une Sous-Image ---

PPMImage* cut_operation(PPMImage* img, int l1, int l2, int c1, int c2) {
    if (img == NULL) {
        fprintf(stderr, "Erreur: Image source inexistante.\n");
        return NULL;
    }

    // Vérification des coordonnées selon la règle du TP: l1 < l2 <= hauteur et c1 < c2 <= largeur
    if (!are_valid_coordinates(img, l1, l2, c1, c2)) {
        fprintf(stderr, "Erreur: Coordonnées de découpe invalides.\n");
        fprintf(stderr, "Rappel: 0 <= l1 < l2 <= %d et 0 <= c1 < c2 <= %d\n", img->height, img->width);
        return NULL;
    }

    int new_height = l2 - l1;
    int new_width = c2 - c1;

    // Créer la nouvelle image découpée
    PPMImage* new_img = create_ppm_image(new_width, new_height, img->max_color);
    if (new_img == NULL) {
        return NULL;
    }

    // Copier les pixels de l'image source à la nouvelle image
    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j++) {
            // new_img[i][j] = img[l1 + i][c1 + j]
            new_img->pixels[i][j] = img->pixels[l1 + i][c1 + j];
        }
    }

    // Mettre à jour la version de l'image (si nécessaire)
    strcpy(new_img->version, img->version);

    return new_img;
}

#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "ppm_io.h"

// Opération 1: Foncer ou éclaircir les pixels dominants
PPMImage* dom_operation(PPMImage* img, char dominant_cible, int ajustement);

// Opération 2: Passer en niveaux de gris
PPMImage* gris_operation(PPMImage* img);

// Opération 3: Créer le négatif
PPMImage* neg_operation(PPMImage* img);

// Opération 5: Découper une sous-image
PPMImage* cut_operation(PPMImage* img, int l1, int l2, int c1, int c2);

#endif // OPERATIONS_H

#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "ppm_io.h"

PPMImage* dom_operation(PPMImage* img, char dominant_cible, int ajustement);
PPMImage* gris_operation(PPMImage* img);
PPMImage* neg_operation(PPMImage* img);
void size_operation(const char* filename);
PPMImage* cut_operation(PPMImage* img, int l1, int l2, int c1, int c2);
PPMImage* fil_operation(PPMImage* img);

#endif
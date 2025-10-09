#include <stdio.h>
#include "ppm_io.h"

int main() {

    // juste pour le test

    printf("Hello World!\n");
    PPMImage* img = create_Image();
    write_ppm(img, "test.ppm");
    return 0;
}
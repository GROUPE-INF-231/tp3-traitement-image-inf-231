#include <stdio.h>
#include <stdlib.h>
#include "ppm_io.h"

int valmedian(int *tab, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (tab[j] < tab[i])
            {
                int temp = tab[i];
                tab[i] = tab[j];
                tab[j] = temp;
            }
        }
    }
    return tab[n / 2];
}

PPMImage *filtmedian(PPMImage *img)
{
    if (img == NULL)
    {
        fprintf(stderr, "Erreur : Image inexistante.\n");
        exit(EXIT_FAILURE);
    }

    PPMImage *filtre = copy_image(img);
    int x[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
    int y[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};

    for (int i = 0; i < img->height; i++)
    {
        for (int j = 0; j < img->width; j++)
        {
            int rval[9], gval[9], bval[9];
            int count = 0;
            for (int k = 0; k < 9; k++)
            {
                int new_i = i + x[k];
                int new_j = j + y[k];

                if (new_i >= 0 && new_i < img->height && new_j >= 0 && new_j < img->width)
                {
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
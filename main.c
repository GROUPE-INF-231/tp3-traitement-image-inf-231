#include <stdio.h>
#include <string.h>
#include "ppm_io.h"

int main() {
    char command[256];
    PPMImage* img = NULL;
    printf("Bienvenue dans PPMViewer ! Tapez 'help' pour la liste des commandes.\n");

    while (1) {
        printf("PPMViewer> ");
        scanf("%s", command);

        // === Commande d’aide ===
        if (strcmp(command, "help") == 0) {
            printf("\nCommandes disponibles :\n");
            printf(" new                   - Creer une nouvelle image PPM\n");
            printf(" load <fichier>        - Charger une image existante\n");
            printf(" save <fichier>        - Sauvegarder l'image actuelle\n");
            printf(" size                  - Afficher la taille de l'image\n");
            printf(" gris <fichier>        - Convertir en niveaux de gris\n");
            printf(" neg <fichier>         - Créer le négatif\n");
            printf(" fil <fichier>         - Appliquer un filtre médian 3x3\n");
            printf(" cut x1 y1 x2 y2 <fichier> - Découper une sous-image\n");
            printf(" dom                   - Afficher la couleur dominante\n");
            printf(" exit                  - Quitter le programme\n\n");
        }
        // Cette partie sert à créer une nouvelle image ppm
        else if (strcmp(command, "new") == 0) {
            if (img) free_ppm_img(img);
            img = create_Image();

            char nouveau[100];
            printf("Entrez le nom du fichier pour enregistrer l'image : ");
            scanf("%s", nouveau);

            write_ppm(img, nouveau);
            printf("Image creee et enregistree sous %s\n", nouveau);
        }        
        // Cette partie sert à l'application du filtre médian
        else if (strcmp(command, "fil") == 0) {
            if (!img) { printf("Aucune image chargee.\n"); continue; }
            char nouveau[100];
            scanf("%s", nouveau);
            PPMImage* filtre = filtmedian(img);
            write_ppm(filtre, nouveau);
            free_ppm_img(filtre);
            printf("Filtre médian appliqué -> %s\n", nouveau);
        }
        // Cette partie est pour la fonctionnalité servant à quitter le programme ppmviewer
        else if (strcmp(command, "exit") == 0) {
            printf("Fin du programme.\n");
            if (img) free_ppm_img(img);
            break;
        }

        else {
            printf("Commande inconnue. Tapez 'help' pour la liste des commandes.\n");
        }
    }
    return 0;

}

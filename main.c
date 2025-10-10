#include <stdio.h>

#include <string.h>
#include "ppm_io.h"
#include "operations.h" // Fonctions gris, neg, dom, cut

// Renommer les fonctions readPicture/imread en read_ppm et freeImagePPM/freePicture en free_ppm_img
// pour uniformiser le code et utiliser le code du fichier ppm_io.c

int main() {
    char command[256];
    PPMImage* img = NULL;
    char filename[100]; // Nom du fichier actuellement chargé

    printf("Bienvenue dans PPMViewer ! Tapez 'help' pour la liste des commandes.\n");

    while (1) {
        printf("PPMViewer> ");
        if (scanf("%s", command) != 1) {
            // Gérer la fin de fichier (EOF) ou erreur de lecture
            break;
        }

        // --- Commande d’aide ---
        if (strcmp(command, "help") == 0) {
            printf("\nCommandes disponibles :\n");
            printf(" new <fichier>              - Creer une nouvelle image PPM et l'enregistrer\n");
            printf(" load <fichier>             - Charger une image existante\n");
            printf(" save <fichier>             - Sauvegarder l'image actuelle (si chargee)\n");
            printf(" size                       - Afficher la taille de l'image chargee\n");
            printf(" gris <fichier_resultat>    - Convertir l'image chargee en niveaux de gris\n");
            printf(" neg <fichier_resultat>     - Créer le négatif de l'image chargee\n");
            printf(" fil <fichier_resultat>     - Appliquer un filtre médian 3x3 a l'image chargee\n");
            printf(" cut l1 l2 c1 c2 <fichier>  - Découper une sous-image (lignes l1 a l2, colonnes c1 a c2)\n");
            printf(" dom R|G|B val <fichier>    - Ajuster la luminosite (val) des pixels a dominante R, G ou B\n");
            printf(" exit                       - Quitter le programme\n\n");
        }

        // --- Commande load ---
        else if (strcmp(command, "load") == 0) {
            char load_file[100];
            if (scanf("%s", load_file) != 1) {
                printf("Erreur: Nom de fichier manquant.\n");
                continue;
            }

            if (img) free_ppm_img(img);
            img = read_ppm(load_file);
            if (img) {
                strcpy(filename, load_file); // Garder une trace du nom de fichier
                printf("Image %s chargee (%d x %d).\n", filename, img->width, img->height);
            } else {
                // read_ppm affiche déjà une erreur, on remet img a NULL
                img = NULL;
            }
        }

        // --- Commande save ---
        else if (strcmp(command, "save") == 0) {
            if (!img) {
                printf("Aucune image chargee.\n");
                continue;
            }
            char save_file[100];
            if (scanf("%s", save_file) != 1) {
                printf("Erreur: Nom de fichier de sauvegarde manquant.\n");
                continue;
            }
            write_ppm(img, save_file);
            printf("Image sauvegardee sous %s\n", save_file);
        }

        // --- Commande size ---
        else if (strcmp(command, "size") == 0) {
            if (!img) {
                printf("Aucune image chargee.\n");
                continue;
            }
            // Opération 4: Afficher la taille
            printf("Taille de l'image: %d x %d pixels (Largeur x Hauteur)\n", img->width, img->height);
        }

        // --- Commande new ---
        else if (strcmp(command, "new") == 0) {
            if (img) free_ppm_img(img);
            img = create_Image(); // Demande largeur, hauteur et pixels a l'utilisateur

            char nouveau[100];
            printf("Entrez le nom du fichier pour enregistrer la nouvelle image : ");
            scanf("%s", nouveau);

            // Retirer le caractère nouvelle ligne créer automatiquement avant write_ppm
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            write_ppm(img, nouveau);
            strcpy(filename, nouveau);
            printf("Image creee et enregistree sous %s\n", nouveau);
        }

        // --- Commande gris ---
        else if (strcmp(command, "gris") == 0) {
            if (!img) {
                printf("Aucune image chargee.\n");
                continue;
            }
            char nouveau[100];
            if (scanf("%s", nouveau) != 1) {
                printf("Erreur: Nom de fichier de resultat manquant.\n");
                continue;
            }

            PPMImage* gris_img = gris_operation(img); // Opération 2
            if (gris_img) {
                write_ppm(gris_img, nouveau);
                free_ppm_img(gris_img);
                printf("Conversion en niveaux de gris effectuee -> %s\n", nouveau);
            }
        }

        // --- Commande neg ---
        else if (strcmp(command, "neg") == 0) {
            if (!img) {
                printf("Aucune image chargee.\n");
                continue;
            }
            char nouveau[100];
            if (scanf("%s", nouveau) != 1) {
                printf("Erreur: Nom de fichier de resultat manquant.\n");
                continue;
            }

            PPMImage* neg_img = neg_operation(img); // Opération 3
            if (neg_img) {
                write_ppm(neg_img, nouveau);
                free_ppm_img(neg_img);
                printf("Négatif de l'image cree -> %s\n", nouveau);
            }
        }

        // --- Commande fil (Filtre médian) ---
        else if (strcmp(command, "fil") == 0) {
            if (!img) {
                printf("Aucune image chargee.\n");
                continue;
            }
            char nouveau[100];
            if (scanf("%s", nouveau) != 1) {
                printf("Erreur: Nom de fichier de resultat manquant.\n");
                continue;
            }

            PPMImage* filtre = filtmedian(img); // Opération 6 (déjà implémentée dans filtre_median.c)
            if (filtre) {
                write_ppm(filtre, nouveau);
                free_ppm_img(filtre);
                printf("Filtre médian appliqué -> %s\n", nouveau);
            }
        }

        // --- Commande cut ---
        else if (strcmp(command, "cut") == 0) {
            if (!img) {
                printf("Aucune image chargee.\n");
                continue;
            }
            int l1, l2, c1, c2;
            char nouveau[100];

            if (scanf("%d %d %d %d %s", &l1, &l2, &c1, &c2, nouveau) != 5) {
                printf("Erreur: Arguments de découpe manquants ou incorrects.\n");
                continue;
            }

            PPMImage* cut_img = cut_operation(img, l1, l2, c1, c2); // Opération 5
            if (cut_img) {
                write_ppm(cut_img, nouveau);
                free_ppm_img(cut_img);
                printf("Découpage effectue et enregistre sous %s\n", nouveau);
            } else {
                printf("Erreur lors du decoupage. Verifiez les coordonnées.\n");
            }
        }

        // --- Commande dom (Foncer/Éclaircir Dominante) ---
        // Format du TP: dom c val fichier.ppm (mais l'exemple d'exécution semble charger l'image avant l'op)
        // Je vais suivre le format de l'exemple: dom R 4. \mesimages\image2.ppm
        else if (strcmp(command, "dom") == 0) {
            if (!img) {
                printf("Aucune image chargee.\n");
                continue;
            }

            char dominant_cible;
            int ajustement;
            char nouveau[100];

            if (scanf(" %c %d %s", &dominant_cible, &ajustement, nouveau) != 3) {
                printf("Erreur: Arguments de 'dom' manquants ou incorrects (Format: dom R|G|B val fichier).\n");
                continue;
            }

            PPMImage* dom_img = dom_operation(img, dominant_cible, ajustement); // Opération 1
            if (dom_img) {
                write_ppm(dom_img, nouveau);
                free_ppm_img(dom_img);
                printf("Operation 'dom' effectuee sur dominante %c avec ajustement %d -> %s\n",
                       dominant_cible, ajustement, nouveau);
            }
        }

        // --- Commande exit ---
        else if (strcmp(command, "exit") == 0) {
            printf("Fin du programme.\n");
            if (img) free_ppm_img(img);
            break;
        }

        else {
            printf("Commande inconnue. Tapez 'help' pour la liste des commandes.\n");
        }

        // Nettoyer le buffer d'entree après chaque commande (utile si des arguments sont lus partiellement)
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    return 0;
}

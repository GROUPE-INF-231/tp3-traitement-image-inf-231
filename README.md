# INF231: Technique de conception d'algorithmes et structures de données.

# TP: Traitement d'images avec les fichiers et tableau 

# SUPERVISEUR 
PR. MELATAGIA Paulin

# OBJECTIF DU TP
l'objectif de ce travail est de concevoir une application en langage C capable de manipuler les images au format PPM (portable PixMap).
Le format PPM permet de stocker une image non compressée sous forme d'une liste de pixel, chacun défini par trois valeurs : R (rouge), G (vert) et B (bleu) comprise entre 0 et 255 

# Organisation de l'implémentation des opérations
Les 6 opérations demandées sont implémentées dans **operations.c**, tandis que les fonctions utilitaires nécessaires au traitement des images PPM se trouvent dans **ppm_io.c**. Le fichier **main.c** gère l'interface avec l'utilisateur et la boucle de commandes. Cette organisation modulaire sépare clairement la logique des opérations de l'interface et des fonctions techniques, ce qui facilite la maintenance et le travail en équipe.

## Fonctions utilitaires
**create_ppm_image** crée une nouvelle image en mémoire avec les dimensions spécifiées. **read_ppm** et **write_ppm** permettent de lire et d’écrire les fichiers PPM. **copy_image** duplique une image existante pour pouvoir la modifier sans toucher à l’originale. **free_ppm_img** libère proprement la mémoire utilisée par une image. **clamp** garantit que les valeurs de couleur restent dans les limites autorisées. **get_dominant_color** identifie la couleur principale d’un pixel. **are_valid_coordinates** vérifie que les coordonnées fournies pour découper l’image sont valides. Enfin, **valmedian** trie un tableau et retourne sa valeur médiane pour appliquer le filtre médian.

## Fonctionnalité 1 - Modification des couleurs dominantes (dom_operation)
Cette fonction lit l'image avec **read_ppm** et fait une copie avec **copy_image**. Elle parcourt ensuite chaque pixel pour trouver sa couleur principale avec **get_dominant_color**. Si le pixel a la couleur demandée, elle ajuste toutes ses composantes avec la valeur donnée, en utilisant **clamp** pour éviter les dépassements. La fonction vérifie d'abord que l'image existe et que la couleur est valide.

## Fonctionnalité 2 - Conversion en noir et blanc (gris_operation)
L'opération transforme l'image en niveaux de gris en calculant la moyenne des trois couleurs de chaque pixel. Après avoir copié l'image originale, elle remplace chaque valeur de couleur par cette moyenne simple. La fonction confirme d'abord que l'image source est disponible.

## Fonctionnalité 3 - Création du négatif (neg_operation)
Pour obtenir le négatif, la fonction inverse chaque couleur en la soustrayant de la valeur maximale autorisée. Cette transformation simple crée l'effet de photo négative. L'image est d'abord copiée puis modifiée pixel par pixel.

## Fonctionnalité 4 - Affichage des dimensions (size_operation)
Cette commande charge rapidement l'image pour lire et afficher sa largeur et hauteur. Elle signale si le fichier est introuvable et libère la mémoire immédiatement après l'affichage.

## Fonctionnalité 5 - Découpe d'image (cut_operation)
La fonction découpe une zone rectangulaire après avoir vérifié que les coordonnées sont valides avec **are_valid_coordinates**. Elle crée une nouvelle image de la taille demandée et copie uniquement la région sélectionnée. Les coordonnées doivent être dans les limites de l'image originale.

## Fonctionnalité 6 - Filtre médian (fil_operation)
Ce filtre réduit le bruit en analysant le voisinage de chaque pixel. Pour chaque couleur, il collecte les valeurs des pixels voisins, les trie avec **valmedian** et prend la valeur du milieu. Le traitement gère automatiquement les bords de l'image où le voisinage est plus petit.

# Contribution des membres du groupe

**• FAGNA TCHAPTCHET Noel Vanys 24G2310** : Développement complet de toutes les fonctions utilitaires nécessaires aux six fonctionnalités demandées. Développement du **main.c** et intégration avec les opérations sur les images PPM (operations.c) ainsi que les fonctions utilitaires (ppm_io.c). Correction intégrale de la fonctionnalité **size_operation** pour l’affichage des dimensions d’une image. Proposition et explication de la programmation modulaire au sein de l’équipe, avec communication active pour guider les membres. Tests et débogage des fonctionnalités développées par les autres membres. Rédaction complete du fichier **README**.

**• DEMANOU FOUDJI Karl Albert Kuete 24F2995**

**• ABONDO Jean Joël 23V2214**

**• CHOUPO FOGAING Rudy Aubin 24G2708**

**• BAKALACK Basile Romain 23V2529**

**• NGUIMGOU ZEPA Eveline Oceane 24F2579**

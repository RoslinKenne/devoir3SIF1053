#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <time.h>

typedef unsigned char UCHAR;

struct IMAGERAW {
    int hauteur;
    int largeur;
    UCHAR *image;
};

typedef struct IMAGERAW IMAGE;

// Fonction pour lire l'image
IMAGE *lire_image() {
    char nom_fichier[80];
    printf("Entrez le nom du fichier image : ");
    scanf("%s", nom_fichier);

    IMAGE *im = (IMAGE *)malloc(sizeof(IMAGE));
    FILE *fp = fopen(nom_fichier, "rb");

    if (fp == NULL) {
        perror("Erreur:");
        return NULL;
    }

    // Demander et valider la hauteur et largeur de l'image
    printf("Entrez la hauteur de l'image : ");
    scanf("%d", &(im->hauteur));
    printf("Entrez la largeur de l'image : ");
    scanf("%d", &(im->largeur));

    im->image = (UCHAR *)malloc(sizeof(UCHAR) * im->hauteur * im->largeur);
    fread(im->image, sizeof(UCHAR), im->hauteur * im->largeur, fp);

    fclose(fp);
    return im;
}

// Fonction pour crypter ou décrypter l'image
IMAGE *crypter_decrypter(IMAGE *im, UCHAR cle) {
    int L = im->largeur;
    int H = im->hauteur;

    for (int m = 0; m < H; m++) {
        for (int n = 0; n < L; n++) {
            im->image[m * L + n] = im->image[m * L + n] ^ cle;
        }
    }

    return im;
}
// Fonction pour Ecrire l'image
void ecrire_image(IMAGE *imageD) {
    char nom_fichier[80];
    printf("Entrez le nom du fichier A Ecrire : ");
    scanf("%s", nom_fichier);

    FILE *fpD = fopen(nom_fichier, "wb");
    fwrite(imageD->image, sizeof(UCHAR), imageD->hauteur * imageD->largeur, fpD);
    fclose(fpD);
}

int main() {
    struct _timeb start, finish;
    unsigned long duration;

    _ftime(&start);

    IMAGE *im = lire_image();

    UCHAR cle;
    printf("Entrez la clé de cryptage (un nombre entre 0 et 255) : ");
    scanf("%hhu", &cle);

    im = crypter_decrypter(im, cle);
    ecrire_image(im);

    free(im->image);
    free(im);

    _ftime(&finish);
    duration = (unsigned long)finish.time * 1000L + (unsigned long)finish.millitm - (unsigned long)start.time * 1000L + (unsigned long)start.millitm;
    printf("Temps d'execution : %lu ms\n", duration);

    return 0;
}

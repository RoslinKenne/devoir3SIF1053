#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>
#include <string.h>

/* Démonstration XOR, pas un chiffrement adapté aux données sensibles. */
static int nombre(const char *s, size_t min, size_t max, size_t *value) {
    char *end; errno = 0;
    if (!s || *s == '-' || *s == '\0') return 0;
    unsigned long long n = strtoull(s, &end, 10);
    if (errno || *end || n < min || n > max) return 0;
    *value = (size_t)n; return 1;
}
int main(int argc, char **argv) {
    char source[4096], destination[4096], h[64], w[64], k[64];
    const char *src, *dst, *hs, *ws, *ks;
    if (argc == 6) { src=argv[1]; dst=argv[2]; hs=argv[3]; ws=argv[4]; ks=argv[5]; }
    else if (argc == 1) {
        printf("Fichier source : "); if (scanf("%4095s",source)!=1) return 1;
        printf("Hauteur : "); if (scanf("%63s",h)!=1) return 1;
        printf("Largeur : "); if (scanf("%63s",w)!=1) return 1;
        printf("Clé XOR (0-255) : "); if (scanf("%63s",k)!=1) return 1;
        printf("Fichier destination : "); if (scanf("%4095s",destination)!=1) return 1;
        src=source; dst=destination; hs=h; ws=w; ks=k;
    } else { fprintf(stderr,"Usage : %s source.raw destination.raw hauteur largeur cle\n",argv[0]); return 1; }
    size_t hauteur, largeur, cle;
    if (!nombre(hs,1,SIZE_MAX,&hauteur) || !nombre(ws,1,SIZE_MAX,&largeur)
        || !nombre(ks,0,255,&cle) || hauteur > SIZE_MAX / largeur) {
        fprintf(stderr,"Dimensions ou clé invalides.\n"); return 1;
    }
    if (strcmp(src,dst)==0) { fprintf(stderr,"Choisissez un fichier destination différent.\n"); return 1; }
    size_t size=hauteur*largeur;
    /* Limite explicite pour les images de cette démonstration. */
    if (size > 100000000) { fprintf(stderr,"Image limitée à 100 millions de pixels.\n"); return 1; }
    FILE *in=fopen(src,"rb"); if (!in) { perror("Lecture"); return 1; }
    unsigned char *data=malloc(size);
    if (!data) { fclose(in); fprintf(stderr,"Mémoire insuffisante.\n"); return 1; }
    if (fread(data,1,size,in)!=size || fgetc(in)!=EOF || ferror(in)) {
        fprintf(stderr,"La taille du fichier ne correspond pas aux dimensions.\n"); free(data); fclose(in); return 1;
    }
    fclose(in);
    clock_t start=clock();
    for (size_t i=0;i<size;i++) data[i]^=(unsigned char)cle;
    FILE *out=fopen(dst,"wb");
    if (!out) { perror("Écriture"); free(data); return 1; }
    int ok=fwrite(data,1,size,out)==size;
    if (fclose(out)!=0) ok=0;
    free(data);
    if (!ok) { fprintf(stderr,"Écriture incomplète.\n"); return 1; }
    printf("Traitement terminé en %.3f ms.\n",1000.0*(clock()-start)/CLOCKS_PER_SEC);
    return 0;
}

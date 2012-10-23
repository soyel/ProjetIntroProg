#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gestion_fichiers.h"

void remplir_fichier(FILE *fichier, char *laby, size_t nbLignes, size_t nbColonnes){
    char chaineTemporaire[4];
    itoa(nbLignes, chaineTemporaire, 10);
    fputs(chaineTemporaire, fichier);
    fputs("\n", fichier);
    memset(&chaineTemporaire[0], 0, sizeof(chaineTemporaire));
    itoa(nbColonnes, chaineTemporaire, 10);
    fputs(chaineTemporaire, fichier);
    fputs("\n", fichier);
    size_t i, j;
    for(i=0; i<nbLignes; i++){
        for(j=0; j<nbColonnes; j++){
            fputc(laby[nbColonnes * i + j], fichier);
        }
        fputs("\n", fichier);
    }
}

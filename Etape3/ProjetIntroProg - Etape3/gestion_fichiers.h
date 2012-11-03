#ifndef GESTION_FICHIERS_H_INCLUDED
#define GESTION_FICHIERS_H_INCLUDED

void remplir_fichier(FILE *fichier, char *laby, size_t nbLignes, size_t nbColonnes);
void lire_fichier_labyrinthe(FILE *fichier, char *laby, size_t nbLignes, size_t nbColonnes);

#endif

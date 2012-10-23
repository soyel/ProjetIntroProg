#ifndef AFFICHAGE_H_INCLUDED
#define AFFICHAGE_H_INCLUDED
#define TAILLE_LIGNE 10

int afficher_accueil();
void afficher_menu();
void afficher_labyrinthe(char *laby, size_t nbLignes, size_t nbColonnes, int score);

#endif // AFFICHAGE_H_INCLUDED

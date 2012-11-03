#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "affichage.h"

int main(void)
{
    int choix_utilisateur;

    srand(time(NULL));
    choix_utilisateur = afficher_accueil();
    if(choix_utilisateur==0)
    {
        return 0;
    }

    afficher_menu();

    return 0;
}

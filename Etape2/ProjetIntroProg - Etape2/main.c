#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "affichage.h"

void vider_buffer(void)
{
    int c = 0;
    while (c != '\n' || c != EOF)
    {
        c = getchar();
    }
}

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

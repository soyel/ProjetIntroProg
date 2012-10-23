#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LIGNES 9
#define COLONNES 23

struct coordonneesMurs{
    int x;
    int y;
    char type;
};

typedef struct cellule cellule;
struct cellule
{
    int x;
    int y;
    int identifiant;
    struct cellule *celluleSuivante;
    struct cellule *premiereCellule;
};

typedef cellule* chemin;

int afficher_accueil();
void creer_labyrinthe();
chemin initialiserChemin(int id, int x, int y);
void ajouterEnfinDeListe(chemin lesChemins, cellule* leChemin);
void jouer();

char laby [LIGNES][COLONNES];

chemin tabDesChemins[LIGNES][COLONNES];

struct coordonneesMurs mesMurs[73];

int positionActuelle_X = 1;
int positionActuelle_Y = 0;

void vider_buffer(void){
    int c = 0;
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

int main(void)
{
    int choix_utilisateur;

    srand(time(NULL));
    choix_utilisateur = afficher_accueil();
    if(choix_utilisateur==0){
        return 0;
    }
    char choix_rejouer = 'A';
    do{
        creer_labyrinthe();
        jouer();
        printf("Bravo! Vous avez gagne!\n");
        do{
             printf("\nVoulez-vous rejouer? (O/N) : ");
             fflush(stdin);
             scanf("%c", &choix_rejouer);
        }while(choix_rejouer != 'O' && choix_rejouer != 'N');
    }while(choix_rejouer == 'O');

    return 0;
}

chemin initialiserChemin(int id, int x, int y){
    cellule* nouvelleCellule = malloc(sizeof(cellule));

    nouvelleCellule->identifiant = id;
    nouvelleCellule->x = x;
    nouvelleCellule->y = y;

    nouvelleCellule->celluleSuivante = NULL;
    nouvelleCellule->premiereCellule = nouvelleCellule;

    return nouvelleCellule;
}

void ajouterEnfinDeListe(chemin lesChemins, cellule* leChemin){
    cellule* temp = lesChemins;
    while(temp->celluleSuivante != NULL)
    {
        temp = temp->celluleSuivante;
    }
    temp->celluleSuivante = leChemin;
    return;
}

void afficher_labyrinth(){
    int i, j;
    for(i=0; i<LIGNES; i++){
        for(j=0; j<COLONNES; j++){
            if(laby[i][j]=='H' || laby[i][j]=='V'){
                laby[i][j]='#';
            }
            printf("%c", laby[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("Aide: Faites deplacer le symbole 'o' vers la sortie, situee en bas a droite.\n");
    return;
}

int afficher_accueil(){
    printf("################################################\n");
    printf("################################################\n");
    printf("###                                          ###\n");
    printf("###                                          ###\n");
    printf("###    ##          #      #####     ##  ##   ###\n");
    printf("###    ##          #      ##  ##     ####    ###\n");
    printf("###    ##         # #     ######      ##     ###\n");
    printf("###    ##        #####    ##  ##      ##     ###\n");
    printf("###    ######   ##   ##   #####       ##     ###\n");
    printf("###                                          ###\n");
    printf("###                1. Jouer                  ###\n");
    printf("###                2. Quitter                ###\n");
    printf("###                                          ###\n");
    printf("###                                          ###\n");
    printf("###  Copyrights R. Kos - ENSICAEN 2012-2013  ###\n");
    printf("###                                          ###\n");
    printf("################################################\n");
    printf("################################################\n");

    int touche = 'o';
    while(touche!='2' && touche!='1'){
        touche = getch();
    }
    if(touche == '2')
    {
        return 0;
    }
    return 1;

}

void creer_labyrinthe(){
    int compteurCell = 1;
    int compteurMur = 1;
    int i, j;


    for(i = 0; i<LIGNES; ++i){
        for(j = 0; j<COLONNES; ++j){
            if(i==0 ||j==0 || i == LIGNES - 1 || j == COLONNES - 1){
                laby[i][j] = '#';
            }else{
                if(i%2!=0 && j%2!=0){
                    laby[i][j] = ' ';
                    tabDesChemins[i][j] = initialiserChemin(compteurCell, i, j);
                    compteurCell +=1;
                }else{
                    if(i%2 == 0 && j%2 == 0){
                        laby[i][j] = '#';
                    }else{
                        mesMurs[compteurMur-1].x = i;
                        mesMurs[compteurMur-1].y = j;
                        if(i%2==0){
                            mesMurs[compteurMur-1].type = 'H';
                            laby[i][j] = 'H';
                        }else{
                            mesMurs[compteurMur-1].type = 'V';
                            laby[i][j] = 'V';
                        }
                        compteurMur +=1;
                    }
                }
            }
        }
    }

    printf("La construction de la carte s'est bien effectuee\n");

	/*printf("Les Murs:\n");
    for(ii=0; ii<7; ii++){
        printf("Coord du mur: %d, %d de type: %c\n", mesMurs[ii].x, mesMurs[ii].y, mesMurs[ii].type);
    }*/

    /*printf("Les cellules:\n");
    for(i = 0; i<LIGNES; ++i){
        for(j = 0; j<COLONNES; ++j){
            if(i!=0 ||j!=0 || i != LIGNES - 1 || j != COLONNES - 1){
                if(i%2!=0 && j%2!=0){
                    printf("Les coordonnees de la cellule %d sont : %d, %d\n", tabDesChemins[i][j]->identifiant, tabDesChemins[i][j]->x, tabDesChemins[i][j]->y);
                    printf("Cette cellule a comme pour debut la cellule %d, dont les coordonnes sont %d, %d\n", tabDesChemins[i][j]->premiereCellule->identifiant, tabDesChemins[i][j]->premiereCellule->x, tabDesChemins[i][j]->premiereCellule->y);
                    if(tabDesChemins[i][j]->celluleSuivante != NULL){
                        printf("Cette cellule a comme pour suite la cellule %d, dont les coordonnes sont %d, %d\n", tabDesChemins[i][j]->celluleSuivante->identifiant, tabDesChemins[i][j]->celluleSuivante->x, tabDesChemins[i][j]->celluleSuivante->y);
                    }
                }
            }

        }
    }*/


    compteurMur = 73;
    int compteurAleatoire;
    int coordXTmp, coordYTmp;
    int coordXPremCell, coordYPremCell;
    int coordXSecCell, coordYSecCell;
    int idPremCell, idSecCell;
    int cellDominante, cell1, cell2;
    cellule* cellule1 = NULL;
    cellule* cellule2 = NULL;
    cellule* tmpDebutChemin = NULL;
    cellule* tmpCelluleSuivante = NULL;
    while(compteurMur > 0){
        //printf("Iteration aleatoire n %d\n", (73 - compteurMur)+1);
        compteurAleatoire = rand()%compteurMur;
        //printf("Tirage aleatoire entre 0 et %d : %d\n", compteurMur, compteurAleatoire);
        coordXTmp = mesMurs[compteurAleatoire].x;
        coordYTmp = mesMurs[compteurAleatoire].y;
        cellDominante = rand()%2;
        //printf("Cellule dominante : %d\n", cellDominante);
        //printf("Coord point al : %d, %d de type %c\n", coordXTmp, coordYTmp, mesMurs[compteurAleatoire].type);
        if(cellDominante == 0){
            cell1 = -1;
            cell2 = 1;
        }else{
            cell1 = 1;
            cell2 = -1;
        }
        if(mesMurs[compteurAleatoire].type=='H'){
            coordXPremCell = coordXTmp + cell1;
            coordXSecCell = coordXTmp + cell2;
            coordYPremCell = coordYTmp;
            coordYSecCell = coordYTmp;
        }else{
            coordXPremCell = coordXTmp;
            coordXSecCell = coordXTmp;
            coordYPremCell = coordYTmp + cell1;
            coordYSecCell = coordYTmp + cell2;
        }
        //printf("CoordPrem %d, %d\nCoordSec %d, %d\n", coordXPremCell, coordYPremCell, coordXSecCell, coordYSecCell);
        cellule1 = tabDesChemins[coordXPremCell][coordYPremCell];
        cellule2 = tabDesChemins[coordXSecCell][coordYSecCell];
        tmpDebutChemin = cellule2 ->premiereCellule;
        idPremCell = cellule1->identifiant;
        idSecCell = cellule2->identifiant;
        /*printf("Les identifiants sont : %d et %d\n", idPremCell, idSecCell);
        printf("Les coordonnees de la cellule dominante sont : %d, %d\n", cellule1->x, cellule1->y);
        printf("Cette cellule a comme pour debut la cellule %d, dont les coordonnes sont %d, %d\n", cellule1->premiereCellule->identifiant, cellule1->premiereCellule->x, cellule1->premiereCellule->y);
        if(cellule1->celluleSuivante != NULL){
            printf("Cette cellule a comme pour suite la cellule %d, dont les coordonnes sont %d, %d\n", cellule1->celluleSuivante->identifiant, cellule1->celluleSuivante->x, cellule1->celluleSuivante->y);
        }
        printf("Les coordonnees de la cellule dominee sont : %d, %d\n", cellule2->x, cellule2->y);
        printf("Cette cellule a comme pour debut la cellule %d, dont les coordonnes sont %d, %d\n", cellule2->premiereCellule->identifiant, cellule2->premiereCellule->x, cellule2->premiereCellule->y);
        if(cellule2->celluleSuivante != NULL){
            printf("Cette cellule a comme pour suite la cellule %d, dont les coordonnes sont %d, %d\n", cellule2->celluleSuivante->identifiant, cellule2->celluleSuivante->x, cellule2->celluleSuivante->y);
        }*/
       // _getch();



        if(idPremCell != idSecCell){
            laby[coordXTmp][coordYTmp] = ' ';
            if((tmpDebutChemin->celluleSuivante)==NULL){
                /*printf("Cas cellule isolee\n");*/
                ajouterEnfinDeListe(cellule1->premiereCellule, cellule2);
                cellule2->premiereCellule = cellule1->premiereCellule;
                cellule2->identifiant = cellule1->identifiant;
            }else{
                /*printf("Cas  cellule avec chemin\n");*/
                tmpCelluleSuivante = tmpDebutChemin;
                while(tmpCelluleSuivante->celluleSuivante != NULL){
                    tmpCelluleSuivante->identifiant = cellule1->identifiant;
                    tmpCelluleSuivante->premiereCellule = cellule1->premiereCellule;
                    tmpCelluleSuivante = tmpCelluleSuivante->celluleSuivante;
                }
                tmpCelluleSuivante->identifiant = cellule1->identifiant;
                tmpCelluleSuivante->premiereCellule = cellule1->premiereCellule;
                ajouterEnfinDeListe(cellule1->premiereCellule, tmpDebutChemin);
            }
        }
        /*printf("Transfert de cellule....\n");
        printf("Les coordonnees de la cellule dominante sont : %d, %d\n", cellule1->x, cellule1->y);
        printf("Cette cellule a comme pour debut la cellule %d, dont les coordonnes sont %d, %d\n", cellule1->premiereCellule->identifiant, cellule1->premiereCellule->x, cellule1->premiereCellule->y);
        if(cellule1->celluleSuivante != NULL){
            printf("Cette cellule a comme pour suite la cellule %d, dont les coordonnes sont %d, %d\n", cellule1->celluleSuivante->identifiant, cellule1->celluleSuivante->x, cellule1->celluleSuivante->y);
        }
        printf("Les coordonnees de la cellule dominee sont : %d, %d\n", cellule2->x, cellule2->y);
        printf("Cette cellule a comme pour debut la cellule %d, dont les coordonnes sont %d, %d\n", cellule2->premiereCellule->identifiant, cellule2->premiereCellule->x, cellule2->premiereCellule->y);
        if(cellule2->celluleSuivante != NULL){
            printf("Cette cellule a comme pour suite la cellule %d, dont les coordonnes sont %d, %d\n", cellule2->celluleSuivante->identifiant, cellule2->celluleSuivante->x, cellule2->celluleSuivante->y);
        }*/
        //_getch();
        mesMurs[compteurAleatoire].x = mesMurs[compteurMur-1].x;
        mesMurs[compteurAleatoire].y = mesMurs[compteurMur-1].y;
        mesMurs[compteurAleatoire].type = mesMurs[compteurMur-1].type;
        compteurMur -= 1;
    }

    laby[1][0] = ' ';
    laby[LIGNES - 2][COLONNES - 1] = ' ';
    printf("Generation du laby s'est bien effectuee\n");

    return;
}
int verifier_possibilite(int x, int y){
    if(laby[positionActuelle_X + x][positionActuelle_Y + y]!='#'){
        return 1;
    }
    return 0;
}
void deplacerAdroite(){
    if(verifier_possibilite(0, 1) == 1){
        laby[positionActuelle_X][positionActuelle_Y] = ' ';
        positionActuelle_Y +=1;
        laby[positionActuelle_X][positionActuelle_Y] = 'o';
        system("CLS");
        afficher_labyrinth();
    }
    return;
}
void deplacerAgauche(){
    if(verifier_possibilite(0, -1) == 1){
        laby[positionActuelle_X][positionActuelle_Y] = ' ';
        positionActuelle_Y -= 1;
        laby[positionActuelle_X][positionActuelle_Y] = 'o';
        system("CLS");
        afficher_labyrinth();
    }
    return;
}
void deplacerEnHaut(){
    if(verifier_possibilite(-1, 0) == 1){
        laby[positionActuelle_X][positionActuelle_Y] = ' ';
        positionActuelle_X -= 1;
        laby[positionActuelle_X][positionActuelle_Y] = 'o';
        system("CLS");
        afficher_labyrinth();
    }
    return;
}
void deplacerEnBas(){
    if(verifier_possibilite(1, 0) == 1){
        laby[positionActuelle_X][positionActuelle_Y] = ' ';
        positionActuelle_X += 1;
        laby[positionActuelle_X][positionActuelle_Y] = 'o';
        system("CLS");
        afficher_labyrinth();
    }
    return;
}

int verifier_position(){
    if(positionActuelle_X == LIGNES - 2 && positionActuelle_Y == COLONNES - 1){
        return 1;
    }
    return 0;
}

void jouer(){
    system("CLS");
    laby[1][0] = 'o';
    afficher_labyrinth();
    char touche = 'l';
    fflush(stdin);
    while(touche != 'f'){
        touche = getch();
        switch(touche){
            case 'd': deplacerAdroite();
                        break;
            case 'q': deplacerAgauche();
                        break;
            case 's': deplacerEnBas();
                        break;
            case 'z': deplacerEnHaut();
                        break;
        }
        if(verifier_position()==1){
            touche = 'f';
            positionActuelle_X = 1;
            positionActuelle_Y = 0;
            laby[LIGNES - 2][COLONNES - 1] = ' ';
        }
    }
    return;
}


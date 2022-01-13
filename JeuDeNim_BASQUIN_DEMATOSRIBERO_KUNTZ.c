#include <stdio.h> /* BASQUIN Thomas | DE MATOS RIBERO Romain | KUNTZ Alban 4.1 */
#include <stdlib.h>
#include <time.h>

#define VMIN 5
#define VMAX 30

struct T_Case
{
    int x;
    int y;
};

typedef struct T_Case T_Case; //on lui dit de remplacer "struct T_Case" par "T_Case"
typedef T_Case *T_Tab_Case;


void Nimber(int nim[][VMAX], int ncol, int nlig)
{
    int x;
    int y;
    nim[ncol-1][nlig-1] = 0; /* initialiation à nCol-1 */

    for (y = nlig-2; y>=0; y--) /*seulement pour la dernière colonne */
    {
        if (nim[ncol-1][y+1] == 1) /* case en dessous */
        {
            if (nim[ncol-1][y+2] == 1){ /* case deux en dessous */
                nim[ncol-1][y] = 0;
            }
        }
        else
        {
            nim[ncol-1][y] = 1;
        }
    }
    for (x = ncol-2; x >= 0; x--) /* autres colonnes */
    {
        // dernière ligne
        if (nim[x+1][nlig-1] == 1) /* case à droite */
        {
            if (nim[x+2][nlig-1] == 1){ /* case deux à droite */
                nim[x][nlig-1] = 0;
            }   
        }
        else
        {
            nim[x][nlig-1] = 1;
        }
        // autres ligne
        for (y = nlig-2; y >= 0; y--)
        {
            if (x != ncol-1)
            {
                if (nim[x+1][y+1] == 0){ /* case en diagonale en bas à droite */
                    nim[x][y] = 0;
                }
            }
            else
            {
                nim[x][y] = 1;
            }
            
        } 
    }
    
}


void Lire_Entier(int *x, int vmin, int vmax) /* Permet de tester si le nombre saisit et bien compris entre une certaine borne minimum et maximum */
{

    do{
        if (*x < vmin || *x > vmax) /* Lors de la rentré d'une donnée, il va vérifier au moins une fois si elle est entre les bornes, et va demander de saisir une nouvelle valeur */
        {                                      /* et repeter ca tant que la valeur n'est pas dans les bornes*/
            printf("\t*--- Erreur ---*\n");
            printf("Merci de saisir une valeur entre %d et %d\n", vmin, vmax);
            printf("Nouvelle valeur : ");
            scanf("%d",x);
        }
    } 
    while(*x < vmin || *x > vmax);

}

void Parametres(int *nlig, int *ncol, int *niveau, int *next) /* Différents paramètres choisit par l'utilisateur*/
{
    printf("Veuillez saisir le nombre de lignes de la grille :\n"); /* Nombres de lignes */
    scanf("%d", nlig);
    Lire_Entier(nlig, VMIN, VMAX);

    printf("\nVeuillez saisir le nombre de colonnes de la grille :\n"); /* Nombres de colonnes */
    scanf("%d", ncol);
    Lire_Entier(ncol, VMIN, VMAX);

    printf("\nQuel niveau de difficulte voulez-vous jouer?\n"); /* Difficulté */
    printf("Choississez entre : 1 - Debutant | 2 - Moyen | 3 - Expert | 4 - Virtuose\n");
    scanf("%d", niveau);
    Lire_Entier(niveau, 1, 4);

    printf("\nQui commence a jouer?\n");
    printf("Choississez entre : 1 - L'ordinateur | 2 - Vous\n"); /* Ordinateur ou Joueur qui commence*/
    scanf("%d", next);
    Lire_Entier(next, 1, 2);
}

void Grilles(T_Case pion, int ncol, int nlig)
{
    int x;
    int y;

    for(y = 0 ; y <= nlig ; y++) /* La grille se "construit" ligne par ligne, de la ligne y=O à y=nlig */ 
    {
        if( y == 0) /* crée d'abord la premiere ligne (numéro des colonnes) */
        {
            printf("\n    "); 
            for(x = 1 ; x<= ncol ; x++)
            {
                printf(" %-2d ", x);
            }
        }
        else /* puis va crée les autres lignes */
        {
            printf("\n%2d ", y);
            
            for(x=1 ; x <= ncol ; x++) /* Verifie si le pion est dans cette case pour soit mettre 0 ou - (0 étant le pion) */
            {
                if(pion.x==x && pion.y==y)
                {
                    printf("| 0 ");
                }
                else
                {
                    printf("| - ");
                }
            }
        printf("|");
        }
    }
}

void Coup_Joueur(T_Case *pion, int ncol, int nlig)
{   
    //On enregistre toutes les cases voisines du pion dans T_Case_

    T_Case voisin[4];
    int nb_vois;
    int choix;

    Voisine(*pion,voisin,&nb_vois, ncol, nlig);
    printf("\n");
    Grilles(*pion, ncol, nlig);
    printf("\nA votre tour de jouer ! ");
    printf("Ou souhaitez-vous deplacer le pion ? : \n");

    //On affiche les différentes cases jouables 

    for (int i = 0; i < nb_vois; i++)
        printf("%d :(%d,%d) | ", i, voisin[i].x, voisin[i].y);//On affiche les différentes cases voisines, donc les choix de déplacement possibles
        
    printf("\n----> ");
    scanf("%d", &choix);

    Lire_Entier(&choix, 0, nb_vois);//On récupère le choix de déplacement du joueur
    *pion = voisin[choix];
    Grilles(*pion, ncol, nlig);
}

void Coup_Ordi_Hasard(T_Case *pion,int ncol,int nlig)
{
    T_Case voisin[4];
    int nb_vois;
    Voisine(*pion,voisin,&nb_vois,ncol,nlig);
    *pion = voisin[rand() % nb_vois];
    printf("\nL'ordinateur deplace le pion en case (%d, %d)\n", pion->x, pion->y);
}

void Coup_Ordi_Gagnant(T_Case *pion, int nimber[][VMAX], int ncol, int nlig)
{
    T_Case voisin[4];
    int nb_vois;
    Voisine(*pion,voisin,&nb_vois,ncol,nlig);

    //L'ordinateur essaye de jouer si possible un coup gagnant 
    for (int i = 1; i < nb_vois; i++)
        if (nimber[voisin[i].x][voisin[i].y] == 0)
        {
            *pion = voisin[i];
            printf("\nL'ordinateur deplace le pion en case (%d, %d) ! \n", pion->x, pion->y);
        }

    *pion = voisin[rand() % nb_vois];
}
    
void Voisine(T_Case pion, T_Tab_Case Voisine, int *nb_vois, int ncol, int nlig)  /* on stock la position du pion dans rangpion puis on vérifie si les positions en x+1, x+2 et y+1, y+2 ne sont pas plus grand que nCol et nLig, si elles ne sont pas plus grandes, on les stock dans les cases voisine */
{
    T_Case rangpion;
    *nb_vois = 0;
    rangpion.y = pion.y + 1;
    rangpion.x = pion.x;
    if (nlig+1 > rangpion.y)
    {
        Voisine[*nb_vois].x = rangpion.x;
        Voisine[*nb_vois].y = rangpion.y;
        rangpion.y++;
        (*nb_vois)++;
        if (rangpion.y < nlig+1)
        {
            Voisine[*nb_vois].x = rangpion.x;
            Voisine[*nb_vois].y = rangpion.y;
            (*nb_vois)++;
        }
    }
    rangpion.y = pion.y;
    rangpion.x = pion.x + 1;
    if (ncol+1 > rangpion.x)
    {
        Voisine[*nb_vois].x = rangpion.x;
        Voisine[*nb_vois].y = rangpion.y;
        rangpion.x++;
        (*nb_vois)++;
        if (rangpion.x < ncol+1)
        {
            Voisine[*nb_vois].x = rangpion.x;
            Voisine[*nb_vois].y = rangpion.y;
            (*nb_vois)++;
        }
    }
}

int main()
{
    srand(time(NULL)); /* Temps à O */
    
    /* Message début de partie */
    printf("Bienvenue sur le jeu de Nim programme en C par BASQUIN Thomas, DE MATOS RIBERO Romain et KUNTZ Alban ! \n");
    printf("\nVous allez jouer une partie contre un ordinateur chacun votre tour\n");
    printf("Avant de commencer, definissez les parametres de la partie ! :)\n\n");

    int ncol = 0, nlig = 0, niveau = 0, next = 0; /* Initialisation des différents paramètres */
    Parametres(&nlig, &ncol, &niveau, &next);

    T_Case pion; /* Initialisation du pion en début de partie en case (1,1) */
    pion.x=1; pion.y=1;

    int nim[VMAX][VMAX]; /* Calcul les 0 et 1 de la grille */
    Nimber(nim, ncol, nlig);

    while (! (pion.x == ncol && pion.y == nlig ) ) /* Boucle déroulement du jeu | Tant que le pion n'est pas dans le puit la partie continue */ 
    {
        if (next == 1) /* si rdinateur qui commence / c'est au tour de l'ordinateur*/
        {
            if (niveau==1) /* Niveau 1, ne joue que coup au hasard */
            {
                Coup_Ordi_Hasard(&pion,ncol, nlig);
            }
            else if (niveau==2) /* Niveau 2, 1/3 de jouer coup gagnant, et donc 2/3 de jouer coup hasard */
            {
                if (rand() % 3 == 1)
                {
                    Coup_Ordi_Gagnant(&pion, nim, ncol, nlig);
                }
                else
                {
                    Coup_Ordi_Hasard(&pion, ncol, nlig);
                }
            }
            else if (niveau == 3) /* Niveau 3, 2/3 de jouer coup gagnant, et donc 1/3 de jouer coup hasard */
            {
                if (rand() % 3 == 1 || rand() % 3 == 2)
                {
                    Coup_Ordi_Gagnant(&pion, nim, ncol, nlig);
                }
                else
                {
                    Coup_Ordi_Hasard(&pion,ncol, nlig);
                }
            }
            else if (niveau == 4) /* Niveau 4, ne joue que coup gagant */
            {
                Coup_Ordi_Gagnant(&pion, nim, ncol, nlig);
            }
            next = 2; /* puis au tour du joueur via Coup_Joueur  */
        }
        else if (next == 2) /* si le joueur commence / c'est a son tour de jouer */
        {
            Coup_Joueur(&pion, ncol, nlig);
            next=1;
        }
    }

    if (next == 1) /* Si la derniere personne a jouer est le joueur alors il a gagné */
        {
            if ( niveau== 1 || niveau==2 || niveau==3)
            {
                printf("\n\nBravo ! Vous avez battu l'ordinateur de niveau %d, tentez de battre l'ordinateur de niveau %d !", niveau, niveau+1);
            }
            else if (niveau == 4)
            {
                printf("\n\nFelicitation ! Vous avez battu le dernier niveau de l'ordinateur !");
            }
        }
    else /* sinon c'est l'ordinateur */
    {
        printf("\n\n L'ordinateur de niveau %d a gagné", niveau);
        printf("\tVous avez perdu.");
    }

    return 0;
    
}
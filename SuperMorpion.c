#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define taille 9

//-----------Struct---------------


typedef struct morpion
{
    int plat[9];
    int gagnant;
} morpion;

typedef struct superMorpion 
{
    morpion supPlat[9];
    int tour;
    int ouJoue;
    int gagnant;
} superMorpion;


typedef struct coup 
{
    int grand;
    int petit;
} coup;

typedef struct coupMinMax 
{
    int heuristic;
    coup move;
} coupMinMax;

//---------MillonChaien---------------------

typedef struct maillon 
{
    superMorpion val;
    struct maillon* next;
    int grand;
    int petit;

} maillon;

typedef struct chaine{
    maillon* first;
} chaine;

chaine* creer_vide()
{
    chaine* res = malloc(sizeof(chaine));
    res->first = NULL;
    return res;
}

bool est_vide(chaine* c)
{
    return c->first == NULL;
}

void addMaillon(chaine* c, superMorpion val, int grand, int petit)
{
    maillon* new = malloc(sizeof(maillon));
    new->val = val;
    new->grand = grand;
    new->petit = petit;
    new->next = c->first;
    c->first = new;
}

void freeChaine(chaine* c)
{
    maillon* current = c->first;
    while(current != NULL)
    {
        maillon* aLib = current;
        current = current->next;
        free(aLib);
    }
    free(c);
}


//-----------------------JEU-------------------------------


void majGagnant(morpion* partie)
{
    /*Change la valeur du gagnant du petit morpion passer en argument*/
    if(partie->gagnant != 0)
    {
        return;
    }


    int lignes[3];
    int collones[3];
    int diag1 = 0;
    int diag2 = 0;
    for(int i = 0; i < 3; i++)
    {
        lignes[i] = 0;
        collones[i] = 0;
        for(int j = 0; j < 3; j++)
        {
            lignes[i] = lignes[i] + partie->plat[i*3 + j];
            collones[i] = collones[i] + partie->plat[i + j*3];
        }
        diag1 = diag1 + partie->plat[i*4];
        diag2 = diag2 + partie->plat[(i+1)*2];
    }

    for(int i = 0; i < 3; i++)
    {
        if(lignes[i] == 3 || collones[i] == 3)
        {
            partie->gagnant = 1;
            return;
        }
        else if(lignes[i] == -3 || collones[i] == -3)
        {
            partie->gagnant = -1;
            return;
        }
    }

    if(diag1 == 3 || diag2 == 3)
    {
        partie->gagnant = 1;
        return;
    }
    else if(diag1 == -3 || diag2 == -3)
    {
        partie->gagnant = -1;
        return;
    }

    for(int i = 0; i < 9; i++)
    {
        if(partie->plat[i] == 0)
        {
            return;
        }
    }
    partie->gagnant = -10;
    return;



}

int aGagnerSuperMorpion(superMorpion supPartie)
{
    /*Renvoie le gagnant 1 = j1; -1 = j2; -10 = egalite; 0 = pas finie*/
    int lignes[3];
    int collones[3];
    int diag1 = 0;
    int diag2 = 0;
    for(int i = 0; i < 3; i++)
    {
        lignes[i] = 0;
        collones[i] = 0;
        for(int j = 0; j < 3; j++)
        {
            lignes[i] = lignes[i] + supPartie.supPlat[3*i + j].gagnant;
            collones[i] = collones[i] + supPartie.supPlat[i + 3*j].gagnant;
        }
        diag1 = diag1 + supPartie.supPlat[i*4].gagnant;
        diag2 = diag2 + supPartie.supPlat[(i+1)*2].gagnant;
    }

    
    for(int i = 0; i < 3; i++)
    {
        if(lignes[i] == 3 || collones[i] == 3)
        {
            return 1;
        }
        else if(lignes[i] == -3 || collones[i] == -3)
        {
            return -1;
        }
    }

    if(diag1 == 3 || diag2 == 3)
    {
        return 1;
    }
    else if(diag1 == -3 || diag2 == -3)
    {
        return -1;
    }

    for(int i = 0; i < 9; i++)
    {
        if(supPartie.supPlat[i].gagnant == 0)
        {
            return 0;
        }
    }
    return -10;
}

void print_supMorpion(superMorpion supPartie)
{
    for(int l = 0; l < 3; l++)
    {
        for(int k = 0; k < 3; k++)
        {
            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    if(supPartie.supPlat[l*3 + i].gagnant == -10)
                    {
                        printf("_");
                    }
                    else if(supPartie.supPlat[l*3 + i].gagnant == 1)
                    {
                        printf("X");
                    }
                    else if(supPartie.supPlat[l*3 + i].gagnant == -1)
                    {
                        printf("0");
                    }
                    else if(supPartie.supPlat[l*3 + i].plat[j + k*3] == 1)
                    {
                        printf("X");
                    }
                    else if(supPartie.supPlat[l*3 +i].plat[j + k*3] == -1)
                    {
                        printf("0");
                    }
                    else
                    {
                        printf(" ");
                    }
                    if(j != 2)
                    {
                        printf("|");
                    }
                }
                if(i != 2)
                {
                    printf("||");
                }
            }

            if(k != 2)
            {
                printf("\n");
                for(int i = 0; i < 5; i++)
                {
                    printf("_");
                }
                printf("||");
                for(int i = 0; i < 5; i++)
                {
                    printf("_");
                }
                printf("||");
                for(int i = 0; i < 5; i++)
                {
                    printf("_");
                }
                printf("\n");
            }
        }
        if(l != 2)
        {
            printf("\n");
            for(int i = 0; i < 18; i++)
            {
                printf("_");
            }
            printf("\n");
            for(int i = 0; i < 18; i++)
            {
                printf("_");
            }
            printf("\n");
        }
    }
    printf("\n");

}



//-------------------Strategie---------------------------
coup strategie_utilisateur(superMorpion supPartie)
{
    int grand;
    int petit;
    printf("C'est au joueur %d de jouer !\n", supPartie.tour);

    if(supPartie.ouJoue == -1)
    {
        printf("Dans quelle grande case voulez vous jouez ? : ");
        scanf("%d", &grand);
        while(grand < 0 || grand > 8 || supPartie.supPlat[grand].gagnant != 0)
        {
            printf("Dans quelle grande case voulez vous jouez ? : ");
            scanf("%d\n", &grand);
        }
    }
    else 
    {
        grand = supPartie.ouJoue;
    }

    printf("Vous jouez dans la grande case %d.\n", grand);
    printf("Dans quelle petite case voulez vous jouez ? : ");
    scanf("%d", &petit);
    while(petit < 0 || petit > 8 || supPartie.supPlat[grand].plat[petit] != 0)
    {
        printf("Dans quelle petite case voulez vous jouez ? : ");
        scanf("%d\n", &petit);
    }

    coup res;
    res.grand = grand;
    res.petit = petit;
    return res;
}

coup strategie_alea(superMorpion supPartie)
{
    int grand;
    int petit;

    if(supPartie.ouJoue == -1)
    {
        grand = rand() % 9;
        while(grand < 0 || grand > 8 || supPartie.supPlat[grand].gagnant != 0)
        {
            grand = rand() % 9;
        }
    }
    else 
    {
        grand = supPartie.ouJoue;
    }

    petit = rand() % 9;
    while(petit < 0 || petit > 8 || supPartie.supPlat[grand].plat[petit] != 0)
    {
        petit = rand() % 9;
    }

    coup res;
    if(grand == -1 || petit == -1)
    {
        printf("Hey\n");
    }
    res.grand = grand;
    res.petit = petit;
    return res;
}

int heuristicMorpion(morpion partie)
{
    int res = 0;
    if(partie.gagnant != 0)
    {
        if(partie.gagnant != -10)
        {
            res = 24 * partie.gagnant;
        }
        
    }
    else
    {

        int lignes[3];
        int collones[3];
        int diag1 = 0;
        int diag2 = 0;
        for(int i = 0; i < 3; i++)
        {
            lignes[i] = 0;
            collones[i] = 0;
            for(int j = 0; j < 3; j++)
            {
                lignes[i] = lignes[i] + partie.plat[i*3 + j];
                collones[i] = collones[i] + partie.plat[i + j*3];
            }
            diag1 = diag1 + partie.plat[i*4];
            diag2 = diag2 + partie.plat[(i+1)*2];
        }

        for(int i = 0; i < 3; i++)
        {
            res = res + lignes[i] + collones[i];
        }
        res = res + diag1 + diag2;

    }
    return res;

}

int heuristicSuperMorpion(superMorpion supPartie)
{
    int res = 0;
    int lignes[3];
    int collones[3];
    int diag1 = 0;
    int diag2 = 0;
    for(int i = 0; i < 3; i++)
    {
        lignes[i] = 0;
        collones[i] = 0;
        for(int j = 0; j < 3; j++)
        {
            lignes[i] = lignes[i] + heuristicMorpion(supPartie.supPlat[i*3 + j]);
            collones[i] = collones[i] + heuristicMorpion(supPartie.supPlat[i + j*3]);
        }
        diag1 = diag1 + heuristicMorpion(supPartie.supPlat[i*4]);
        diag2 = diag2 + heuristicMorpion(supPartie.supPlat[(i+1)* 2]);
    }

    for(int i = 0; i < 3; i++)
    {
        res = res + lignes[i] + collones[i];
    }
    res = res + diag1 + diag2;
    return res;
}

int heuristic(superMorpion supPartie)
{
    int caseJ1 = 0;
    int caseJ2 = 0;
    for(int i = 0; i < 9; i++)
    {
        if(supPartie.supPlat[i].gagnant != 0)
        {
            if(supPartie.supPlat[i].gagnant == 1)
            {
                caseJ1 = caseJ1 + 9;
            }
            else if(supPartie.supPlat[i].gagnant == -1)
            {
                caseJ2 = caseJ2 + 9;
            }
        }
        else
        {
            for(int j = 0; j < 9; j++)
            {
                if(supPartie.supPlat[i].plat[j] == 1)
                {
                    caseJ1++;
                }
                else if(supPartie.supPlat[i].plat[j] == -1)
                {
                    caseJ2++;
                }
            }
        }
    }
    return (caseJ1 - caseJ2);
}

int partieAlea(superMorpion supPartie, bool aAff);

int heuristicAlea10(superMorpion supPartie)
{
    int score = 0;
    for(int k = 0; k < 5; k++)
    {
        int res = partieAlea(supPartie, false);
        if(res != -10)
        {
            score = score + res;
        }
        
    }
    return score;
}

int heuristicAlea100(superMorpion supPartie)
{
    int score = 0;
    for(int k = 0; k < 50; k++)
    {
        int res = partieAlea(supPartie, false);
        if(res != -10)
        {
            score = score + res;
        }
        
    }
    return score;
}

chaine* prochainEtat(superMorpion supPartie)
{
    chaine* res = creer_vide();
    if(supPartie.ouJoue == -1)
    {
        for(int i = 0; i < 9; i++)
        {
            if(supPartie.supPlat[i].gagnant == 0)
            {
                for(int j = 0; j < 9; j++)
                {
                    if(supPartie.supPlat[i].plat[j] == 0)
                    {
                        superMorpion new = supPartie;
                        new.supPlat[i].plat[j] = new.tour;
                        new.tour = new.tour * (-1);
                        majGagnant(&new.supPlat[i]);
                        if(new.supPlat[j].gagnant == 0)
                        {
                            new.ouJoue = j;
                        }
                        else
                        {
                            new.ouJoue = -1;
                        }
                        

                        addMaillon(res, new, i, j);
                    }
                }
            }
          
        }
    }
    else
    {
        int i = supPartie.ouJoue;
        if(supPartie.supPlat[i].gagnant == 0)
        {
            for(int j = 0; j < 9; j++)
            {
                if(supPartie.supPlat[i].plat[j] == 0)
                {
                    superMorpion new = supPartie;
                    new.supPlat[i].plat[j] = supPartie.tour;
                    new.tour = new.tour * (-1);
                    majGagnant(&new.supPlat[i]);
                    if(new.supPlat[j].gagnant == 0)
                    {
                        new.ouJoue = j;
                    }
                    else
                    {
                        new.ouJoue = -1;
                    }
                    
                    addMaillon(res, new, i, j);
                }
            }
        }
    }
    return res;
}

coupMinMax min(chaine* nextState, int profondeurActuel, int profondeurMax, int currentMinMax, int (*heur)(superMorpion), bool aff);
coupMinMax max(chaine* nextState, int profondeurActuel, int profondeurMax, int currentMinMax, int (*heur)(superMorpion), bool aff);
coupMinMax min_max(superMorpion supPartie, int profondeurActuel, int profondeurMax, int currentMinMax, int (*heur)(superMorpion), bool aff);

coupMinMax max(chaine* nextState, int profondeurActuel, int profondeurMax, int currentMinMax, int (*heur)(superMorpion), bool aff)
{
    coupMinMax res;
    res.move.grand = 0;
    res.move.petit = 0;
    res.heuristic = INT_MIN;
    maillon* current = nextState->first;
    if(current == NULL)
    {
        printf("Max vide\n");
    }

    if(aff)
    {
        for(int i = 0; i < profondeurActuel; i++)
        {
            printf(" - ");
        }
        printf("Max\n");
    }
  
    while(current != NULL)
    {
        bool aAff = aff;
        //bool aAff = current->val.supPlat[2].plat[8] == 1 && current->val.supPlat[8].plat[6] == -1;
        int minMax = min_max(current->val, profondeurActuel +1, profondeurMax, res.heuristic, (*heur), aAff).heuristic;

        if(aff)
        {
            
            for(int i = 0; i < profondeurActuel; i++)
            {
                printf(" - ");
            }
            printf("max : %d\n", minMax);
            //print_supMorpion(current->val)
        }

        if(minMax >= res.heuristic)
        {
            res.heuristic = minMax;
            res.move.grand = current->grand;
            res.move.petit = current->petit;
            
        }
        if(res.heuristic > currentMinMax)
        {
            break;
        }
        current = current->next;
    }
    if(aff)
    {
        for(int i = 0; i < profondeurActuel; i++)
        {
            printf(" - ");
        }
        printf("Fin max : %d\n", res.heuristic);
    }
  
    
    return res;
}

coupMinMax min(chaine* nextState, int profondeurActuel, int profondeurMax, int currentMinMax, int (*heur)(superMorpion), bool aff)
{
    coupMinMax res;
    res.move.grand = 0;
    res.move.petit = 0;
    res.heuristic = INT_MAX;
    maillon* current = nextState->first;

    if(current == NULL)
    {
        printf("Min vide\n");
    }

    if(aff)
    {
        for(int i = 0; i < profondeurActuel; i++)
        {
            printf(" - ");
        }
        printf("Min\n");
    }

    while(current != NULL)
    {
        bool aAff = aff;
        //bool aAff = current->val.supPlat[2].plat[8] == 1 && current->val.supPlat[8].plat[6] == -1;
        int minMax = min_max(current->val, profondeurActuel +1, profondeurMax, res.heuristic, (*heur), aAff).heuristic;
        if(aff)
        {
            for(int i = 0; i < profondeurActuel; i++)
            {
                printf(" - ");
            }
            printf("min : %d\n", minMax);
            //print_supMorpion(current->val);
        }
        if(minMax <= res.heuristic)
        {
            res.heuristic = minMax;
            res.move.grand = current->grand;
            res.move.petit = current->petit;
         
        }
        if(res.heuristic < currentMinMax)
        {
            break;
        }
        current = current->next;
    }
    if(aff)
    {
        for(int i = 0; i < profondeurActuel; i++)
        {
            printf(" - ");
        }
        printf("Fin min : %d\n", res.heuristic);
    }

    return res;
}

coupMinMax min_max(superMorpion supPartie, int profondeurActuel, int profondeurMax, int currentMinMax, int (*heur)(superMorpion), bool aff)
{
    coupMinMax res;
    if(supPartie.tour == 1)
    {
        if(aGagnerSuperMorpion(supPartie) != 0)
        {
            res.move.petit = 0;
            res.move.grand = 0;
            res.heuristic = INT_MIN;
        }
        else if(profondeurActuel == profondeurMax)
        {
            res.move.petit = 0;
            res.move.grand = 0;
            res.heuristic = (*heur)(supPartie);
        }
        else
        {
            chaine* c = prochainEtat(supPartie);
            res = max(c, profondeurActuel, profondeurMax, currentMinMax, (*heur), aff);
            freeChaine(c);
        }

    }
    else
    {
        if(aGagnerSuperMorpion(supPartie) != 0)
        {
            res.move.petit = 0;
            res.move.grand = 0;
            res.heuristic = INT_MAX;
        }
        else if(profondeurActuel == profondeurMax)
        {
            res.move.petit = 0;
            res.move.grand = 0;
            res.heuristic = (*heur)(supPartie);
        }
        else
        {
            chaine* c = prochainEtat(supPartie);
            res = min(c, profondeurActuel, profondeurMax, currentMinMax, (*heur), aff);
            freeChaine(c);
        }
    }


    if(res.move.grand == -1 || res.move.petit == -1)
    {
        printf("Slt\n");
    }
    return res;
}





//------------------------Simulation---------------------------------
superMorpion initiale() 
{
    superMorpion supPartie;
    for(int i = 0; i < 9; i++)
    {
        supPartie.supPlat[i].gagnant = 0;
        for(int j = 0; j < 9; j++)
        {
            supPartie.supPlat[i].plat[j] = 0;
        }
    }
    supPartie.tour = 1;
    supPartie.gagnant = 0;
    supPartie.ouJoue = -1;
    return supPartie;
}

int partieAlea(superMorpion supPartie, bool aAff)
{
    
    
    if(aAff)
    {
        print_supMorpion(supPartie);
        printf("\n\n");
    }
    
   

    while(supPartie.gagnant == 0)
    {
        coup nextCoup;
        if(supPartie.tour == 1)
        {
            nextCoup = strategie_alea(supPartie);
        }
        else
        {
            nextCoup = strategie_alea(supPartie);
        }
        supPartie.supPlat[nextCoup.grand].plat[nextCoup.petit] = supPartie.tour;
        majGagnant(&supPartie.supPlat[nextCoup.grand]);

        supPartie.gagnant = aGagnerSuperMorpion(supPartie);

        if(supPartie.supPlat[nextCoup.petit].gagnant == 0)
        {
            supPartie.ouJoue = nextCoup.petit;
        }
        else
        {
            supPartie.ouJoue = -1;
        }
        supPartie.tour = supPartie.tour * (-1);
        if(aAff)
        {
            
            print_supMorpion(supPartie);
            printf("\n\n");
        }
    }

    return supPartie.gagnant;


}

int partie(superMorpion supPartie, bool aAff)
{
    
    
    if(aAff)
    {
        print_supMorpion(supPartie);
        printf("\n\n");
    }
    
   

    while(supPartie.gagnant == 0)
    {
        coup nextCoup;
        if(supPartie.tour == 1)
        {
            nextCoup =  min_max(supPartie, 0, 4, INT_MAX, heuristic, false).move;
            //nextCoup = strategie_alea(supPartie);
            //nextCoup = strategie_utilisateur(supPartie);
        }
        else
        {
            nextCoup =  min_max(supPartie, 0, 4, INT_MIN, heuristicSuperMorpion, false).move;
            //printf("Heuristic : %d\n", heuristicSuperMorpion(supPartie));
            //nextCoup = strategie_alea(supPartie);
            //nextCoup = strategie_utilisateur(supPartie);
        }
        supPartie.supPlat[nextCoup.grand].plat[nextCoup.petit] = supPartie.tour;
        majGagnant(&supPartie.supPlat[nextCoup.grand]);

        supPartie.gagnant = aGagnerSuperMorpion(supPartie);

        if(supPartie.supPlat[nextCoup.petit].gagnant == 0)
        {
            supPartie.ouJoue = nextCoup.petit;
        }
        else
        {
            supPartie.ouJoue = -1;
        }
        supPartie.tour = supPartie.tour * (-1);
        if(aAff)
        {
            int att;
            print_supMorpion(supPartie);
            printf("\n\n");
            //scanf("%d", &att);
        }
    }

    return supPartie.gagnant;


}

void parties(int nb_parties, bool aAff)
{
    int j1 = 0;
    int j2 = 0;
    int nulle = 0;
    for(int i = 0; i < nb_parties; i++)
    {
        int res = partie(initiale(),aAff);
        if(res == 1)
        {
            j1++;
        }
        else if(res == -1)
        {
            j2++;
        }
        else
        {
            nulle++;
        }
    }

    printf("j1 : %d\nj2 : %d\nnulle : %d\n", j1, j2, nulle);
}

void print_prochain(superMorpion supPartie)
{
    chaine* next = prochainEtat(supPartie);
    maillon* current = next->first;
    while(current != NULL)
    {
        printf("%d\n", current->val.supPlat[0].plat[0]);
        print_supMorpion(current->val);
        printf("\n\n");
        current = current->next;
    }
}

int main()
{
    float temps;
    clock_t t1, t2;

    t1 = clock();

    srand(time(NULL));
    parties(1, true);

    t2 = clock();
    temps = (float)(t2-t1)/CLOCKS_PER_SEC;
    printf("temps = %f\n", temps);

}
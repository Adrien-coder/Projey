#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void Ouverture(int *valeurs)
{
    FILE *fichier = fopen("/home/adrien/Prog_C/Projey/setup.txt", "r");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
    }
    else
    {

        int i = 0;

        while (fscanf(fichier, "%d", &valeurs[i]) == 1)
        {
            i++;
        }

        fclose(fichier);

        // Affichage des valeurs (pour vérification)
        for (int j = 0; j < i; j++)
        {
            printf("%d ", valeurs[j]);
        }
        printf("\n");
    }
}

int isInDoubles(int *doubles, int i, int val)
{
    for (int j = 0; j < i; j++)
    {
        if (doubles[j] == val)
        {
            return 1;
        }
    }
    return 0;
}

void Generation(int *valeurs, int **reseau)
{
    int doubles_L[valeurs[3]];
    int doubles_C[valeurs[4]];

    for (int i = 0; i < valeurs[3]; i++)
    {
        /*Création des routes horizontales*/
        int ligne = rand() % valeurs[0];
        while (isInDoubles(doubles_L, i, ligne))
        {
            ligne = rand() % valeurs[0];
        }
        doubles_L[i] = ligne;
        for (int j = 0; j < valeurs[1]; j++)
        {
            reseau[ligne][j] = 1;
        }
    }

    for (int j = 0; j < valeurs[4]; j++)
    {
        /*Création des routes verticales*/
        int colonne = rand() % valeurs[1];
        while (isInDoubles(doubles_C, j, colonne))
        {
            colonne = rand() % valeurs[1];
        }
        doubles_C[j] = colonne;
        for (int i = 0; i < valeurs[0]; i++)
        {
            if (reseau[i][colonne] == 1)
            {
                reseau[i][colonne] = 5;
            }
            else
            {
                reseau[i][colonne] = 2;
            }
        }
    }

    for (int i = 0; i < valeurs[2]; i++)
    {
        /*Création des vehicules*/
        int ligne = rand() % valeurs[0];
        int colonne = rand() % valeurs[1];
        while (reseau[ligne][colonne] == 3 || reseau[ligne][colonne] < 1)
        {
            colonne = rand() % valeurs[1];
            ligne = rand() % valeurs[0];
        }
        if (reseau[ligne][colonne] == 1) // Si la voiture est sur un Ouest-Est
        {
            reseau[ligne][colonne] = 3;
        }
        if (reseau[ligne][colonne] == 2) // Si la voiture est sur un Nord-Sud
        {
            reseau[ligne][colonne] = 4;
        }
    }
}

void clearScreen()
{
    int n;
    for (n = 0; n < 10; n++)
        printf("\n\n\n\n\n\n\n\n\n\n");
}

void Affichage(int *valeurs, int **reseau)
{
    int lignes = valeurs[0];
    int colonnes = valeurs[1];

    clearScreen();
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            if (reseau[i][j] == 1)
            {
                printf("-");
            }
            else if (reseau[i][j] == 2)
            {
                printf("|");
            }
            else if (reseau[i][j] == 3 || reseau[i][j] == 4)
            {
                printf("*");
            }
            else if (reseau[i][j] == 5)
            {
                printf("+");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
    sleep(1);
}

int EvolutionLigne(int i, int *valeurs, int **reseau)
{
    int prec;
    for (int k = valeurs[1] - 1; k >= 0; k--)
    {
        if (reseau[i][k] == 3)
        {

            if (reseau[(i + 1) % valeurs[0]][k] != 0)
            {
                prec = 5;
            }
            else
            {
                prec = 1;
            }
            if (k + 1 < valeurs[1])
            {
                reseau[i][k] = prec;
                reseau[i][k + 1] = 3;
                return 1;
            }
            else
            {
                reseau[i][k] = prec;
                valeurs[2]--;
                return 1;
            }
        }
    }
    return (0);
}

int EvolutionColonne(int j, int *valeurs, int **reseau)
{
    int prec;
    for (int k = valeurs[0] - 1; k >= 0; k--)
    {
        if (reseau[k][j] == 4)
        {

            if (reseau[k][(j + 1) % valeurs[1]] != 0)
            {
                prec = 5;
            }
            else
            {
                prec = 2;
            }

            if (k + 1 < valeurs[0])
            {
                reseau[k][j] = prec;
                reseau[k + 1][j] = 4;
                return 1;
            }
            else
            {
                reseau[k][j] = prec;
                valeurs[2]--;
                return 1;
            }
        }
    }
    return (0);
}

int EvolutionSeq(int *valeurs, int **reseau)
{
    int evo = 0;
    int lignes = valeurs[0];
    int colonnes = valeurs[1];
    for (int i = 0; i < lignes; i++)
    {
        if (reseau[i][0] > 0)
        {
            evo = EvolutionLigne(i, valeurs, reseau);
            if (evo != 0)
            {
                return 1;
            }
        }
    }

    for (int j = 0; j < colonnes; j++)
    {
        if (reseau[0][j] > 0)
        {
            evo = EvolutionColonne(j, valeurs, reseau);
            if (evo != 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

int main()
{
    /*Définition des variables principales*/
    int valeurs[5];
    Ouverture(valeurs);
    int lignes = valeurs[0];
    int colonnes = valeurs[1];

    srand(time(NULL));

    // Création du tableau dynamique
    int **reseau = (int **)malloc(lignes * sizeof(int *));
    for (int i = 0; i < lignes; i++)
    {
        reseau[i] = (int *)malloc(colonnes * sizeof(int));
    }
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            reseau[i][j] = 0;
        }
    }

    Generation(valeurs, reseau);
    Affichage(valeurs, reseau);
    int evo = 1;
    while (evo != 0)
    {
        evo = EvolutionSeq(valeurs, reseau);
        Affichage(valeurs, reseau);
    }
    return 0;
}
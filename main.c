#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t dmutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct
{
    int **reseau;
    int *valeurs;
} plateau;

void Ouverture(plateau *p)
{
    FILE *fichier = fopen("/home/adrien/Prog_C/Projey/setup.txt", "r");
    if (fichier == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
    }
    else
    {
        int nb_valeurs = 0;
        // ... (code pour compter le nombre de valeurs)

        // Allouer de la mémoire pour le tableau de valeurs
        p->valeurs = (int *)malloc(nb_valeurs * sizeof(int));
        if (p->valeurs == NULL)
        {
            printf("Erreur d'allocation mémoire.\n");
            return;
        }

        // Lire les valeurs dans le tableau
        int i = 0;
        while (fscanf(fichier, "%d", &p->valeurs[i]) == 1)
        {
            i++;
        }

        fclose(fichier);

        // ... (affichage des valeurs)
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

void Generation(plateau p)
{
    int doubles_L[p.valeurs[3]];
    int doubles_C[p.valeurs[4]];

    for (int i = 0; i < p.valeurs[3]; i++)
    {
        /*Création des routes horizontales*/
        int ligne = (rand() % (p.valeurs[0] - 2)) + 1;
        while (isInDoubles(doubles_L, i, ligne)
        ||p.reseau[(p.valeurs[0]+ligne+1)%p.valeurs[0]][0]==1
        ||p.reseau[(p.valeurs[0]+ligne+2)%p.valeurs[0]][0]==1
        ||p.reseau[(p.valeurs[0]+ligne-1)%p.valeurs[0]][0]==1
        ||p.reseau[(p.valeurs[0]+ligne-2)%p.valeurs[0]][0]==1
        )
        {
            ligne = (rand() % (p.valeurs[0] - 2)) + 1;;
        }
        doubles_L[i] = ligne;
        for (int j = 0; j < p.valeurs[1]; j++)
        {
            p.reseau[ligne][j] = 1;
        }
    }

    for (int j = 0; j < p.valeurs[4]; j++)
    {
        /*Création des routes verticales*/
        int colonne = (rand() % (p.valeurs[1] - 2)) + 1;
        while (isInDoubles(doubles_C, j, colonne)
        ||p.reseau[0][(p.valeurs[1]+colonne+1)%p.valeurs[1]]==2
        ||p.reseau[0][(p.valeurs[1]+colonne+2)%p.valeurs[1]]==2
        ||p.reseau[0][(p.valeurs[1]+colonne-1)%p.valeurs[1]]==2
        ||p.reseau[0][(p.valeurs[1]+colonne-2)%p.valeurs[1]]==2)
        {
            colonne = (rand() % (p.valeurs[1] - 2)) + 1;
        }
        doubles_C[j] = colonne;
        for (int i = 0; i < p.valeurs[0]; i++)
        {
            if (p.reseau[i][colonne] == 1)
            {
                p.reseau[i][colonne] = 5;
                p.reseau[i - 1][colonne - 1] = 51;
                p.reseau[i + 1][colonne + 1] = 52;
            }
            else
            {
                p.reseau[i][colonne] = 2;
            }
        }
    }
}

void Generation_voiture(plateau p)
{
    for (int i = 0; i < p.valeurs[2]; i++)
    {
        /*Création des vehicules*/
        int ligne = rand() % p.valeurs[0];
        int colonne = rand() % p.valeurs[1];
        while (p.reseau[ligne][colonne] > 2 || p.reseau[ligne][colonne] < 1)
        {
            colonne = rand() % p.valeurs[1];
            ligne = rand() % p.valeurs[0];
        }
        if (p.reseau[ligne][colonne] == 1) // Si la voiture est sur un Ouest-Est
        {
            p.reseau[ligne][colonne] = 3;
        }
        if (p.reseau[ligne][colonne] == 2) // Si la voiture est sur un Nord-Sud
        {
            p.reseau[ligne][colonne] = 4;
        }
    }
}

void Generation_voiture_thread(plateau *p, int *voiture)
{
    /*Création des vehicules*/
    int ligne = rand() % p->valeurs[0];
    int colonne = rand() % p->valeurs[1];
    while (p->reseau[ligne][colonne] > 2 || p->reseau[ligne][colonne] < 1)
    {
        colonne = rand() % p->valeurs[1];
        ligne = rand() % p->valeurs[0];
    }
    if (p->reseau[ligne][colonne] == 1) // Si la voiture est sur un Ouest-Est
    {
        p->reseau[ligne][colonne] = 3;
    }
    if (p->reseau[ligne][colonne] == 2) // Si la voiture est sur un Nord-Sud
    {
        p->reseau[ligne][colonne] = 4;
    }
    voiture[0] = ligne;
    voiture[1] = colonne;
    voiture[2] = p->reseau[ligne][colonne];
}

void clearScreen()
{
    int n;
    for (n = 0; n < 10; n++)
        printf("\n\n\n\n\n\n\n\n\n\n");
}

void Affichage(plateau p)
{
    int lignes = p.valeurs[0];
    int colonnes = p.valeurs[1];

    clearScreen();
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            if (p.reseau[i][j] == 1)
            {
                printf("-");
            }
            else if (p.reseau[i][j] == 2)
            {
                printf("|");
            }
            else if (p.reseau[i][j] == 3)
            {
                printf("*");
            }
            else if (p.reseau[i][j] == 4)
            {
                printf("*");
            }
            else if (p.reseau[i][j] == 5)
            {
                printf("+");
            }
            else if (p.reseau[i][j] == 6)
            {
                printf("+");
            }
            else if (p.reseau[i][j] == 51)
            {
                printf("V");//V
            }
            else if (p.reseau[i][j] == 62)
            {
                printf("V");//V
            }
            else if (p.reseau[i][j] == 52)
            {
                printf("R");//R
            }
            else if (p.reseau[i][j] == 61)
            {
                printf("R");//R
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n\n\n");
}

int EvolutionLigne(int i, plateau p)
{
    int prec;
    for (int k = p.valeurs[1] - 1; k >= 0; k--)
    {
        if (p.reseau[i][k] == 3)
        {

            if (p.reseau[i + 1][k] == 0 || p.reseau[i + 1][k] > 10)
            {
                prec = 1;
            }
            else
            {
                prec = 5;
            }
            if (k + 1 < p.valeurs[1])
            {
                p.reseau[i][k] = prec;
                p.reseau[i][k + 1] = 3;
                return 1;
            }
            else
            {
                p.reseau[i][k] = prec;
                p.valeurs[2]--;
                return 1;
            }
        }
    }
    return (0);
}

int EvolutionColonne(int j, plateau p)
{
    int prec;
    for (int k = p.valeurs[0] - 1; k >= 0; k--)
    {
        if (p.reseau[k][j] == 4)
        {

            if (p.reseau[k][j + 1] == 0 || p.reseau[k][j + 1] > 10)
            {
                prec = 2;
            }
            else
            {
                prec = 6;
            }

            if (k + 1 < p.valeurs[0])
            {
                p.reseau[k][j] = prec;
                p.reseau[k + 1][j] = 4;
                return 1;
            }
            else
            {
                p.reseau[k][j] = prec;
                p.valeurs[2]--;
                return 1;
            }
        }
    }
    return (0);
}
int changeFeu(plateau p)
{
    int evo = 0;
    for (int i = 0; i < p.valeurs[0]; i++)
        for (int j = 0; j < p.valeurs[1]; j++)
        {
            if (p.reseau[i][j] == 5)
            {
                p.reseau[i][j] = 6;
                evo = 1;
            }
            else if (p.reseau[i][j] == 51)
            {
                p.reseau[i][j] = 61;
                evo = 1;
            }
            else if (p.reseau[i][j] == 52)
            {
                p.reseau[i][j] = 62;
                evo = 1;
            }
        }
    return evo;
}
int EvolutionSeq(plateau p)
{
    int evo = 0;
    int lignes = p.valeurs[0];
    int colonnes = p.valeurs[1];
    for (int i = 0; i < lignes; i++)
    {
        if (p.reseau[i][0] > 0 && p.reseau[i][0] < 10)
        {
            evo = EvolutionLigne(i, p);
            if (evo != 0)
            {
                return 1;
            }
        }
    }
    evo = changeFeu(p);
    if (evo != 0)
    {
        return 1;
    }

    for (int j = 0; j < colonnes; j++)
    {
        if (p.reseau[0][j] > 0 && p.reseau[0][j] < 10)
        {
            evo = EvolutionColonne(j, p);
            if (evo != 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

void GenerationThreads(plateau p, int **ListeVehicules)
{
    int cpt = 0;
    for (int i = 0; i < p.valeurs[0]; i++)
    {
        for (int j = 0; j < p.valeurs[1]; j++)
        {
            if (p.reseau[i][j] == 3 || p.reseau[i][j] == 4)
            {
                ListeVehicules[cpt][0] = i;
                ListeVehicules[cpt][1] = j;
                ListeVehicules[cpt][2] = p.reseau[i][j];
                cpt++;
            }
        }
    }
}

void *sequ_feux(void *arg)
{
    plateau *p = (plateau *)arg;
    pthread_mutex_lock(&dmutex);
    int condition=1;
    pthread_mutex_unlock(&dmutex);
    while (condition)
    {
        sleep(5);
        pthread_mutex_lock(&dmutex);
        for (int i = 0; i < p->valeurs[0]; i++)
        {
            for (int j = 0; j < p->valeurs[1]; j++)
            {
                if (p->reseau[i][j] == 5)
                {
                    p->reseau[i][j] = 6;
                }
                else if (p->reseau[i][j] == 51)
                {
                    p->reseau[i][j] = 61;
                }
                else if (p->reseau[i][j] == 52)
                {
                    p->reseau[i][j] = 62;
                }
                else if (p->reseau[i][j] == 6)
                {
                    p->reseau[i][j] = 5;
                }
                else if (p->reseau[i][j] == 61)
                {
                    p->reseau[i][j] = 51;
                }
                else if (p->reseau[i][j] == 62)
                {
                    p->reseau[i][j] = 52;
                }
            }
        }
        Affichage(*p);
        condition=p->valeurs[2];
        pthread_mutex_unlock(&dmutex);
    }
}

int evo_voiture(plateau p, int *voiture)
{
    if (voiture[2] == 3) // La voiture est sur un axe horizontal
    {
        if (voiture[1] >= (p.valeurs[1] - 1)) // La voiture est arrivée au bout
        {
            p.reseau[voiture[0]][voiture[1]] = 1;
            return (0);
        }
        else
        {
            if (p.reseau[voiture[0] + 1][voiture[1] + 1] == 52)
            { // La voiture est sur une intersection
                p.reseau[voiture[0]][voiture[1]] = 5;
            }
            else if (p.reseau[voiture[0] + 1][voiture[1] + 1] == 62)
            { // La voiture est sur une intersection
                p.reseau[voiture[0]][voiture[1]] = 6;
            }
            else if (p.reseau[voiture[0]][voiture[1] + 1] == 6)
            {
                return (1);
            }
            else if (p.reseau[voiture[0]][voiture[1]+1] == 3)
            {
                return (1);
            }
            else if (p.reseau[voiture[0]][voiture[1]+1] == 4)
            {
                return (1);
            }
            else
            {
                p.reseau[voiture[0]][voiture[1]] = 1;
            }
            voiture[1]++;
            p.reseau[voiture[0]][voiture[1]] = 3;
            return (1);
        }
    }
    else if (voiture[2] == 4)
    {
        if (voiture[0] == p.valeurs[0] - 1)
        {
            p.reseau[voiture[0]][voiture[1]] = 2;
            return (0);
        }
        else
        {
            if (p.reseau[voiture[0] + 1][voiture[1] + 1] == 62)
            {
                p.reseau[voiture[0]][voiture[1]] = 6;
            }
            else if(p.reseau[voiture[0] + 1][voiture[1] + 1] == 52){
                p.reseau[voiture[0]][voiture[1]] = 5;
            }
            else if (p.reseau[voiture[0] + 1][voiture[1]] == 5)
            {
                return (1);
            }
            else if (p.reseau[voiture[0] + 1][voiture[1]] == 3)
            {
                return (1);
            }
            else if (p.reseau[voiture[0] + 1][voiture[1]] == 4)
            {
                return (1);
            }
            else
            {
                p.reseau[voiture[0]][voiture[1]] = 2;
            }
            voiture[0]++;
            p.reseau[voiture[0]][voiture[1]] = 4;
            return (1);
        }
    }
    return (-1);
}

void *create_voiture(void *arg)
{
    plateau *p = (plateau *)arg;
    int voiture[3];
    pthread_mutex_lock(&dmutex);
    Generation_voiture_thread(p, voiture);
    pthread_mutex_unlock(&dmutex);
    int evo = 1;
    while (evo)
    {
        pthread_mutex_lock(&dmutex);
        evo = evo_voiture(*p, voiture);
        Affichage(*p);
        pthread_mutex_unlock(&dmutex);
        usleep(1000*1000);
    }
    pthread_mutex_lock(&dmutex);
    p->valeurs[2]--;
    pthread_mutex_unlock(&dmutex);
}

int main()
{
    /*Définition des variables principales*/
    plateau p;
    Ouverture(&p);
    int lignes = p.valeurs[0];
    int colonnes = p.valeurs[1];

    srand(time(NULL));

    // Création du tableau dynamique
    p.reseau = (int **)malloc(lignes * sizeof(int *));
    for (int i = 0; i < lignes; i++)
    {
        p.reseau[i] = (int *)malloc(colonnes * sizeof(int));
    }
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            p.reseau[i][j] = 0;
        }
    }

    // Menu
    printf("Choisir le mode de simulation :\n 1 - Mode 1 : version sequentielle\n 2 - Mode 2 : version threads\n");
    int menu;
    scanf("%d", &menu);
    if (menu == 1)
    {

        Generation(p);
        Generation_voiture(p);
        Affichage(p);
        int evo = 1;
        while (evo != 0)
        {
            evo = EvolutionSeq(p);
            Affichage(p);
            sleep(1);
        }
    }
    else if (menu == 2)
    {
        Generation(p);
        Affichage(p);
        pthread_t feux;
        pthread_t voiture[p.valeurs[2]];
        pthread_create(&feux, NULL, sequ_feux, &p);
        int nb_voiture=p.valeurs[2];
        for (int i = 0; i < nb_voiture; i++)
        {
            pthread_create(&voiture[i], NULL, create_voiture, &p);
            sleep(1);
        }
        for (int i = 0; i < nb_voiture; i++)
        {
            pthread_join(voiture[i], NULL);
        }
        pthread_join(feux, NULL);
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "coureur.h"
#include "Liste.h"


struct Liste_coureur* initialiser_Liste_coureur()
{
    struct Liste_coureur* liste = malloc(sizeof(struct Liste_coureur));
    liste->coureur_1er = NULL;
    liste->coureur_fin = NULL;
    return liste;
}




int main()
{
    struct Liste_coureur* liste = initialiser_Liste_coureur();
    
    struct cellule_coureur* courant = malloc(sizeof(struct cellule_coureur));
        
    errno_t err;
    FILE* f;
    err = fopen_s(&f,"C:\\Users\\Admin\\source\\repos\\TP LANGAGE C\\TP LANGAGE C\\fichier_coureurs.txt", "r"); // contenir adresse du fichier , en C , on remplace "\" = "\\" //
    int nombre_etapes;
    int nombre_equipes;

    //----------------------- Declarer un classement par equipe -------------------------//

        struct classe_equipe* classe_equipe = malloc(sizeof(struct classe_equipe));
        classe_equipe->equipe_1er = malloc(sizeof(struct cellule_equipe));
        classe_equipe->equipe_fin = malloc(sizeof(struct cellule_equipe));
        classe_equipe->equipe_fin = NULL;
        struct cellule_equipe* nom_�quipes = malloc(sizeof(struct cellule_equipe)); // pointeur parcouri dans le classemnt par equipe
        nom_�quipes = classe_equipe->equipe_1er;

    //-----------------------------------------------------------------------------------//

    //---------------------------------------------- Fonction de lire un fichier text ----------------------------------------------//

    if(f)
    {
        
      fscanf_s(f, "%10d\n", &nombre_etapes);
      fscanf_s(f, "%10d\n", &nombre_equipes);
        for (;;)
        {
          
            char nom_�quipe[50];

           
            fgets(nom_�quipe,50,f);
            printf("string nom equipe : %s \n", nom_�quipe);
            //fscanf_s(f, "%s \n", nom_�quipe, _countof(nom_�quipe));
            strcpy_s(nom_�quipes->equipe.nom,50, nom_�quipe);
            //printf("count of nom equipe :  %d ", _countof(nom_�quipe));
            nom_�quipes->equipe.temps_cumul� = 0;
            nom_�quipes->equipe.nombre_meilleur_coeur = 0;
            nom_�quipes->equipe.nombre_coureur_arriv� = 0;
            nom_�quipes->equipe_suivant = malloc(sizeof(struct cellule_equipe));
            
            printf("%s\n", nom_�quipe);

            for (int i = 0; i < 5; i++)
            {
                    int j = 0; 
                    char Nom[50];
                    char Pr�nom[50];
                    int  dossard ;
                    char virgule[2];
                    fscanf_s(f, "%d ", &dossard);
                    virgule[0] = fgetc(f);
                    
                    /*fgets(tout_infor, 100, f);
                    printf("tout infor %s", tout_infor);
                    fgets(tout_infor, 100, f);
                    printf("tout infor %s", tout_infor);
                    */

                    Nom[0] = fgetc(f);
                    while (Nom[j] != ',')
                        {
                            j++; 
                            Nom[j] = fgetc(f);

                           // printf("get char %c      ", Nom[j]);
                            
                        }
                    Nom[j] = '\0';
                   
                    //virgule[1] = fgetc(f);

                    fgets(Pr�nom, 20, f);

                    //printf("nom %s \n", Nom);
                    //printf("nom %s \n",Pr�nom);
                    //printf("%d \n", dossard);

                   
                    //fscanf_s(f, "%s ", Nom ,_countof(Nom));
                    //fscanf_s(f, "%s \n",Pr�nom ,_countof(Pr�nom));
                    struct coureur* coureur = Creer_Coureur(Nom, Pr�nom, dossard, nom_�quipe);
                    printf(" %d       %s   %s \n", dossard , Nom , Pr�nom);                                    
                    ajouter_coureur_en_fin(liste, *coureur);

                }

            if (feof(f))
              {
                 nom_�quipes->equipe_suivant = classe_equipe->equipe_fin;
                 break ;
              }  

            nom_�quipes = nom_�quipes->equipe_suivant;

        }          
     

     printf("Nombres d'etapes : %d \n", nombre_etapes);
     printf("Nombres d'equipes : %d \n", nombre_equipes);
     printf("\n\n         >>>>>>>>> START <<<<<<<<<<<< \n\n");
     printf("-----------------------------------------------------------------------");

    }

//------------------------------------------------------------------------------------------------------------------------------//
    
//---------------------------------------------EFFACER LES COUREURS DOPES ------------------------------------------------------//
    
    for (int i = 1; i <= nombre_etapes; i++)
    {
        printf("\n \n ---------------- COMMENCER %der ETAPES ------------------- \n \n", i);
        int j = 0; // nombre des coureurs //
        srand(time(NULL));
        courant = aller_d�but(liste);
        while (!courant_en_Fin(courant))
        {
            int temps = rand(); // donner le temps cumul� dans chaque �tapes //
            Ajouter_temps(&(courant->infor), temps);

            //------------------------- Sous_fonction pour effacer les coureurs dop� -------------------------//

                if (temps < 2000) // !!!!!! condition al�atoire du temps, lorsque le coureur est arriv� tr�s vite !!!!!//
                {

                    printf("     <!!!> VERIFIER LES INFORMATIONS SUR LE COUREURS SUSPECTE DE DOPAGE ... \n");
                    printf("\n XXXXXXXXXXXXXXXXXXX\n \n");
                    afficher_Coureur(courant);
                    printf("\n XXXXXXXXXXXXXXXXXXX\n");
                    printf(" \n >>>>>>> coureur %s %s    est DOPE ! HORS DE LA COURSE !!  \n",courant->infor.Nom, courant->infor.Pr�nom);
                    effacer_coureur_point�e(liste, courant); // effacer coureur dop� //
                    courant = avancer(courant);
                    j--; // nombre des coureurs - 1 
                }
                else courant = avancer(courant);

            //----------------------------------------------------------------------------------------------//

            j++; // nombre des coureurs + 1 
            
        }

        printf("\n \n ------ CLASSEMENT DES %d COUREURS APRES %der ETAPES -------- \n \n",j,i);
        liste = trier_liste(liste);
        afficher_liste_coureur(liste);
        printf("\n \n      ------ TERMINE LE CLASSEMENT DE %der ETAPES -------- \n \n",i);
        printf("-----------------------------------------------------------------------");


        
    }
    
//-----------------------------------------------------------------------------------------------------------------------------//



    //-------------------------le classement par equipe -------------------------//
    //-----------------------temps cumul� des 3 meilleurs------------------------//
    //--equipe ayant moins de 3 coureurs arriv�e apr�s n etapes ne peut classer--//
    
    
    courant = aller_d�but(liste);            // preparer pour parcourir //
    nom_�quipes = classe_equipe->equipe_1er; // preparer pour parcourir //

    while (!courant_en_Fin(courant))
    {
        nom_�quipes = classe_equipe->equipe_1er;
        
        while(nom_�quipes != NULL)
        {
            char nom1[50];
            strcpy_s(nom1,30, courant->infor.Equipe);
            char nom2[50];
            strcpy_s(nom2,30, nom_�quipes->equipe.nom);

            if (strcmp(nom1,nom2) == 0)
            {
                nom_�quipes->equipe.temps_cumul� += courant->infor.temps;
                nom_�quipes->equipe.nombre_meilleur_coeur ++;

                if (nom_�quipes->equipe.nombre_meilleur_coeur > 3) // condition pour compter seulement 3 meilleurs coureurs dans un �quipe //
                {
                    nom_�quipes->equipe.temps_cumul� -= courant->infor.temps;
                }
            } 
            
            if (strcmp(nom1, nom2) == 0)
            {
                nom_�quipes->equipe.nombre_coureur_arriv�++; // compter le nombre de coureurs arriv� apr�s le derni�re �tape //
            }    

            nom_�quipes = nom_�quipes->equipe_suivant;
        }
        courant = avancer(courant);
    }

    //--------------------- Programme pour verifier les �quipes accept� dans le classement par equipe -------------------------//

        nom_�quipes = classe_equipe->equipe_1er; // preparer pour reparcourir //

        while (nom_�quipes != NULL)
        {
            printf("\n   nombre de coureurs arrive de l'equipe %s : %d ", nom_�quipes->equipe.nom, nom_�quipes->equipe.nombre_coureur_arriv�);
            if (nom_�quipes->equipe.nombre_coureur_arriv� < 3)
            {
                printf("  >>>>>>>  Cet equipe ne peut classer dans le classement par equipe !!!");
                effacer_equipe_point�e(classe_equipe, nom_�quipes);
            }
            nom_�quipes = nom_�quipes->equipe_suivant;
        }
    //------------------------------------------------------------------------------------------------------------------------//

    classe_equipe = trier_classe_equipe(classe_equipe); // trier le classement d'equipe // 

    //--------------------- AFFICHER LE CLASSEMENT PAR EQUIPE ------------------------//

        nom_�quipes = classe_equipe->equipe_1er;

        printf("\n \n \n         ------- CLASSEMENT PAR EQUIPE DANS LE COURSE ---------\n \n");
        if (nom_�quipes == NULL) printf("                 [[[[[[TOUS EST PARTI]]]]]]]]]       ");
        int n = 0;

        while (nom_�quipes != NULL)
        {
            n++;
            printf("\n-------  %d er place de Classement par equipe dans le course  --------- \n",n);

            printf("                Nom de l'equipe : %s \n", nom_�quipes->equipe.nom);
            printf("      temps cumule par equipe :  %d h %d m %d s \n", (nom_�quipes->equipe.temps_cumul�) / 3600, ((nom_�quipes->equipe.temps_cumul�) % 3600) / 60, (((nom_�quipes->equipe.temps_cumul�) % 3600) % 60));
            nom_�quipes = nom_�quipes->equipe_suivant;
        }
    
    //------------------------------------------------------------------------------//
    
    free(liste); 
    free(courant);
    fclose(f);	


    return 0;
}





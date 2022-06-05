#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<errno.h>

//A structure that corresponds to the characteristics of a character
typedef struct{
	char nom[20];
	float valeur;
	int attaque;
	int defense;
	int pointsdevie;
	char techspecial[100];
	int attS;
	int tour;
	} Personnage;

//A structure that corresponds to a player and therefore to a team
typedef struct{
	Personnage tabperso[3];
	char nom[20];
}Joueur;

//A procedure that corresponds to the character's lifebar
void jauge(Personnage j){
    int i;
    printf("\x1B[31m");
    printf("Jauge de vie :");
    if (j.pointsdevie<=0){
        printf(" ");
    }
    else{
        for (i=0;i<j.pointsdevie;i++){
        printf(" *");
        }
    }
    printf("\n");
    printf("\x1B[34m");
}

//A function that allows to read the characteristics file of the characters
Personnage* lirefichier(int* nmbperso){
	Personnage* tab = NULL;
	FILE* fichier = NULL;
	int nmblettre;
	fichier = fopen("caracteristique.txt","r");
	if (fichier == NULL){
		printf("ouverture du fichier impossible \n");
		printf("code d'erreur = %d \n", errno);
		//printf("message d'erreur = %s \n",strerror(errno));
		exit(1);
	}
	char ligneperso[1000];
	while (fgets(ligneperso,999,fichier)!=NULL){
		*nmbperso = *nmbperso+1;
	}
	tab = malloc(*nmbperso * sizeof(Personnage));
	rewind(fichier);
	for (int i = 0; i < *nmbperso; i++){

		fscanf(fichier,"%s", tab[i].nom);
		//printf("%s", tab[i].nom);
		fscanf(fichier,"%f",&tab[i].valeur);
		fscanf(fichier,"%d",&tab[i].attaque);
		fscanf(fichier,"%d",&tab[i].defense);
		fscanf(fichier,"%d",&tab[i].pointsdevie);
		fscanf(fichier,"%s", tab[i].techspecial);
		fscanf(fichier,"%d",&tab[i].attS);
		tab[i].tour=5;

	}
	fclose(fichier);
	return tab;
}

//A procedure that displays the names of the characters in a table
void affichetabperso(Personnage tab[],int t){
	int i;
	for (i=0;i<t;i++){
		printf("%s\n",tab[i].nom );
	}
	printf("\n");
}
//A procedure that displays the values of the characters in a table
void affichetabval(Personnage tab[], int t){
	int i;
	printf("\n liste des personnages  \n");
	for (i=0;i<t;i++){
		printf(" %d : %s : valeur : %f | attaque : %d | défense : %d | points de vie : %d | tech.spécial : %s : %d \n",i+1, tab[i].nom, tab[i].valeur, tab[i].attaque, tab[i].defense, tab[i].pointsdevie, tab[i].techspecial, tab[i].attS);
	}
}

//A function that returns 1 if the player's team is alive, 0 otherwise
int equipe_en_vie(Joueur j){
	int i;
	for(i=0;i<3;i++){
		if (j.tabperso[i].pointsdevie <= 0){
			 return 1;
		}
		else {
			return 0;
		}
	}
}

//A procedure that corresponds to a round of play against the computer
void tour_combat_ordi(Joueur joueurA,Joueur joueurB){
	int a;
	int b;
	int i;
	printf("Joueur 1 : Si vous voulez faire une attaque simple, tapez 1 et si vous voulez faire une attaque spéciale, tapez 2\n");
	while ((equipe_en_vie(joueurA)==0)&&(equipe_en_vie(joueurB)==0)){
	for(i=0;i<3;i++){
        printf("\x1B[34m");
		printf("\n ==>>Bataille %s contre %s<<== \n C'est parti !\n", joueurA.tabperso[i].nom, joueurB.tabperso[i].nom);
		printf("\nJoueur 1, choisissez votre attaque (Tapez 1 ou 2)\n");
		scanf("%d", &a);
		while (a<1 || a>2){
			printf("erreur, Choisissez entre 1 et 2 \n");
			scanf("%d",&a);
		}
		if (a == 2){
			joueurB.tabperso[i].pointsdevie = joueurB.tabperso[i].pointsdevie - joueurA.tabperso[i].attS;
		}
		if (a == 1){
			joueurA.tabperso[i].attaque = joueurA.tabperso[i].attaque - 1;
			joueurB.tabperso[i].pointsdevie = joueurB.tabperso[i].pointsdevie - 1;
		}
		jauge(joueurB.tabperso[i]);
		printf("\n %s est à %d de points de vie\n", joueurB.tabperso[i].nom, joueurB.tabperso[i].pointsdevie);
		srand(time(NULL));
		b = rand()%2+3;
		printf("\nL'ordinateur a joué\n");
		//scanf("%d", &b);
		if (b == 4){
			joueurA.tabperso[i].pointsdevie = joueurA.tabperso[i].pointsdevie - joueurB.tabperso[i].attS;
		}
		if (b == 3){
			joueurB.tabperso[i].attaque = joueurB.tabperso[i].attaque - 1;
			joueurB.tabperso[i].pointsdevie = joueurB.tabperso[i].pointsdevie - 1;
		}
		jauge(joueurA.tabperso[i]);
		printf("\n %s est à %d de points de vie\n", joueurA.tabperso[i].nom, joueurA.tabperso[i].pointsdevie);
	}
	}
	if (equipe_en_vie(joueurA)==1){
        printf("\x1B[33m");
		printf("\nLe joueur 1 a gagné!\n");
	}
	else if (equipe_en_vie(joueurB)==1){
        printf("\x1B[33m");
		printf("\nLe joueur 2 a gagné!\n");
	}
}

//A procedure that corresponds to a round of play against another player
void tour_combat(Joueur joueurA, Joueur joueurB){
	int a;
	int b;
	int i;
	printf("Joueur 1 : Si vous voulez faire une attaque simple, tapez 1 et si vous voulez faire une attaque spéciale, tapez 2 \n Joueur B : Si vous voulez faire une attaque simple, tapez 3 et si vous voulez faire une attaque spéciale, tapez 4 \n");
	while ((equipe_en_vie(joueurA)==0)&&(equipe_en_vie(joueurB)==0)){
	for(i=0;i<3;i++){
        printf("\x1B[34m");
		printf("\n ==>>Bataille %s contre %s<<== \n C'est parti !\n", joueurA.tabperso[i].nom, joueurB.tabperso[i].nom);
		printf("\nJoueur 1, choisissez votre attaque (Tapez 1 ou 2)\n");
		scanf("%d", &a);
		while (a<1 || a>2){
			printf("erreur, Choisissez entre 1 et 2 \n");
			scanf("%d",&a);
		}
		if (a == 2){
			joueurB.tabperso[i].pointsdevie = joueurB.tabperso[i].pointsdevie - joueurA.tabperso[i].attS;
		}
		if (a == 1){
			joueurA.tabperso[i].attaque = joueurA.tabperso[i].attaque - 1;
			joueurB.tabperso[i].pointsdevie = joueurB.tabperso[i].pointsdevie - 1;
		}
		jauge(joueurB.tabperso[i]);
		printf("\n %s est à %d de points de vie\n", joueurB.tabperso[i].nom, joueurB.tabperso[i].pointsdevie);
		printf("\nJoueur 2, choisissez votre attaque (Tapez 3 ou 4)\n");
		scanf("%d", &b);
		while (b<3 || b>4){
			printf("erreur, Choisissez entre 3 et 4 \n");
			scanf("%d",&b);
		}
		if (b == 4){
			joueurA.tabperso[i].pointsdevie = joueurA.tabperso[i].pointsdevie - joueurB.tabperso[i].attS;
		}
		if (b == 3){
			joueurB.tabperso[i].attaque = joueurB.tabperso[i].attaque - 1;
			joueurB.tabperso[i].pointsdevie = joueurB.tabperso[i].pointsdevie - 1;
		}
		jauge(joueurA.tabperso[i]);
		printf("\n %s est à %d de points de vie\n", joueurA.tabperso[i].nom, joueurA.tabperso[i].pointsdevie);
	}
	}
	if (equipe_en_vie(joueurA)==1){
        printf("\x1B[33m");
		printf("\nLe joueur 1 a gagné!\n");
	}
	else if (equipe_en_vie(joueurB)==1){
        printf("\x1B[33m");
		printf("\nLe joueur 2 a gagné!\n");
	}
}

int main(){
	int nmbperso=0;
	char a;
	int choix;
	int y;
	Personnage p;
	Personnage persoJoueur1[3];
	Personnage persoJoueur2[3];
	Personnage* tabperso;
	Joueur joueur1;
	Joueur joueur2;
	tabperso=lirefichier(&nmbperso);
	printf("perso :%d", nmbperso);
	affichetabval(tabperso,nmbperso);
	printf("\n");
	printf("Premier joueur, Choisissez 3 personnages \n");
	printf("Choisissez le 1er personnage en fonction du numéro attribué\n");
	scanf("%d",&choix);
	while (choix<=0 || choix>7){
		printf("erreur, donnez un chiffre compris entre 1 et 7 \n");
		scanf("%d",&choix);
	}
	persoJoueur1[0]=tabperso[choix-1];
	joueur1.tabperso[0] = persoJoueur1[0];
	printf("Choisissez le 2e personnage en fonction du numéro attribué\n");
	scanf("%d",&choix);
	while (choix<=0 || choix>7){
		printf("erreur, donnez un chiffre compris entre 1 et 7 \n");
		scanf("%d",&choix);
	}
	persoJoueur1[1]=tabperso[choix-1];
	joueur1.tabperso[1] = persoJoueur1[1];
	printf("Choisissez le 3e personnage en fonction du numéro attribué\n");
	scanf("%d",&choix);
	while (choix<=0 || choix>7){
		printf("erreur, donnez un chiffre compris entre 1 et 7 \n");
		scanf("%d",&choix);
	}
	persoJoueur1[2]=tabperso[choix-1];
	joueur1.tabperso[2] = persoJoueur1[2];
	printf("\n");
	printf("Choisissez 3 personnages qui correspondront à l'équipe du 2e joueur \n");
	printf("Choisissez le 1er personnage en fonction du numéro attribué\n");
	scanf("%d",&choix);
	while (choix<=0 || choix>7){
		printf("erreur, donnez un chiffre compris entre 1 et 7 \n");
		scanf("%d",&choix);
	}
	persoJoueur2[0]=tabperso[choix-1];
	joueur2.tabperso[0] = persoJoueur2[0];
	printf("Choisissez le 2e personnage en fonction du numéro attribué\n");
	scanf("%d",&choix);
	while (choix<=0 || choix>7){
		printf("erreur, donnez un chiffre compris entre 1 et 7 \n");
		scanf("%d",&choix);
	}
	persoJoueur2[1]=tabperso[choix-1];
	joueur2.tabperso[1] = persoJoueur2[1];
	printf("Choisissez le 3e personnage en fonction du numéro attribué\n");
	scanf("%d",&choix);
	while (choix<=0 || choix>7){
		printf("erreur, donnez un chiffre compris entre 1 et 7 \n");
		scanf("%d",&choix);
	}
	persoJoueur2[2]=tabperso[choix-1];
	joueur2.tabperso[2] = persoJoueur2[2];
	printf("Membres de l'équipe 1:\n");
	affichetabperso(persoJoueur1,3);
	printf("Membres de l'équipe 2:\n");
	affichetabperso(persoJoueur2,3);
	printf("Voulez-vous jouer contre l'ordinateur (Tapez 5) ou avec un ami (Tapez 6)?\n");
	scanf("%d",&y);
	while (y<5 || y>6){
			printf("erreur, Choisissez entre 5 et 6 \n");
			scanf("%d",&y);
		}
	if (y==5){
		tour_combat_ordi(joueur1, joueur2);
	}
	else if (y==6){
		tour_combat( joueur1, joueur2);
	}
	return 0;
}


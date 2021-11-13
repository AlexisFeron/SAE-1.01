/*
  \file stock.c
  \author Alexis Feron / Clément Laporte
  \date 11/10/2021
  \brief Gère les stocks
*/
#include <stdio.h>
#include <string.h>
#include "stock.h"
#define TAILLE 10
#define TAILLEMOT 20

int fGlobale(void){
   int tnumProd[TAILLE], tQt[TAILLE], tSecu[TAILLE], tlog=0,choix, fRV;
   float tPrix[TAILLE];
   char tNom[TAILLE][TAILLEMOT];
   tlog=fchargement(tnumProd,tQt,tPrix,tSecu,TAILLE,tNom);
   if(tlog==-2){
      printf("Erreur : Tableaux trop petits\n");
      return -1;
   }
   if(tlog==-1){
      printf("Erreur : Ouverture fichier stock.txt\n");
      return -1;
   }
   choix=fMenu();
   while(choix!=0){
      if(choix==1){
	fEtatStock(tnumProd, tQt, tPrix, tSecu, tlog,tNom);
      }
      else if(choix==2){
         fTraitementCom(tnumProd,tQt,tPrix,tSecu,tlog);
      }
      else if(choix==3){
         fProdRupt(tnumProd,tQt,tSecu, tlog);
      }
      else if(choix==4){
         fRV=fRecapVente();
         if(fRV==-1){
            printf("Problème d'ouverture du fichier\n");
         }
      }
      else if(choix==5){
	tlog=fEnregAppro(tnumProd,tQt,tPrix,tSecu,tlog,tNom);
      }
      else{
         printf("Choix inconnu\n");
      }
      choix=fMenu();
   }
   fSauvegarde(tnumProd, tQt, tPrix, tSecu, tlog, tNom);
   return 0;
}

int fchargement(int tnumProd[],int tQt[],float tPrix[],int tSecu[],int tmax, char tNom[][TAILLEMOT]){
   FILE *fe;
   int numProd,qt,secu,tlog=0;
   char nom[TAILLEMOT];
   float prix;
   fe = fopen("stock.txt","r");
   if(fe==NULL){
      return -1;
   }
   fscanf(fe,"%d %s %d %f %d",&numProd,nom,&qt,&prix,&secu);
   while(feof(fe)==0){
      if(tlog==tmax){
         fclose(fe);
         return -2;
      }
      tnumProd[tlog]=numProd;
      tQt[tlog]=qt;
      tPrix[tlog]=prix;
      tSecu[tlog]=secu;
      strcpy(tNom[tlog],nom);
      tlog++;
      fscanf(fe,"%d %s %d %f %d",&numProd,&nom,&qt,&prix,&secu);
   }
   fclose(fe);
   return tlog;
}

int fMenu(void){
   int menu;
   printf("\n----------------------------------------------------\n");
   printf("Menu :");
   printf("\t1 - Affichage du stock\n");
   printf("\t2 - Traiter une commande (réalise un devis)\n");
   printf("\t3 - Produits en risque de rupture\n");
   printf("\t4 - Récapitulatif des ventes\n");
   printf("\t5 - Faire un approvisionnement\n");
   printf("\n\t0 - QUITTER\n\n");
   printf("Choix : ");
   scanf("%d", &menu);
   printf("----------------------------------------------------\n\n");
   return menu;
}

void fEtatStock(int tnumProd[],int tQt[],float tPrix[],int tSecu[],int tlog,char tNom[][TAILLEMOT]){
   int i,num,qt,secu;
   float prix;
   char nom[TAILLEMOT];
   printf("Références\tNom\t\tQuantité\tPrix unitaire\tSeuil de sécurité\n");
   for(i=0;i<tlog;i++){
      num=tnumProd[i];
      qt=tQt[i];
      prix=tPrix[i];
      secu=tSecu[i];
      strcpy(nom,tNom[i]);
      printf("%d\t\t%s\t%d\t\t%.2f\t\t%d\n", num, nom, qt, prix, secu);
   }
}

void fTraitementCom(int tnumProd[],int tQt[],float tPrix[],int tSecu[],int tlog){
   int tNProdCom[TAILLE],tQtCom[TAILLE],tlogCom=0, com;
   float tPrixCom[TAILLE];
   fSaisieCom(tnumProd,tQt,tPrix,tNProdCom,tQtCom,tPrixCom,tlog,&tlogCom);
   fDevis(tNProdCom,tQtCom,tPrixCom,tlogCom);
   printf("\n********************\n\n");
   printf("Voulez vous passer la commande ? (0=oui/1=non):");
   scanf("%d",&com);
   printf("\n");
   if(com==0){
      fMiseAJourStock(tQt,tnumProd,tlog,tNProdCom,tQtCom,tlogCom);
      fVenteEffectue(tNProdCom,tQtCom,tPrixCom,tlogCom);
      printf("La commande a bien été effectuée\n");
      fProdRupt(tnumProd,tQt,tSecu,tlog);
   }
   else{
      printf("La commande a été annulée\n");
   }
}

void fSaisieCom(int tnumProd[],int tQt[],float tPrix[],int tNProdCom[],int tQtCom[],float tPrixCom[],int tlog,int *tlogCom){
   int verif, qte, i, nProdu, conti=0;
   *tlogCom=0;
   while(conti==0){
      printf("Numéro du produit : ");
      scanf("%d",&nProdu);
      verif=fVerifProd(tnumProd,nProdu,tlog,&i);
      while (nProdu<=0 || verif==2){
         printf("Erreur, n° de produit incorrect, retaper : ");
         scanf("%d",&nProdu);
         verif=fVerifProd(tnumProd,nProdu,tlog,&i);
      }
      tNProdCom[*tlogCom]=nProdu;
      tPrixCom[*tlogCom]=tPrix[i];
      printf("Quantité: ");
      scanf("%d",&qte);
      verif=fVerifQt(qte,tQt,i);
      while(qte<=0 || verif==2){
         printf("Erreur, quantité incorrect, retaper: ");
         scanf("%d",&qte);
         verif=fVerifQt(qte,tQt,i);
      }
      tQtCom[*tlogCom]=qte;
      (*tlogCom)++;
      printf("Ajouter d'autres produits (0=oui 1=non): ");
      scanf("%d",&conti);
      printf("\n");
   }
}

int fVerifProd(int tnumProd[],int nProdu,int tlog,int* i){
   int j;
   for(j=0; j<tlog; j++){
      if (nProdu==tnumProd[j]){
         *i=j;
         return 1;
      }
   }
   return 2;
}

int fVerifQt(int qt,int tQt[], int pos){
   if(tQt[pos]>=qt){
      return 1;
   }
   return 2;
}

void fDevis(int tNProdCom[],int tQtCom[],float tPrixCom[],int tlogCom){
   int prodCom, qtCom, prixCom,i=0;
   float prix=0;
   printf("********************\n");
   printf("\nDEVIS :\n\n");
   printf("Produits\tQuantité\tPrix\tPrix total\n");
   while(i<tlogCom){
      prix=prix+(tQtCom[i]*tPrixCom[i]);
      printf("%d\t\t%d\t\t%.2f€\t%.2f€\n", tNProdCom[i],tQtCom[i],tPrixCom[i],tQtCom[i]*tPrixCom[i]);
      i++;
   }
   printf("\nPrix total :\t\t\t\t%.2f€\n", prix);
}

void fMiseAJourStock(int tQt[],int tnumProd[],int tlog,int tNProdCom[],int tQtCom[],int tlogCom){
   int k,pro,i,nProdu;
   for(k=0;k<tlogCom;k++){
      nProdu=tNProdCom[k];
      pro=fVerifProd(tnumProd,nProdu,tlog,&i);
      tQt[i]=tQt[i]-tQtCom[k];
   }
}

int fVenteEffectue(int tNProdCom[],int tQtCom[],float tPrixCom[],int tlogCom){
   FILE *fe;
   int qt,i, numC;
   fe=fopen("vente.txt","r");
   if(fe==NULL) return -1;
   while(feof(fe)==0){
      fscanf(fe,"%d %*d %*d %*f %*f",&numC);
   }
   fclose(fe);
   FILE *fs;
   fs=fopen("vente.txt","a");
   if(fs==NULL) return -1;
   for(i=0;i<tlogCom;i++){
      fprintf(fs,"%d\t%d\t%d\t%.2f\t%.2f\n",numC+1,tNProdCom[i],tQtCom[i],tPrixCom[i],tPrixCom[i]*tQtCom[i]);
   }
   fclose(fs);
}

void fProdRupt(int tnumProd[],int tQt[],int tSecu[], int tlog){
   int rupt=0,i=0;
   while(i<tlog){
      if(tQt[i]<=tSecu[i]){
         printf("Attention, le produit n°%d est en risque de rupture (%d en stock sur %d produits minimums)\n", tnumProd[i],tQt[i], tSecu[i]);
         rupt++;
      }
      i++;
   }
   if(rupt==0){
      printf("Bonne nouvelle, aucun produit en risque de rupture\n");
   }
}

int fRecapVente(void){
   FILE *fe;
   int numCom,numProd,qt, numComEnCour;
   float prixU,prixT,total=0;
   fe=fopen("vente.txt","r");
   if(fe==NULL){
      return -1;
   }
   fscanf(fe,"%d %d %d %f %f",&numCom,&numProd,&qt,&prixU,&prixT);
   numComEnCour=numCom;
   printf("N° de commande\t Référence du produit\t Quantité\t Prix unitaire\t Total\n");
   while(feof(fe)==0){
      printf("%d\t\t %d\t\t\t %d\t\t %.2f\t\t %.2f\n",numCom,numProd,qt,prixU,prixT);
      total=total + prixT;
      fscanf(fe,"%d %d %d %f %f",&numCom,&numProd,&qt,&prixU,&prixT);
      if(numComEnCour!=numCom){
         printf("\nTotal:\t\t\t\t\t\t\t\t\t %.2f\n",total);
         printf("********************\n");
         printf("N° de commande\t Référence du produit\t Quantité\t Prix unitaire\t Total\n");
         total=0;
         numComEnCour=numCom;
      }
   }
   printf("\nTotal:\t\t\t\t\t\t\t\t\t %.2f\n",total);
   return 0;
}

int fEnregAppro(int tnumProd[],int tQt[],float tPrix[],int tSecu[],int tlog,char tNom[][TAILLEMOT]){
   int nProdu, qte, secu,verif,i;
   float prix;
   char nom[TAILLEMOT];
   verif=fSaisieAppro(&nProdu,&qte,&prix,&secu,tnumProd,tlog,&i,nom);
   printf("\n********************\n");  
   if(verif==1){
      printf("\nLe produit existe déjà\n");
      tQt[i]+=qte;
      printf("Le stock a donc été mis à jour (vous avez ajoutez %d produits au stock)\n",qte);
      printf("Le stock du produit n°%d est donc dorénavant de %d produits\n", tnumProd[i],tQt[i]);
   }
   else{
      printf("\nLe produit n'existe pas et a donc été ajouté à notre stock :\n");
      printf("N° produit: %d\n",nProdu);
      printf("Nom %s\n",nom);
      printf("Quantité %d\n",qte);
      printf("Prix unitaire: %.2f\n",prix);
      printf("Seuil de sécurité: %d\n",secu);
      tnumProd[tlog]=nProdu;
      tQt[tlog]=qte;
      tPrix[tlog]=prix;
      tSecu[tlog]=secu;
      strcpy(tNom[tlog],nom);
      tlog++;
   }
   return tlog;
}

int fSaisieAppro(int* nProdu, int* qte, float* prix, int* secu,int tnumProd[],int tlog,int* i, char nom[]){
   int verif;
   printf("Numéro du produit : ");
   scanf("%d",nProdu);
   while (*nProdu<=0){
      printf("Erreur, n° de produit incorrect, retaper : ");
      scanf("%d",nProdu);
   }
   printf("Quantité: ");
   scanf("%d",qte);
   while(*qte<=0){
      printf("Erreur, quantité incorrecte(ne peut pas être inférieur ou égale à zero), retaper: ");
      scanf("%d",qte);
   }
   verif=fVerifProd(tnumProd,*nProdu,tlog,i);
   if(verif==1){
      return verif;
   }
   else{
      printf("Nom du produit (mettre des underscores'_' à la place des espaces): ");
      scanf("%s",nom);
      printf("Prix unitaire du produit: ");
      scanf("%f",prix);
      while(*prix<=0){
         printf("Erreur, prix unitaire du produit incorrect, retaper: ");
         scanf("%f",prix);
      }
      printf("Seuil de sécurité: ");
      scanf("%d",secu);
      while(*secu<=0){
         printf("Erreur, seuil de sécurité incorrect, retaper: ");
         scanf("%d",secu);
      }
      return verif;
   }
}

void fSauvegarde(int tnumProd[],int tQt[],float tPrix[],int tSecu[],int tlog, char tNom[][TAILLEMOT]){
   FILE *fs;
   int numProd,qt,secu,i;
   float prix;
   char nom[TAILLEMOT];
   fs=fopen("stock.txt","w");
   for(i=0;i<tlog;i++){
      numProd=tnumProd[i];
      qt=tQt[i];
      prix=tPrix[i];
      secu=tSecu[i];
      strcpy(nom,tNom[i]);
      fprintf(fs,"%d\t%s\t%d\t%.2f\t%d\n",numProd,nom,qt,prix,secu);
   }
   fclose(fs);
}

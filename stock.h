/**
   \file stock.h
   \author Alexis Feron / Clément Laporte
   \date 11/10/2021
   \brief Gestion de stock
*/

#define TAILLEMOT 20

/**
 \brief fGlobale appelle les autres fonctions
*/
int fGlobale(void);

/**
 \brief Charge les données dans des tableaux
 \param tnumProd tableau des numéros des produit
 \param tQt tableau des quantités des produits
 \param tPrix tableau des prix des produits
 \param tSecu tableau des seuils de sécurité des produits
 \param tmax taille physique des tableaux
 \param tNom tableau des noms de produits
 \return -1 problèmme d'ouverture, -2 tableaux trop petit, sinon renvoie la taille logique
*/
int fchargement(int tnumProd[],int tQt[],float tPrix[],int tSecu[],int tmax, char tNom[][TAILLEMOT]);

/**
 \brief affiche du menu
 \return le choix de l'utilisateur
*/
int fMenu(void);

/**
 \brief Affiche le stock grâce au fichier stock.txt
 \param tnumProd tableau des numéros des produit
 \param tQt tableau des quantités des produits
 \param tPrix tableau des prix des produits
 \param tSecu tableau des seuils de sécurité des produits
 \param tlog taille logique des tableaux
 \param tNom tableau des noms de produits
*/
void fEtatStock(int tnumProd[],int tQt[],float tPrix[],int tSecu[],int tlog, char tNom[][TAILLEMOT]);

/**
 \brief Réalise les étapes d'une commande (Choix du produit, quantité, devis, vente)
 \param tnumProd tableau des numéros de produit
 \param tQt tableau des quantités des produits
 \param tPrix tableau des prix des produits
 \param tSecu tableau des seuils de sécurité des produits
 \param tlog taille logique des tableaux
*/
void fTraitementCom(int tnumProd[],int tQt[],float tPrix[],int tSecu[],int tlog);

/**
 \brief Saisi de la commande et vérifie si le produit existe et qu'il y a la quantité 
 demandé avec fVerifProd et fVerifQt
 \param tnumProd tableau des numéros de produit
 \param tQt tableau des quantités des produits
 \param tPrix tableau des prix des produits
 \param tNProdCom tableau des numéros de produit pour les commandes
 \param tQtCom tableau des quantités des produits pour les commandes
 \param tPrixCom tableau des prix des produits pour les commandes
 \param tlog taille logique des tableaux
 \param tlogCom taille logique des tableau pour les commandes
*/
void fSaisieCom(int tnumProd[],int tQt[],float tPrix[],int tNProdCom[],int tQtCom[],float tPrixCom[],int tlog,int *tlogCom);

/**
 \brief Vérifie si le produit existe
 \param tnumProd tableau des numéros de produit
 \param nProd numéro de référence du produit
 \param tlog taille logique des tableaux
 \param i position dans le tableau si le produit à était trouvé
 \return 1 si le N° de produit est trouvé sinon 2 s'il n'as pas était trouvé
*/
int fVerifProd(int tnumProd[],int nProd,int tlog,int* i);

/**
 \brief Vérifie si quantité est suffisante pour la commande
 \param qt quantité d'un produit
 \param tQt tableau des quantités des produits
 \param pos position dans le tableau
 \return  1 si la quantité est suffisante sinon 2 si la quantité n'est pas suffisante
*/
int fVerifQt(int qt,int tQt[], int pos);

/**
 \brief Réalise un devis
 \param tNProdCom tableau des numéros de produit pour les commandes
 \param tQtCom tableau des quantités des produits pour les commandes
 \param tPrixCom tableau des prix des produits pour les commandes
 \param tlogCom taille logique des tableaux pour les commandes
*/
void fDevis(int tNProdCom[],int tQtCom[],float tPrixCom[],int tlogCom);

/**
 \brief Ecrit dans le fichier vente.txt les commandes effectuées
 \param tNProdCom tableau des numéros de produit pour les commandes
 \param tQtCom tableau des quantités des produits pour les commandes
 \param tPrixCom tableau des prix des produits pour les commandes
 \param tlogCom taille logique des tableaux pour les commandes
 \return -1 problèmme d'ouverture de fichier
*/
int fVenteEffectue(int tNProdCom[],int tQtCom[],float tPrixCom[],int tlogCom);

/**
 \brief Met à jour le stock après confirmation de la commande
 \param tQt tableau des quantités des produits
 \param tnumProd tableau des numéros de produit
 \param tlog taille logique des tableaux
 \param tNProdCom tableau des numéros des produits pour les commandes
 \param tQtCom tableau des quantités des produits pour les commandes
 \param tlogCom taille logique des tableaux pour les commandes
*/
void fMiseAJourStock(int tQt[],int tnumProd[],int tlog,int tNProdCom[],int tQtCom[],int tlogCom);

/**
 \brief Vérifie quel produit est en risque de rupture de stock
 \param tnumProd tableau des numéros de produit
 \param tQt tableau des quantités des produits
 \param tSecu tableau des seuils de sécurité des produits
 \param tlog taille logique des tableaux
*/
void fProdRupt(int tnumProd[],int tQt[],int tSecu[],int tlog);

/**
 \brief Affiche toutes les ventes effectuées
 \return -1 si problèmme d'ouverture, 0 si tout c'est bien passé
*/
int fRecapVente(void);

/**
 \brief Saisi et enregistre de manière controlé l'approvisionnement en utilisant fSaisieAppro
 \param tnumProd tableau des numéros de produit
 \param tQt tableau des quantités des produits
 \param tPrix tableau des prix des produits
 \param tSecu tableau des seuils de sécurité des produits
 \param tlog taille logique des tableaux
 \return la taille logique des tableaux
 \param tNom tableau des noms de produits
*/
int fEnregAppro(int tnumProd[],int tQt[],float tPrix[],int tSecu[],int tlog, char tNom[][TAILLEMOT]);

/**
 \brief Saisi et controle l'approvisionnement avec fVerifProd
 \param nProd numéro du produit
 \param qte quantité du produit
 \param prix prix du produit
 \param secu seuil de sécurité du produit
 \param tnumProd tableau des numéros de produit
 \param tlog taille logique des tableaux
 \param nom nom du nouveau produit
 \return le résultat de verif
*/
int fSaisieAppro(int* nProd, int* qte, float* prix, int* secu,int tnumProd[],int tlog,int* i, char nom[]);

/**
 \brief Enregistre les nouvelles données dans le fichier stock.txt
 \param tnumProd tableau des numéros de produit
 \param tQt tableau des quantités des produits
 \param tPrix tableau des prix des produits
 \param tSecu tableau des seuils de sécurité des produits
 \param tlog taille logique des tableaux
 \param tNom tableau des noms de produits
*/
void fSauvegarde(int tnumProd[],int tQt[],float tPrix[],int tSecu[],int tlog,  char tNom[][TAILLEMOT]);

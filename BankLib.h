#ifndef BANKLIB_H_INCLUDED
#define BANKLIB_H_INCLUDED

typedef struct
{
    int codeP;
    char rue[20];
    char ville [20];
} adresse;

typedef struct
{
    char Nom_client[50];
    char Prenom_client[60];
    adresse adresse ;
    char Email[100];
    char Num_tel[25];
    long int Num_compte;
    long int pin;
    char statut [20];
    float solde;
} Compte;

typedef struct
{
    long int Num_compte;
    char type[20];
    float montant;
    long int Vers_Num_compte;
} Transaction;

long int generateUniqueId();

void Depot();

void Retrait();

void Transferer();

void inscriptionClient();

void modifierClient();

void supprimerClient();

void AfficherClients();

void RechercheCompteNom();

void RechercheCompteNum();

void TriComptesNom();

void TriComptesSolde();

void HistoriqueTransaction();

void TotalFonds();

void menu();

void menuClientBeforeLogin();

void menuAdminBeforeLogin();

void Login();

void menuClientAfterLogin();

void menuAdminAfterLogin();

#endif // BANKLIB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "BankLib.h"

bool session=false;
long int session_id;
bool user;
bool admin;

long int generateUniqueId()
{
    return (long)time(NULL);
}

void Depot()
{
    Transaction T;  // Declare a transaction object
    Compte c;  // Declare an account object
    char buffer[300];  // Declare a buffer to temporarily hold file data

    T.Num_compte = session_id;  // Assign the current session's account number to the transaction
    system("cls");  // Clear the screen for better visibility

    // Prompt the user to input the deposit amount
    printf("saisir le montant a deposer : ");
    scanf("%f", &T.montant);  // Read the deposit amount from the user

    strcpy(T.type, "Depot");  // Set the transaction type to "Depot"
    T.Vers_Num_compte = session_id;  // Set the destination account number (same as session ID)

    // Open the transaction file in append mode
    FILE *tran = fopen("Transaction.csv", "a");
    if (tran == NULL)    // Check if the file opened successfully
    {
        printf("Le fichier n existe pas\n");  // Print error message if file doesn't exist
        exit(1);  // Exit the program if the file cannot be opened
    }

    // Write the transaction details to the transaction file
    fprintf(tran, "%ld,%s,%f,%ld\n", T.Num_compte, T.type, T.montant, T.Vers_Num_compte);
    fclose(tran);  // Close the transaction file after writing

    // Open the account file in read-write mode
    FILE *fpt = fopen("Gestion_des_comptes_clients.csv", "r+");
    FILE *temp = fopen("temp.csv", "w");  // Open a temporary file to write updated data

    if (fpt == NULL)    // Check if the account file opened successfully
    {
        printf("Le fichier n existe pas\n");  // Print error message if file doesn't exist
        exit(1);  // Exit the program if the file cannot be opened
    }

    fgets(buffer, sizeof(buffer), fpt);  // Read the header line from the account file
    fputs(buffer, temp);  // Write the header line to the temporary file

    // Read each account record and process it
    while (fscanf(fpt, "%[^,],%[^,],%d-%[^-]-%[^,],%[^,],%[^,],%f,%ld,%[^,\n]\n",
                  c.Nom_client, c.Prenom_client, &c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                  c.Email, c.Num_tel, &c.solde, &c.Num_compte, c.statut) == 10)
    {

        if (session_id == c.Num_compte)    // Check if the account number matches the session ID
        {
            c.solde = c.solde + T.montant;  // Add the deposit amount to the account balance
        }

        // Write the updated account data to the temporary file
        fprintf(temp, "%s,%s,%d-%s-%s,%s,%s,%f,%ld,%s\n",
                c.Nom_client, c.Prenom_client, c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                c.Email, c.Num_tel, c.solde, c.Num_compte, c.statut);
    }

    fclose(fpt);  // Close the original account file
    fclose(temp);  // Close the temporary file

    // Replace the old account file with the updated temporary file
    remove("Gestion_des_comptes_clients.csv");  // Delete the old account file
    rename("temp.csv", "Gestion_des_comptes_clients.csv");  // Rename temporary file to the original file name

    // Print a confirmation message with the deposited amount
    printf("\n");
    printf("vous avez deposer %.3f DHs dans votre compte...", T.montant);

    sleep(3);  // Pause execution for 3 seconds (adjust this based on the system)
    system("cls");  // Clear the screen after the pause
}


void Retrait()
{
    Transaction T;  // Declare a transaction object
    bool op = false;  // Flag to track whether the withdrawal is successful
    Compte c;  // Declare an account object
    char buffer[300];  // Buffer to temporarily hold file data

    T.Num_compte = session_id;  // Assign the current session's account number to the transaction
    system("cls");  // Clear the screen for better visibility

    // Prompt the user to input the withdrawal amount
    printf("saisir le montant a soustraire : ");
    scanf("%f", &T.montant);  // Read the withdrawal amount from the user

    strcpy(T.type, "Retrait");  // Set the transaction type to "Retrait" (Withdrawal)
    T.Vers_Num_compte = session_id;  // Set the destination account number (same as session ID)

    // Open the account file in read-write mode and the temporary file for updates
    FILE *fpt = fopen("Gestion_des_comptes_clients.csv", "r+");
    FILE *temp = fopen("temp.csv", "w");  // Open a temporary file to write updated data

    if (fpt == NULL)    // Check if the account file opened successfully
    {
        printf("Le fichier n existe pas\n");  // Print error message if file doesn't exist
        exit(1);  // Exit the program if the file cannot be opened
    }

    fgets(buffer, sizeof(buffer), fpt);  // Read and copy the header line from the account file
    fputs(buffer, temp);  // Write the header to the temporary file

    // Read each account record and process it
    while (fscanf(fpt, "%[^,],%[^,],%d-%[^-]-%[^,],%[^,],%[^,],%f,%ld,%[^,\n]\n",
                  c.Nom_client, c.Prenom_client, &c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                  c.Email, c.Num_tel, &c.solde, &c.Num_compte, c.statut) == 10)
    {

        if (session_id == c.Num_compte)    // Check if the account number matches session ID
        {
            if (c.solde >= T.montant)    // Check if the account has enough balance for withdrawal
            {
                c.solde = c.solde - T.montant;  // Subtract the withdrawal amount from the balance
                op = true;  // Set the operation flag to true indicating the withdrawal was successful
            }
        }

        // Write the updated account data to the temporary file
        fprintf(temp, "%s,%s,%d-%s-%s,%s,%s,%f,%ld,%s\n",
                c.Nom_client, c.Prenom_client, c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                c.Email, c.Num_tel, c.solde, c.Num_compte, c.statut);
    }

    fclose(fpt);  // Close the original account file
    fclose(temp);  // Close the temporary file

    // Replace the old account file with the updated temporary file
    remove("Gestion_des_comptes_clients.csv");  // Delete the old account file
    rename("temp.csv", "Gestion_des_comptes_clients.csv");  // Rename the temporary file to the original file name

    // Check if the operation was successful and log the transaction
    if (op == true)
    {
        FILE *tran = fopen("Transaction.csv", "a");  // Open the transaction file in append mode
        if (tran == NULL)    // Check if the transaction file opened successfully
        {
            printf("Le fichier n existe pas\n");  // Print error message if file doesn't exist
            exit(1);  // Exit the program if the file cannot be opened
        }
        // Log the withdrawal transaction in the transaction file
        fprintf(tran, "%ld,%s,%f,%ld\n", T.Num_compte, T.type, T.montant, T.Vers_Num_compte);
        fclose(tran);  // Close the transaction file after logging

        // Print confirmation message for successful withdrawal
        printf("\n");
        printf("vous avez soustraire %.3f DHs de votre compte...", T.montant);
    }
    // If the operation was not successful (insufficient funds)
    else if (op == false)
    {
        printf("vous n'avez pas assez d'argent dans votre compte pour cette operation...");  // Print insufficient funds message
    }

    sleep(5);  // Pause execution for 5 seconds to allow user to read the message
    system("cls");  // Clear the screen after the pause
}

void Transferer()
{
    Transaction T;  // Declare a transaction object
    bool op = false;  // Flag to check if the transaction can be completed (sufficient funds)
    bool user_vers = false;  // Flag to check if the receiving account exists
    Compte c;  // Declare an account object
    char buffer[300];  // Buffer to temporarily hold file data

    T.Num_compte = session_id;  // Assign the current session's account number to the transaction
    system("cls");  // Clear the screen for better visibility

    // Prompt the user to input the transfer amount
    printf("Veuillez saisir le montant a transferer : ");
    scanf("%f", &T.montant);  // Read the transfer amount from the user

    // Prompt the user to input the receiving account ID
    printf("Veuillez saisir l'ID du compte vers lequel transferer le montant : ");
    scanf("%ld", &T.Vers_Num_compte);  // Read the receiving account ID from the user

    strcpy(T.type, "Transfert");  // Set the transaction type to "Transfert" (Transfer)

    // Open the account file in read-write mode
    FILE *fpt = fopen("Gestion_des_comptes_clients.csv", "r+");
    if (fpt == NULL)    // Check if the account file opened successfully
    {
        printf("Le fichier n existe pas\n");  // Print error message if file doesn't exist
        exit(1);  // Exit the program if the file cannot be opened
    }
    fgets(buffer, sizeof(buffer), fpt);  // Read and copy the header line from the account file

    // Read each account record and process it
    while (fscanf(fpt, "%[^,],%[^,],%d-%[^-]-%[^,],%[^,],%[^,],%f,%ld,%[^,\n]\n",
                  c.Nom_client, c.Prenom_client, &c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                  c.Email, c.Num_tel, &c.solde, &c.Num_compte, c.statut) == 10)
    {

        if (session_id == c.Num_compte)    // Check if the account number matches session ID
        {
            if (c.solde >= T.montant)    // Check if the account has enough balance for the transfer
            {
                op = true;  // Set the operation flag to true indicating the transfer is possible
            }
        }
        if (T.Vers_Num_compte == c.Num_compte)    // Check if the receiving account exists
        {
            user_vers = true;  // Set the flag to true if the receiving account is valid
        }
    }

    // If both conditions (valid receiving account and sufficient funds) are true, proceed with the transfer
    if (user_vers == true && op == true)
    {
        FILE *temp = fopen("temp.csv", "w");  // Open a temporary file to write updated data
        fseek(fpt, 0, SEEK_SET);  // Move the file pointer to the beginning
        fgets(buffer, sizeof(buffer), fpt);  // Copy the header to the temporary file
        fputs(buffer, temp);

        // Read each account record and process the transfer
        while (fscanf(fpt, "%[^,],%[^,],%d-%[^-]-%[^,],%[^,],%[^,],%f,%ld,%[^,\n]\n",
                      c.Nom_client, c.Prenom_client, &c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                      c.Email, c.Num_tel, &c.solde, &c.Num_compte, c.statut) == 10)
        {

            if (session_id == c.Num_compte)    // If it's the current account, subtract the transfer amount
            {
                c.solde = c.solde - T.montant;
            }
            else if (T.Vers_Num_compte == c.Num_compte)      // If it's the receiving account, add the transfer amount
            {
                c.solde = c.solde + T.montant;
            }
            // Write the updated account data to the temporary file
            fprintf(temp, "%s,%s,%d-%s-%s,%s,%s,%f,%ld,%s\n",
                    c.Nom_client, c.Prenom_client, c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                    c.Email, c.Num_tel, c.solde, c.Num_compte, c.statut);
        }
        fclose(fpt);  // Close the original account file
        fclose(temp);  // Close the temporary file
    }

    // Replace the old account file with the updated temporary file
    remove("Gestion_des_comptes_clients.csv");  // Delete the old account file
    rename("temp.csv", "Gestion_des_comptes_clients.csv");  // Rename the temporary file to the original file name

    // If the transaction was successful, log it and print a success message
    if (op == true && user_vers == true)
    {
        FILE *tran = fopen("Transaction.csv", "a");  // Open the transaction file in append mode
        if (tran == NULL)    // Check if the transaction file opened successfully
        {
            printf("Le fichier n existe pas\n");  // Print error message if file doesn't exist
            exit(1);  // Exit the program if the file cannot be opened
        }
        // Log the transfer transaction in the transaction file
        fprintf(tran, "%ld,%s,%f,%ld\n", T.Num_compte, T.type, T.montant, T.Vers_Num_compte);
        fclose(tran);  // Close the transaction file

        // Print confirmation message for successful transfer
        printf("\n");
        printf("vous avez transferer %.3f DHs vers le compte avec l'ID %ld...", T.montant, T.Vers_Num_compte);
    }
    // If there are insufficient funds, print an error message
    else if (op == false)
    {
        printf("Echec!! vous n'avez pas assez d'argent dans votre compte pour cette operation...");
    }
    // If the receiving account is invalid, print an error message
    else if (user_vers == false)
    {
        printf("Echec!! ID du compte recepteur non valide...");
    }

    sleep(5);  // Pause execution for 5 seconds to allow user to read the message
    system("cls");  // Clear the screen after the pause
}


void inscriptionClient()
{
    system("cls");  // Clear the screen

    Compte C;  // Declare a client account object
    // Open the client accounts file in append mode
    FILE *fpt = fopen("Gestion_des_comptes_clients.csv", "a");
    // Open the login details file in append mode
    FILE *log = fopen("Login.csv", "a");

    // Check if the files opened successfully
    if (fpt == NULL || log == NULL)
    {
        printf("Le fichier n existe pas\n");  // Print error message if any file doesn't exist
        exit(1);  // Exit the program if files cannot be opened
    }

    // Prompt the user to enter their last name
    printf("Veuiller saisir votre nom : ");
    while (getchar() != '\n');  // Clear any leftover newline characters in the input buffer
    scanf("%[^\n]", C.Nom_client);  // Read the last name
    C.Nom_client[0] = toupper(C.Nom_client[0]);  // Capitalize the first letter of the last name
    fprintf(fpt, "%s,", C.Nom_client);  // Write the last name to the file

    // Prompt the user to enter their first name
    printf("Veuiller saisir votre prenom : ");
    while (getchar() != '\n');  // Clear the buffer
    scanf("%[^\n]", C.Prenom_client);  // Read the first name
    C.Prenom_client[0] = toupper(C.Prenom_client[0]);  // Capitalize the first letter of the first name
    fprintf(fpt, "%s,", C.Prenom_client);  // Write the first name to the file

    // Prompt the user to enter their address (postal code, street, city)
    printf("Veuiller saisir votre adresse(code postal-rue-ville) :");
    while (getchar() != '\n');  // Clear the buffer
    scanf("%d-%[^-]-%[^\n]", &C.adresse.codeP, C.adresse.rue, C.adresse.ville);  // Read the address
    fprintf(fpt, "%d-%s-%s,", C.adresse.codeP, C.adresse.rue, C.adresse.ville);  // Write the address to the file

    // Prompt the user to enter their email
    printf("Veuiller saisir votre email :");
    scanf("%s", C.Email);  // Read the email
    fprintf(fpt, "%s,", C.Email);  // Write the email to the file

    // Prompt the user to enter their phone number
    printf("Veuiller saisir votre numero de telephone:");
    scanf("%s", C.Num_tel);  // Read the phone number
    fprintf(fpt, "%s,", C.Num_tel);  // Write the phone number to the file

    // Set the initial balance to 0
    C.solde = 0;
    fprintf(fpt, "%d,", C.solde);  // Write the balance to the file

    // Generate a unique account number for the client
    C.Num_compte = generateUniqueId();
    fprintf(fpt, "%d,", C.Num_compte);  // Write the account number to the file

    // Generate a random PIN for the client between 1000 and 9999
    srand((unsigned int) time(NULL));  // Seed the random number generator with the current time
    C.pin = rand() % 9000 + 1000;  // Generate a random 4-digit PIN

    // Set the account status to "actif"
    strcpy(C.statut, "actif");
    fprintf(fpt, "%s\n", C.statut);  // Write the account status to the file

    // Write the login details (account number, PIN, and status) to the login file
    fprintf(log, "%ld,%ld,%s\n", C.Num_compte, C.pin, C.statut);

    system("cls");  // Clear the screen
    // Display success messages to the user
    printf("votre compte est cree avec succes(compte actif)!\n");
    printf(" Et votre Numéro de Compte est: %ld\n", C.Num_compte);
    printf(" Et votre code pin est: %ld\n", C.pin);

    fclose(fpt);  // Close the client accounts file
    fclose(log);  // Close the login file

    fflush(stdin);  // Clear the input buffer
    // Prompt the user to press any key to return to the main menu
    printf("\nTAPER SUR N IMPORTE QUEL BUTTON DANS LE CALVIER POUR RETOURNER AU MENU DE L UTILISATEUR...");
    getchar();  // Wait for the user to press a key
    system("cls");  // Clear the screen again
}

void modifierClient()
{
    system("cls");  // Clear the screen
    long int id;  // Declare a variable to store the account number
    char buffer[300];  // Buffer to store a line of text
    int choix;  // Variable to store the user's choice for account status
    bool ClientTrouve = false;  // Flag to check if the client is found
    FILE *temp;  // Temporary file pointer
    // Prompt the user to enter the account number to modify
    printf("Entrez le numéro de compte à modifier: ");
    scanf("%ld", &id);  // Read the account number

    Compte c;  // Declare an account object

    // Open the client accounts file for reading
    FILE *fpt = fopen("Gestion_des_comptes_clients.csv", "r");
    // Open a temporary file for writing
    temp = fopen("temp.csv", "w");

    // Check if files were opened successfully
    if (fpt == NULL || temp == NULL)
    {
        printf("Le fichier n'existe pas\n");  // Print error message if files don't exist
        exit(1);  // Exit the program if files cannot be opened
    }

    // Read the header from the client accounts file
    fgets(buffer, sizeof(buffer), fpt);
    // Write the header to the temporary file
    fputs(buffer, temp);

    // Loop through the client accounts in the file
    while (fscanf(fpt, "%[^,],%[^,],%d-%[^-]-%[^,],%[^,],%[^,],%f,%ld,%[^,\n]\n",
                  c.Nom_client, c.Prenom_client, &c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                  c.Email, c.Num_tel, &c.solde, &c.Num_compte, c.statut) == 10)
    {
        // Check if the account number matches
        if (id == c.Num_compte)
        {
            ClientTrouve = true;  // Mark that the client was found

            // Prompt the user to enter the new last name
            printf("Veuiller saisir votre nom : ");
            while (getchar() != '\n');  // Clear the input buffer
            scanf("%[^\n]", c.Nom_client);  // Read the new last name
            c.Nom_client[0] = toupper(c.Nom_client[0]);  // Capitalize the first letter of the last name

            // Prompt the user to enter the new first name
            printf("Veuiller saisir votre prenom : ");
            while (getchar() != '\n');  // Clear the input buffer
            scanf("%[^\n]", c.Prenom_client);  // Read the new first name
            c.Prenom_client[0] = toupper(c.Prenom_client[0]);  // Capitalize the first letter of the first name

            // Prompt the user to enter the new address
            printf("Veuiller saisir la nouvelle adresse (code postal-rue-ville) : ");
            while (getchar() != '\n');  // Clear the input buffer
            scanf("%d-%[^-]-%[^\n]", &c.adresse.codeP, c.adresse.rue, c.adresse.ville);  // Read the new address

            // Prompt the user to enter the new email
            printf("Veuiller saisir le nouveau email : ");
            scanf("%s", c.Email);  // Read the new email

            // Prompt the user to enter the new phone number
            printf("Veuiller saisir le nouveau numero de telephone : ");
            scanf("%s", c.Num_tel);  // Read the new phone number

            // Prompt the user to decide whether to deactivate the account
            do
            {
                printf("Mettre le compte inactif? (1 pour oui et 0 pour non) : ");
                scanf("%d", &choix);  // Read the choice (1 for yes, 0 for no)
            }
            while (choix != 1 && choix != 0);    // Ensure a valid choice is made

            // Set the account status based on the user's choice
            if (choix == 1)
            {
                strcpy(c.statut, "inactif");  // Set the account status to inactive
            }
            else
            {
                strcpy(c.statut, "actif");  // Set the account status to active
            }
        }

        // Write the updated account information to the temporary file
        fprintf(temp, "%s,%s,%d-%s-%s,%s,%s,%f,%ld,%s\n",
                c.Nom_client, c.Prenom_client, c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                c.Email, c.Num_tel, c.solde, c.Num_compte, c.statut);
    }

    fclose(fpt);  // Close the client accounts file
    fclose(temp);  // Close the temporary file

    // Replace the original file with the temporary file
    remove("Gestion_des_comptes_clients.csv");
    rename("temp.csv", "Gestion_des_comptes_clients.csv");

    // Open the login file for reading
    FILE *log = fopen("Login.csv", "r");
    // Open another temporary file for writing
    temp = fopen("temp1.csv", "w");

    // Check if files were opened successfully
    if (log == NULL || temp == NULL)
    {
        printf("Le fichier Login.csv n'existe pas\n");  // Print error message if login file doesn't exist
        exit(2);  // Exit the program if the login file cannot be opened
    }

    // Loop through the login file to update the account status
    while (fscanf(log, "%ld,%ld,%[^\n]\n", &c.Num_compte, &c.pin, c.statut) == 3)
    {
        // Check if the account number matches
        if (id == c.Num_compte)
        {
            // Update the account status in the login file based on the user's choice
            if (choix == 1)
            {
                strcpy(c.statut, "inactif");
            }
            else
            {
                strcpy(c.statut, "actif");
            }

            // Write the updated login details to the temporary file
            fprintf(temp, "%ld,%ld,%s\n", c.Num_compte, c.pin, c.statut);
        }
        else
        {
            // Write the unchanged login details to the temporary file
            fprintf(temp, "%ld,%ld,%s\n", c.Num_compte, c.pin, c.statut);
        }
    }

    fclose(log);  // Close the login file
    fclose(temp);  // Close the temporary file

    // Replace the original login file with the updated temporary file
    remove("Login.csv");
    rename("temp1.csv", "Login.csv");

    // Check if the client was found and display a message
    if (!ClientTrouve)
    {
        printf("Client introuvable!\n");  // Print error message if client not found
        getch();  // Wait for user input
        system("cls");  // Clear the screen
    }
    else
    {
        printf("Client trouvé et modifié!\n");  // Print success message if client was found and modified
        getch();  // Wait for user input
        system("cls");  // Clear the screen
    }
}

void supprimerClient()
{
    system("cls");  // Clear the screen
    long int id;  // Declare a variable to store the account number
    char buffer[300];  // Buffer to store a line of text
    int choix;  // Variable for additional user choices (though unused here)
    bool ClientTrouve = false;  // Flag to check if the client was found
    FILE *temp;  // Temporary file pointer
    // Prompt the user to enter the account number to delete
    printf("Entrez le numero de compte a supprimer: ");
    scanf("%ld", &id);  // Read the account number

    Compte c;  // Declare an account object

    // Open the client accounts file for reading
    FILE *fpt = fopen("Gestion_des_comptes_clients.csv", "r");
    // Open a temporary file for writing
    temp = fopen("temp.csv", "w");

    // Check if files were opened successfully
    if (fpt == NULL || temp == NULL)
    {
        printf("Le fichier n'existe pas\n");  // Print error message if files don't exist
        exit(1);  // Exit the program if files cannot be opened
    }

    // Read the header from the client accounts file
    fgets(buffer, sizeof(buffer), fpt);
    // Write the header to the temporary file
    fputs(buffer, temp);

    // Loop through the client accounts in the file
    while (fscanf(fpt, "%[^,],%[^,],%d-%[^-]-%[^,],%[^,],%[^,],%f,%ld,%[^,\n]\n",
                  c.Nom_client, c.Prenom_client, &c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                  c.Email, c.Num_tel, &c.solde, &c.Num_compte, c.statut) == 10)
    {
        // Check if the account number matches
        if (id == c.Num_compte)
        {
            ClientTrouve = true;  // Mark that the client was found
            continue;  // Skip writing this client's information to the temporary file
        }

        // Write the client's information to the temporary file (if not deleted)
        fprintf(temp, "%s,%s,%d-%s-%s,%s,%s,%f,%ld,%s\n",
                c.Nom_client, c.Prenom_client, c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                c.Email, c.Num_tel, c.solde, c.Num_compte, c.statut);
    }

    fclose(fpt);  // Close the client accounts file
    fclose(temp);  // Close the temporary file

    // Replace the original file with the temporary file
    remove("Gestion_des_comptes_clients.csv");
    rename("temp.csv", "Gestion_des_comptes_clients.csv");

    // Open the login file for reading
    FILE *log = fopen("Login.csv", "r");
    // Open another temporary file for writing
    temp = fopen("temp1.csv", "w");

    // Check if files were opened successfully
    if (log == NULL || temp == NULL)
    {
        printf("Le fichier Login.csv n'existe pas\n");  // Print error message if login file doesn't exist
        exit(2);  // Exit the program if the login file cannot be opened
    }

    // Loop through the login file to remove the client from the login records
    while (fscanf(log, "%ld,%ld,%[^\n]\n", &c.Num_compte, &c.pin, c.statut) == 3)
    {
        // Check if the account number matches
        if (id == c.Num_compte)
        {
            continue;  // Skip writing this account to the temporary file (effectively removing it)
        }
        else
        {
            // Write the unchanged login details to the temporary file
            fprintf(temp, "%ld,%ld,%s\n", c.Num_compte, c.pin, c.statut);
        }
    }

    fclose(log);  // Close the login file
    fclose(temp);  // Close the temporary file

    // Replace the original login file with the updated temporary file
    remove("Login.csv");
    rename("temp1.csv", "Login.csv");

    // Check if the client was found and display a message
    if (!ClientTrouve)
    {
        printf("Client introuvable!\n");  // Print error message if client not found
        getch();  // Wait for user input
        system("cls");  // Clear the screen
    }
    else
    {
        printf("Client trouve et supprime!\n");  // Print success message if client was found and deleted
        getch();  // Wait for user input
        system("cls");  // Clear the screen
    }
}


void AfficherClients()
{
    system("cls");  // Clear the screen
    Compte c;  // Declare a variable to store client information
    char buffer[300];  // Buffer to store a line of text
    FILE *fpt = fopen("Gestion_des_comptes_clients.csv", "r");  // Open the file containing the client accounts for reading

    if (fpt == NULL)    // Check if the file exists and was opened successfully
    {
        printf("Le fichier n existe pas\n");  // Print error message if the file does not exist
        exit(1);  // Exit the program if the file cannot be opened
    }

    fgets(buffer, sizeof(buffer), fpt);  // Read the header line from the file (skip the first line)

    while (fscanf(fpt, "%[^,],%[^,],%d-%[^-]-%[^,],%[^,],%[^,],%f,%ld,%[^,\n]\n",
                  c.Nom_client, c.Prenom_client, &c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                  c.Email, c.Num_tel, &c.solde, &c.Num_compte, c.statut) == 10)    // Loop through the client records in the file
    {
        printf("%s\t%s\t%d-%s-%s\t%s\t%s\t%f\t%ld\t%s\n",  // Print the client details in a formatted way
               c.Nom_client, c.Prenom_client, c.adresse.codeP, c.adresse.rue, c.adresse.ville,
               c.Email, c.Num_tel, c.solde, c.Num_compte, c.statut);
    }

    fclose(fpt);  // Close the client accounts file

    while (getchar() != '\n');  // Wait for user input before clearing the screen
    printf("\nTAPER SUR N IMPORTE QUEL BUTTON DANS LE CALVIER POUR RETOURNER AU MENU DE L UTILISATEUR...");  // Prompt the user to press any key
    getchar();  // Wait for user to press a key

    system("cls");  // Clear the screen after the user has pressed a key
}


void RechercheCompteNom()
{
    bool clientTrouve=false; // Flag to check if the client is found
    Compte c; // Declare a structure for storing client information
    char nom[50],buffer[300]; // Declare variables for storing the search name and buffer for reading
    system("cls"); // Clear the screen
    printf("saisir le nom a chercher : "); // Ask the user to input a name to search for
    while (getchar() != '\n'); // Clear the input buffer
    scanf("%[^\n]",nom); // Get the name input by the user
    nom[0]=toupper(nom[0]); // Capitalize the first letter of the name

    // Open the client data file
    FILE *fpt=fopen("Gestion_des_comptes_clients.csv","r");
    if(fpt==NULL) // Check if the file exists
    {
        printf("Le fichier n existe pas\n"); // Print error message if file doesn't exist
        exit(1); // Exit the program if file is not found
    }
    fgets(buffer,sizeof(buffer),fpt); // Read the first line (header) of the file
    // Loop to read each line of the file and parse the client data
    while (fscanf(fpt, "%[^,],%[^,],%d-%[^-]-%[^,],%[^,],%[^,],%f,%ld,%[^,\n]\n",
                  c.Nom_client, c.Prenom_client, &c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                  c.Email, c.Num_tel, &c.solde, &c.Num_compte, c.statut) == 10)
    {
        if(strcmp(c.Nom_client,nom)==0) // Check if the client's name matches the input
        {
            clientTrouve=true; // Set the flag to true if the client is found
            // Print client information
            printf("%s\t%s\t%d-%s-%s\t%s\t%s\t%f\t%ld\t%s\n",
                   c.Nom_client, c.Prenom_client, c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                   c.Email, c.Num_tel, c.solde, c.Num_compte, c.statut);
            break; // Exit the loop once the client is found
        }
    }
    fclose(fpt); // Close the file after reading

    // If the client was not found, display a message
    if(clientTrouve==false)
    {
        printf("Client introuvable!\n");
    }

    fflush(stdin); // Clear the input buffer
    printf("appuyer sur entree pour revenir au menu precedent..."); // Prompt the user to press Enter to return
    getchar(); // Wait for the user to press Enter
    system("cls"); // Clear the screen
}


void RechercheCompteNum()
{
    char buffer[300]; // Buffer to hold the data from the file
    bool clientTrouve=false; // Flag to check if the client is found
    Compte c; // Declare a structure for storing client information
    long int id; // Variable to store the account ID input by the user
    system("cls"); // Clear the screen
    printf("saisir l'ID a chercher : "); // Ask the user to input the account ID to search for
    scanf("%ld",&id); // Get the account ID input by the user

    // Open the client data file
    FILE *fpt=fopen("Gestion_des_comptes_clients.csv","r");
    if(fpt==NULL) // Check if the file exists
    {
        printf("Le fichier n existe pas\n"); // Print error message if file doesn't exist
        exit(1); // Exit the program if file is not found
    }
    fgets(buffer,sizeof(buffer),fpt); // Read the first line (header) of the file
    // Loop to read each line of the file and parse the client data
    while (fscanf(fpt, "%[^,],%[^,],%d-%[^-]-%[^,],%[^,],%[^,],%f,%ld,%[^,\n]\n",
                  c.Nom_client, c.Prenom_client, &c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                  c.Email, c.Num_tel, &c.solde, &c.Num_compte, c.statut) == 10)
    {
        if(c.Num_compte==id) // Check if the account ID matches the input
        {
            clientTrouve=true; // Set the flag to true if the client is found
            // Print client information
            printf("%s\t%s\t%d-%s-%s\t%s\t%s\t%f\t%ld\t%s\n",
                   c.Nom_client, c.Prenom_client, c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                   c.Email, c.Num_tel, c.solde, c.Num_compte, c.statut);
            break; // Exit the loop once the client is found
        }
    }
    fclose(fpt); // Close the file after reading

    // If the client was not found, display a message
    if(clientTrouve==false)
    {
        printf("Client introuvable!\n");
    }

    fflush(stdin); // Clear the input buffer
    printf("appuyer sur entree pour revenir au menu precedent..."); // Prompt the user to press Enter to return
    getchar(); // Wait for the user to press Enter
    system("cls"); // Clear the screen
}

void TriComptesNom()
{
    system("cls"); // Clear the screen
    char buffer[300]; // Buffer to hold the first line (header) from the file
    Compte c[1000], ct; // Array to hold client data, temporary variable for swapping
    int i, j, nbr_clients = 0; // Variables for looping and counting the number of clients
    // Open the file in read and write mode, and create a temporary file for storing sorted data
    FILE *fpt = fopen("Gestion_des_comptes_clients.csv", "r+");
    FILE *temp = fopen("temp.csv", "w");
    if (fpt == NULL || temp == NULL) // Check if files are opened successfully
    {
        printf("Le fichier n existe pas\n"); // Print error message if the file doesn't exist
        exit(1); // Exit if the files cannot be opened
    }

    fgets(buffer, sizeof(buffer), fpt); // Read the first line (header) from the file
    fputs(buffer, temp); // Write the header to the temporary file

    // Read the remaining client data from the file and store it in the array
    while (fscanf(fpt, "%[^,],%[^,],%d-%[^-]-%[^,],%[^,],%[^,],%f,%ld,%[^,\n]\n",
                  c[nbr_clients].Nom_client, c[nbr_clients].Prenom_client, &c[nbr_clients].adresse.codeP, c[nbr_clients].adresse.rue, c[nbr_clients].adresse.ville,
                  c[nbr_clients].Email, c[nbr_clients].Num_tel, &c[nbr_clients].solde, &c[nbr_clients].Num_compte, c[nbr_clients].statut) == 10)
    {
        nbr_clients++; // Increment the client count
    }

    // Sort the clients alphabetically by their last name
    for (i = 0; i < nbr_clients - 1; i++) // Outer loop for the sorting
    {
        for (j = i + 1; j < nbr_clients; j++) // Inner loop for comparing clients
        {
            if (strcmp(c[i].Nom_client, c[j].Nom_client) > 0) // Compare last names
            {
                ct = c[i]; // Swap clients if they are not in the correct order
                c[i] = c[j];
                c[j] = ct;
            }
        }
    }

    // Write the sorted client data back to the temporary file
    for (i = 0; i < nbr_clients; i++)
    {
        fprintf(temp, "%s,%s,%d-%s-%s,%s,%s,%f,%ld,%s\n",
                c[i].Nom_client, c[i].Prenom_client, c[i].adresse.codeP, c[i].adresse.rue, c[i].adresse.ville,
                c[i].Email, c[i].Num_tel, c[i].solde, c[i].Num_compte, c[i].statut);
    }

    fclose(fpt); // Close the original file
    fclose(temp); // Close the temporary file

    remove("Gestion_des_comptes_clients.csv"); // Delete the original file
    rename("temp.csv", "Gestion_des_comptes_clients.csv"); // Rename the temporary file to replace the original

    printf("operation avec succes..."); // Print success message
    getch(); // Wait for the user to press a key
    system("cls"); // Clear the screen
}

void TriComptesSolde()
{
    system("cls"); // Clear the screen
    char buffer[300]; // Buffer to hold the first line (header) from the file
    Compte c[1000], ct; // Array to hold client data and a temporary variable for swapping
    int i, j, nbr_clients = 0; // Variables for looping and counting the number of clients

    // Open the file in read and write mode, and create a temporary file for storing sorted data
    FILE *fpt = fopen("Gestion_des_comptes_clients.csv", "r+");
    FILE *temp = fopen("temp.csv", "w");

    if (fpt == NULL || temp == NULL) // Check if the files are opened successfully
    {
        printf("Le fichier n existe pas\n"); // Print error message if the file doesn't exist
        exit(1); // Exit if the files cannot be opened
    }

    fgets(buffer, sizeof(buffer), fpt); // Read the first line (header) from the file
    fputs(buffer, temp); // Write the header to the temporary file

    // Read the remaining client data from the file and store it in the array
    while (fscanf(fpt, "%[^,],%[^,],%d-%[^-]-%[^,],%[^,],%[^,],%f,%ld,%[^,\n]\n",
                  c[nbr_clients].Nom_client, c[nbr_clients].Prenom_client, &c[nbr_clients].adresse.codeP, c[nbr_clients].adresse.rue, c[nbr_clients].adresse.ville,
                  c[nbr_clients].Email, c[nbr_clients].Num_tel, &c[nbr_clients].solde, &c[nbr_clients].Num_compte, c[nbr_clients].statut) == 10)
    {
        nbr_clients++; // Increment the client count
    }

    // Sort the clients based on their balance (solde)
    for (i = 0; i < nbr_clients - 1; i++) // Outer loop for sorting
    {
        for (j = i + 1; j < nbr_clients; j++) // Inner loop for comparing clients
        {
            if (c[i].solde > c[j].solde) // Compare balances of two clients
            {
                ct = c[i]; // Swap the clients if their balances are not in order
                c[i] = c[j];
                c[j] = ct;
            }
        }
    }

    // Write the sorted client data back to the temporary file
    for (i = 0; i < nbr_clients; i++)
    {
        fprintf(temp, "%s,%s,%d-%s-%s,%s,%s,%f,%ld,%s\n",
                c[i].Nom_client, c[i].Prenom_client, c[i].adresse.codeP, c[i].adresse.rue, c[i].adresse.ville,
                c[i].Email, c[i].Num_tel, c[i].solde, c[i].Num_compte, c[i].statut);
    }

    fclose(fpt); // Close the original file
    fclose(temp); // Close the temporary file

    remove("Gestion_des_comptes_clients.csv"); // Delete the original file
    rename("temp.csv", "Gestion_des_comptes_clients.csv"); // Rename the temporary file to replace the original

    printf("operation avec succes..."); // Print success message
    getch(); // Wait for the user to press a key
    system("cls"); // Clear the screen
}

void HistoriqueTransaction()
{
    system("cls"); // Clear the screen
    long int id_hist; // Variable to store the account number for which transaction history is requested
    int nbr = 0; // Variable to count the transactions (currently unused)
    char buffer[300]; // Buffer to hold a line from the transaction file
    Transaction T; // Structure to store transaction details
    bool ClientTrouve = false; // Flag to check if the client is found

    // Prompt the user to enter the account number for which to generate transaction history
    printf("Entrez le numero de compte a generer l'historique des transactions : ");
    scanf("%ld", &id_hist);

    // Open the transaction file for reading
    FILE *tran = fopen("Transaction.csv", "r");
    if (tran == NULL) // Check if the file exists
    {
        printf("Le fichier n existe pas\n"); // Print error if the file doesn't exist
        exit(1); // Exit if the file can't be opened
    }

    // Open a file to store the filtered transaction history
    FILE *idtran = fopen("idtran.csv", "w");
    if (idtran == NULL) // Check if the file can be opened
    {
        printf("Le fichier n existe pas\n"); // Print error if the file can't be opened
        fclose(tran); // Close the transaction file
        exit(2); // Exit if the file can't be opened
    }

    fgets(buffer, sizeof(buffer), tran); // Read the first line (header) from the transaction file
    fputs(buffer, idtran); // Write the header to the new file

    // Read the transaction details from the transaction file and filter them
    while (fscanf(tran, "%ld,%[^,],%f,%ld\n", &T.Num_compte, T.type, &T.montant, &T.Vers_Num_compte) == 4)
    {
        if (T.Num_compte == id_hist) // Check if the transaction belongs to the specified account
        {
            ClientTrouve = true; // Set the flag to true since the client is found

            // Print and write deposit or withdrawal transactions
            if (strcmp(T.type, "Depot") == 0 || strcmp(T.type, "Retrait") == 0)
            {
                printf("%ld\t%-10s\t%f\n", T.Num_compte, T.type, T.montant); // Print transaction details
                fprintf(idtran, "%ld,%s,%f\n", T.Num_compte, T.type, T.montant); // Write transaction to file
            }
            // Print and write transfer transactions
            else if (strcmp(T.type, "Transfert") == 0)
            {
                printf("%ld\t%-10s\t%f\t%ld\n", T.Num_compte, T.type, T.montant, T.Vers_Num_compte); // Print transaction details
                fprintf(idtran, "%ld,%s,%f,%ld\n", T.Num_compte, T.type, T.montant, T.Vers_Num_compte); // Write transaction to file
            }
        }
    }

    fclose(tran); // Close the transaction file
    fclose(idtran); // Close the transaction history file

    // Check if the client was found and handle accordingly
    if (ClientTrouve == true)
    {
        printf("\nfichier generer pour l'historique des transactions du compte d'ID %ld...", id_hist); // Success message
        printf("\nAppuyer sur entree pour quitter..."); // Prompt to exit
    }
    else if (ClientTrouve == false)
    {
        remove("idtran.csv"); // Remove the file if no transactions are found for the client
        printf("Client introuvable!\n"); // Print error message if the client is not found
    }

    getch(); // Wait for user input before exiting
    system("cls"); // Clear the screen
}


void TotalFonds()
{
    system("cls"); // Clear the screen
    Compte c; // Structure to store account information
    char buffer[300]; // Buffer to hold a line from the file
    float fonds = 0; // Variable to accumulate the total funds

    // Open the client accounts file for reading
    FILE *fpt = fopen("Gestion_des_comptes_clients.csv", "r");

    // Check if the file exists and can be opened
    if (fpt == NULL)
    {
        printf("Le fichier n existe pas\n"); // Print error if the file doesn't exist
        exit(1); // Exit if the file can't be opened
    }

    // Read the header line (which we don't need to process)
    fgets(buffer, sizeof(buffer), fpt);

    // Read the account details and sum the balance (solde)
    while (fscanf(fpt, "%[^,],%[^,],%d-%[^-]-%[^,],%[^,],%[^,],%f,%ld,%[^,\n]\n",
                  c.Nom_client, c.Prenom_client, &c.adresse.codeP, c.adresse.rue, c.adresse.ville,
                  c.Email, c.Num_tel, &c.solde, &c.Num_compte, c.statut) == 10)
    {
        fonds += c.solde; // Add the balance to the total funds
    }

    // Print the total funds in the bank
    printf("il y'a %.3f DHs dans la banque...", fonds);

    getch(); // Wait for user input before exiting
    system("cls"); // Clear the screen
}

void menu()
{
    system("cls"); // Clear the screen
    int choix; // Variable to store user choice
    while (1) // Infinite loop to keep displaying the menu until a valid choice is made
    {
        printf("\n---------- Choix de l espace : ----------\n\n");
        printf("1. Acceder a l espace du client\n");
        printf("2. Acceder a l espace de l administrateur\n");
        printf("0. Quitter l'application\n");

        // Get the user choice
        scanf("%d", &choix);

        switch (choix) // Process the user's choice
        {
        case 1: // If the user chooses to access the client space
            system("cls"); // Clear the screen
            menuClientBeforeLogin(); // Call the client menu function
            break;
        case 2: // If the user chooses to access the admin space
            system("cls"); // Clear the screen
            menuAdminBeforeLogin(); // Call the admin menu function
            break;
        case 0: // If the user chooses to quit
            system("cls"); // Clear the screen
            printf("Merci d'avoir utiliser notre application!"); // Thank the user
            sleep(3); // Wait for 3 seconds before exiting
            exit(10); // Exit the application with code 10
        default: // If the user enters an invalid choice
            printf("Choix invalide! Réessayez.\n"); // Inform the user of the invalid choice
            sleep(2); // Wait for 2 seconds
            system("cls"); // Clear the screen
        }
    }
}

void menuClientBeforeLogin()
{
    system("cls"); // Clear the screen
    fflush(stdin); // Flush the input buffer
    int choix; // Variable to store user choice
    while (1) // Infinite loop to keep displaying the menu until a valid choice is made
    {
        printf("\n---------- Espace clients : ----------\n\n");
        printf("1. Login\n");
        printf("2. faire une inscription\n");
        printf("0. Quitter\n");

        // Get the user choice
        scanf("%d", &choix);

        switch (choix) // Process the user's choice
        {
        case 1: // If the user chooses to login
            Login(); // Call the login function
            break;
        case 2: // If the user chooses to register
            inscriptionClient(); // Call the client registration function
            break;
        case 0: // If the user chooses to quit
            system("cls"); // Clear the screen
            menu(); // Return to the main menu
            break;
        default: // If the user enters an invalid choice
            printf("\nChoix invalide! Réessayez.\n"); // Inform the user of the invalid choice
            sleep(2); // Wait for 2 seconds
            system("cls"); // Clear the screen
        }
    }
}

void menuAdminBeforeLogin()
{
    system("cls"); // Clear the screen
    fflush(stdin); // Flush the input buffer
    int choix; // Variable to store user choice
    while (1) // Infinite loop to keep displaying the menu until a valid choice is made
    {
        printf("\n---------- Espace Administrateur : ----------\n\n");
        printf("1. Login\n");
        printf("0. Quitter\n");

        // Get the user choice
        scanf("%d", &choix);

        switch (choix) // Process the user's choice
        {
        case 1: // If the user chooses to login
            Login(); // Call the login function
            break;
        case 0: // If the user chooses to quit
            system("cls"); // Clear the screen
            menu(); // Return to the main menu
            break;
        default: // If the user enters an invalid choice
            printf("\nChoix invalide! Réessayez.\n"); // Inform the user of the invalid choice
            sleep(2); // Wait for 2 seconds
            system("cls"); // Clear the screen
        }
    }
}

void Login()
{
    system("cls"); // Clear the screen
    Compte c; // Declare a Compte variable to store account information
    bool actif = false; // Flag to check if the account is active
    long int num_cmpt_entry, pin_cmpt_entry; // Variables to store entered account number and pin
    user = false; // Initialize user flag to false
    admin = false; // Initialize admin flag to false
    session = false; // Initialize session flag to false

    // Prompt user to enter account number and pin
    printf("saisir votre numero de compte : ");
    scanf("%ld", &num_cmpt_entry);
    printf("saisir votre code de pin : ");
    scanf("%ld", &pin_cmpt_entry);

    // Special case for admin login
    if (num_cmpt_entry == 0 && pin_cmpt_entry == 7421)
    {
        actif = true; // Set the account as active
        session = true; // Start a session
        user = false; // Set the user flag to false (admin)
        admin = true; // Set the admin flag to true
    }

    // Open the login file to validate entered credentials
    FILE *log = fopen("Login.csv", "r");
    while (fscanf(log, "%ld,%ld,%[^\n]\n", &c.Num_compte, &c.pin, c.statut) == 3)
    {
        // Check if account and pin match the entered credentials
        if (num_cmpt_entry == c.Num_compte && pin_cmpt_entry == c.pin && session == false)
        {
            session_id = c.Num_compte; // Store session ID
            session = true; // Start session
            user = true; // Set user flag to true
            admin = false; // Set admin flag to false

            // Check if account status is active
            if (strcmp(c.statut, "actif") == 0)
            {
                actif = true; // Set account as active
                break;
            }
            else if (strcmp(c.statut, "actif") != 0)
            {
                actif == false; // Set account as inactive (but the condition is incorrect here, should use = instead of ==)
                break;
            }
        }
    }
    fclose(log); // Close the login file

    // If admin credentials are correct, proceed to admin menu
    if (admin == true && user == false)
    {
        menuAdminAfterLogin();
    }
    // If user credentials are correct and account is active, proceed to client menu
    else if (admin == false && user == true && actif == true)
    {
        menuClientAfterLogin();
    }
    // If account is inactive, show message and return to main menu
    if (actif == false && user == true)
    {
        printf("votre compte est inactif\nVeuillez contacter l'administrateur pour le rendre actif...");
        sleep(3);
        menu(); // Return to main menu
    }
    // If credentials are invalid, show error and ask the user to retry
    if (admin == false && user == false)
    {
        printf("numero de compte ou code pin non valide!\nVeuillez Ressayer...\n");
        sleep(2); // Wait for 2 seconds before retrying
        Login(); // Retry login
    }
}


void menuClientAfterLogin()
{
    system("cls"); // Clear the screen
    int choix; // Variable to store user's menu choice
    while (1) // Infinite loop to keep showing the menu until user chooses to exit
    {
        // Display client menu options
        printf("\n---------- Espace clients : ----------\n\n");
        printf("1. Deposer une somme\n");
        printf("2. Retraiter une somme\n");
        printf("3. transferer une somme\n");
        printf("0. Quitter\n");
        printf("\nEntrez votre choix: ");
        scanf("%d", &choix); // Get the user's choice

        // Switch case to handle different choices
        switch (choix)
        {
        case 1:
            Depot(); // Call the function to deposit money
            break;
        case 2:
            Retrait(); // Call the function to withdraw money
            break;
        case 3:
            Transferer(); // Call the function to transfer money
            break;
        case 0:
            system("cls"); // Clear the screen
            session = false; // Log out the session
            admin = false; // Set admin flag to false
            user = false; // Set user flag to false
            menu(); // Go back to the main menu
            break;
        default:
            // If the choice is invalid, display an error message
            printf("Choix invalide! Réessayez.\n");
        }
    }
}

void menuAdminAfterLogin()
{
    system("cls"); // Clear the screen
    int choix; // Variable to store the administrator's menu choice
    while (1) // Infinite loop to keep showing the menu until the admin chooses to exit
    {
        // Display administrator menu options
        printf("\n---------- Espace Administrateur : ----------\n\n");
        printf("1. Modifier un client\n");
        printf("2. Supprimer un client\n");
        printf("3. Afficher les clients\n");
        printf("4. Rechercher compte par nom\n");
        printf("5. Rechercher compte par numero de compte\n");
        printf("6. Trier les comptes par nom\n");
        printf("7. Trier les comptes par solde\n");
        printf("8. Generer rapport sur l'historique des transactions d'un compte donne\n");
        printf("9. Afficher total des fonds disponibles dans la banque\n");
        printf("0. Quitter\n");
        printf("\nEntrez votre choix: ");
        scanf("%d", &choix); // Get the admin's choice

        // Switch case to handle different choices
        switch (choix)
        {
        case 1:
            modifierClient(); // Modify a client's information
            break;
        case 2:
            supprimerClient(); // Delete a client
            break;
        case 3:
            AfficherClients(); // Display the list of clients
            break;
        case 4:
            RechercheCompteNom(); // Search for a client by name
            break;
        case 5:
            RechercheCompteNum(); // Search for a client by account number
            break;
        case 6:
            TriComptesNom(); // Sort the accounts by name
            break;
        case 7:
            TriComptesSolde(); // Sort the accounts by balance
            break;
        case 8:
            HistoriqueTransaction(); // Generate a report of transactions for a given account
            break;
        case 9:
            TotalFonds(); // Display the total available funds in the bank
            break;
        case 0:
            system("cls"); // Clear the screen
            session = false; // Log out the session
            admin = false; // Set admin flag to false
            user = false; // Set user flag to false
            menu(); // Go back to the main menu
            break;
        default:
            // If the choice is invalid, display an error message
            printf("Choix invalide! Réessayez.\n");
        }
    }
}

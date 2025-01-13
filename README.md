# Banking_System_EMSI
This project simulates a banking system with functionalities for both clients and administrators. It allows clients to manage their accounts, deposit, withdraw, transfer money, and check their balances, while administrators can manage client accounts and generate reports.

## Functions Overview

### 1. **menu()**
   - Displays the main menu with options to access either the client or administrator space, or quit the application.

### 2. **menuClientBeforeLogin()**
   - Displays the menu for clients before they log in, allowing them to either log in or register.

### 3. **menuAdminBeforeLogin()**
   - Displays the menu for administrators before they log in, allowing them to log in or quit.

### 4. **Login()**
   - Handles the login process for both clients and administrators. Validates the account number and pin.

### 5. **menuClientAfterLogin()**
   - Displays the menu for clients after logging in. Clients can perform actions such as depositing money, withdrawing, transferring, etc.

### 6. **menuAdminAfterLogin()**
   - Displays the menu for administrators after logging in. Administrators can manage clients, view transactions, and generate reports.

### 7. **Depot()**
   - Allows a client to deposit money into their account.

### 8. **Retrait()**
   - Allows a client to withdraw money from their account.

### 9. **Transferer()**
   - Allows a client to transfer money from their account to another account.

### 10. **modifierClient()**
   - Allows the administrator to modify a client’s account details.

### 11. **supprimerClient()**
   - Allows the administrator to delete a client’s account from the system.

### 12. **AfficherClients()**
   - Displays the details of all clients in the system.

### 13. **RechercheCompteNom()**
   - Allows searching for a client’s account using their name.

### 14. **RechercheCompteNum()**
   - Allows searching for a client’s account using the account number.

### 15. **TriComptesNom()**
   - Sorts the clients' accounts alphabetically by their name.

### 16. **TriComptesSolde()**
   - Sorts the clients' accounts by their balance (ascending order).

### 17. **HistoriqueTransaction()**
   - Displays the transaction history for a specific account.

### 18. **TotalFonds()**
   - Calculates and displays the total amount of funds across all accounts in the bank.

### 19. **inscriptionClient()**
   - Allows a client to register by providing their details.

### 20. **modifierClient()**
   - Allows an administrator to modify the details of a client account.

### 21. **supprimerClient()**
   - Allows an administrator to delete a client account.

---

## File Structure

- **Gestion_des_comptes_clients.csv**: Contains all client account details such as name, account number, balance, etc.
- **Transaction.csv**: Stores transaction details such as deposits, withdrawals, and transfers.
- **Login.csv**: Holds the login credentials for users and administrators.

---

## Requirements

- C compiler (e.g., GCC)
- Standard C libraries

## How to Run

1. Compile the code with a C compiler.
2. Run the compiled program.
3. Follow the on-screen prompts to interact with the system.

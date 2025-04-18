#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_TRANSACTIONS 1000
#define FILENAME "ecommerce_finance.dat"

typedef struct {
    int id;
    char date[20];
    char description[100];
    double amount;
    char type; // 'I' for income, 'E' for expense
} Transaction;

Transaction transactions[MAX_TRANSACTIONS];
int transactionCount = 0;

// Function prototypes
void loadTransactions();
void saveTransactions();
void addTransaction();
void viewTransactions();
void financialSummary();
void searchTransactions();
void deleteTransaction();
void editTransaction();
void mainMenu();
void clearInputBuffer();
void displayTitle();

int main() {
    loadTransactions();
    mainMenu();
    return 0;
}

void displayTitle() {
    printf("\n*********************************************\n");
    printf("* E-Commerce Financial Management System 2024 *\n");
    printf("*********************************************\n");
}

void clearInputBuffer() {
    while (getchar() != '\n');
}

void loadTransactions() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        return;
    }

    transactionCount = fread(transactions, sizeof(Transaction), MAX_TRANSACTIONS, file);
    fclose(file);
}

void saveTransactions() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("Error saving data!\n");
        return;
    }

    fwrite(transactions, sizeof(Transaction), transactionCount, file);
    fclose(file);
}

void addTransaction() {
    if (transactionCount >= MAX_TRANSACTIONS) {
        printf("Transaction limit reached. Cannot add more.\n");
        return;
    }

    Transaction newTrans;
    newTrans.id = transactionCount > 0 ? transactions[transactionCount-1].id + 1 : 1;

    // Get current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(newTrans.date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    printf("Enter description: ");
    fgets(newTrans.description, sizeof(newTrans.description), stdin);
    newTrans.description[strcspn(newTrans.description, "\n")] = '\0';

    printf("Enter amount: ");
    scanf("%lf", &newTrans.amount);
    clearInputBuffer();

    printf("Enter type (I for Income, E for Expense): ");
    scanf("%c", &newTrans.type);
    newTrans.type = toupper(newTrans.type);
    clearInputBuffer();

    if (newTrans.type != 'I' && newTrans.type != 'E') {
        printf("Invalid transaction type. Please enter 'I' or 'E'.\n");
        return;
    }

    transactions[transactionCount++] = newTrans;
    saveTransactions();
    printf("Transaction added successfully!\n");
}

void viewTransactions() {
    if (transactionCount == 0) {
        printf("No transactions recorded yet.\n");
        return;
    }

    printf("\n%-5s %-12s %-30s %-10s %-8s\n", "ID", "Date", "Description", "Amount", "Type");
    printf("-----------------------------------------------------------------\n");

    for (int i = 0; i < transactionCount; i++) {
        printf("%-5d %-12s %-30s $%-9.2f %-8s\n",
               transactions[i].id,
               transactions[i].date,
               transactions[i].description,
               transactions[i].amount,
               transactions[i].type == 'I' ? "Income" : "Expense");
    }
}

void financialSummary() {
    double totalIncome = 0, totalExpense = 0, profit = 0;

    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].type == 'I') {
            totalIncome += transactions[i].amount;
        } else {
            totalExpense += transactions[i].amount;
        }
    }

    profit = totalIncome - totalExpense;

    printf("\nFinancial Summary\n");
    printf("----------------\n");
    printf("Total Income:  $%.2f\n", totalIncome);
    printf("Total Expenses: $%.2f\n", totalExpense);
    printf("Net Profit:    $%.2f\n", profit);

    if (totalIncome > 0) {
        double expenseRatio = (totalExpense / totalIncome) * 100;
        printf("Expense to Income Ratio: %.2f%%\n", expenseRatio);
    }
}

void searchTransactions() {
    if (transactionCount == 0) {
        printf("No transactions to search.\n");
        return;
    }

    int choice;
    printf("\nSearch by:\n");
    printf("1. Description\n");
    printf("2. Date\n");
    printf("3. Amount Range\n");
    printf("4. Type (Income/Expense)\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    clearInputBuffer();

    char searchStr[100];
    double minAmount, maxAmount;
    char type;
    int found = 0;

    printf("\n%-5s %-12s %-30s %-10s %-8s\n", "ID", "Date", "Description", "Amount", "Type");
    printf("-----------------------------------------------------------------\n");

    switch (choice) {
        case 1:
            printf("Enter description to search: ");
            fgets(searchStr, sizeof(searchStr), stdin);
            searchStr[strcspn(searchStr, "\n")] = '\0';

            for (int i = 0; i < transactionCount; i++) {
                if (strstr(transactions[i].description, searchStr) != NULL) {
                    printf("%-5d %-12s %-30s $%-9.2f %-8s\n",
                           transactions[i].id,
                           transactions[i].date,
                           transactions[i].description,
                           transactions[i].amount,
                           transactions[i].type == 'I' ? "Income" : "Expense");
                    found = 1;
                }
            }
            break;

        case 2:
            printf("Enter date (YYYY-MM-DD) to search: ");
            fgets(searchStr, sizeof(searchStr), stdin);
            searchStr[strcspn(searchStr, "\n")] = '\0';

            for (int i = 0; i < transactionCount; i++) {
                if (strcmp(transactions[i].date, searchStr) == 0) {
                    printf("%-5d %-12s %-30s $%-9.2f %-8s\n",
                           transactions[i].id,
                           transactions[i].date,
                           transactions[i].description,
                           transactions[i].amount,
                           transactions[i].type == 'I' ? "Income" : "Expense");
                    found = 1;
                }
            }
            break;

        case 3:
            printf("Enter minimum amount: ");
            scanf("%lf", &minAmount);
            printf("Enter maximum amount: ");
            scanf("%lf", &maxAmount);
            clearInputBuffer();

            for (int i = 0; i < transactionCount; i++) {
                if (transactions[i].amount >= minAmount && transactions[i].amount <= maxAmount) {
                    printf("%-5d %-12s %-30s $%-9.2f %-8s\n",
                           transactions[i].id,
                           transactions[i].date,
                           transactions[i].description,
                           transactions[i].amount,
                           transactions[i].type == 'I' ? "Income" : "Expense");
                    found = 1;
                }
            }
            break;

        case 4:
            printf("Enter type (I for Income, E for Expense): ");
            scanf("%c", &type);
            type = toupper(type);
            clearInputBuffer();

            if (type != 'I' && type != 'E') {
                printf("Invalid type.\n");
                return;
            }

            for (int i = 0; i < transactionCount; i++) {
                if (transactions[i].type == type) {
                    printf("%-5d %-12s %-30s $%-9.2f %-8s\n",
                           transactions[i].id,
                           transactions[i].date,
                           transactions[i].description,
                           transactions[i].amount,
                           transactions[i].type == 'I' ? "Income" : "Expense");
                    found = 1;
                }
            }
            break;

        default:
            printf("Invalid choice.\n");
            return;
    }

    if (!found) {
        printf("No transactions found matching your criteria.\n");
    }
}

void deleteTransaction() {
    if (transactionCount == 0) {
        printf("No transactions to delete.\n");
        return;
    }

    viewTransactions();
    int id;
    printf("Enter ID of transaction to delete: ");
    scanf("%d", &id);
    clearInputBuffer();

    int found = 0;
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].id == id) {
            found = 1;
            // Shift all elements after index i left by one
            for (int j = i; j < transactionCount - 1; j++) {
                transactions[j] = transactions[j+1];
            }
            transactionCount--;
            saveTransactions();
            printf("Transaction deleted successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("Transaction with ID %d not found.\n", id);
    }
}

void editTransaction() {
    if (transactionCount == 0) {
        printf("No transactions to edit.\n");
        return;
    }

    viewTransactions();
    int id;
    printf("Enter ID of transaction to edit: ");
    scanf("%d", &id);
    clearInputBuffer();

    int found = 0;
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].id == id) {
            found = 1;

            printf("Edit description (current: %s): ", transactions[i].description);
            fgets(transactions[i].description, sizeof(transactions[i].description), stdin);
            transactions[i].description[strcspn(transactions[i].description, "\n")] = '\0';

            printf("Edit amount (current: %.2f): ", transactions[i].amount);
            scanf("%lf", &transactions[i].amount);
            clearInputBuffer();

            printf("Edit type (current: %c): ", transactions[i].type);
            scanf("%c", &transactions[i].type);
            transactions[i].type = toupper(transactions[i].type);
            clearInputBuffer();

            if (transactions[i].type != 'I' && transactions[i].type != 'E') {
                printf("Invalid transaction type. Keeping previous value.\n");
                transactions[i].type = 'I'; // Default to Income if invalid
            }

            saveTransactions();
            printf("Transaction updated successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("Transaction with ID %d not found.\n", id);
    }
}

void mainMenu() {
    int choice;

    do {
        displayTitle();
        printf("\nMain Menu\n");
        printf("1. Add Transaction\n");
        printf("2. View All Transactions\n");
        printf("3. Financial Summary\n");
        printf("4. Search Transactions\n");
        printf("5. Delete Transaction\n");
        printf("6. Edit Transaction\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                addTransaction();
                break;
            case 2:
                viewTransactions();
                break;
            case 3:
                financialSummary();
                break;
            case 4:
                searchTransactions();
                break;
            case 5:
                deleteTransaction();
                break;
            case 6:
                editTransaction();
                break;
            case 0:
                printf("Exiting system. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

        if (choice != 0) {
            printf("\nPress Enter to continue...");
            getchar();
        }
    } while (choice != 0);
}

#include <stdio.h>
#include <string.h>

int main() {
    FILE *fp;
    char name[50], category[50];
    float amount;
    int choice;

    char payerNames[10][50];    
    float spent[10] = {0};   
    int totalPayers = 0;

    char allPeople[10][50];    
    int totalPeople = 0;

    float total = 0;

    printf("===== EXPENSE TRACKER + SPLITTER =====\n");

    do {
        printf("\nMenu:\n");
        printf("1. Add Expense\n");
        printf("2. View All Expenses\n");
        printf("3. Calculate Split\n");
        printf("4. Exit\n");
        printf("5. Exit & Reset Data\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            fp = fopen("expenses.txt", "a");
            if (fp == NULL) {
                printf("Error opening file!\n");
                return 1;
            }

            printf("Enter name of person who paid: ");
            scanf("%s", name);
            printf("Enter category (Food/Travel/Hotel etc): ");
            scanf("%s", category);
            printf("Enter amount spent: ");
            scanf("%f", &amount);

            fprintf(fp, "%s %s %.2f\n", name, category, amount);
            fclose(fp);

            printf("Expense added and saved successfully!\n");
        }

        else if (choice == 2) {
            fp = fopen("expenses.txt", "r");
            if (fp == NULL) {
                printf("No expenses found yet.\n");
                continue;
            }

            printf("\n%-15s %-15s %-10s\n", "Name", "Category", "Amount");
            printf("--------------------------------------------\n");

            total = 0;
            while (fscanf(fp, "%s %s %f", name, category, &amount) == 3) {
                printf("%-15s %-15s ₹%.2f\n", name, category, amount);
                total += amount;
            }
            fclose(fp);

            printf("--------------------------------------------\n");
            printf("Total Expenses = ₹%.2f\n", total);
        }

        else if (choice == 3) {

            fp = fopen("expenses.txt", "r");
            if (fp == NULL) {
                printf("No expenses found yet.\n");
                continue;
            }

            for (int i = 0; i < 10; i++) {
                spent[i] = 0;
                strcpy(payerNames[i], "");
            }

            total = 0;
            totalPayers = 0;

            while (fscanf(fp, "%s %s %f", name, category, &amount) == 3) {
                int found = 0;

                for (int i = 0; i < totalPayers; i++) {
                    if (strcmp(name, payerNames[i]) == 0) {
                        spent[i] += amount;
                        found = 1;
                        break;
                    }
                }

                if (!found && totalPayers < 10) {
                    strcpy(payerNames[totalPayers], name);
                    spent[totalPayers] = amount;
                    totalPayers++;
                }

                total += amount;
            }
            fclose(fp);

            if (total == 0) {
                printf("No expenses to calculate.\n");
                continue;
            }

            printf("Enter total number of people in the trip: ");
            scanf("%d", &totalPeople);

            for (int i = 0; i < totalPeople; i++) {
                printf("Enter name %d: ", i + 1);
                scanf("%s", allPeople[i]);
            }

            float perPerson = total / totalPeople;

            printf("\n===== EXPENSE SPLIT SUMMARY =====\n");
            printf("Total People: %d\n", totalPeople);
            printf("Total Paid: %d\n", totalPayers);
            printf("Total Expense: ₹%.2f\n", total);
            printf("Each person should pay: ₹%.2f\n\n", perPerson);

            for (int i = 0; i < totalPayers; i++) {
                float diff = spent[i] - perPerson;
                if (diff > 0)
                    printf("%s should RECEIVE ₹%.2f\n", payerNames[i], diff);
                else if (diff < 0)
                    printf("%s should PAY ₹%.2f\n", payerNames[i], -diff);
                else
                    printf("%s is settled.\n", payerNames[i]);
            }

            printf("\nPeople who didn’t pay anything:\n");
            for (int i = 0; i < totalPeople; i++) {
                int paid = 0;

                for (int j = 0; j < totalPayers; j++) {
                    if (strcmp(allPeople[i], payerNames[j]) == 0)
                        paid = 1;
                }

                if (!paid)
                    printf("%s should PAY ₹%.2f\n", allPeople[i], perPerson);
            }
        }

        else if (choice == 4) {
            printf("Exiting... Data saved.\n");
        }

        else if (choice == 5) {
            if (remove("expenses.txt") == 0)
                printf("All data deleted. Fresh start next time!\n");
            else
                printf("No data found to delete.\n");
            break;
        }

        else {
            printf("Invalid choice! Try again.\n");
        }

    } while (choice != 4);

    return 0;
}

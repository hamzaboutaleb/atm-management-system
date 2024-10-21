#include "header.h"

int updateAccount(struct User u, char *input)
{
    char *sql;
    int acc_id;
    int rc, option;
    sqlite3_stmt *stmt;

    if ((acc_id = atoi(input)) <= 0 && strcmp(input, "0") != 0)
    {
        printf("\n\nUnknown account!!");
        fflush(stdout);
        sleep(1);
        return 1;
    }
    if (!checkExistingAccByUser(u.name, acc_id))
    {
        printf("\n\nUnknown account!!");
        fflush(stdout);
        sleep(1);
        return 1;
    }
    system("clear");
    printf("Which information do you want to update?\n\n[1]- Phone number\n[2]- country\n-> ");
    getPrompt(&input);
    option = atoi(input);
    free(input);
    if (option == 1)
    {
    invalidPhonenumber:
        system("clear");
        printf("Enter the new phone number: ");
        getPrompt(&input);
        if (!strIsInt(input))
        {
            free(input);
            printf("\n\nInvalid phone number!!\n\n");
            fflush(stdout);
            sleep(1);
            goto invalidPhonenumber;
        }
        sql = "UPDATE records SET phone_number = ? WHERE user_name = ? AND account_id = ?";
    }
    else if (option == 2)
    {
        system("clear");
        printf("Enter the new country: ");
        getPrompt(&input);
        sql = "UPDATE records SET country = ? WHERE user_name = ? AND account_id = ?";
    }
    else
    {
        printf("\nInvalid operation!\n");
        fflush(stdout);
        sleep(1);
        return 1;
    }
    dbQuery(db, sql, &stmt);
    dbBindText(stmt, 1, input);
    dbBindText(stmt, 2, u.name);
    dbBindInt(stmt, 3, acc_id);
    if (dbStep(stmt) == SQLITE_DONE)
        return 0;
    dbFinalize(stmt);
    return 1;
}
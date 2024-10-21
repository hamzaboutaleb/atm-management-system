#include "header.h"

static double getAmount(char *user, int id)
{
    char *sql;
    int rc;
    double amount;
    sqlite3_stmt *stmt;

    sql = "SELECT balance FROM records WHERE user_name = ? AND account_id = ?";
    dbQuery(db, sql, &stmt);
    dbBindText(stmt, 1, user);
    dbBindInt(stmt, 2, id);
    rc = dbStep(stmt);
    if (rc == SQLITE_ROW)
    {
        amount = getColumnDouble(stmt, 0);
        dbFinalize(stmt);
        return amount;
    }
    dbFinalize(stmt);
    return 1;
}

int transaction(struct User u, char *input)
{
    char *sql;
    int acc_id;
    int rc;
    double new_amount, old_amount, current_amount;
    sqlite3_stmt *stmt;
    acc_id = atoi(input);
    if (acc_id <= 0 && strcmp(input, "0") != 0)
    {
        printf("\n\nUnknown account!!");
        fflush(stdout);
        sleep(1);
        return false;
    }
    if (!checkExistingAccByUser(u.name, acc_id))
    {
        printf("\n\nUnknown account!!");
        fflush(stdout);
        sleep(1);
        return false;
    }
    if (isFixedAccount(acc_id))
    {
        printf("\n\nYou cannot deposit or withdraw cash in fixed accounts!!\n\n");
        fflush(stdout);
        sleep(1);
        return false;
    }
invalidAmount:
    system("clear");
    printf("Do you want to:\n\t\t[1]- Withdraw\n\t\t[2]- Deposit\n\t\t[3]- Back to the main menu\n-> ");
    getPrompt(&input);
    int option = atoi(input);
    free(input);
    if (option != 1 && option != 2 && option != 3)
    {
        printf("\nInvalid operation!\n");
        fflush(stdout);
        sleep(1);
        goto invalidAmount;
    }
    old_amount = getAmount(u.name, acc_id);

    sql = "UPDATE records SET balance = ? WHERE user_name = ? AND account_id = ?";
    if (option == 1)
    {
        printf("\nEnter the amount you want to withdraw: $");
        getPrompt(&input);
        if (checkAmount(input))
        {
            free(input);
            printf("\n\nInvalid amount!!\n\n");
            fflush(stdout);
            sleep(1);
            goto invalidAmount;
        }
        new_amount = old_amount - strtod(input, NULL);
        free(input);
        if (new_amount < 0)
        {
            printf("\n\nThe amount you chose to withdraw is superior to your available balance!!!");
            fflush(stdout);
            sleep(1);
            return false;
        }
    }
    else if (option == 2)
    {
        printf("\nEnter the amount you want to deposit: $");
        getPrompt(&input);
        if (checkAmount(input))
        {
            free(input);
            printf("\n\nInvalid amount!!\n\n");
            sleep(1);
            goto invalidAmount;
        }
        current_amount = strtod(input, NULL);
        free(input);
        if (current_amount <= 0)
        {
            printf("\n\nThe amount you chose to deposit is invalid!!");
            return false;
        }
        new_amount = old_amount + current_amount;
    }
    else
        return false;
    dbQuery(db, sql, &stmt);
    dbBindDouble(stmt, 1, new_amount);
    dbBindText(stmt, 2, u.name);
    dbBindInt(stmt, 3, acc_id);
    dbStep(stmt);
    if (rc == SQLITE_DONE)
        return true;
    dbFinalize(stmt);
    return false;
}
#include "header.h"

static double getAmount(char *user, int id, int *status)
{
    char *sql;
    int rc;
    const char *type;
    double amount;
    sqlite3_stmt *stmt;

    sql = "SELECT balance, type_of_account FROM records WHERE user_name = ? AND account_id = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_bind_int(stmt, 2, id);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        amount = sqlite3_column_double(stmt, 0);
        type = sqlite3_column_text(stmt, 1);
        if (!strcmp(type, "fixed01") || !strcmp(type, "fixed02") || !strcmp(type, "fixed03"))
            *status = 1;
        return amount;
    }
    handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    return 1;
}

int transaction(struct User u, char *input)
{
    char *sql;
    int acc_id;
    int status;
    int rc;
    double new_amount, old_amount, current_amount;
    sqlite3_stmt *stmt;

    if ((acc_id = atoi(input)) <= 0 && strcmp(input, "0") != 0)
    {
        printf("\n\nUnknown account!!");
        fflush(stdout);
        sleep(1);
        return 1;
    }
    if (checkExistingAccByUser(u.name, acc_id))
    {
        printf("\n\nUnknown account!!");
        fflush(stdout);
        sleep(1);
        return 1;
    }
invalidAmount:
    status = 0;
    old_amount = getAmount(u.name, acc_id, &status);
    if (status && strcmp(input, "3") != 0)
    {
        printf("\n\nYou cannot deposit or withdraw cash in fixed accounts!!\n\n");
        fflush(stdout);
        sleep(1);
        return 1;
    }
    system("clear");
    printf("Do you want to:\n\t\t[1]- Withdraw\n\t\t[2]- Deposit\n\t\t[3]- Back to the main menu\n-> ");
    getPrompt(&input);
    if (strcmp(input, "1") != 0 && strcmp(input, "2") != 0 && strcmp(input, "3") != 0)
    {
        printf("\nInvalid operation!\n");
        fflush(stdout);
        sleep(1);
        goto invalidAmount;
    }
    sql = "UPDATE records SET balance = ? WHERE user_name = ? AND account_id = ?";
    if (strcmp(input, "1") == 0)
    {
        printf("\nEnter the amount you want to withdraw: $");
        getPrompt(&input);
        if (checkAmount(input))
        {
            printf("\n\nInvalid amount!!\n\n");
            fflush(stdout);
            sleep(1);
            goto invalidAmount;
        }
        new_amount = old_amount - strtod(input, NULL);
        if (new_amount < 0)
        {
            printf("\n\nThe amount you chose to withdraw is superior to your available balance!!!");
            fflush(stdout);
            sleep(1);
            return 1;
        }
    }
    else if (strcmp(input, "2") == 0)
    {
        printf("\nEnter the amount you want to deposit: $");
        getPrompt(&input);
        if (checkAmount(input))
        {
            printf("\n\nInvalid amount!!\n\n");
            sleep(1);
            goto invalidAmount;
        }
        current_amount = strtod(input, NULL);
        if (current_amount <= 0)
        {
            printf("\n\nThe amount you chose to deposit is invalid!!");
            return 1;
        }
        new_amount = old_amount + current_amount;
    }
    else
        return 1;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_bind_double(stmt, 1, new_amount);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_bind_text(stmt, 2, u.name, -1, SQLITE_STATIC);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_bind_int(stmt, 3, acc_id);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE)
        return 0;
    else
        handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    sqlite3_finalize(stmt);
    return 1;
}
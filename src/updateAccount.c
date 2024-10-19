#include "header.h"

int updateAccount(struct User u, char *input)
{
    char *sql;
    int acc_id;
    int rc;
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
    system("clear");
    printf("Which information do you want to update?\n\n[1]- Phone number\n[2]- country\n-> ");
    getPrompt(&input);
    if (strcmp(input, "1") == 0)
    {
invalidPhonenumber:
        system("clear");
        printf("Enter the new phone number: ");
        getPrompt(&input);
        if (strIsInt(input))
        {
            printf("\n\nInvalid phone number!!\n\n");
            fflush(stdout);
            sleep(1);
            goto invalidPhonenumber;
        }
        sql = "UPDATE records SET phone_number = ? WHERE user_name = ? AND account_id = ?";
    } else if (strcmp(input, "2") == 0)
    {
        system("clear");
        printf("Enter the new country: ");
        getPrompt(&input);
        sql = "UPDATE records SET country = ? WHERE user_name = ? AND account_id = ?";
    } else
    {
        printf("\nInvalid operation!\n");
        fflush(stdout);
        sleep(1);
        return 1;
    }
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_bind_text(stmt, 1, input, -1, SQLITE_STATIC);
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
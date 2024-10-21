#include "header.h"

int transfer(struct User u, char *input)
{
    char *sql;
    int acc_id;
    int user_id;
    int status;
    int rc;
    sqlite3_stmt *stmt;

    if ((acc_id = atoi(input)) <= 0 && strcmp(input, "0") != 0)
    {
        printf("\n\nUnknown account!!");
        fflush(stdout);
        sleep(1);
        return false;
    }
    if (checkAccount(u, input, 2))
    {
        fflush(stdout);
        sleep(1);
        return false;
    }
invalidUser:
    printf("\nWhich user you want to transfer ownership to (username): ");
    getPrompt(&input);
    if (!checkExistingUser(input, &user_id))
    {
        free(input);
        printf("\n\nUnknown user!!");
        fflush(stdout);
        sleep(1);
        goto invalidUser;
    }
    sql = "UPDATE records SET user_name = ?, user_id = ? WHERE user_name = ? AND account_id = ?";
    dbQuery(db, sql, &stmt);
    dbBindText(stmt, 1, input);
    dbBindInt(stmt, 2, user_id);
    dbBindText(stmt, 3, u.name);
    dbBindInt(stmt, 4, acc_id);
    rc = dbStep(stmt);
    dbFinalize(stmt);
    free(input);
    if (rc == SQLITE_DONE)
        return true;
    return false;
}
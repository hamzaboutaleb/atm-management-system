#include "header.h"

int transfer(struct User u, char * input)
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
        return 1;
    }
    if (checkAccount(u, input, 2))
    {
        fflush(stdout);
        sleep(1);
        return 1;
    }
invalidUser:
    printf("\nWhich user you want to transfer ownership to (username): ");
    getPrompt(&input);
    if (checkExistingUser(input, &user_id))
    {
        printf("\n\nUnknown user!!");
        fflush(stdout);
        sleep(1);
        goto invalidUser;
    }
    sql = "UPDATE records SET user_name = ?, user_id = ? WHERE user_name = ? AND account_id = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_bind_text(stmt, 1, input, -1, SQLITE_STATIC);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_bind_int(stmt, 2, user_id);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_bind_text(stmt, 3, u.name, -1, SQLITE_STATIC);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_bind_int(stmt, 4, acc_id);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE)
        return 0;
    else
        handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    sqlite3_finalize(stmt);
    return 1;
}
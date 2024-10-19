#include "header.h"

int deleteAccount(struct User u, char *input)
{
    char *sql;
    int acc_id;
    int rc;
    sqlite3_stmt *stmt;

    if (checkAccount(u, input, 1))
        return 1;
    acc_id = atoi(input);
    sql = "DELETE FROM records WHERE user_name = ? AND account_id = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_bind_text(stmt, 1, u.name, -1, SQLITE_STATIC);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_bind_int(stmt, 2, acc_id);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE)
        return 0;
    else
        handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    sqlite3_finalize(stmt);
    return 1;
}
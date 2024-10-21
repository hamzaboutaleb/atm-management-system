#include "header.h"

void registerMenu(struct User **u)
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    getPrompt(&(*u)->name);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    getPrompt(&(*u)->password);
    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
}

int insertUser(struct User u)
{
    char *sql;
    int rc;
    sqlite3_stmt *stmt;
    if (userNameExist(u.name))
        return 1;
    sql = "INSERT INTO users (name, pass) VALUES (?, ?)";
    dbQuery(db, sql, &stmt);
    dbBindText(stmt, 1, u.name);
    dbBindText(stmt, 2, u.password);

    rc = dbStep(stmt);
    if (rc == SQLITE_CONSTRAINT)
        return 1;

    dbFinalize(stmt);
    return 0;
}

int userNameExist(char *username)
{
    char *sql;
    int rc, exists = 0;
    sqlite3_stmt *stmt;

    sql = "SELECT COUNT(*) FROM users WHERE name = ?";
    dbQuery(db, sql, &stmt);
    dbBindText(stmt, 1, username);
    if (dbStep(stmt) == SQLITE_ROW)
        exists = getColumnInt(stmt, 0) > 0 ? 1 : 0;
    rc = sqlite3_finalize(stmt);
    printf("%d\n", rc);
    return exists;
}
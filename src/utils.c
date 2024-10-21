#include "header.h"

void getPrompt(char **str)
{
    *str = NULL;
    size_t size = 0;
    ssize_t chars_read;

    chars_read = getline(str, &size, stdin);
    if (chars_read < 0)
    {
        fprintf(stderr, "getline error\n");
        exit(1);
    }
    if (chars_read > 0 && (*str)[chars_read - 1] == '\n')
        (*str)[chars_read - 1] = '\0';
}

static int countChar(char *str, int index, char delim)
{
    while (str[index] && str[index] != delim)
        index++;
    return index;
}
static int countWord(char *str, char delim)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (str[i])
    {
        if (str[i] == delim || !str[i + 1])
            count++;
        i++;
    }
    return count;
}

static void arrMemSet(char **arr, int len)
{
    while (len >= 0)
        arr[len--] = NULL;
}

static void fillStr(char *toFill, char *filler, char delim, int index)
{
    int i;

    i = 0;
    while (filler[index] && filler[index] != delim)
    {
        toFill[i] = filler[index];
        index++;
        i++;
    }
}
char **split(char *str, char delim)
{
    char **result;
    int i, j;
    int delim_index;
    int len;

    i = 0;
    j = 0;
    delim_index = 0;
    if ((len = countWord(str, delim)) == 0)
        return NULL;
    result = malloc(sizeof(char *) * (len + 1));
    arrMemSet(result, len);
    while (j < len)
    {
        delim_index = countChar(str, i, delim);
        result[j] = malloc(delim_index + 1);
        memset(result[j], '\0', delim_index + 1);
        fillStr(result[j++], str, delim, i);
        i = delim_index + 1;
    }
    return result;
}

void getDateFromStrs(char **str, struct Date *date)
{
    date->month = atoi(str[0]);
    date->day = atoi(str[1]);
    date->year = atoi(str[2]);
}

int countArr(char **arr)
{
    int i;

    i = -1;
    while (arr[++i] != NULL)
        ;
    return i;
}

void handleDbError(int rc, sqlite3 *db, const char *errmsg)
{
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        if (errmsg)
            sqlite3_free((void *)errmsg);
        sqlite3_close(db);
        exit(1);
    }
}

void handleStatementError(int rc, sqlite3 *db, const char *errmsg, sqlite3_stmt *stmt)
{
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        if (errmsg)
            sqlite3_free((void *)errmsg);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }
}

int checkExistingAccByUser(char *user, int id)
{
    char *sql;
    int rc, exist = false;
    sqlite3_stmt *stmt;

    sql = "SELECT COUNT(*) FROM records WHERE user_name = ? AND account_id = ?";
    dbQuery(db, sql, &stmt);
    dbBindText(stmt, 1, user);
    dbBindInt(stmt, 2, id);
    rc = dbStep(stmt);
    if (rc == SQLITE_ROW)
    {
        exist = getColumnInt(stmt, 0) > 0 ? true : false;
    }
    return exist;
}

int checkAmount(char *str)
{
    char *tmp;
    double res;

    res = strtod(str, &tmp);
    if (res <= 0 || strlen(tmp) != 0)
        return 1;
    return 0;
}

int checkExistingUser(char *user, int *user_id)
{
    char *sql;
    int rc;
    sqlite3_stmt *stmt;

    sql = "SELECT id FROM users WHERE name = ?;";
    dbQuery(db, sql, &stmt);
    dbBindText(stmt, 1, user);
    rc = dbStep(stmt);
    if (rc == SQLITE_ROW)
    {
        *user_id = getColumnInt(stmt, 0);
        dbFinalize(stmt);
        return true;
    }
    else if (rc == SQLITE_DONE)
        return false;
    return false;
}

int strIsInt(char *str)
{
    int i;

    i = 0;
    if (!strlen(str))
        return 0;
    while (str[i])
    {
        if (!isdigit(str[i]))
            return 0;
        i++;
    }
    return 1;
}

int checkExistingAcc(int id)
{
    sqlite3_stmt *stmt;
    int exists = 0;

    const char *query = "SELECT COUNT(*) FROM records WHERE account_id = ?";

    if (dbQuery(db, query, &stmt) == SQLITE_OK)
    {
        dbBindInt(stmt, 1, id);
        if (dbStep(stmt) == SQLITE_ROW)
        {
            exists = getColumnInt(stmt, 0);
        }
        dbFinalize(stmt);
    }
    return exists > 0 ? 1 : 0;
}

void success(struct User u)
{
    int option;
    char *input;

    sleep(1);
invalid:
    printf("\n\nEnter 1 to go to the main menu and 0 to exit!\n");
    getPrompt(&input);
    option = atoi(input);
    if (option <= 0 && strcmp(input, "0") != 0)
    {
        option = 2;
    }
    free(input);
    switch (option)
    {
    case 1:
        mainMenu(u);
        break;
    case 0:
        freeUser(&u);
        sqlite3_close(db);
        exit(0);
    default:
        printf("Insert a valid operation!\n");
        fflush(stdout);
        sleep(1);
        system("clear");
        goto invalid;
    }
}

int validate_date(const char *date)
{
    regex_t regex;
    int ret;

    const char *pattern = "^(0[1-9]|1[0-2])/(0[1-9]|[12][0-9]|3[01])/([0-9]{4})$";
    ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret)
    {
        fprintf(stderr, "Could not compile regex\n");
        return 0;
    }

    ret = regexec(&regex, date, 0, NULL, 0);
    regfree(&regex);
    return (ret == 0);
}

void freeUser(struct User *u)
{
    if (u->name)
        free(u->name);
    if (u->password)
        free(u->password);
}

void freeRecord(struct Record *r)
{
    if (r->accountType)
        free(r->accountType);
    if (r->country)
        free(r->country);
    if (r->deposit)
        free(r->deposit);
    if (r->name)
        free(r->name);
    if (r->phone)
        free(r->phone);
}

bool isFixedAccount(int id)
{
    sqlite3_stmt *stmt;
    char *query;
    char *type;

    query = "SELECT type_of_account FROM records WHERE account_id = ?";
    dbQuery(db, query, &stmt);
    dbBindInt(stmt, 1, id);
    if (dbStep(stmt) == SQLITE_ROW)
    {
        type = getColumnText(stmt, 0);
        if (!strcmp(type, "fixed01") || !strcmp(type, "fixed02") || !strcmp(type, "fixed03"))
            return true;
    }
    return false;
}

void getTodayDate(struct Date *d, char **buffer)
{
    time_t now;
    struct tm *local;
    *buffer = malloc(sizeof(char) * 20);

    time(&now);
    local = localtime(&now);
    d->day = local->tm_mday;
    d->year = local->tm_year + 1900;
    d->month = local->tm_mon + 1;
    strftime(*buffer, 20, "%m/%d/%Y", local);
}
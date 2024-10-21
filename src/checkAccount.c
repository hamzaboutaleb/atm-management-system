#include "header.h"

void checkAllAccounts(struct User u)
{
    char *sql;
    sqlite3_stmt *stmt;

    sql = "SELECT * FROM records WHERE user_name = ?";
    dbQuery(db, sql, &stmt);
    dbBindText(stmt, 1, u.name);
    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (dbStep(stmt) == SQLITE_ROW)
    {
        printf("_____________________\n");
        printRecord(stmt);
    }
    dbFinalize(stmt);
    success(u);
}

static void getInterest(const char *accType, double amount, char **date)
{
    double interest;
    int type_of_fixed;

    type_of_fixed = 0;
    interest = 0.0;
    if (strcmp(accType, "current") == 0)
    {
        printf("\n\nYou will not get interests because the account is of type current\n");
        fflush(stdout);
        return;
    }
    else if (strcmp(accType, "saving") == 0)
    {
        printf("\n\nYou will get %.2f as interest on day %s of every month\n", amount * (0.07 / 12), date[1]);
        fflush(stdout);
        return;
    }
    else if (strcmp(accType, "fixed01") == 0)
    {
        type_of_fixed = 1;
        interest = amount * (0.04 / 12);
    }
    else if (strcmp(accType, "fixed02") == 0)
    {
        type_of_fixed = 2;
        interest = amount * (0.05 / 12);
    }
    else
    {
        type_of_fixed = 3;
        interest = amount * (0.08 / 12);
    }
    printf("\n\nYou will get %.2f as interest on %s/%s/%d\n", interest * (type_of_fixed * 12), date[0], date[1], atoi(date[2]) + type_of_fixed);
}

void printRecord(sqlite3_stmt *stmt)
{
    int user_id = getColumnInt(stmt, 1);
    const char *name = getColumnText(stmt, 2);
    int account_id = getColumnInt(stmt, 3);
    const char *date_of_creation = getColumnText(stmt, 4);
    const char *country = getColumnText(stmt, 5);
    const char *phone_number = getColumnText(stmt, 6);
    double amount = getColumnDouble(stmt, 7);
    const char *accType = getColumnText(stmt, 8);
    printf("\nAccount number:%d\nDeposit Date:%s \ncountry:%s \nPhone number:%s \nAmount deposited: $%.2f \nType Of Account:%s\n",
           account_id, date_of_creation, country, phone_number, amount, accType);
}

int checkAccount(struct User u, char *input, int op)
{
    char *sql;
    int rc;
    int acc_id;
    char **date;
    sqlite3_stmt *stmt;

    if ((acc_id = atoi(input)) <= 0 && strcmp(input, "0") != 0)
    {
        printf("\n\nUnknown account!!");
        fflush(stdout);
        sleep(1);
        return 1;
    }
    sql = "SELECT * FROM records WHERE user_name = ? AND account_id = ?";
    dbQuery(db, sql, &stmt);
    dbBindText(stmt, 1, u.name);
    dbBindInt(stmt, 2, acc_id);
    if (dbStep(stmt) == SQLITE_ROW)
    {
        if (op == 1)
        {
            system("clear");
            printf("\t\t====== Deleted account =====\n\n");
            const char *date_of_creation = sqlite3_column_text(stmt, 4);
            double amount = sqlite3_column_double(stmt, 7);
            const char *accType = sqlite3_column_text(stmt, 8);
            printRecord(stmt);
            sleep(1);
        }
        else if (op == 2)
        {
            system("clear");
            printf("\t\t====== Transfering account N° %d =====\n\n", acc_id);
            const char *date_of_creation = sqlite3_column_text(stmt, 4);
            double amount = sqlite3_column_double(stmt, 7);
            const char *accType = sqlite3_column_text(stmt, 8);
            printRecord(stmt);
            sleep(1);
        }
        else
        {
            system("clear");
            printf("\t\t====== account N° %d =====\n\n", acc_id);
            const char *date_of_creation = getColumnText(stmt, 4);
            double amount = getColumnDouble(stmt, 7);
            const char *accType = getColumnText(stmt, 8);
            printRecord(stmt);
            date = split((char *)date_of_creation, '/');
            getInterest(accType, amount, date);
            success(u);
        }
    }
    else
    {
        printf("\n\nUnknown account!!");
        fflush(stdout);
        sleep(1);
        return 1;
    }
    return 0;
}
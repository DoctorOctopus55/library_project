#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED
#include "book.h"

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 20
#define MAX_BORROWED_BOOKS 3
#define MAX_NUMBER_OF_BORROWED_BOOKS 3
#define WARNED -1

typedef struct{
    char userName[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    int borrowedBooks[MAX_BORROWED_BOOKS];
    int borrowDays[MAX_NUMBER_OF_BORROWED_BOOKS]; //the day that borrowed books were borrowed 1-365
    int numBorrow;
}RegularUser;

typedef struct{
    char userName[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
}AdminUser;

void borrowBook(RegularUser *rglUser, Books *book);
void returnBook(RegularUser *rglUser, Books *book);
void warnUser(RegularUser rglUser);
void writeRegularUser(FILE *fp, RegularUser user);
void displayBooks();
int searchRegularUserByName(RegularUser rglUser);

#endif // USER_H_INCLUDED

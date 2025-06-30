#ifndef BOOK_H_INCLUDED
#define BOOK_H_INCLUDED

#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50
#define MAX_USERNAME_LENGTH 50
#define MAX_CATEGORY_LENGTH 50

#define BOOK_PATH "books/books.txt"
#define TEMP_BOOK_PATH "books/tmp.txt"


typedef struct{
    int id;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    char category[MAX_CATEGORY_LENGTH];
    int isBorrowed; //is in the shelf or not
    char borrower[MAX_USERNAME_LENGTH]; //the name of the one who borrowed
    int borrowDay;  //the day it is borrowed. 1-365
}Books;

void searchBook(Books *book);
void addBook(Books *book);
void removeBook(Books book);
void updateBookManually(Books *updatingbook, Books mirrorBook);
void displayBooksBy(Books book, int byWhat);
void writeBook(FILE *fp, Books book);

int isBookAvailable(Books book);
int searchBookById(Books book);

#endif // BOOK_H_INCLUDED

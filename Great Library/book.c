#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h"
#include "user.h"
#include "fileProcess.h"

//book-> id, title, author, category, isBorrowed, borrowedBy, borrowDay
void writeBook(FILE *fp, Books book){
    fprintf(fp, "%d|%s|%s|%s|%d|%s|%d\n", book.id, book.title, book.author, book.category, book.isBorrowed, book.borrower, book.borrowDay);
} //writes the file with the given template. Its used in various functions.


void searchBook(Books *book){ //searches book by the id and overwrites everything on the input Book variable, based on the files.
    FILE *fp=fopen(BOOK_PATH,"r");
    if(fp==NULL)
        perror("File Not Found: ");

    Books tmp;
    int found=0;
    while(readBookFile(fp, &tmp)==7){ //read book, with a previously declared function (it was !=EOF before)
        if(book->id==tmp.id){ //this if block overwrites the parameter variable, ifthe ids match
            strcpy(book->author, tmp.author);
            strcpy(book->title, tmp.title);
            strcpy(book->borrower, tmp.borrower);
            strcpy(book->category, tmp.category);
            book->isBorrowed=tmp.isBorrowed;
            book->borrowDay=tmp.borrowDay;
            book->id=tmp.id;
            found=1;
        }
    }
    if(!found) //if no id matches with any id inside of the file
        printf("Book Couldnt Found");

    fclose(fp);
}


void addBook(Books *book){ //adds book to the file, if it doesnt exist
    FILE *fp=fopen(BOOK_PATH, "r");
    if (fp==NULL)
        perror("File not found: ");

    int idExists = 0, index=0;
    Books temp; //read and overwrite this each line; if the ids match, the whole function stops adding process

    while(readBookFile(fp, &temp)==7){ //reads book file, if its 7 variable found in the file, then the function returns 9 and while interval works properly
        if (temp.id==book->id){
            idExists=1;
            break;
        }
        index++;
    }
    fclose(fp);

    FILE *fp_write=fopen(BOOK_PATH, "a");
    if (fp_write==NULL)
        perror("File not found: ");

    if(!idExists){
        fseek(fp_write, 0, SEEK_END); // move to end before writing
        writeBook(fp_write, *book); //if id doesnt exist, it will be written
        printf("The Book is Added\n");
    }else
        printf("Book with ID %d already exists.\n", book->id);
    fclose(fp_write);

}


void removeBook(Books book){ //remove books by the given book. Giving id for the book parameter is enough for this function to work.
    FILE *fp=fopen(BOOK_PATH, "r");
    FILE *tmfp=fopen(TEMP_BOOK_PATH, "w");

    if(fp==NULL || tmfp==NULL)
        perror("One of the files isnt not found: ");

    Books tmpBook;
    while(readBookFile(fp, &tmpBook)==7){
        if(tmpBook.id==book.id && book.isBorrowed==0) //skips the book while writing to the new file, if the ids match and book isnt borrowed
            printf("The Id:%d\nIf It has Borrowed:%d\nRemoval accomplished\n", book.id, book.isBorrowed);
        else
            writeBook(tmfp, tmpBook);
    }

    fclose(fp);
    fclose(tmfp);

    if(remove(BOOK_PATH)!=0) //if remove process doesnt get completed, it will be catched and shown in the console.
        perror("Error deleting books.txt");

    if(rename(TEMP_BOOK_PATH, BOOK_PATH)!= 0) //if rename process doesnt get completed, it will be catched and shown in the console
        perror("Error renaming tmp.txt to books.txt");
}


void updateBookManually(Books *updatingBook, Books mirrorBook){ //updates given book, by a mirror book. It updates both the first parameter and the file
    FILE *fp=fopen(BOOK_PATH, "r");
    if(fp==NULL)
        perror("File is not found: ");

    FILE *ftmp=fopen(TEMP_BOOK_PATH, "w");
    if(ftmp==NULL)
        perror("File is not found: ");

    Books tmpBook;
    int isFound=0;
    while(readBookFile(fp, &tmpBook)==7){ //so long as the dataset template is proper, this should work
        if(updatingBook->id==tmpBook.id){ //this block updates, if ids match
            tmpBook.id=mirrorBook.id;
            tmpBook.isBorrowed=mirrorBook.isBorrowed;
            tmpBook.borrowDay=mirrorBook.borrowDay;

            strcpy(tmpBook.title, mirrorBook.title);
            strcpy(tmpBook.author, mirrorBook.author);
            strcpy(tmpBook.category, mirrorBook.category);
            strcpy(tmpBook.borrower, mirrorBook.borrower);
            isFound=1;
            }
        writeBook(ftmp, tmpBook); //writes no matter the id is found or not
    }
    if(isFound==1)
        printf("Successfully changed\n");
    else
        printf("File could not be changed\n");
    fclose(fp);
    fclose(ftmp);

    if(remove(BOOK_PATH)!=0) //catch problems if removal process doesnt get completed
        perror("Error deleting books.txt");

    if(rename(TEMP_BOOK_PATH, BOOK_PATH)!= 0) //catch problems if renaming process doesnt get completed
        perror("Error renaming tmp.txt to books.txt");
}


int isBookAvailable(Books book){ //finds if the book is available or not, then returns true or false
    FILE *fp=fopen(BOOK_PATH, "r");
    if(fp==NULL)
        perror("File couldnt be found: ");

    Books temp;
    int isAvailable=0, isFound=0;
    while(readBookFile(fp, &temp)==7){ //readBookFile functions should always return 7, if it doesnt; then there is a problem with the dataset
        if(book.id==temp.id){
            isAvailable=!(temp.isBorrowed); //if a book found on the files by the same id, the book is checked by the isBorrowed. If is Borrowed true, then it must be not avaible
            isFound=1;
            break;
        }
    }
    if(!isFound)
        printf("The book with %d id couldnt be found\n", book.id);

    fclose(fp);
    return isAvailable;
}


int searchBookById(Books book){ //searches books by the id; if its found in the file, it returns true, otherwise false
    FILE *fp=fopen(BOOK_PATH, "r");
    if(fp==NULL)
        perror("File couldnt found: ");

    Books temp;
    int isAvailable=0;
    while(readBookFile(fp, &temp)==7){ //readBookFile should always return 7; if it doesnt return that, then there must be a problem with the dataset
        if (book.id == temp.id) {
            isAvailable = temp.isBorrowed;
            break;
        }
    }
    fclose(fp);
    return isAvailable;
}


void displayBooksBy(Books book, int byWhat){ //displays books by what user wants. Everything is checked.
    FILE *fp=fopen(BOOK_PATH, "r");
    if(fp==NULL)
        perror("File couldnt be found: ");

    Books tempBook;
    int index=0, isFound=0;
    switch(byWhat){
        case 1: //displays books with the same name
            while(readBookFile(fp, &tempBook)==7){
                if(strcmp(book.title, tempBook.title)==0){
                    printf("%d. BOOK NAME: '%s' BOOK AUTHOR:%s BOOK CATEGORY: '%s' IS BORROWED: %d\n", ++index, tempBook.title, tempBook.author, tempBook.category, tempBook.isBorrowed);
                    isFound=1;
                }
            }
            if(isFound==0)
                printf("Couldnt be found any book");
            break;
        case 2: //displays books with the same author
            while(readBookFile(fp, &tempBook)==7){
                if(strcmp(book.author, tempBook.author)==0){
                    printf("%d. BOOK NAME: '%s' BOOK AUTHOR: '%s' BOOK CATEGORY: '%s' IS BORROWED: %d\n", ++index, tempBook.title, tempBook.author, tempBook.category, tempBook.isBorrowed);
                    isFound=1;
                }
            }
            if(isFound==0)
                printf("Couldnt be found any book\n");
            break;
        case 3: //displays books with the same category
            while(readBookFile(fp, &tempBook)!=EOF){
                if(strcmp(book.category, tempBook.category)==0){
                    printf("%d. BOOK NAME: '%s' BOOK AUTHOR: '%s' BOOK CATEGORY: '%s' IS BORROWED: %d\n", ++index, tempBook.title, tempBook.author, tempBook.category, tempBook.isBorrowed);
                    isFound=1;
                }
            }
            if(isFound==0)
                printf("Couldnt be found any book\n");
            break;
        default:
            printf("Outside the Range of the function\n");
            break;
    }
    fclose(fp);
}

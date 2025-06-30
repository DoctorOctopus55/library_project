#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "user.h"
#include "book.h"
#include "fileProcess.h"
#include "rapor.h"

void writeRegularUser(FILE *fp, RegularUser user){ //writes regular users by the given template
    fprintf(fp, "%s|%s|%d|%d|%d|%d|%d|%d|%d\n",
                user.userName, user.password,
                user.borrowedBooks[0], user.borrowDays[0],
                user.borrowedBooks[1], user.borrowDays[1],
                user.borrowedBooks[2], user.borrowDays[2],
                user.numBorrow);
}


void borrowBook(RegularUser *rglUser, Books *book){ //borrowing book process
    FILE *readbook=fopen(BOOK_PATH, "r"); //open book file for reading
    FILE *writebook=fopen(TEMP_BOOK_PATH, "w"); //open book file for writing
    FILE *readuser=fopen(REGULAR_USER_PATH, "r"); //open user file for reading
    FILE *writeuser=fopen(TEMPORARY_REGULAR_USER_PATH, "w"); //open user file for writing

    if(readbook==NULL || writebook==NULL || readuser==NULL || writeuser==NULL){
        perror("File couldnt be found: ");
        return;
    }
    Books temp;
    int today=getTodayDayOfYear(); //looks at the calendar and finds the day of the year
    while(readBookFile(readbook, &temp)==7){ //readBook should always return 7
        if(temp.id==book->id && temp.isBorrowed==0 && book->isBorrowed==0){ //if id matches, update the user for borrowing and update the book information
            temp.isBorrowed=1; //update the book's isBorrowed parameter to a true, which is 1
            temp.borrowDay=today; //updates when did it borrow.
            strcpy(temp.borrower, rglUser->userName); //update book's borrower

            rglUser->borrowedBooks[rglUser->numBorrow]=temp.id; //update user's borrowed books with the id
            rglUser->borrowDays[rglUser->numBorrow]=today; //update the books borrowday on the user's parameter with today
            rglUser->numBorrow++; //increase the number of books that this user borrowed
        }
        writeBook(writebook, temp); //write book line by line, if the block at the top doesnt entered, nothing will be updated
    }

    RegularUser tmpUser; //declare a temporary user
    while(readRegularUserFile(readuser, &tmpUser)==9){ //read each line of the user file
        if(strcmp(rglUser->userName, tmpUser.userName)==0){ //if usernames match, update the temporary user. Temporary user will be written straigtly to the new file
            tmpUser.borrowedBooks[tmpUser.numBorrow]=rglUser->borrowedBooks[rglUser->numBorrow-1]; //regular users number of borrowed books just increased in line 39, because of this, we want to use 'borrowedBooks[numBorrow-1]'
            tmpUser.borrowDays[tmpUser.numBorrow]=rglUser->borrowDays[rglUser->numBorrow-1]; //same logic as line 47
            tmpUser.numBorrow=rglUser->numBorrow; //update the number of books on the file
        }
        writeRegularUser(writeuser, tmpUser); //update the user that borrowed a book
    }
    fclose(readbook);
    fclose(writebook);
    fclose(readuser);
    fclose(writeuser);

    if(remove(BOOK_PATH)!=0)
        perror("Error deleting books.txt");

    if(rename(TEMP_BOOK_PATH, BOOK_PATH)!= 0)
        perror("Error renaming tmp.txt to books.txt");

    if(remove(REGULAR_USER_PATH)!=0)
        perror("Error deleting regularUsers.txt");

    if(rename(TEMPORARY_REGULAR_USER_PATH, REGULAR_USER_PATH)!= 0)
        perror("Error renaming tmp.txt to regularUsers.txt");
}


void returnBook(RegularUser *rglUser, Books *book){ //returning book process
    FILE *readbook=fopen(BOOK_PATH, "r"); //open book file for reading
    FILE *writebook=fopen(TEMP_BOOK_PATH, "w"); //open book file for writing
    FILE *readuser=fopen(REGULAR_USER_PATH, "r"); //open user file for reading
    FILE *writeuser=fopen(TEMPORARY_REGULAR_USER_PATH, "w"); //open user file for writing

    if(readbook==NULL || writebook==NULL || readuser==NULL || writeuser==NULL){
        perror("File or files couldnt be found: ");
        return;
    }

    Books temp;
    while(readBookFile(readbook, &temp)==7){ //reads from the first file, writes to a temporary one
        if(temp.id==book->id && temp.isBorrowed==1 && book->isBorrowed==1){ //checks if the book is actually borrowed, and the user is actually present on the dataset
            temp.isBorrowed=0; //update book's isBorrowed parameter as false. Which means its no longer occupied.
            strcpy(temp.borrower, "x"); //update the books borrower as no one, which is x
            temp.borrowDay=0; //the book isnt borrowed, so the day that borrowed must be 0
        }
        writeBook(writebook, temp); //write each line, creating a new temporary file
    }
    RegularUser tmpuser;
    int isFound=0;
    while(readRegularUserFile(readuser, &tmpuser)==9){ //reads from the first file, writes to a temporary file
        if(strcmp(tmpuser.userName, rglUser->userName)==0){ //checks if user's name is present on the dataset
            for(int i=0; i<3;i++){ //checks each of the book that can be borrowed, when the book that is getting returned found, it will be updated
                if(tmpuser.borrowedBooks[i]==book->id){
                    tmpuser.borrowedBooks[i]=0; //book gets removed from this users info
                    tmpuser.borrowDays[i]=0; //book gets removed from this users info
                    tmpuser.numBorrow--; //book gets removed from this users info
                    printf("The id that matches with the book found and returned back to the shelfs\n");
                    isFound=1;
                }
            }
        }
        writeRegularUser(writeuser, tmpuser);
    }
    if(!isFound)
        printf("The book that matches with the id couldnt be found\n");

    fclose(readbook);
    fclose(writebook);
    fclose(readuser);
    fclose(writeuser);

    if(remove(BOOK_PATH)!=0)
        perror("Error deleting books.txt");

    if(rename(TEMP_BOOK_PATH, BOOK_PATH)!= 0)
        perror("Error renaming tmp.txt to books.txt");

    if(remove(REGULAR_USER_PATH)!=0)
        perror("Error deleting regularUsers.txt");

    if(rename(TEMPORARY_REGULAR_USER_PATH, REGULAR_USER_PATH)!= 0)
        perror("Error renaming tmp.txt to regularUsers.txt");
}



void warnUser(RegularUser rglUser){ //warns user; if he is already warned, he gets to be unwarned. numBorrow becomes -1 if the user gets to be warned, if he gets unwarned, then numBorrow updates by the books that currently held by the user
    FILE *fp=fopen(REGULAR_USER_PATH, "r");
    FILE *tmfp=fopen(TEMPORARY_REGULAR_USER_PATH, "w");
    if(fp==NULL || tmfp==NULL)
        perror("The files couldnt have found");

    RegularUser tmpUser;
    int isFound=0;
    while(readRegularUserFile(fp, &tmpUser) == 9){

        if(strcmp(tmpUser.userName, rglUser.userName)==0){
            if(tmpUser.numBorrow!=WARNED){
                tmpUser.numBorrow=WARNED;
                printf("'%s' successfully warned. He will see this once he logs into its account.\nUntil his warn retaken, he wont be able to use the great library.\n", rglUser.userName);
                getchar();
            }
            else{ //if the user is warned, then number of books that he borrowed updates to -1; if he gets unwarned, we must count the books that he borrowed previously and update number of books parameter accordingly
                int index=0;
                for(int i=0;i<3;i++){
                    if(tmpUser.borrowedBooks[i]!=0){
                        index++;
                    }
                }
                tmpUser.numBorrow=index;
                printf("User called '%s' is successfully unwarned. He is not prohibited to use libraries assets anymore.\n", tmpUser.userName);
                getchar();
            }
            isFound=1;
        }
        writeRegularUser(tmfp, tmpUser);
        }

        fclose(fp);
        fclose(tmfp);

        if(isFound==0)
            printf("No name called '%s' found", rglUser.userName);

        if(remove(REGULAR_USER_PATH)!=0)
            perror("Error deleting regularUsers.txt");
        if(rename(TEMPORARY_REGULAR_USER_PATH, REGULAR_USER_PATH)!= 0)
            perror("Error renaming tmp.txt to regularUsers.txt");
}


void displayBooks(){ //diplays all the books
    FILE *fp=fopen(BOOK_PATH, "r");
    if(fp==NULL)
        perror("File couldnt found: ");

    Books book;
    while(readBookFile(fp, &book)==7)
        printf("ID: %d NAME: %s AUTHOR: %s CATEGORY: %s ISBORROWED: %d BORROWED BY: %s BORROWDAY: %d\n", book.id, book.title, book.author, book.category, book.isBorrowed, book.borrower, book.borrowDay);
    fclose(fp);
}


int searchRegularUserByName(RegularUser rglUser){ //checks if a user with given id present on the file
    FILE *fp=fopen(REGULAR_USER_PATH,"r");
    if(fp==NULL)
        perror("File couldnt found: ");


    RegularUser tmpUser;
    while(readRegularUserFile(fp, &tmpUser)==9){
        if(strcmp(tmpUser.userName, rglUser.userName)==0){
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

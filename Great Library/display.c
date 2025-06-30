#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "user.h"
#include "book.h"
#include "fileProcess.h"
#include "rapor.h"
#include "display.h"

void display_welcome_screen();
void admin_screen();
void user_screen();
void clear_input_buffer();

void clear_input_buffer(){ //this cleans the scanf, primarily used after a scanf so that nothing gets stacked.
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void safelyAskNumber(int *choice, int low, int high){ //it allows to ask a number, within a given range. Entering char or string doesnt kill scanf that takes integer
    while(1){ //asks till the input is in a shape that we want
        if (scanf("%d", choice)!= 1){
            while (getchar() != '\n'); // Clear the buffer if scanf fails
            printf("Enter a valid number\n");
        }
        else if(*choice>=high || *choice<=low) //asks until its in the given range
                printf("Enter a valid number inside the range\n");
        else
            break;
    }
}

void display_welcome_screen() {
    char choice;
    int go=1;
    while (go) {
        system("cls||clear"); // Clear screen (works for Windows and Unix)
        printf("\n===== THE GREAT LIBRARY =====\n");
        printf("1. Login as Administrator\n");
        printf("2. Login as Regular User\n");
        printf("3. Exit\n");
        printf("Enter your choice (1-3): ");

        scanf(" %c", &choice);
        clear_input_buffer();

        switch (choice) {
            case '1': //sends to admin login screen
                admin_screen(&go);
                break;
            case '2': //sends to regular user login screen
                user_screen(&go);
                break;
            case '3': //exits the system
                printf("\nExiting system. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();
                break;
        }
    }
}


void admin_screen(int *go){//go allows us to break from the initial interval, which is in display_welcome_screen
    char choice; //for dashboard. It asks user to select an action
    AdminUser adm_usr;

    system("cls||clear");
    printf("\n===== ADMINISTRATOR LOGIN =====\n");
    printf("Username: ");
    scanf("%s", adm_usr.userName);
    printf("Password: ");
    scanf("%s", adm_usr.password);
    clear_input_buffer(); //sometimes scanf traps \n or something like that on buffer zone. Later when we try to use scanf, it brings problem. This solves the problem

    // Very simple authentication (in real app, use proper security)
    if (!searchAdminUser(&adm_usr)) { //if the admin isnt found on the file
        printf("\nInvalid credentials! Access denied.\n");
        printf("Press Enter to return to main menu...");
        getchar();
        return; //goes back to welcome_screen
    }
    //if it passes the block on the top, it enters into this while loop
    while(1){
        system("cls || clear");
        printf("\n===== %s's DASHBOARD =====\n", adm_usr.userName);
        printf("1. Add Book\n");
        printf("2. Remove Book\n");
        printf("3. Update Book\n");
        printf("4. View All Books\n");
        printf("5. Add User\n");
        printf("6. Remove User\n");
        printf("7. Warn A User\n");
        printf("8. Display All the Users\n");
        printf("9. View All the Delayed Books\n");
        printf("0. Logout\n");
        printf("Enter your choice (0-9): ");

        scanf("%c", &choice);
        clear_input_buffer();

        switch(choice){
            case '1': //adding book process
                Books book1;
                printf("\nThe Name of the new Book:\n");
                scanf("%49[^\n]", book1.title);
                clear_input_buffer();

                printf("\nThe Author of the new Book:\n");
                scanf("%19[^\n]", book1.author);
                clear_input_buffer();

                printf("\nThe ID of the new Book (ONLY ENTER NUMBER):\n");
                scanf("%d", &book1.id);
                clear_input_buffer();

                printf("\nThe Category of the new Book:\n");
                scanf("%49[^\n]", book1.category);
                clear_input_buffer();

                book1.isBorrowed=0;
                book1.borrowDay=0;
                strcpy(book1.borrower, "x");
                addBook(&book1);

                printf("Press Enter to continue...");
                getchar();
                break;

            case '2': //removes a book from the file
                Books book2;
                printf("\nThe ID of the book you intend to delete: \n");
                scanf("%d", &book2.id);
                clear_input_buffer();

                removeBook(book2);

                printf("Press Enter to continue...");
                getchar();
                break;
            case '3': //updates a book from the file
                Books book3;
                Books mirrorBook;
                printf("The id of the book you want to update: \n");
                scanf("%d", &book3.id);
                clear_input_buffer();

                searchBook(&book3);
                printf("Here is the original book:\nID:%d NAME:%s AUTHOR:%s CATEGORY:%s IS BORROWED:%d BORROWED BY:%s BORROW DAY:%d\n", book3.id, book3.title, book3.author, book3.category, book3.isBorrowed, book3.borrower, book3.borrowDay);
                printf("---------------------------------------------------------\n");
                printf("The New Name: \n");
                scanf("%49[^\n]", mirrorBook.title);
                clear_input_buffer();

                printf("The new ID: \n");
                scanf("%d", &mirrorBook.id);
                clear_input_buffer();

                printf("The new Author: \n");
                scanf("%49[^\n]", mirrorBook.author);
                clear_input_buffer();

                printf("The New Category: \n");
                scanf("%49[^\n]", mirrorBook.category);
                clear_input_buffer();

                printf("The New Borrow Day: \n");
                scanf("%d", &mirrorBook.borrowDay);
                clear_input_buffer();

                printf("The New Borrower (make sure its a user): \n");
                scanf("%49[^\n]", mirrorBook.borrower);
                clear_input_buffer();

                printf("If it is borrowed or not: ");
                scanf("%d", &mirrorBook.isBorrowed);
                clear_input_buffer();

                updateBookManually(&book3, mirrorBook);
                printf("Press Enter to continue...");
                getchar();
                break;
            case '4': //displays every book
                displayBooks();
                printf("Press Enter to continue...");
                getchar();
                break;
            case '5': //adding a new user process
                RegularUser rgl_usr1;

                while(1){
                    printf("Username: ");
                    scanf("%49[^\n]", rgl_usr1.userName);
                    clear_input_buffer();
                    if(searchRegularUserByName(rgl_usr1)){
                        printf("'%s' is already taken\n", rgl_usr1.userName);
                        continue;
                    }

                    printf("Password: ");
                    scanf("%19[^\n]", rgl_usr1.password);
                    clear_input_buffer();

                    rgl_usr1.borrowDays[0]=0;
                    rgl_usr1.borrowDays[1]=0;
                    rgl_usr1.borrowDays[2]=0;

                    rgl_usr1.borrowedBooks[0]=0;
                    rgl_usr1.borrowedBooks[1]=0;
                    rgl_usr1.borrowedBooks[2]=0;
                    rgl_usr1.numBorrow=0;

                    break;
                }
                addRegularUser(rgl_usr1);

                printf("Press Enter to continue...");
                getchar();
                break;

            case '6': //Process of removing a user
                RegularUser rgl_usr2;
                char choice;

                printf("\nThe Name of The User: \n");
                scanf("%49[^\n]", rgl_usr2.userName);
                clear_input_buffer();

                printf("Are You Sure About Removing '%s' (y/n) ", rgl_usr2.userName);
                scanf("%c", &choice);
                clear_input_buffer();
                if(choice=='y')
                    removeRegularUser(rgl_usr2);

                printf("Press Enter to continue...");
                getchar();
                break;
            case '7': //warn or unwarn process
                RegularUser rgl_usr3;

                printf("Enter the username of the person, you want to warn or unwarn him, if he has one: ");
                scanf("%49[^\n]", rgl_usr3.userName);
                warnUser(rgl_usr3);

                printf("Press Enter to continue...");
                getchar();
                break;
            case '8': //diplay all regular users
                displayRegularUsers();

                printf("Press Enter to continue...");
                getchar();
                break;
            case '9': //check the delays
                checkAllBooksDelay();
                printf("Press Enter to continue...");
                getchar();
                break;
            case '0':
                return; // Return to welcome screen
            default:
                printf("\nInvalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();
                break;
        }
}
}


void user_screen(int *go){
    char choice; //asks from the user for action
    RegularUser rgl_usr; //the user that enters
    int today=getTodayDayOfYear();

    system("cls||clear");
    printf("\n===== USER LOGIN =====\n");
    printf("Username: ");
    scanf("%19s", rgl_usr.userName);
    printf("Password: ");
    scanf("%19s", rgl_usr.password);
    clear_input_buffer();

    if(!searchRegularUser(&rgl_usr)){
        printf("\nInvalid credentials! Access denied.\n");
        printf("Press Enter to return to main menu...");
        getchar();
        return;
    }
    if(searchRegularUser(&rgl_usr) && rgl_usr.numBorrow==WARNED){ //checks if the user is warned or not
        printf("\n======== YOU ARE WARNED =========\n");
        printf("If you want to continue using the great library, you must return back your delayed books.\n");
        printf("You are being warned by the great administors of this library. Next time you might be permanently banned\n");
        printf("Press enter to continue...");
        getchar();
    }
    while (1) {
        system("cls");
        printf("\n===== %s's DASHBOARD =====\n", rgl_usr.userName);
        printf("1. Search Books\n");
        printf("2. Borrow Book\n");
        printf("3. Return Book\n");
        printf("4. View My Borrowed Books\n");
        printf("5. Logout\n");
        printf("Enter your choice (1-5): ");

        scanf(" %c", &choice);
        clear_input_buffer();

        switch (choice) {
            case '1': //searching for book process
                char c;
                printf("\nSearch for book based of name, author or category.\nEnter 1 to search by name.\nEnter 2 to search by author\nEnter 3 to search by category\nEnter 4 to display all the books\n");
                scanf("%c", &c);
                clear_input_buffer();

                switch(c){
                    Books book1;
                    case '1': //show by name
                        printf("Enter the books name: ");
                        scanf("%49[^\n]", book1.title);
                        clear_input_buffer();
                        displayBooksBy(book1, 1);
                        break;
                    case '2': //show by author
                        printf("Enter the books author: ");
                        scanf("%49[^\n]", book1.author);
                        clear_input_buffer();
                        displayBooksBy(book1, 2);
                        break;
                    case '3': //show by category
                        printf("fiction, dystopian, romance, adventure, historical, fantasy, science, philosophy, children, Mystery\nEnter the books category: ");
                        scanf("%49[^\n]", book1.category);
                        clear_input_buffer();
                        displayBooksBy(book1, 3);
                        break;
                    case '4'://show all the books
                        printf("===============================\n");
                        displayBooks();
                        break;
                    default:
                        printf("Enter a valid number.\n");
                        break;
                }
                printf("Press Enter to continue...");
                getchar();
                break;
//------------------------------------------------------------------------------
            case '2': //borrowing process
                Books book2;

                if(rgl_usr.numBorrow==WARNED){ //if there is a warn, then the user isnt allowed to borrow book
                    printf("You are not permitted to borrow any book\n");
                    printf("Press Enter to continue...");
                    getchar();
                    break;
                }
                printf("\nID of the book you want to Borrow(ONLY NUMBER): ");
                scanf("%d", &book2.id);
                clear_input_buffer();

                if(isBookAvailable(book2) && rgl_usr.numBorrow<3){ //if book isnt borrowed and the user doesnt have more than 3 books, the block works
                    searchBook(&book2);
                    searchRegularUser(&rgl_usr);

                    printf("user name: %s number of borrow:%d \n", rgl_usr.userName, rgl_usr.numBorrow);
                    printf("book id: %d book title: %s book author: %s book category: %s\n", book2.id, book2.title, book2.author, book2.category);
                    borrowBook(&rgl_usr, &book2);
                }
                else if(rgl_usr.numBorrow>=3)
                    printf("You are not allowed to borrow more book\n");

                else if(!isBookAvailable(book2))
                    printf("That book is already borrowed\n");

                printf("Press Enter to continue...");
                getchar();
                break;
//------------------------------------------------------------------------------
            case '3': //returning process
                Books book3;
                printf("\nID's of the books you have:\n");
                printf("%s %d %d %d\n", rgl_usr.userName, rgl_usr.borrowedBooks[0], rgl_usr.borrowedBooks[1], rgl_usr.borrowedBooks[2]);
                printf("\nEnter the ID of the book you want to return: ");

                safelyAskNumber(&book3.id, 0, 501);
                clear_input_buffer();

                searchBook(&book3); //writes the blanks in the book3
                returnBook(&rgl_usr, &book3);

                printf("Press Enter to continue...");
                getchar();
                break;
//------------------------------------------------------------------------------
            case '4': //viewing users books that he borrowed
                Books brBook1, brBook2, brBook3;
                brBook1.id=rgl_usr.borrowedBooks[0];
                brBook2.id=rgl_usr.borrowedBooks[1];
                brBook3.id=rgl_usr.borrowedBooks[2];
                printf("Borrowed Books:\n");
                if(brBook1.id!=0){
                    searchBook(&brBook1);
                    printf("NAME: %s AUTHOR: %s CATEGORY: %s IS BORROWED: %d BORROW DAY: %d DELAY: %d\n", brBook1.title, brBook1.author, brBook1.category, brBook1.isBorrowed, brBook1.borrowDay, calculateOverdue(brBook1.borrowDay, today, ALLOWED_DAYS));
                }
                if(brBook2.id!=0){
                    searchBook(&brBook2);
                    printf("NAME: %s AUTHOR: %s CATEGORY: %s IS BORROWED: %d BORROW DAY: %d DELAY: %d\n", brBook2.title, brBook2.author, brBook2.category, brBook2.isBorrowed, brBook2.borrowDay, calculateOverdue(brBook2.borrowDay, today, ALLOWED_DAYS));

                }
                if(brBook3.id!=0){
                    searchBook(&brBook3);
                    printf("NAME: %s AUTHOR: %s CATEGORY: %s IS BORROWED: %d BORROW DAY: %d DELAY: %d\n", brBook3.title, brBook3.author, brBook3.category, brBook3.isBorrowed, brBook3.borrowDay, calculateOverdue(brBook3.borrowDay, today, ALLOWED_DAYS));

                }
                if(brBook1.id==0&&brBook2.id==0&&brBook3.id==0)
                    printf("No book has found that you borrowed before\n");

                printf("Press Enter to continue...");
                getchar();
                break;
//------------------------------------------------------------------------------
            case '5':
                return; // Return to welcome screen
            default:
                printf("\nInvalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();
                break;
        }
    }
}

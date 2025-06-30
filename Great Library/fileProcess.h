#ifndef FILEPROCESS_H_INCLUDED
#define FILEPROCESS_H_INCLUDED

#define REGULAR_USER_PATH "users/regularUsers.txt"
#define ADMIN_USER_PATH "users/adminUsers.txt"
#define TEMPORARY_REGULAR_USER_PATH "users/temp.txt"

int readRegularUserFile(FILE *fp, RegularUser *tmpUser);
int readAdminUserFile(FILE *fp, AdminUser *tmpUser);
int readBookFile(FILE *fp, Books *temp);
int searchRegularUser(RegularUser *rglUser);
int searchAdminUser(AdminUser *admUser);

void addRegularUser(RegularUser rglUser);
void removeRegularUser(RegularUser rglUser);
void displayRegularUsers();


#endif // FILEPROCESS_H_INCLUDED

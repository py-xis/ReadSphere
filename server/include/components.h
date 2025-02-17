#ifndef ALLSTRUCTS_H
#define ALLSTRUCTS_H

struct ISBNNode{
    int isbn;
    int returned;
    struct ISBNNode *next; 
};

struct User{
    int userId;

    char firstName[10];
    char middleName[10];
    char lastName[10];

    char username[10];
    char password[10];

    struct ISBNNode *head;
    int borrowedCount;

    int isAdmin;
};



struct UserNode{
    int userId;
    int returned;
    struct UserNode *next;
};

struct Book{

    char bookName[20];
    char author[40];
    int ISBN;
    
    int totalCopies;
    int availableCopies;

    struct UserNode *head;
};

#endif
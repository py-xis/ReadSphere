#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<pthread.h>
#include "../include/authentication.h"
#include "../include/components.h"
#include "../include/pds.h"
#include "../include/server.h"

int studentId = 0;

void printUser(const struct User *user) {
    printf("User ID: %d\n", user->userId);
    printf("First Name: %s\n", user->firstName);
    printf("Middle Name: %s\n", user->middleName);
    printf("Last Name: %s\n", user->lastName);
    printf("Username: %s\n", user->username);
    printf("Password: %s\n", user->password);
    printf("Borrowed Count: %d\n", user->borrowedCount);
    printf("Is Admin: %s\n", user->isAdmin ? "Yes" : "No");

    // Print borrowed books
    printf("Borrowed Books:\n");
    struct ISBNNode *current = user->head;
    while (current != NULL) {
        printf("  ISBN: %d, Returned: %s\n", current->isbn, current->returned ? "Yes" : "No");
        current = current->next;
    }
}

int
addUserServer(char *firstName, char *middleName, char *lastName, char *username, char *password){

    pthread_mutex_lock(&lock);
    struct User newUser;

    strcpy(newUser.firstName, firstName);
    strcpy(newUser.middleName, middleName);
    strcpy(newUser.lastName, lastName);
    strcpy(newUser.username, username);
    strcpy(newUser.password, password);
    newUser.head = NULL;
    newUser.borrowedCount = 0;
    newUser.isAdmin = 0;

    printUser(&newUser);
    int putStatus = put_rec_by_key(studentId,&newUser, USER);
    printf("addUserServer %d:putStatus %d\n", getpid(), putStatus);
    if(putStatus == PDS_DUPLICATE_KEY_ADD){
        pthread_mutex_unlock(&lock);
        return USER_ALREADY_EXISTS;
    }else if(putStatus == PDS_ADD_FAILED){
        pthread_mutex_unlock(&lock);
        return USER_NOT_ADDED;
    }else if(putStatus == PDS_REPO_NOT_OPEN){
        pthread_mutex_unlock(&lock);
        return REPO_ERROR;
    }
    pthread_mutex_unlock(&lock);
    return USER_ADDED;
}

int match_username(void *rec, void *non_ndx_key) {
    struct User *user = (struct User *)rec;
    char *username = (char *)non_ndx_key;
    return strcmp(user->username, username);
}

int 
userAuthenticateServer(char *username, char *password, int *userId, int *isAdmin){
    pthread_mutex_lock(&lock);
    printf("userAuthenticateServer got %s %s\n", username, password);
    
    struct User thisUser;
    int io_count = 0;
    int status = get_rec_by_non_ndx_key(username, &thisUser, match_username, &io_count, USER);
    
    printUser(&thisUser);
    if(status == PDS_REPO_NOT_OPEN){
        pthread_mutex_unlock(&lock);
        return REPO_ERROR;
    }else if(status == PDS_REC_NOT_FOUND){
        pthread_mutex_unlock(&lock);
        return USER_DOES_NOT_EXIST;
    }

    if(strcmp(thisUser.password, password) == 0){
        *userId = thisUser.userId;
        *isAdmin = thisUser.isAdmin;
        if(thisUser.isAdmin){
            pthread_mutex_unlock(&lock);
            return AUTHENTICATED_ADMIN;
        }else{
            pthread_mutex_unlock(&lock);
            return AUTHENTICATED_USER;
        }
    }
    pthread_mutex_unlock(&lock);
    return NOT_AUTHENTICATED;
}

int 
giveAdminAccessServer(int userId){
    pthread_mutex_lock(&lock);
    struct User thisUser;
    int status = get_rec_by_ndx_key(userId, &thisUser, USER) != PDS_SUCCESS;
    
    if(status == PDS_REPO_NOT_OPEN){
        pthread_mutex_unlock(&lock);
        return REPO_ERROR;
    }else if(status == PDS_REC_NOT_FOUND){
        pthread_mutex_unlock(&lock);
        return USER_DOES_NOT_EXIST;
    }

    thisUser.isAdmin = 1;

    int modStatus = modify_by_ndx_key(userId, &thisUser, USER);

    if(modStatus == PDS_REPO_NOT_OPEN){
        pthread_mutex_unlock(&lock);
        return REPO_ERROR;
    }else if(modStatus == PDS_MODIFY_FAILED){
        pthread_mutex_unlock(&lock);
        return ADMIN_NOT_GRANTED;
    }
    pthread_mutex_unlock(&lock);
    return ADMIN_GRANTED;
}
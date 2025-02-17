#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include "../include/pds.h"
#include "../include/bst.h"

struct PDS_RepoInfo repo_handle[2];

int lockRepoExcl(int which) {
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;

    // Get the file descriptor from FILE *
    int fd = fileno(repo_handle[which].pds_data_fp);
    
    // Attempt to set an exclusive lock on the file
    while (fcntl(fd, F_SETLKW, &fl) == -1) {
        perror("lockRepoExcl: fcntl");
        // Handle the error appropriately, e.g., retry or exit
    }

    return PDS_REPO_LOCKED_EXCL;
}

int unlockRepoExcl(int which) {
    struct flock fl;
    fl.l_type = F_UNLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;

    // Get the file descriptor from FILE *
    int fd = fileno(repo_handle[which].pds_data_fp);
    
    // Attempt to unlock the file
    while (fcntl(fd, F_SETLKW, &fl) == -1) {
        perror("unlockRepoExcl: fcntl");
        // Handle the error appropriately, e.g., retry or exit
    }

    return PDS_REPO_UNLOCKED_EXCL;
}

int lockRepoShare(int which) {
    struct flock fl;
    fl.l_type = F_RDLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;

    // Get the file descriptor from FILE *
    int fd = fileno(repo_handle[which].pds_data_fp);
    
    // Attempt to set a shared lock on the file
    while (fcntl(fd, F_SETLKW, &fl) == -1) {
        perror("lockRepoShare: fcntl");
        // Handle the error appropriately, e.g., retry or exit
    }

    return PDS_REPO_LOCKED_SHA;
}

int unlockRepoShare(int which) {
    struct flock fl;
    fl.l_type = F_UNLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;

    // Get the file descriptor from FILE *
    int fd = fileno(repo_handle[which].pds_data_fp);
    
    // Attempt to unlock the file
    while (fcntl(fd, F_SETLKW, &fl) == -1) {
        perror("unlockRepoShare: fcntl");
        // Handle the error appropriately, e.g., retry or exit
    }

    return PDS_REPO_UNLOCKED_SHA;
}


int 
pds_create(char *repo_name) {
    char fileDirectory[] = "data/";
    char fileName[strlen(repo_name) + 5]; // ".dat\0"
    char indexName[strlen(repo_name) + 5]; // ".ndx\0"

    strcpy(fileName, repo_name);
    strcpy(indexName, repo_name);

    strcat(fileName, ".dat");
    strcat(indexName, ".ndx");

    char filePathName[strlen(fileDirectory) + strlen(fileName) + 1];
    char indexPathName[strlen(fileDirectory) + strlen(indexName) + 1];

    strcpy(filePathName, fileDirectory);
    strcpy(indexPathName, fileDirectory);

    strcat(filePathName, fileName);
    strcat(indexPathName, indexName);

    FILE *filePtr = fopen(filePathName, "wb");
    FILE *indexPtr = fopen(indexPathName, "wb");

    if (filePtr == NULL || indexPtr == NULL) {
        perror("pds_create");
        if (filePtr) fclose(filePtr);
        if (indexPtr) fclose(indexPtr);
        return PDS_FILE_ERROR;
    }

    int zero = 0;
    fwrite(&zero, sizeof(int), 1, indexPtr);

    fclose(filePtr);
    fclose(indexPtr);

    return PDS_SUCCESS;
}

int 
pds_open(char *repoName, int recSize, const int which){

    if(repo_handle[which].repo_status == PDS_REPO_CLOSED){
        return PDS_REPO_NOT_OPEN;
    }

    char fileDirectory[] = "data/";
    char fileName[strlen(repoName) + 5]; // ".dat\0"
    char indexName[strlen(repoName) + 5]; // ".ndx\0"

    strcpy(fileName, repoName);
    strcpy(indexName, repoName);

    strcat(fileName, ".dat");
    strcat(indexName, ".ndx");

    char filePathName[strlen(fileDirectory) + strlen(fileName) + 1];
    char indexPathName[strlen(fileDirectory) + strlen(indexName) + 1];

    strcpy(filePathName, fileDirectory);
    strcpy(indexPathName, fileDirectory);

    strcat(filePathName, fileName);
    strcat(indexPathName, indexName);

    repo_handle[which].pds_data_fp = fopen(filePathName, "rb+");
    repo_handle[which].pds_ndx_fp = fopen(indexPathName, "rb+");

    if((repo_handle[which].pds_data_fp == NULL )|| (repo_handle[which].pds_ndx_fp == NULL) ){
        perror("pds_open");
        if(repo_handle[which].pds_data_fp) fclose(repo_handle[which].pds_data_fp);
        if(repo_handle[which].pds_ndx_fp) fclose(repo_handle[which].pds_ndx_fp);
        return PDS_FILE_ERROR;
    }

    strcpy(repo_handle[which].pds_name, repoName);
    repo_handle[which].rec_count = 0;
    repo_handle[which].rec_size = recSize;
    repo_handle[which].repo_status = PDS_REPO_OPEN;

    return pds_load_ndx(which);   
}

int
pds_load_ndx(const int which){
    
    fseek(repo_handle[which].pds_ndx_fp,0, SEEK_SET);
    // fread(repo_handle[which].pds_ndx_fp, &repo_handle[which].rec_count, sizeof(repo_handle[which].rec_count);
    fread(&repo_handle[which].rec_count, sizeof(int), 1, repo_handle[which].pds_ndx_fp);

    for(int i = 0; i < repo_handle[which].rec_count; i++){
        struct PDS_NdxInfo *temp = malloc(sizeof(struct PDS_NdxInfo));
        // read(repo_handle[which].pds_ndx_fp, temp, sizeof(struct PDS_NdxInfo));
        fread(temp, sizeof(struct PDS_NdxInfo), 1, repo_handle[which].pds_ndx_fp);
        bst_add_node(&(repo_handle[which].ndx_root), temp -> key, temp);
    }

    return PDS_SUCCESS;
}


int 
put_rec_by_key(int key, void *rec, const int which){
    // Implementing exclusive write lock
    if(repo_handle[which].repo_status == PDS_REPO_CLOSED){
        return PDS_REPO_NOT_OPEN;
    }

    // printPDSRepoInfo(USER);

    while(lockRepoExcl(which) != PDS_REPO_LOCKED_EXCL);
    
    struct BST_Node *search = bst_search(repo_handle[which].ndx_root, key);

    if(search != NULL){
        struct PDS_NdxInfo *indexInfo = search -> data;
        
        if(indexInfo -> is_deleted == 1){
            fseek(repo_handle[which].pds_data_fp, indexInfo -> offset, SEEK_SET);
            // write(repo_handle[which].pds_data_fp, &key, sizeof(int));
            fwrite(&key, sizeof(int),1, repo_handle[which].pds_data_fp);
            // write(repo_handle[which].pds_data_fp, rec, repo_handle[which].rec_size);
            fwrite(rec, repo_handle[which].rec_size, 1, repo_handle[which].pds_data_fp);
            indexInfo -> is_deleted = 0;    
            return PDS_SUCCESS;
        }

        return PDS_DUPLICATE_KEY_ADD;
    }

    fseek(repo_handle[which].pds_data_fp, 0, SEEK_END);
    struct PDS_NdxInfo *temp = malloc(sizeof(struct PDS_NdxInfo));
    
    if(temp == NULL){
        perror("put_rec_by_key");
        return PDS_ADD_FAILED;
    }

    temp -> key = key;
    temp -> offset = ftell(repo_handle[which].pds_data_fp);
    temp -> is_deleted = 0;

    int addStatus = bst_add_node(&repo_handle[which].ndx_root, key, temp);

    if(addStatus == BST_DUP_KEY || addStatus == BST_NULL){
        return PDS_ADD_FAILED;
    }

    if(fwrite(&key, sizeof(int), 1, repo_handle[which].pds_data_fp) == 0){
        perror("put_rec_by_key:write:key");
    }

    if(fwrite(rec, repo_handle[which].rec_size, 1, repo_handle[which].pds_data_fp) == 0){
        perror("put_rec_by_key:write:rec");
    }

    repo_handle[which].rec_count += 1;

    // Unlocking the file
    while(unlockRepoExcl(which) != PDS_REPO_UNLOCKED_EXCL);

    // printPDSRepoInfo(USER);

    return PDS_SUCCESS;
}

int 
get_rec_by_ndx_key(int key, void *rec, const int which){

    if(repo_handle[which].repo_status == PDS_REPO_CLOSED){
        return PDS_REPO_NOT_OPEN;
    }

    while(lockRepoShare(which) != PDS_REPO_LOCKED_SHA);

    struct BST_Node *search = bst_search(repo_handle[which].ndx_root,key);

    if(search == NULL){
        return PDS_REC_NOT_FOUND;
    }

    struct PDS_NdxInfo *temp = search -> data;

    if(temp -> is_deleted == 1){
        return PDS_REC_NOT_FOUND;
    }

    fseek(repo_handle[which].pds_data_fp, 0, SEEK_SET);
    
    int k;
    fread( &k, sizeof(int),1,repo_handle[which].pds_data_fp);
    fread(rec, sizeof(rec),1,repo_handle[which].pds_data_fp);

    while(unlockRepoShare(which) != PDS_REPO_UNLOCKED_SHA);

    return PDS_SUCCESS;
}

 int 
 get_rec_by_non_ndx_key(void *non_ndx_key, void *rec, int (*matcher)(void *rec, void *non_ndx_key), int *io_count, const int which) {

    // Implementing the shared lock
    if (repo_handle[which].repo_status == PDS_REPO_CLOSED) {
        return PDS_REPO_NOT_OPEN;
    }

    while(lockRepoShare(which) != PDS_REPO_LOCKED_SHA);

    *io_count = 0;
    fseek(repo_handle[which].pds_data_fp, 0, SEEK_SET);

    int k;
    void *temp;
    while (fread(&k, sizeof(int),1,repo_handle[which].pds_data_fp) != 0) {
        fread(temp, repo_handle[which].rec_size,1,repo_handle[which].pds_data_fp);
        *io_count = *io_count + 1;
        if (matcher(rec, non_ndx_key) == 0) {
            struct BST_Node *search = bst_search(repo_handle[which].ndx_root, k);
            struct PDS_NdxInfo *indexInfo = search -> data;
            if (search == NULL || search->data == NULL || indexInfo->is_deleted == 1) {
                *io_count = -1;
                return PDS_REC_NOT_FOUND;
            }
            memcpy(rec, temp, repo_handle[which].rec_size);
            unlockRepoShare(which);
            return PDS_SUCCESS;
        }
    }

    *io_count = -1;

    unlockRepoShare(which);

    return PDS_REC_NOT_FOUND;
}

int modify_by_ndx_key(int key, void *rec, int which){

    if(repo_handle[which].repo_status == PDS_REPO_CLOSED){
        return PDS_REPO_NOT_OPEN;
    }

    while(lockRepoExcl(which) != PDS_REPO_LOCKED_EXCL);

    struct BST_Node *search = bst_search(repo_handle[which].ndx_root, key);

    struct PDS_NdxInfo *indexInfo = search -> data;

    if(indexInfo->is_deleted == 1){
        PDS_MODIFY_FAILED;
    }

    int k;
    fseek(repo_handle[which].pds_data_fp, indexInfo -> offset, SEEK_SET);
    fread(&k, sizeof(int),1,repo_handle[which].pds_data_fp);
    fwrite(rec, repo_handle[which].rec_size,1,repo_handle[which].pds_data_fp);

    return PDS_SUCCESS;
}

int
delete_rec_by_ndx_key(int key, const int which){

    if(repo_handle[which].repo_status == PDS_REPO_CLOSED){
        return PDS_REPO_NOT_OPEN;
    }

    // Implementing exclusive write lock
    while(lockRepoExcl(which) != PDS_REPO_LOCKED_EXCL);

    struct BST_Node *search = bst_search(repo_handle[which].ndx_root, key);

    if(search == NULL){
        return PDS_REC_NOT_FOUND;
    }

    struct PDS_NdxInfo *indexInfo = search -> data;

    if(indexInfo == NULL || indexInfo -> is_deleted == 1){
        return PDS_DELETE_FAILED;
    }

    indexInfo -> is_deleted = 1;

     // Unlocking the file
    while(unlockRepoExcl(which) != PDS_REPO_UNLOCKED_EXCL);

    return PDS_SUCCESS;
}

void 
preOrder(struct BST_Node *node, const int which){
    if(node == NULL)
        return;

    struct PDS_NdxInfo *var = node -> data;

    fwrite(var, sizeof(struct PDS_NdxInfo),1,repo_handle[which].pds_ndx_fp);

    preOrder(node -> left_child, which);
    preOrder(node -> right_child, which);
}

int
pds_close(const int which){

    if(repo_handle[which].repo_status == PDS_REPO_CLOSED){
        return PDS_REPO_NOT_OPEN;
    }

    fclose(repo_handle[which].pds_data_fp);
    fclose(repo_handle[which].pds_ndx_fp);

    char fileDirectory[] = "data/";

    char indexName[strlen(repo_handle[which].pds_name) + 5]; // ".ndx\0"

    strcpy(indexName, repo_handle[which].pds_name);

    strcat(indexName, ".ndx");

    char indexPathName[strlen(fileDirectory) + strlen(indexName) + 1];

    strcpy(indexPathName, fileDirectory);

    strcat(indexPathName, indexName);

    repo_handle[which].pds_ndx_fp = fopen(indexPathName, "wb");

    if((repo_handle[which].pds_ndx_fp == NULL) ){
        perror("pds_close");
        if(repo_handle[which].pds_ndx_fp) fclose(repo_handle[which].pds_ndx_fp);
        return PDS_FILE_ERROR;
    }

    fwrite(&repo_handle[which].rec_count, sizeof(repo_handle[which].rec_count),1,repo_handle[which].pds_ndx_fp);

    preOrder(repo_handle[which].ndx_root, which);

    fclose(repo_handle[which].pds_ndx_fp);

    bst_destroy(repo_handle[which].ndx_root);

    return PDS_SUCCESS;
}
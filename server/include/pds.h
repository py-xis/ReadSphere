#ifndef PDS_H
#define PDS_H

// Error codes
#define PDS_SUCCESS 200
#define PDS_FILE_ERROR 404
#define PDS_ADD_FAILED 2
#define PDS_REC_NOT_FOUND 3
#define PDS_LOAD_NDX_FAILED 4
#define PDS_REPO_ALREADY_OPEN 12
#define PDS_NDX_SAVE_FAILED 13
#define PDS_REPO_NOT_OPEN 14
#define PDS_DELETE_FAILED 15


#define PDS_DUPLICATE_KEY_ADD 16
#define PDS_REPO_LOCKED_EXCL 17
#define PDS_REPO_UNLOCKED_EXCL 18
#define PDS_REPO_LOCKED_SHA 19
#define PDS_REPO_UNLOCKED_SHA 20
#define PDS_MODIFY_FAILED 21
#define PDS_MODIFIED 22

#define REPO_ERROR 119


// Repository status values
#define PDS_REPO_OPEN 10
#define PDS_REPO_CLOSED 11

// Repo indentifier
#define BOOK 0
#define USER 1


struct PDS_NdxInfo{
	int key;
	int offset;
	int is_deleted; // 0 - not deleted, 1 - deleted
};

struct PDS_RepoInfo{
	char pds_name[30];
	FILE *pds_data_fp;
	FILE *pds_ndx_fp;
	int repo_status; 
	int rec_size; // For fixed length records
	struct BST_Node *ndx_root;
	int rec_count;
};

extern struct PDS_RepoInfo repo_handle[2];

int pds_create( char *repo_name);

// pds_open
// Open the data file and index file in rb+ mode
// Update the fields of PDS_RepoInfo appropriately
// Build BST and store in pds_bst by reading index entries from the index file
// Close only the index file
int pds_open( char *repo_name, int rec_size, int which);

// pds_load_ndx
// Internal function used by pds_open to read index entries into BST
int pds_load_ndx(int which);

// put_rec_by_key
// Seek to the end of the data file
// Create an index entry with the current data file location using ftell
// Add index entry to BST using offset returned by ftell
// Write the key at the current data file location
// Write the record after writing the key
int put_rec_by_key( int key, void *rec, int which);

// get_rec_by_key
// Search for index entry in BST
// Seek to the file location based on offset in index entry
// Read the key at the current file location 
// Read the record after reading the key
int get_rec_by_ndx_key( int key, void *rec, int which);

// search by non_ndx key
int get_rec_by_non_ndx_key(void *key, void *rec, int (*matcher)(void *rec, void *key), int *io_count, const int which);

// modify by ndx key
int modify_by_ndx_key(int key, void *rec, int which);

// delete by ndx_key
int delete_rec_by_ndx_key( int key, const int which);


// pds_close
// Open the index file in wb mode (write mode, not append mode)
// Unload the BST into the index file by traversing it in PRE-ORDER (overwrite the entire index file)
// Free the BST by call bst_destroy()
// Close the index file and data file
int pds_close();

int lockRepoExcl(int which);

int unlockRepoExcl(int which);

int lockRepoShare(int which);

int unlockRepoShare(int which);


#endif

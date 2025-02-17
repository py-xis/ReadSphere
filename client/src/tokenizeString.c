#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to split the input string into tokens
char** splitString(const char* str, int* tokenCount) {
    // Duplicate the input string to avoid modifying the original string
    char* strCopy = strdup(str);
    if (strCopy == NULL) {
        perror("strdup failed");
        exit(EXIT_FAILURE);
    }

    // Count the number of tokens
    char* token = strtok(strCopy, " ");
    int count = 0;
    while (token != NULL) {
        count++;
        token = strtok(NULL, " ");
    }

    // Allocate memory for the array of tokens
    char** tokens = malloc(count * sizeof(char*));
    if (tokens == NULL) {
        perror("malloc failed");
        free(strCopy);
        exit(EXIT_FAILURE);
    }

    // Tokenize the string again and store the tokens
    strcpy(strCopy, str);
    token = strtok(strCopy, " ");
    int index = 0;
    while (token != NULL) {
        tokens[index] = strdup(token);
        if (tokens[index] == NULL) {
            perror("strdup failed");
            free(strCopy);
            for (int i = 0; i < index; i++) {
                free(tokens[i]);
            }
            free(tokens);
            exit(EXIT_FAILURE);
        }
        index++;
        token = strtok(NULL, " ");
    }

    // Free the duplicate string copy
    free(strCopy);

    // Set the token count
    *tokenCount = count;

    return tokens;
}

// Function to free the allocated memory for tokens
void freeTokens(char** tokens, int tokenCount) {
    for (int i = 0; i < tokenCount; i++) {
        free(tokens[i]);
    }
    free(tokens);
}


// int main(){
//     char *string = "This is a space seperated string";
//     int numberOfTokens = 0;
//     char **tokens = splitString(string, &numberOfTokens);
//     for(int i = 0; i < numberOfTokens; i++){
//         printf("%d %s\n", i, tokens[i]);
//     }

//     return 0;
// }
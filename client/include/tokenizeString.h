#ifndef TOKENIZESTRING_H
#define TOKENIZESTRING_H

char** splitString(const char* str, int* tokenCount);
void freeTokens(char** tokens, int tokenCount);

#endif
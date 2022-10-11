//-------------------------------
// Lars Jensen CS201 Spring 2021
// Aidan Brown
//-------------------------------

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_CHILDREN 2
#define NUM_ELEMENTS 1000
#define BUFLEN 10100
#define LINELEN 256
char buffer[BUFLEN];

//thread stuff
#define NUM_THREADS 4
typedef struct {
    int startIndex; // start index in buffer[] at which to begin searching
    int endIndex; // end index
    int maxRunLength; // result: the longest run found of any character in buffer[],
// between startIndex and endIndex (inclusive)
    char maxRunChar; // result: the character in the longest run
} SearchInfo;

int readFile(char *filename, int *numChars);

int main(int argc, char *argv[]) {
    int rc, numChars;
    if (argc < 2) {
        printf("ERROR: need a filename\n");
        return 8;
    }
    rc = readFile(argv[1], &numChars);
    if (rc != 0)
        return 8;
    int len = strlen(buffer);
    for (int i = 0; i < len; ++i){
        printf("%c", buffer[i]);
    }
    // here’s where you’ll create the threads that do the actual searching
    return 0;
}

int readFile(char *filename, int *numChars){
    FILE *fp = fopen(filename, "r"); // filename is the char* passed to your readFile() function
    if (fp == NULL) {
        printf("ERROR: cannot open file '%s'\n", filename);
        return 1;
    }
    char line[LINELEN];
    int i = 0;
    int len = 0;
    int bufferPos = 0;
    char *chp;
    // now you can read from the file
    chp = fgets(line, LINELEN, fp);
    if (chp == NULL) {
        printf("file is empty\n");
        fclose(fp);
        return 8;
    }
    int j = 0;
    while (chp != NULL) {
        len = strlen(line);
        if (line[len-1] == '\n')
            len = len - 1;
        for (i=0; i<len; ++i) {
            // append the contents of the line[] buffer to the global buffer
            buffer[i + bufferPos] = line[i];
        }
        bufferPos = strlen(buffer);
        chp = fgets(line, LINELEN, fp);
    } // while not at end of file


    return 0;
}




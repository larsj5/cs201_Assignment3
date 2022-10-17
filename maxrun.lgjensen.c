//-------------------------------
// Lars Jensen CS201 Spring 2021
// Aidan Brown
//-------------------------------

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_ELEMENTS 100
#define BUFLEN 110
#define LINELEN 256
char buffer[BUFLEN];

//thread stuff
#define NUM_THREADS 4
typedef struct {
    int startIndex;   // start index in buffer[] at which to begin searching
    int endIndex;     // end index
    int maxRunLength; // result: the longest run found of any character in buffer[],
                      // between startIndex and endIndex (inclusive)
    char maxRunChar;  // result: the character in the longest run
} SearchInfo;

int readFile(char *filename, int *numChars);

void *searchFunction(void *param);

int main(int argc, char *argv[]) {
    int rc, numChars;
    if (argc < 2) {
        printf("ERROR: need a filename\n");
        return 8;
    }
    rc = readFile(argv[1], &numChars);
    if (rc != 0)
        return 8;

    SearchInfo data[NUM_THREADS];   // holds data we want to give to child thread
    pthread_t tid[NUM_THREADS];    // thread identifier
    int maxVal;
    int i, j, idx, elementsPerThread;

    idx = 0;
    elementsPerThread = NUM_ELEMENTS / NUM_THREADS;
    for (j=0; j<NUM_THREADS-1; ++j) {
        data[j].startIndex = idx;
        data[j].endIndex = idx + elementsPerThread - 1;
        idx = idx + elementsPerThread;
    }

    // yes, I wish there were a min() function in C
    data[j].startIndex = idx;
    if (data[NUM_THREADS-1].startIndex < NUM_ELEMENTS - 1)
        data[NUM_THREADS-1].endIndex = NUM_ELEMENTS - 1;

    // create and start the threads
    for (i=0; i<NUM_THREADS; ++i) {
        // create and start a child thread
        pthread_create(&tid[i], NULL, searchFunction, &data[i]);
    }

    // wait for the child threads to terminate
    for (i=0; i<NUM_THREADS; ++i) {
        pthread_join(tid[i], NULL);
    }

    // gather data from the individual results
    maxVal = data[0].maxRunLength;
    for (i=0; i<NUM_THREADS; ++i) {
        if (data[i].maxRunLength > maxVal)
            maxVal = data[i].maxRunLength;
    }

    printf("maximum value over the whole array is %d\n", maxVal);

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

//-----------------------------------------------------------------
// This is the function that computers the max over a range of the
// global array A[]. The SumStruct passed through param tells this
// function over which values to do the max.

void *searchFunction(void *param) {
    //convert parameter into search info struct
    SearchInfo *data;
    data = (SearchInfo *) param;

    //initialize variables to keep track of run and max run of characters
    int maxrun = 0;
    int run = 1; //run is initialized to 1 because every char will be at least a run of 1

    //loop through the portion of the array that we're searching, padding the end index by 10 to ensure
    // that we catch any runs that are on the edges.
    for (int i = data->startIndex; i < data->endIndex + 10; ++i) {
        //while character is equal to next, increment run and i
        while (buffer[i] == buffer [i + 1] && i < NUM_ELEMENTS){
            run++;
            i++;
        }
        //set max run equal to the run if the current run is greater than max run
        if (run > maxrun){
            maxrun = run;
            data->maxRunChar = buffer[i]; //update the character that the run was of
        }
        run = 1; //set run back to 1 because each char has a run of 1 (itself)
    }
    data->maxRunLength = maxrun; //update max run length of search info struct
    printf("(R) max is %d of character %c\n", data->maxRunLength, data->maxRunChar);
    pthread_exit(NULL); //exit thread
}




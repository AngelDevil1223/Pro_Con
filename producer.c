#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void getOutFileName(char *inFileName, char *outFileName);
unsigned char parity(unsigned char);
void bit_writter(unsigned char* , FILE* );
void makeError(unsigned char *, int );
void frame(char *, char *);


int main(int argc, char *argv[]) {
    unsigned char garbageChar;
    char character;
    char outFileName[50];
    FILE *inFilePointer, *outFilePointer;
    // opening files
    inFilePointer = fopen(argv[1], "rt");
    outFilePointer = fopen("tempfile", "wt");
    getOutFileName(argv[1], outFileName);
    character = fgetc(inFilePointer);
    // here we write the data with parity into a tempfile 
    while (character != EOF) {
        garbageChar = parity(character);
        fwrite(&garbageChar, 1, 1, outFilePointer);
        character = fgetc(inFilePointer);
    }

    // closing files
    fclose(inFilePointer);
    fclose(outFilePointer);

    frame("tempfile", outFileName);

    remove("tempfile");
    
    return 0;
}


void frame(char *inFileName, char *outFileName) {
    // the amount of data we read in a single time = 64
    unsigned char buffer[64];
    int buf;
    char lengthOfChar;
    FILE *infilePtr, *outfilePtr;

    // opening files
    infilePtr = fopen(inFileName, "rb");
    outfilePtr = fopen(outFileName, "wb");

    while((buf = fread(buffer, 1, 64, infilePtr)) != 0){
        lengthOfChar = buf;
        fwrite("\22\22", 1, 2, outfilePtr);
        fwrite(&lengthOfChar, 1, 1, outfilePtr);
        // simulating transmission error
        makeError(buffer, buf);
        fwrite(buffer, 1, buf, outfilePtr);
    }
    // closing the files
    fclose(infilePtr);
    fclose(outfilePtr);
}

void makeError(unsigned char *arr, int lenght) {
    int errors;
    // seed setup
    srand(time(0));

    errors = rand() % 3;
    for (int i=0; i < errors; i++)
        arr[rand() % lenght] -= rand() % 32;
}



unsigned char parity(unsigned char character) {
    int numberOfOnes = 0;
    char temp = character;
    for (int i=1; temp != 0; i++) {
        if (temp % 2 == 1)
            numberOfOnes++;
        temp /= 2;
    }
    int change = 128 * !(numberOfOnes % 2);
    return character + change;
}

void getOutFileName(char *inFileName, char *outFileName)
{
    int lenght = strlen(inFileName); 
    for(int i = 0 ; i < lenght; ++i){
        if(inFileName[i] == '.'){
            outFileName[i] = '\0';
            break;
        }
        outFileName[i] = inFileName[i];
    }
    // add .binf to th name
    strcat(outFileName, ".binf");
}

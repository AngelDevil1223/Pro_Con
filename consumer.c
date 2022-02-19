#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int errorChecker(unsigned char *arr, size_t len);
void reverseFraming(FILE* inputFilePtr, FILE* outFilePtr);

int main(int argc, char **argv) {
    FILE *inputFilePtr, *outFilePtr;
    /// reading this file
    inputFilePtr = fopen(argv[1], "rb");
    // opening a new file 
    outFilePtr = fopen(argv[2], "wt");

    reverseFraming(inputFilePtr, outFilePtr);

    // close files
    fclose(inputFilePtr);
    fclose(outFilePtr);

    return 0;
    
}

int errorChecker(unsigned char *arr, size_t len) {
    int numberOfOnes;
    int temp;
    int numberOfErrors = 0;
    for (int i=0; i<len; i++) {
        numberOfOnes = 0;
        if(arr[i] < 128){
            temp = arr[i];
        }
        else{
            temp = arr[i] - 128;
        }
        for (int i = 1; temp != 0; ++i) {
            if (temp % 2)
                numberOfOnes++;
            temp /= 2;
        }

        if ((numberOfOnes % 2 == 1) && (arr[i] > 128))
            numberOfErrors++;
        else if ((numberOfOnes % 2 == 0) && (arr[i] < 128))
            numberOfErrors++;
    }
    return numberOfErrors;
}

void reverseFraming(FILE* inputFilePtr, FILE* outFilePtr) {
    int totalErrors = 0;
    unsigned char buffer[67];
    int bufferLen;
    char ch;

    // at first we get only the first 3 control character and then read
    // the lenght of the content specified in the file
    while (fread(buffer, 1, 3, inputFilePtr) != 0) {
        bufferLen = buffer[2];
        fread(buffer, 1, bufferLen, inputFilePtr);
        // checking for errors
        totalErrors += errorChecker(buffer, bufferLen);

        for (int i = 0; i < bufferLen; i++) {
            if (buffer[i] < 128)
                ch = buffer[i];
            else
                ch = buffer[i] - 128;

            fwrite(&ch, 1, 1, outFilePtr);
        }
    }

    printf("Total errors in the file %d\n", totalErrors);
}

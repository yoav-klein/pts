#include<stdio.h>
#include <unistd.h>

int main() {
    
    // Open a file in writing mode
    char filename[50];
    sprintf(filename, "file-%d", getpid());
    FILE *fptr = fopen(filename, "w");

    // Write some text to the file
    fprintf(fptr, "Some text");

    // Close the file
    fclose(fptr);

    return 0;
}

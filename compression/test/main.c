#include <iostream>
#include <fstream>

using namespace std;

int main () {
    FILE *streamIn;
    streamIn = fopen("./Untitled.bmp", "r");
    if (streamIn == (FILE *)0) {
        printf("File opening error ocurred. Exiting program.\n");
        return 0;
    }

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, streamIn);

    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    int image[width*height][3];
    int i = 0;

    for(i=0;i<width*height;i++) {
        image[i][2] = getc(streamIn);
        image[i][1] = getc(streamIn);
        image[i][0] = getc(streamIn);
        printf("pixel %d : [%d,%d,%d]\n",i+1,image[i][0],image[i][1],image[i][2]);
    }

    fclose(streamIn);

    return 0;
}

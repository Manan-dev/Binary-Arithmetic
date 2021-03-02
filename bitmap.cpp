// Bitmap lab
// Manan Patel
// 2/19/2021
// This lab is done to play aorund with binary files
// Got help from - Zach McMichael, Fatima Bowers(student)

#include <cstdint>
#include <cstdio>

using namespace std;

#pragma pack(push, 2)
struct BitmapFileHeader
{
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
};

struct BitmapInfoHeader
{
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitcount;
    uint32_t compression;
    uint32_t x_pixels_per_meter;
    uint32_t y_pixels_per_meter;
    uint32_t color_used;
    uint32_t color_important;
};
#pragma pack(pop)

struct Pixel{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};


int main(int argc, char **argv)
{

    FILE *fin = fopen(argv[1], "rb");;
    BitmapFileHeader fh;
    BitmapInfoHeader ih;
    Pixel *_pixel;

    // error check arguments
    if (argc != 3)
    {
        printf("Usage: %s (inputfile) (outputfile)\n", argv[0]);
        return 1;
    }

    if (fin == nullptr)
    {
        perror("Error: ");
        return 1;
    }

    // read and seek
    fread(&fh, 1, sizeof(fh), fin);
    fread(&ih, 1, sizeof(ih), fin);
    fseek(fin, fh.offset, SEEK_SET); 

    // Allocate memory for pixels
    _pixel = new Pixel[ih.height * ih.width];

    int padding_x = 0;
    int px_width = ih.width * sizeof(Pixel);

    // calculate padding
    if ((px_width % 4) != 0){
        padding_x = 4 - (px_width % 4);
    }

    // go through each row and skip padding
    for (unsigned int i = 0; i < ih.height; i++){
        fread(_pixel + i * ih.width, 1, px_width, fin);
        fseek(fin, padding_x, SEEK_CUR);
    }

    fclose(fin);

    // flip();
    Pixel tempStr;
    int startingIndex = 0;
    int endingIndex = 0;

    // goes through full height and half width and swaps pixels
    for (unsigned int column = 0; column < ih.height; column++){
        for (unsigned int row = 0; row < ih.width/2; row++){
            startingIndex = column * ih.width + row;
            endingIndex = column * ih.width + (ih.width - row - 1);
            tempStr = _pixel[startingIndex];
            _pixel[startingIndex] = _pixel[endingIndex];
            _pixel[endingIndex] = tempStr;
        }
    }
    // invert();
    // invetrs rgb values by subtracting by max value
    for (unsigned int i = 0; i < (ih.height * ih.width); i++){
            _pixel[i].r = 255 - _pixel[i].r;
            _pixel[i].g = 255 - _pixel[i].g;
            _pixel[i].b = 255 - _pixel[i].b;
    }

    // open new file for writing
    FILE *fout = fopen(argv[2], "wb");

    if (argc != 3)
    {
        printf("Usage: %s (inputfile) (outputfile)\n", argv[0]);
        return 1;
    }

    if (fout == nullptr)
    {
        perror("Error: ");
        return 1;
    }

    // read and seek
    fwrite(&fh, 1, sizeof(fh), fout);
    fwrite(&ih, 1, sizeof(ih), fout);
    fseek(fout, fh.offset, SEEK_SET); 

    char arr[] = {0,0,0}; 
    // writes all pixels then adds padding
    for (unsigned int i = 0; i < ih.height; i++){
        fwrite(_pixel + i * ih.width, 1, px_width, fout);
        fwrite(arr, 1, padding_x, fout);
    }
    fclose(fout);
    delete[]_pixel;
    return 0;
}


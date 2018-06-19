// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize infile outfile\n");
        return 1;
    }

    // remember filenames
    // store number as number not char
    int n = atoi(argv[1]);
        if(n < 1 || n > 100)
        {
            printf("n should be between 1 and 100\n");
            return 1;
        }
    char *infile = argv[2];
    char *outfile = argv[3];

    // setup variables for padding of the infile and padding of the outfile
    int inpad = 0;
    int outpad = 0;


    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
        // printf("sizeof(BITMAPFILEHEADER) %lu\n", sizeof(BITMAPFILEHEADER));
        // printf("")

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);



    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    bi.biWidth *= n;
    bi.biHeight *= n;
    bi.biSizeImage = bi.biWidth * bi.biHeight *3;


    printf("bi.biWidth value is: %i\n", bi.biWidth);
    printf("bi.biHeight value is: %i\n", bi.biHeight);
    printf("bi.biSizeImage value is: %i\n", bi.biSizeImage);

    // this should be reading the infile bfSize
    printf("bf.bfSize infile value is: %i\n", bf.bfSize);


    // bf.Size outfile value is total size of file in bytes including pixels, padding and headers
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + padding;



    typedef struct
    {
        WORD bfType;
        DWORD bfSize;
        WORD bfReserved1;
        WORD bfReserved2;
        DWORD bfOffBits;
    } __attribute__((__packed__))
    BITMAPFILEHEADER;



    // size_t fread(void* ptr, size_t size, size_t blocks, FILE* fp);



    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);



    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // edit here to account for outfile's BITMAPINFOHEADERs resize




    // if(bi.biSizeImage == (bi.biWidth * bi.biHeight *3))
    // {
    //     // no padding needed
    //     int padding = 0;
    //     int padding2 = 0;
    // }
    // else
    // {
    //     // determine padding for scanlines
    //     padding =  (4 - (bi.biWidth * n * sizeof(RGBTRIPLE)) % 4) % 4;
    //     padding2 =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // }



    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + padding;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);


        outpad =  (4 - (bi.biWidth * n * sizeof(RGBTRIPLE)) % 4) % 4;
        inpad =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
        int paddingtest =  (4 - (bi.biWidth * 100 * sizeof(RGBTRIPLE)) % 4) % 4;

        int bisizeimagetest = bi.biWidth * bi.biHeight *3;
        int bfsizetest = bisizeimagetest + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);



            // printf("char *n: %c\n", *n);
        printf("int outpad %d\n", inpad);
        printf("int inpad %d\n", outpad);
        printf("int paddingtest %d\n", paddingtest);
        printf("int bisizeimagetest %d\n", bisizeimagetest);
        printf("int bfsizetest %d\n", bfsizetest);
        printf("int bf.bfSize %d\n", bf.bfSize);


        // then add it back (to demonstrate how)
        for (int k = 0; k < outpad; k++)
        {
            //adding padding back into the image
            fputc(0x00, outptr);
            printf("k: %i\n", k);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}





// a way of adjusting for padding


// for (int a = 0; a < height; a++) {
//     for (int b = 0; b < width*3; b++) {
//         if (bmpArray[a*(width*3 + padding)+b] < 127) {
//             bmpArray[a*(width*3 + padding)+b] = 0;
//         } else {
//             bmpArray[a*(width*3 + padding)+b] = 255;
//         }
//     }
//     for (int pad = 0; pad < padding; pad++) {
//         bmpArray[a*(width*3 + padding) + 3*width + pad] = 0x00;
//     }
// }

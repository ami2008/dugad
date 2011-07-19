/**
 * file name  : detect.c
 * authors    : Dave Pederson
 * created    : Dec 14, 2010
 *
 * modifications:
 * Date:         Name:            Description:
 * ------------  ---------------  ----------------------------------------------
 * Dec 14, 2010  Dave Pederson    Creation
 * Jul 19, 2011  Dave Pederson    Code cleanup
 */
#include <netpbm/pgm.h>
#include "dugad.h"

int main(int argc, char *argv[])
{
    FILE *lena;
    gray maxval, **image;
    int row, rows, cols, format;
    int i, j, n, k;
    unsigned int key = 1292026896;

    // Open input file
    if ((lena = fopen("images/wm.pgm", "rb")) == NULL) {
        fprintf(stderr, "Failed to open PGM image lena_wm.pgm\n");
        exit(1);
    }

    // Initialize and read PGM image
    pgm_init(&argc, argv);
    pgm_readpgminit(lena, &cols, &rows, &maxval, &format);
    image = pgm_allocarray(cols, rows);
    for (row = 0; row < rows; row++) {
        pgm_readpgmrow(lena, image[row], cols, maxval, format);
    }

    // Copy image into buffer
    n = rows * cols;
    double buffer[n];
    for (i = 0, k = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            buffer[k++] = image[i][j];
        }
    }

    // Generate watermark
    double wm[n];
    dugad_generate_watermark(key, wm, n);

    // Look for watermark in buffer
    if (dugad_detect_watermark(buffer, wm, n)) {
        printf("Image DOES contain watermark\n");
    } else {
        printf("Image does NOT contain watermark\n");
    }

    // Cleanup
    fclose(lena);
    pgm_freearray(image, rows);
    return 0;
}

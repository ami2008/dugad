/**
 * file name  : embed.c
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
    FILE *lena, *lena_out;
    gray maxval, **image;
    int row, rows, cols, format;
    int i, j, k, n;
    unsigned key = 1292026896;
    double data = 0.0;

    // Open input and output image files
    if ((lena = fopen("images/lena.pgm", "rb")) == NULL) {
        fprintf(stderr, "Failed to open PGM image lena.pgm\n");
        exit(1);
    }
    if ((lena_out = fopen("images/wm.pgm", "wb")) == NULL) {
        fclose(lena);
        fprintf(stderr, "Failed to open PGM image lena_wm.pgm\n");
        exit(1);
    }

    // Initialize and read input PGM image
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

    // Generate and embed watermark
    double wm[n];
    dugad_generate_watermark(key, wm, n);
    dugad_embed_watermark(buffer, wm, n);

    // Write watermarked buffer into PGM image
    for (i = 0, k = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            data = buffer[k++];
            image[i][j] = data > 255 ? 255 : data < 0 ? 0 : data;
        }
    }

    // Write watermarked image to output file
    pgm_writepgminit(lena_out, cols, rows, maxval, 0);
    for (row = 0; row < rows; row++) {
        pgm_writepgmrow(lena_out, image[row], cols, maxval, 0);
    }

    // Cleanup
    fclose(lena);
    fclose(lena_out);
    pgm_freearray(image, rows);
    return 0;
}

/**
 * file name  : embed.c
 * authors    : Dave Pederson
 * created    : Dec 14, 2010
 *
 * modifications:
 * Date:         Name:            Description:
 * ------------  ---------------  ----------------------------------------------
 * Dec 14, 2010  Dave Pederson    Creation
 */
#include <netpbm/pgm.h>
#include "dugad.h"

int main(int argc, char *argv[])
{
    FILE *lena, *lena_out;
    gray maxval, **image;
    int row, rows, cols, format;
    // Open input and output image files
    if ((lena = fopen("images/lena.pgm", "rb")) == NULL) {
        fprintf(stderr, "Failed to open PGM image lena.pgm\n");
        exit(1);
    }
    if ((lena_out = fopen("images/wm.pgm", "wb")) == NULL) {
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
    // Generate watermark and copy image matrix into row vector
    int i, j, k = 0, n = rows * cols;
    double wm[n], img[n];
    unsigned int key = 1292026896;
    dugad_generate_watermark(key, wm, n);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            img[k++] = image[i][j];
        }
    }
    // Embed watermark
    dugad_embed_watermark(img, wm, n);
    k = 0;
    double imgk = 0.0;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            imgk = img[k++];
            image[i][j] = imgk > 255 ? 255 : imgk < 0 ? 0 : imgk;
        }
    }
    // Write output PGM image
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

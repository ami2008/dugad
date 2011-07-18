/**
 * file name  : detect.c
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
    FILE *lena;
    gray maxval, **image;
    int row, rows, cols, format;
    // Open watermarked PGM
    if ((lena = fopen("images/wm.pgm", "rb")) == NULL) {
        fprintf(stderr, "Failed to open PGM image lena_wm.pgm\n");
        exit(1);
    }
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
    // Look for watermark in image
    if (dugad_detect_watermark(img, wm, n)) {
        printf("Image DOES contain watermark\n");
    } else {
        printf("Image does NOT contain watermark\n");
    }
    // Cleanup
    fclose(lena);
    pgm_freearray(image, rows);
    return 0;
}

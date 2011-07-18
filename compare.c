/**
 * file name  : compare.c
 * authors    : Dave Pederson
 * created    : Jan 05, 2010
 *
 * modifications:
 * Date:         Name:            Description:
 * ------------  ---------------  ----------------------------------------------
 * Jan 05, 2010  Dave Pederson    Creation
 */
#include <netpbm/pgm.h>

int main(int argc, char *argv[])
{
    FILE *lena, *wm, *compare;
    gray maxval0, **image0;
    gray maxval1, **image1;
    int i, j, row, rows, cols, format0, format1;
    // Open original and watermarked image files
    if ((lena = fopen("images/lena.pgm", "rb")) == NULL) {
        fprintf(stderr, "Failed to open PGM image lena.pgm\n");
        exit(1);
    }
    if ((wm = fopen("images/wm.pgm", "rb")) == NULL) {
        fprintf(stderr, "Failed to open PGM image lena.pgm\n");
        exit(1);
    }
    // Initialize and read input PGM images
    pgm_init(&argc, argv);
    pgm_readpgminit(lena, &cols, &rows, &maxval0, &format0);
    image0 = pgm_allocarray(cols, rows);
    for (row = 0; row < rows; row++) {
        pgm_readpgmrow(lena, image0[row], cols, maxval0, format0);
    }
    pgm_readpgminit(wm, &cols, &rows, &maxval1, &format1);
    image1 = pgm_allocarray(cols, rows);
    for (row = 0; row < rows; row++) {
        pgm_readpgmrow(wm, image1[row], cols, maxval0, format0);
    }
    
    // Determine the modified pixels
    unsigned int p0, p1, value;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            p0 = image0[i][j];
            p1 = image1[i][j];
            value = abs(p0 - p1);
            if (value < 2) {
                image1[i][j] = value;
            }
        }
    }    

    // Initialize and write compare image
    if ((compare = fopen("images/compare.pgm", "wb")) == NULL) {
        fprintf(stderr, "Failed to open PGM image out.pgm\n");
        exit(1);
    }
    // Write compare PGM image
    pgm_writepgminit(compare, cols, rows, maxval1, 0);
    for (row = 0; row < rows; row++) {
        pgm_writepgmrow(compare, image1[row], cols, maxval1, 0);
    }
    // Cleanup
    fclose(lena);
    fclose(wm);
    fclose(compare);
    pgm_freearray(image0, rows);
    pgm_freearray(image1, rows);
    return 0;
}

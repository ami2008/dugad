/**
 * file name  : wavelet.h
 * authors    : Dave Pederson
 * created    : Dec 09, 2010
 *
 * modifications:
 * Date:         Name:            Description:
 * ------------  ---------------  ----------------------------------------------
 * Dec 09, 2010  Dave Pederson    Creation
 * Dec 14, 2010  Dave Pederson    Removed function pointers; now uses 
 *                                daubechies 4 coefficient transform directly
 */
#ifndef _WAVELET_H_
#define _WAVELET_H_

#include "daub.h"

/** Transform an array using a wavelet filter
 */
static int wavelet_transform(double *a, int n)
{
    if (!a || n < 4) {
        return 0;
    }
    int i; for (i = n; i >= 4; i >>= 1) {
        if (!daub4_transform(a, i)) {
            return 0;
        }
    }
    return 1;
}

/** Transpose an array using a wavelet filter (inverse transform)
 */
static int wavelet_inverse(double *a, int n)
{
    if (!a || n < 4) {
        return 0;
    }
    int i; for (i = 4; i <= n; i <<= 1) {
        if (!daub4_inverse(a, i)) {
            return 0;
        }
    }
    return 1;
}

#endif /* _WAVELET_H_ */

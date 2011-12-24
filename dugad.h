/**
 * file name  : dugad.h
 * authors    : Dave Pederson
 * created    : Dec 09, 2010
 *
 * modifications:
 * Date:         Name:            Description:
 * ------------  ---------------  ----------------------------------------------
 * Dec 09, 2010  Dave Pederson    Creation
 * Dec 14, 2010  Dave Pederson    Removed wavelet filter function pointers
 */
#ifndef _DUGAD_H_
#define _DUGAD_H_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wavelet.h"

#define ALPHA 0.2
#define T1 40.0
#define T2 50.0

/** 
 * Generate a watermark of pseudo-random numbers seeded by a secret key
 *
 * This routine implements Algorithm P (Polar method for normal deviates) from 
 * Knuth's "The Art of Computer Programming", Vol 2, 3rd ed, p122.
 */
static int dugad_generate_watermark(unsigned int key, double *wm, int n)
{
    if (!wm || n < 2) {
        return 0;
    }
    srand(key);
    double u1, u2, v1, v2, s, wmi, wmi1;
    int i; for (i = 0; i < n; i += 2) {
        do {
            u1 = (double) random() / (double) RAND_MAX;
            u2 = (double) random() / (double) RAND_MAX;
            v1 = (2.0 * u1) - 1.0;
            v2 = (2.0 * u2) - 1.0;
            s = v1 * v1 + v2 * v2;
        }
        while (s >= 1.0);
        if (s == 0) {
            wm[i] = 0;
            wm[i+1] = 0;
        } else {
            s = sqrt((-2.0 * log(s)) / s);
            wmi = v1 * s;
            wmi1 = v2 * s;
            wm[i] = wmi;
            wm[i+1] = wmi1;
        }
    }
    return 1;
}

/** Embed a watermark (dugad algorithm). Assumes len(img) == len(wm) == n.
 */
static int dugad_embed_watermark(double *img, double *wm, int n)
{
    if (!img || !wm || n < 4) {
        return 0;
    }
    // Apply the transform filter
    if (!wavelet_transform(img, n)) {
        fprintf(stderr, "wavelet transform failed\n");
        return 0;
    }
    // Apply the watermark to coefficients
    int i; for (i = n >> 3; i < n; ++i) {
        if (img[i] > T1) {
            img[i] += (ALPHA * fabs(img[i]) * wm[i]);
        }
    }
    // Apply the inverse filter
    if (!wavelet_inverse(img, n)) {
        fprintf(stderr, "wavelet inverse failed\n");
        return 0;
    }
    return 1;
}

/** Determines whether a watermark is present (dugad algorithm).
 *  Assumes len(img) == len(wm) == n.
 */
static int dugad_detect_watermark(double *img, double *wm, int n)
{
    if (!img || !wm || n < 4) {
        return 0;
    }
    // Copy the image
    size_t size = n * sizeof(double);
    double *tmp = memcpy(malloc(size), img, size);
    // Apply the transform filter to the copy
    if (!wavelet_transform(tmp, n)) {
        fprintf(stderr, "wavelet transform failed\n");
        free(tmp);
        return 0;
    }
    // Determine number of matches, threshold sum and correlation sum
    double s, z, ssum, zsum;
    s = z = ssum = zsum = 0.0;
    int i, m; for (i = n >> 3, m = 0; i < n; ++i) {
        if (tmp[i] > T2) {
            ssum += fabs(tmp[i]);
            zsum += tmp[i] * wm[i];
            ++m;
        }
    }
    free(tmp);
    // Calculate threshold and correlation
    if (m > 0) {
        s = (ALPHA / (2.0 * (double) m)) * ssum;
        z = (1.0 / (double) m) * zsum;
    }
    fprintf(stdout, "threshold(s) = %f, correlation(z) = %f\n", s, z);
    return (z > s) ? 1 : 0;
}

#endif /* _DUGAD_H_ */

/**
 * file name  : daub.h
 * authors    : Dave Pederson
 * created    : Dec 09, 2010
 *
 * modifications:
 * Date:         Name:            Description:
 * ------------  ---------------  ----------------------------------------------
 * Dec 09, 2010  Dave Pederson    Creation
 * Dec 14, 2010  Dave Pederson    Removed malloc of tmp buffers
 */
#ifndef _DAUB4_H_
#define _DAUB4_H_

// forward transform scaling (smoothing) coefficients
#define H0 0.4829629131445341
#define H1 0.8365163037378079
#define H2 0.2241438680420134
#define H3 -0.1294095225512604

// forward transform wavelet coefficients
#define G0 H3
#define G1 -H2
#define G2 H1
#define G3 -H0

// inverse transform scaling (smoothing) coefficients
#define IH0 H2
#define IH1 G2
#define IH2 H0
#define IH3 G0

// inverse transform wavelet coefficients
#define IG0 H3
#define IG1 G3
#define IG2 H1
#define IG3 G1

/** Daubechies 4 coefficient transform algorithm
 *
 *  Based on code by Ian Kaplan:
 *  http://www.bearcave.com/misl/misl_tech/wavelets/daubechies
 */
static int daub4_transform(double *a, const int n)
{
    if (!a || n < 4) {
        return 0;
    }
    const int h = n >> 1;
    double tmp[n];
    int i, j; for (i = 0, j = 0; j < n - 3; j += 2, ++i) {
        // Apply high-pass filter
        tmp[i  ] = H0*a[j] + H1*a[j+1] + H2*a[j+2] + H3*a[j+3];
        // Apply low-pass filter
        tmp[i+h] = G0*a[j] + G1*a[j+1] + G2*a[j+2] + G3*a[j+3];
    }
    // Handle periodic boundary conditions (wrap-around)
    tmp[i  ] = H0*a[n-2] + H1*a[n-1] + H2*a[0] + H3*a[1];
    tmp[i+h] = G0*a[n-2] + G1*a[n-1] + G2*a[0] + G3*a[1];
    for (i = 0; i < n; ++i) {
        a[i] = tmp[i];
    }
    return 1;
}

/** Daubechies 4 coefficient inverse transform algorithm
 *
 *  Based on code by Ian Kaplan:
 *  http://www.bearcave.com/misl/misl_tech/wavelets/daubechies
 */
static int daub4_inverse(double *a, const int n)
{
    if (!a || n < 4) {
        return 0;
    }
    const int h = n >> 1;
    const int hp1 = h + 1;
    double tmp[n];
    // Handle periodic boundary conditions (wrap-around)
    tmp[0] = IH0*a[h-1] + IH1*a[n-1] + IH2*a[0] + IH3*a[h];
    tmp[1] = IG0*a[h-1] + IG1*a[n-1] + IG2*a[0] + IG3*a[h];
    int i, j; for (i = 0, j = 2; i < h - 1; ++i) {
        // Apply inverse high-pass filter
        tmp[j++] = IH0*a[i] + IH1*a[i+h] + IH2*a[i+1] + IH3*a[i+hp1];
        // Apply inverse low-pass filter
        tmp[j++] = IG0*a[i] + IG1*a[i+h] + IG2*a[i+1] + IG3*a[i+hp1];
    }
    for (i = 0; i < n; ++i) {
        a[i] = tmp[i];
    }
    return 1;
}

#endif /* _DAUB4_H_ */

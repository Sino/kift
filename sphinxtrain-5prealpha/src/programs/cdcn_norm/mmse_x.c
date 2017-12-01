/* ====================================================================
 * Copyright (c) 1989-2005 Carnegie Mellon University.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * This work was supported in part by funding from the Defense Advanced 
 * Research Projects Agency and the National Science Foundation of the 
 * United States of America, and the CMU Sphinx Speech Consortium.
 *
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY ``AS IS'' AND 
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
 * NOR ITS EMPLOYEES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ====================================================================
 *
 */
/*************************************************************************
 *
 * mmse_x finds the cepstrum vectors x for the whole utterance that minimize
 * the mean squared error.
 * Coded by Alex Acero (acero@s),  November 1989 
 * Modified by Bhiksha, June 94, Lotsa changes.
 * Modified again by Bhiksha to eliminate Hardcoded dimensionality
 *
 *************************************************************************/

#include <stdio.h>
#include <math.h>

#include "header.h"

void mmse_x(float **variance,	/* Speech cepstral variances of modes */
	    float *prob,	/* Ratio of a-prori mode probs. to mod variance */
	    float *tilt,	/* Spectral tilt cepstrum */
	    float *noise,	/* Noise estimate */
	    float **means,	/* The cepstrum means */
	    float **corrbook,	/* The correction factor's codebook */
	    int num_codes,	/* Number of codewords in codebook */
	    float **z,		/* The input cepstrum */
	    float **x,		/* The true speech cepstrum */
	    int num_frames,	/* Number of frames in utterance */
	    int Ndim		/* Dimensionality of data */
    )
{
	float distance,		/* distance value */
	 den,			/* Denominator for reestimation */
	 fk,			/* Probabilities for different codewords */
	  xk[64];		/* The solution for every codeword */
	int i,			/* Index frames in utterance */
	 j,			/* Index coefficients within frame */
	 k;			/* Index codewords in codebook */

	/* Reestimate x vector for all frames in utterance */
	for (i = 0; i < num_frames; i++) {
		/* Initialize cleaned vector x */
		for (j = 0; j < Ndim; j++)
			x[i][j] = 0.0;
		for (j = 0; j < Ndim; j++)
			xk[j] = z[i][j] - tilt[j] - corrbook[0][j];
		distance = dist(xk, means[0], variance[0], Ndim);
		fk = exp((double) -distance / 2) * prob[0];
		den = fk;

		/* Reestimate vector x across all codewords */
		for (k = 1; k < num_codes; k++) {
			/* Find estimated vector for codeword k and update x */
			for (j = 0; j < Ndim; j++)
				xk[j] = z[i][j] - tilt[j] - corrbook[k][j];
			distance = dist(xk, means[k], variance[k], Ndim);
			fk = exp((double) -distance / 2) * prob[k];
			for (j = 0; j < Ndim; j++)
				x[i][j] += xk[j] * fk;
			den += fk;
		}

		/* Normalize the estimated x vector across codewords */
		if (den != 0)
			for (j = 0; j < Ndim; j++)
				x[i][j] /= den;
	}
}

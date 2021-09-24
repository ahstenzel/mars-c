/*
 *  mars_rand.h
 *  Provides a custom RNG function to override the std rand library
 */

#ifndef MARS_RAND_H
#define MARS_RAND_H

#include <stdint.h>

#if defined(MARS_RNG_MWC)
/*=======================================================*/
/* MWC                                                   */
/* Multiply-with-carry. Fast and simple, but not the     */
/* most robust.                                          */
/*=======================================================*/
	static uint32_t mwc_z = 362436069;
	static uint32_t mwc_w = 521288629;
	#define MWC_ZNEW (mwc_z=36969*(mwc_z&65535)+(mwc_z>>16))
	#define MWC_WNEW (mwc_w=18000*(mwc_w&65535)+(mwc_z>>16))
	#define MWC ((MWC_ZNEW<<16)+MWC_WNEW)
	#define RAND MWC
  #define RAND_SEED(s) { mwc_z = seed; mwc_w = 532388629; }
#else
/*=======================================================*/
/* std                                                   */
/* Use the standard C rand() function.                   */
/*=======================================================*/
	#define RAND rand()
  #define RAND_SEED(s) srand(s)
#endif

#endif  // MARS_RAND_H
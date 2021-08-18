/*
 *	mars_common.h
 *	Common declarations and inclusions to be used throughout the project.
 */
#ifndef MARS_COMMON_H
#define MARS_COMMON_H

// Library includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Typedefs
typedef unsigned int (*fptr_t)();           // Function pointer type with no arguments
typedef unsigned int (*fptrfl_t)(float);    // Function pointer type with one float argument

// Platform specific code
#if defined(_WIN32)
#include <Windows.h>

// Mimic linux timeval struct
typedef struct {
	long tv_sec;
	long tv_usec;
} timeval;

// Mimic linux gettimeofday function for wall clock time measurements
int gettimeofday(struct timeval * tp, struct timezone * tzp);

#elif defined(__linux__)
#include <sys/time.h>
#endif

#endif

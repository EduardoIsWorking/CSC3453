#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include "sim.h"     // for PCB

// Reads lines: "<pid> <arrival_ms> <burst_ms>"
// Ignores blank lines and lines starting with '#'
// Returns heap-allocated PCB array in *out, count in *n.
// Returns 0 on success, nonzero on error.
int parseInput(const char *path, PCB **out, int *n);

#endif
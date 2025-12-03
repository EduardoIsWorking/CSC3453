#ifndef PARSER_H
#define PARSER_H
#include "pager.h"
int readRefs(const char *path, Page *out, int nExpected); // returns 0 on success
#endif

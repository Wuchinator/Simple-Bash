#ifndef S21_CAT
#define S21_CAT

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct options {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} opt;

int parser(int argc, char *argv[], opt *options);
int reader(int argc, char *argv[], opt *options);
void process_file(char current, char lastChar, int flag, int strcount,
                  opt *options, FILE *f);
#endif
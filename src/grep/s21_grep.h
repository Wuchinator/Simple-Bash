#ifndef GREP
#define GREP

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 2048
struct flag {
  bool e, i, v, c, l, n, h, s, f, o;
  int linesCount, filesCount, eCount;
} grep = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

char pattern[MAXLEN] = {0};
int flagI(int cflags);
void filework(FILE* file, regmatch_t regmatch[], char* argv[], regex_t regex,
              size_t str_number, char* arr);
int flagO(int is_new_line, regmatch_t regmatch[], int status, char* arr,
          regex_t regex);
void flagL(char* argv[]);
void parser(int argc, char* argv[]);
void reader(int argc, char* argv[]);
void flagE();
void flagF();
void flagN(size_t str_number);
void flagC(char* argv[]);
void output(char* argv[], FILE* file);

#endif
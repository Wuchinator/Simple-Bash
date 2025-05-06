#include "s21_grep.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    parser(argc, argv);
    reader(argc, argv);
  }
  return 0;
}

void parser(int argc, char *argv[]) {
  int flag;
  while ((flag = getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0)) != -1) {
    switch (flag) {
      case 'e':
        grep.e = true;
        grep.eCount++;
        flagE();
        break;
      case 'i':
        grep.i = true;
        break;
      case 'v':
        grep.v = true;
        break;
      case 'c':
        grep.c = true;
        break;
      case 'l':
        grep.l = true;
        break;
      case 'n':
        grep.n = true;
        break;
      case 'h':
        grep.h = true;
        break;
      case 's':
        grep.s = true;
        break;
      case 'f':
        grep.f = true;
        flagF();
        break;
      case 'o':
        grep.o = true;
        break;
      default:
        exit(EXIT_FAILURE);
    }
    if (grep.v && grep.o) grep.o = false;
    if (grep.v && grep.c) grep.o = false;
  }
  if (!grep.e && !grep.f) strcat(pattern, argv[optind++]);
}

void reader(int argc, char *argv[]) {
  grep.filesCount = argc - optind;
  while (optind < argc) {
    FILE *file = fopen(argv[optind], "r");
    if (file != NULL) {
      output(argv, file);
      fclose(file);
    } else if (!grep.s) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n",
              argv[optind]);
    }
    ++optind;
  }
}

void filework(FILE *file, regmatch_t regmatch[], char **argv, regex_t regex,
              size_t str_number, char *arr) {
  while (!feof(file)) {
    if (fgets(arr, MAXLEN, file)) {
      int is_new_line = 1;
      int status = regexec(&regex, arr, 1, regmatch, 0);
      if (grep.v) status = status ? 0 : 1;
      if (status != REG_NOMATCH) {
        if (!grep.c && !grep.l) {
          if (!grep.h && grep.filesCount > 1) printf("%s:", argv[optind]);
          flagN(str_number);
          flagO(is_new_line, regmatch, status, arr, regex);
          if (!grep.o) printf("%s", arr);
          if (arr[strlen(arr) - 1] != '\n' && is_new_line) printf("\n");
        }
        grep.linesCount++;
      }
      str_number++;
    }
  }
}

void output(char *argv[], FILE *file) {
  regex_t regex;
  int cflags = REG_EXTENDED;
  char arr[MAXLEN];
  regmatch_t regmatch[1] = {0};
  size_t str_number = 1;
  grep.linesCount = 0;
  regcomp(&regex, pattern, cflags);
  filework(file, regmatch, argv, regex, str_number, arr);
  flagI(cflags);
  flagC(argv);
  flagL(argv);
  regfree(&regex);
}

int flagO(int is_new_line, regmatch_t regmatch[], int status, char *arr,
          regex_t regex) {
  if (grep.o && !grep.v) {
    is_new_line = 0;
    char *ptr = arr;
    while (!status) {
      printf("%.*s\n", (int)(regmatch[0].rm_eo - regmatch[0].rm_so),
             ptr + regmatch[0].rm_so);
      ptr += regmatch[0].rm_eo;
      status = regexec(&regex, ptr, 1, regmatch, REG_NOTBOL);
    }
  }
  return is_new_line;
}

void flagN(size_t str_number) {
  if (grep.n) printf("%lu:", str_number);
}

void flagL(char **argv) {
  if (grep.l && grep.linesCount) printf("%s\n", argv[optind]);
}

int flagI(int cflags) {
  if (grep.i) {
    cflags = REG_ICASE;
  }
  return cflags;
}

void flagC(char *argv[]) {
  if (grep.c) {
    if (!grep.h && grep.filesCount > 1) printf("%s:", argv[optind]);
    if (grep.l && grep.linesCount) {
      printf("1\n");
    } else {
      printf("%d\n", grep.linesCount);
    }
  }
}

void flagE() {
  if (grep.eCount > 1) {
    strcat(pattern, "|");
  }
  strcat(pattern, optarg);
}

void flagF() {
  FILE *file = NULL;
  char arr[MAXLEN] = {0};
  if ((file = fopen(optarg, "r"))) {
    while (fgets(arr, MAXLEN, file) != NULL) {
      if (arr[strlen(arr) - 1] == '\n') {
        arr[strlen(arr) - 1] = 0;
      }
      if (grep.eCount > 0) {
        strcat(pattern, "|");
      }
      if (*arr == '\0') {
        strcat(pattern, ".");
      } else {
        strcat(pattern, arr);
      }
      grep.eCount++;
    }
    fclose(file);
  } else {
    printf("grep: No such file or directory\n");
    exit(EXIT_FAILURE);
  }
}
#include "s21_cat.h"

int main(int argc, char *argv[]) {
  opt options = {0};
  if (parser(argc, argv, &options) == 1) {
    reader(argc, argv, &options);
  }
  return 0;
}

int parser(int argc, char *argv[], opt *options) {
  int status = 1;
  int opt;
  int optionInd;
  static struct option long_options[] = {
      {"number-nonblank", no_argument, 0, 'b'},
      {"number", no_argument, 0, 'n'},
      {"squeeze-blank", no_argument, 0, 's'},
      {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "+benstvTE", long_options,
                            &optionInd)) != -1) {
    switch (opt) {
      case 'b':
        options->b = true;
        break;
      case 'e':
        options->e = true;
        options->v = true;
        break;
      case 't':
        options->t = true;
        options->v = true;
        break;
      case 'n':
        options->n = true;
        break;
      case 's':
        options->s = true;
        break;
      case 'T':
        options->t = true;
        break;
      case 'E':
        options->e = true;
        break;
      case 'v':
        options->v = true;
        break;
      default:
        fprintf(stderr, "usage: %s benstvTE\n", argv[0]);
        status = -1;
        break;
    }
  }
  return status;
}



int reader(int argc, char *argv[], opt *options) {
  int status = 1;
  for (int i = optind; i < argc; i++) {
    FILE *f = fopen(argv[i], "r");
    int strcount = 1;
    char current = '\0';
    char lastChar = '\n';
    int flag = 0;

    if (f != NULL) {
      process_file(current, lastChar, flag, strcount, options, f);
      fclose(f);
    } else {
      fprintf(stderr, "No such file or directory");
      status = -1;
    }
  }
  return status;
}

void process_file(char current, char lastChar, int flag, int strcount,
                  opt *options, FILE *f) {
  while ((current = fgetc(f)) != EOF) {
    if (options->s == true && current == '\n' && lastChar == '\n' &&
        flag == 0) {
      flag = 1;
    } else if (options->s == true && current != '\n' && flag == 1) {
      if (options->n == true) {
        printf("%6d\t", strcount);
        strcount++;
      }
      if (options->e == true) {
        printf("$");
      }
      printf("\n");

      flag = 0;
    }

    if (options->b) {
      options->n = false;
    }
    if (lastChar == '\n' && ((options->b && current != '\n') || options->n) &&
        flag == 0) {
      printf("%6d\t", strcount++);
    }
    if (options->v) {
      if ((current >= 0 && current < 9) || (current > 10 && current < 32)) {
        printf("^");
        current += 64;
      }
      if (current == 127) {
        printf("^?");
        lastChar = current;
        continue;
      }
    }
    if (options->e && current == '\n' && flag == 0) {
      printf("$");
    }
    if (options->t && current == '\t') {
      printf("^");
      current = 'I';
    }

    lastChar = current;
    if (flag == 0) {
      printf("%c", current);
    }
  }
}

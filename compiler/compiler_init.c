#define MAIN

#include "compiler.h"

void compiler_init(void) {
  label_init();
}

static void usage(void) {
  fprintf(stderr, "usage: acme source.ac\n");
  exit(1);
}

int main(int argc, char **argv)
{
  extern FILE *yyin;  /* Used by lex */
  if(argc != 1) usage();
  n_blocks = 0;
  char *input_file_name = argv[1];
  char *output_file_name = acme_strdup(input_file_name);
  char *p = strrchr(input_file_name, '.');
  if(strcmp(p,".ac")!=0) usage();
  *p++ = 'c';
  *p = '\0';
  output_file = fopen(output_file_name, "w");
  acme_free(output_file_name);
  yyin = fopen(input_file_name, "r");
  compiler_init();
  yyparse();
  fclose(yyin);
}


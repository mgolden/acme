#include <acme_error.h>

#include <stdio.h>
#include <stdlib.h>

static void e_base(const char *level, const char *error) {
  fprintf(stderr, "%s: %s\nat %s on line %d\n", level, error, yyget_text(), yyget_lineno());
}

void e_warning(const char *error) {
  e_base("warning", error);
}

void e_error(const char *error) {
  e_base("error", error);
}

void e_fatal(const char *error) {
  e_base("fatal", error);
  exit(1);
}

#include "acme_error.h"

#include "lex.yy.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static void e_base(const char *level, const char *error, va_list argptr) {
  fprintf(stderr, "%s: ", level);
  vfprintf(stderr, error, argptr);
  fprintf(stderr, "\nat %s on line %d\n", yyget_text(), yyget_lineno());
}

void e_warning(const char *error, ...) {
  va_list argptr;
  va_start(argptr, error);
  e_base("warning", error, argptr);
  va_end(argptr);
}

void e_error(const char *error, ...) {
  va_list argptr;
  va_start(argptr, error);
  e_base("error", error, argptr);
  va_end(argptr);
}

void e_fatal(const char *error, ...) {
  va_list argptr;
  va_start(argptr, error);
  e_base("fatal", error, argptr);
  va_end(argptr);
  exit(1);
}

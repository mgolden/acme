#include <acme_error.h>

static void e_base(char *error) {
  fprintf(stderr, "error: %s\nat %s on line %d\n", error, yy_get_text(), yy_get_lineno());
}

void e_error(char *error) {
  e_base(error);
}

void e_fatal(char *error) {
  e_base(error);
  exit(1);
}

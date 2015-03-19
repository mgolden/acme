#ifndef ACME_ERROR_H
#define ACME_ERROR_H

void e_warning(const char *error, ...);
void e_error(const char *error, ...);
void e_fatal(const char *error, ...);

#endif

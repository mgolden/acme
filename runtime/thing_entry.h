#ifndef THING_ENTRY_H
#define THING_ENTRY_H

#include "../acme_types.h"

#include "ut.h"

struct _thing_entry {
  /* key */
  symbol sym;
  /* The data held */
  thing *t;
  /* Internal, used only by uthash; makes this structure hashable */
  UT_hash_handle hh;
};

void create_thing_entry(thing_entry *thing_table, symbol sym, thing *t);

#endif

#ifndef THING_ENTRY_H
#define THING_ENTRY_H

#include "../acme_types.h"

#include "ut.h"

  UT_hash_handle hh;
};

void create_thing_entry(thing_entry *thing_table, symbol sym, thing *t);

#endif

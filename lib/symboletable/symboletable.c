#include "symboletable.h"
#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

static hashtable *symboletable = nullptr;

// str_hashfun : l'une des fonctions de pré-hachage conseillées par Kernighan et
// Pike pour les chaines de caractères.
size_t str_hashfun(const char *s) {
  size_t h = 0;
  for (const unsigned char *p = (const unsigned char *)s; *p != '\0'; ++p) {
    h = 37 * h + *p;
  }
  return h;
}

void symboletable_init(void) {
  symboletable = hashtable_empty((int (*)(const void *, const void *))strcmp,
                                 (size_t(*)(const void *))str_hashfun, 1.0);
}
#include "symboletable.h"
#include "../../utils.h"
#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static hashtable *symboletable = nullptr; // table de symboles globale
static info_algo *current_algo = nullptr; // algorithme en cours de construction

// str_hashfun : l'une des fonctions de pré-hachage conseillées par Kernighan et
// Pike pour les chaines de caractères.
size_t str_hashfun(const char *s) {
  size_t h = 0;
  for (const unsigned char *p = (const unsigned char *)s; *p != '\0'; ++p) {
    h = 37 * h + *p;
  }
  return h;
}

void *symboletable_init(void) {
  symboletable = hashtable_empty((int (*)(const void *, const void *))strcmp,
                                 (size_t(*)(const void *))str_hashfun, 1.0);
  return symboletable;
}

void *symboletable_add(const char *id) {
  if (symboletable == nullptr) {
    return nullptr;
  }

  printf(";symboletable_add : %s\n", id);

  info_algo *algo = malloc(sizeof(*algo));
  if (algo == nullptr) {
    return nullptr;
  }
  algo->id = id;
  algo->type = UNDEF;
  algo->nb_param = 0;
  algo->nb_varloc = 0;
  algo->param = hashtable_empty((int (*)(const void *, const void *))strcmp,
                                (size_t(*)(const void *))str_hashfun, 1.0);
  algo->varloc = hashtable_empty((int (*)(const void *, const void *))strcmp,
                                 (size_t(*)(const void *))str_hashfun, 1.0);
  if (algo->param == nullptr || algo->varloc == nullptr) {
    free(algo);
    return nullptr;
  }
  current_algo = hashtable_add(symboletable, id, algo);
  if (current_algo != algo) {
    free(algo);
    return nullptr;
  }
  if (current_algo == nullptr) {
    free(algo);
    return nullptr;
  }
  return algo;
}

void symboletable_dispose(void) {
  if (symboletable != nullptr) {
    hashtable_dispose(&symboletable);
  }
}
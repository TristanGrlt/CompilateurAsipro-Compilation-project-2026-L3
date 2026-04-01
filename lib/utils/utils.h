#ifndef UTILS__H
#define UTILS__H

#include "hashtable.h"

typedef enum {
  INT_T,
  BOOL_T,
  UNDEF,
} type_s;

// Enssemble des informations d'un algorithme
typedef struct {
  const char *id;      // id unique de l'algorithme
  type_s type;         // type de retour
  int nb_param;        // nombre de paramètres
  int nb_varloc;       // nombre de variables locales
  type_s *param_types; // types des paramètres dans l'ordre de déclaration
  hashtable *param;    // table de hashage des paramètres
  hashtable *varloc;   // table de hashage des variables locales
} info_algo;

// Ensemble des informations d'une variable qu'elle soit un paramètre ou une
// variable locale
typedef struct {
  char *id; // id unique de la variable
  int nb; // numéro d'arrivé de la variable dans l'algorithme celon sont type
  type_s type; // type de la variable
  int used
} info_var;

#endif
#ifndef SYMBOLETABLE__H
#define SYMBOLETABLE__H

#include "hashtable.h"
#include "utils.h"

// symboletable : initialise la table de symboles. Doit être appelée avant toute
// utilisation de la table de symboles. Renvoie un pointeur nul en cas de
// dépassement de capacité. Renvoie sinon un pointeur vers la table de symboles.
void *symboletable_init(void);

// symboletable_add : ajoute à la table de symboles l'identifiant id. Renvoie
// un pointeur nul en cas de dépassement de capacité. Renvoie sinon un pointeur
// vers la valeur associée à id dans la table de symboles.
void *symboletable_add(const char *id);

// symboletable_get : renvoie un pointeur vers la valeur associée à id dans la
// table de symboles, ou un pointeur nul si id n'est pas présent dans la table
// de symboles.
info_algo *symboletable_get(const char *id);

// symboletable_get_var : renvoie un pointeur vers la valeur associée à id dans
// les variable locale puis des parametre de la table de symboles, ou un
// pointeur nul si id n'est pas présent dans les variables de la table de
// symboles.
info_var *symboletable_get_var(const char *id);
info_var *symboletable_get_var_loc(const char *id);
info_var *symboletable_get_var_param(const char *id);

// symboletable_add_param : ajoute à l'algorithme courrant le paramètre id
// de type type. Renvoie un pointeur nul en cas de dépassement de capacité.
// Renvoie sinon un pointeur vers la valeur associée à id dans la table de
// symboles.
void *symboletable_add_param(const char *id, type_s type);

// symboletable_add_varloc : ajoute à l'algorithme courrant la variable locale
// id de type type. Renvoie un pointeur nul en cas de dépassement de capacité.
void *symboletable_add_varloc(const char *id, type_s type);

// symboletable_dispose : libère les ressources allouées à la table de
// symboles.
void symboletable_dispose(void);

info_algo *symboletable_get_current();

// Change l'algorithme courant
void symboletable_set_current(const char *id);

#endif
#ifndef SYMBOLETABLE__H
#define SYMBOLETABLE__H

#include "../../utils.h"
#include "hashtable.h"

// symboletable : initialise la table de symboles. Doit être appelée avant toute
// utilisation de la table de symboles. Renvoie un pointeur nul en cas de
// dépassement de capacité. Renvoie sinon un pointeur vers la table de symboles.
void *symboletable_init(void);

// symboletable_add : ajoute à la table de symboles l'identifiant id. Renvoie
// un pointeur nul en cas de dépassement de capacité. Renvoie sinon un pointeur
// vers la valeur associée à id dans la table de symboles.
void *symboletable_add(const char *id);

// symboletable_add_param : ajoute à l'algorithme courrant le paramètre id de
// type type. Renvoie un pointeur nul en cas de dépassement de capacité. Renvoie
// sinon un pointeur vers la valeur associée à id dans la table de symboles.
void *symboletable_add_param(const char *id, type_s type);

// symboletable_dispose : libère les ressources allouées à la table de
// symboles.
void symboletable_dispose(void);

#endif
# Compilateur algosipro

Tristan Groult, Louis Guerain

## Flex

Définition de tous les mots-clef du langage et de leur conversion en tokens pour être transmis à Bison.

## Bison

Définition de la grammaire. La règle de départ attend une liste d’algorithmes et un `\CALL{...}{...}` final, qui sera affiché sur la sortie standard.

La compilation se fait en deux passes, tout en ne lisant le fichier source qu’une seule fois. Pour cela, nous utilisons un Arbre de Syntaxe Abstrait (AST).

### Phase 1

#### AST

Un AST est un arbre contenant des nœuds de différents types, qui permet de sauvegarder toutes les informations du fichier en cours de lecture durant la première phase. Le but est d’obtenir une représentation arborescente du programme en cours de compilation et d’effectuer les premières vérifications.

Pour cette implémentation, nous avons choisi un AST ternaire avec les champs `left`, `right` et `center`. La plupart des instructions sauvegardées ne nécessitent que les deux premiers champs. Dans notre projet, seule l’instruction `\IF` nécessite un arbre ternaire, car elle doit contenir une référence vers la condition, les instructions dans le corps du bloc et, finalement, celles du bloc `\ELSE`.

#### Table des symbole

Durant le remplissage de l’AST, nous alimentons également une table des symboles, qui est une table de hachage contenant les algorithmes. Elle possède les champs suivants :

```c
typedef struct {
  const char *id;      // id unique de l'algorithme
  type_s type;         // type de retour
  int nb_param;        // nombre de paramètres
  int nb_varloc;       // nombre de variables locales
  type_s *param_types; // types des paramètres dans l'ordre de déclaration
  hashtable *param;    // table de hashage des paramètres
  hashtable *varloc;   // table de hashage des variables locales
} info_algo;

typedef struct {
  char *id;     // id unique de la variable
  int nb;       // numéro d'arrivée de la variable dans l'algorithme selon son type
  type_s type;  // type de la variable
} info_var;
```

### Phase 2

#### AST

Maintenant que notre arbre représente une vue d’ensemble de notre code, nous appelons la fonction `generate_asm()`, qui prend en entrée la racine de l’arbre créé durant la phase 1 et écrit le code assembleur correspondant en réalisant des appels récursifs à cette même fonction, afin de générer le code de ses successeurs.

## asm

Les fichiers `asm.c` et `asm.h` contiennent une grande partie du code assembleur généré, afin d’améliorer la lisibilité du compilateur. De plus, nous avons défini de nombreuses macros. Dans un premier temps, nous avons défini chaque nom de registre comme une macro équivalente à sa représentation sous forme de chaîne de caractères. Pour toutes les instructions assembleur utilisées dans ce compilateur, nous avons associé une macro-fonction portant leur nom et attendant en entrée le même nombre de registres que la réelle instruction assembleur. Cela évite un très grand nombre d’écritures via une fonction et améliore grandement la lisibilité.

Ce fichier contient aussi la fonction `get_label()`, qui permet de générer un label assembleur unique en le sufixant avec un entier.

## Fonctionnalités implémentées

- La possibilité de faire des appels récursifs.
- La vérification des types lors des opérations.
- L’inférence des types non définis.
- La vérification des types des paramètres.
- Optimisations optionel :
  - Calcule à la compilation du résultat d'opérations entres constante (Pliage de Constantes).
  - Simplification arithmétique (x + 0, x \* 1, etc)

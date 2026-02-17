SHELL=/bin/sh
LEX=flex
YACC=bison
CC=gcc
CFLAGS=-g -std=c2x -pedantic -Wall -Werror -Wwrite-strings -MMD -MP \
       #-DVERBOSE
OPT=
LDFLAGS=
# --nounput: ne génère pas la fonction yyunput() inutile
# --DYY_NO_INPUT: ne prend pas en compte la fonction input() inutile
# -D_POSIX_SOURCE: déclare la fonction fileno()
LEXOPTS=-D_POSIX_SOURCE -DYY_NO_INPUT --nounput
YACCOPTS=

PROG=compil
BUILD_DIR=build

# Librairies: découverte dynamique des fichiers .c et .h dans lib/
LIB_DIRS=$(wildcard lib/*/)
LIB_SRC=$(wildcard lib/*/*.c)
LIB_OBJ=$(patsubst lib/hashtable/%.c,build/hashtable/%.o,$(filter lib/hashtable/%.c,$(LIB_SRC)))
LIB_INCLUDES=$(addprefix -I, $(LIB_DIRS))

# Fichiers générés par lex/yacc
LEX_OBJ=$(BUILD_DIR)/lex.yy.o
YACC_OBJ=$(BUILD_DIR)/$(PROG).tab.o

# Dépendances
DEP=$(LEX_OBJ:.o=.d) $(YACC_OBJ:.o=.d) $(LIB_OBJ:.o=.d)

# Cible principale
$(PROG): $(LEX_OBJ) $(YACC_OBJ) $(LIB_OBJ) | $(BUILD_DIR)
	$(CC) $^ -o $@ $(LDFLAGS) 

# Génération des fichiers lex/yacc
lex.yy.c: $(PROG).l $(PROG).tab.h
	$(LEX) $(LEXOPTS) $<

lex.yy.h: $(PROG).l
	$(LEX) $(LEXOPTS) --header-file=$@ $<

$(PROG).tab.c $(PROG).tab.h: $(PROG).y lex.yy.h
	$(YACC) $(YACCOPTS) $< -d -v --graph

# Compilation des fichiers lex/yacc
$(BUILD_DIR)/lex.yy.o: lex.yy.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OPT) $(LIB_INCLUDES) $< -c -o $@

$(BUILD_DIR)/$(PROG).tab.o: $(PROG).tab.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OPT) $(LIB_INCLUDES) $< -c -o $@

# Compilation des librairies - règles spécifiques par librairie
build/hashtable/%.o: lib/hashtable/%.c | $(BUILD_DIR) build/hashtable
	$(CC) $(CFLAGS) $(OPT) $(LIB_INCLUDES) $< -c -o $@

# Création du dossier build et de ses sous-dossiers
$(BUILD_DIR) build/hashtable:
	mkdir -p $@

# Inclusion des dépendances
-include $(DEP)

clean:
	touch $(PROG).output $(PROG).vcg
	rm -f $(PROG) $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d $(PROG).output $(PROG).vcg 
	rm -f lex.yy.* $(PROG).tab.* compil.gv
	rm -rf $(BUILD_DIR)

distclean: clean
	rm -rf $(BUILD_DIR)

.PHONY: all clean distclean

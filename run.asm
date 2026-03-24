;symboletable_add : fibonacci
; Algorithme fibonacci ajouté à la table de symboles
;symboletable_add_param : n type : 2
;symboletable_add : fact_iter
; Algorithme fact_iter ajouté à la table de symboles
;symboletable_add_param : n type : 2
;symboletable_add_varloc : res type : 0
;symboletable_add_varloc : i type : 0
;symboletable_add : sum_evens
; Algorithme sum_evens ajouté à la table de symboles
;symboletable_add_param : start type : 2
;symboletable_add_param : end type : 2
;symboletable_add_varloc : sum type : 0
;symboletable_add_varloc : rem type : 0
;symboletable_add_varloc : i type : 0
;symboletable_add : math_stress
; Algorithme math_stress ajouté à la table de symboles
;symboletable_add_param : a type : 2
;symboletable_add_param : b type : 2
;symboletable_add_param : c type : 2
;symboletable_add_varloc : x type : 0
;symboletable_add_varloc : y type : 0
;symboletable_add_varloc : z type : 0
;symboletable_add : main_algo
; Algorithme main_algo ajouté à la table de symboles
;symboletable_add_param : dummy type : 2
;symboletable_add_varloc : f type : 0
;symboletable_add_varloc : it type : 0
;symboletable_add_varloc : se type : 0
;symboletable_add_varloc : ms type : 0
	const ax,main
	jmp ax
:msgerr0
@string "[!] Erreur : Division par 0\n"
:err0
	const ax,msgerr0
	callprintfs ax
	end
;;;;;;;;;; PROGRAMME ;;;;;;;;;;
;;;;;;;;;; ALGO ;;;;;;;;;;
;;;;;;;;;; CONFIGURATION ALGO ;;;;;;;;;;
:fibonacci
	push bp
	cp bp,sp
	push ax
	push bx
	push cx
	push dx
;;;;;;;;;; DEBUT ALGO ;;;;;;;;;;
;;;;;;;;;; Lecture du paramètre ;;;;;;;;;;
	cp ax,bp
	const bx,4
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,1
	push ax
;;;;;;;;;; LEQ ;;;;;;;;;;
	pop bx
	pop ax
	const cx,is_leq_2
	cmp ax,bx
	jmpc cx
	sless ax,bx
	jmpc cx
	const ax,0
	const dx,end_leq_3
	jmp dx
:is_leq_2
	const ax,1
:end_leq_3
	push ax
	pop ax
	const cx,else_0
	const bx,0
	cmp ax,bx
	jmpc cx
;;;;;;;;;; RETURN ;;;;;;;;;;
;;;;;;;;;; Lecture du paramètre ;;;;;;;;;;
	cp ax,bp
	const bx,4
	sub ax,bx
	loadw bx,ax
	push bx
	pop ax
	cp cx,bp
	const bx,6
	sub cx,bx
	storew ax,cx
;;;;;;;;;; RESTAURATION ALGO ;;;;;;;;;;
	pop dx
	pop cx
	pop bx
	pop ax
	pop bp
	ret
;;;;;;;;;; FIN ALGO ;;;;;;;;;;
;;;;;;;;;;  ;;;;;;;;;;
	const cx,end_if_1
	jmp cx
:else_0
;;;;;;;;;; RETURN ;;;;;;;;;;
;;;;;;;;;; CALL ;;;;;;;;;;
	const ax,0
	push ax
; Réserver espace pour 0 variables locales
;;;;;;;;;; Lecture du paramètre ;;;;;;;;;;
	cp ax,bp
	const bx,4
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,1
	push ax
;;;;;;;;;; SOUSTRACTION ;;;;;;;;;;
	pop bx
	pop ax
	sub ax,bx
	push ax
;;;;;;;;;; CALL DE L'ALGO ;;;;;;;;;;
	const ax,fibonacci
	call ax
;;;;;;;;;; Restauration variable ;;;;;;;;;;
	pop ax
;;;;;;;;;; CALL ;;;;;;;;;;
	const ax,0
	push ax
; Réserver espace pour 0 variables locales
;;;;;;;;;; Lecture du paramètre ;;;;;;;;;;
	cp ax,bp
	const bx,4
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,2
	push ax
;;;;;;;;;; SOUSTRACTION ;;;;;;;;;;
	pop bx
	pop ax
	sub ax,bx
	push ax
;;;;;;;;;; CALL DE L'ALGO ;;;;;;;;;;
	const ax,fibonacci
	call ax
;;;;;;;;;; Restauration variable ;;;;;;;;;;
	pop ax
;;;;;;;;;; ADDITION ;;;;;;;;;;
	pop bx
	pop ax
	add ax,bx
	push ax
	pop ax
	cp cx,bp
	const bx,6
	sub cx,bx
	storew ax,cx
;;;;;;;;;; RESTAURATION ALGO ;;;;;;;;;;
	pop dx
	pop cx
	pop bx
	pop ax
	pop bp
	ret
;;;;;;;;;; FIN ALGO ;;;;;;;;;;
;;;;;;;;;;  ;;;;;;;;;;
:end_if_1
;;;;;;;;;; RESTAURATION ALGO ;;;;;;;;;;
	pop dx
	pop cx
	pop bx
	pop ax
	pop bp
	ret
;;;;;;;;;; FIN ALGO ;;;;;;;;;;
;;;;;;;;;;  ;;;;;;;;;;
;;;;;;;;;; ALGO ;;;;;;;;;;
;;;;;;;;;; CONFIGURATION ALGO ;;;;;;;;;;
:fact_iter
	push bp
	cp bp,sp
	push ax
	push bx
	push cx
	push dx
;;;;;;;;;; DEBUT ALGO ;;;;;;;;;;
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,1
	push ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,8
	sub cx,bx
	pop ax
	storew ax,cx
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,1
	push ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,6
	sub cx,bx
	pop ax
	storew ax,cx
;;;;;;;;;; BOUCLE DOWHILE ;;;;;;;;;;
:start_while_4
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,6
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; Lecture du paramètre ;;;;;;;;;;
	cp ax,bp
	const bx,4
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; LEQ ;;;;;;;;;;
	pop bx
	pop ax
	const cx,is_leq_6
	cmp ax,bx
	jmpc cx
	sless ax,bx
	jmpc cx
	const ax,0
	const dx,end_leq_7
	jmp dx
:is_leq_6
	const ax,1
:end_leq_7
	push ax
	pop ax
	const cx,end_while_5
	const bx,0
	cmp ax,bx
	jmpc cx
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,8
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,6
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; MULTIPLICATION ;;;;;;;;;;
	pop bx
	pop ax
	mul ax,bx
	push ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,8
	sub cx,bx
	pop ax
	storew ax,cx
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,6
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,1
	push ax
;;;;;;;;;; ADDITION ;;;;;;;;;;
	pop bx
	pop ax
	add ax,bx
	push ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,6
	sub cx,bx
	pop ax
	storew ax,cx
	const cx,start_while_4
	jmp cx
:end_while_5
;;;;;;;;;; RETURN ;;;;;;;;;;
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,8
	sub ax,bx
	loadw bx,ax
	push bx
	pop ax
	cp cx,bp
	const bx,10
	sub cx,bx
	storew ax,cx
;;;;;;;;;; RESTAURATION ALGO ;;;;;;;;;;
	pop dx
	pop cx
	pop bx
	pop ax
	pop bp
	ret
;;;;;;;;;; FIN ALGO ;;;;;;;;;;
;;;;;;;;;;  ;;;;;;;;;;
;;;;;;;;;; RESTAURATION ALGO ;;;;;;;;;;
	pop dx
	pop cx
	pop bx
	pop ax
	pop bp
	ret
;;;;;;;;;; FIN ALGO ;;;;;;;;;;
;;;;;;;;;;  ;;;;;;;;;;
;;;;;;;;;; ALGO ;;;;;;;;;;
;;;;;;;;;; CONFIGURATION ALGO ;;;;;;;;;;
:sum_evens
	push bp
	cp bp,sp
	push ax
	push bx
	push cx
	push dx
;;;;;;;;;; DEBUT ALGO ;;;;;;;;;;
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,0
	push ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,12
	sub cx,bx
	pop ax
	storew ax,cx
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; Lecture du paramètre ;;;;;;;;;;
	cp ax,bp
	const bx,6
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,8
	sub cx,bx
	pop ax
	storew ax,cx
;;;;;;;;;; BOUCLE DOWHILE ;;;;;;;;;;
:start_while_8
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,8
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; Lecture du paramètre ;;;;;;;;;;
	cp ax,bp
	const bx,4
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; LEQ ;;;;;;;;;;
	pop bx
	pop ax
	const cx,is_leq_10
	cmp ax,bx
	jmpc cx
	sless ax,bx
	jmpc cx
	const ax,0
	const dx,end_leq_11
	jmp dx
:is_leq_10
	const ax,1
:end_leq_11
	push ax
	pop ax
	const cx,end_while_9
	const bx,0
	cmp ax,bx
	jmpc cx
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,8
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,8
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,2
	push ax
;;;;;;;;;; DIVISION ;;;;;;;;;;
	pop bx
	pop ax
	const cx,err0
	div ax,bx
	jmpe cx
	push ax
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,2
	push ax
;;;;;;;;;; MULTIPLICATION ;;;;;;;;;;
	pop bx
	pop ax
	mul ax,bx
	push ax
;;;;;;;;;; SOUSTRACTION ;;;;;;;;;;
	pop bx
	pop ax
	sub ax,bx
	push ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,10
	sub cx,bx
	pop ax
	storew ax,cx
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,10
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,0
	push ax
;;;;;;;;;; EQ ;;;;;;;;;;
	pop bx
	pop ax
	const cx,is_eq_14
	cmp ax,bx
	jmpc cx
	const ax,0
	const dx,end_eq_15
	jmp dx
:is_eq_14
	const ax,1
:end_eq_15
	push ax
	pop ax
	const cx,else_12
	const bx,0
	cmp ax,bx
	jmpc cx
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,12
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,8
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; ADDITION ;;;;;;;;;;
	pop bx
	pop ax
	add ax,bx
	push ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,12
	sub cx,bx
	pop ax
	storew ax,cx
	const cx,end_if_13
	jmp cx
:else_12
:end_if_13
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,8
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,1
	push ax
;;;;;;;;;; ADDITION ;;;;;;;;;;
	pop bx
	pop ax
	add ax,bx
	push ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,8
	sub cx,bx
	pop ax
	storew ax,cx
	const cx,start_while_8
	jmp cx
:end_while_9
;;;;;;;;;; RETURN ;;;;;;;;;;
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,12
	sub ax,bx
	loadw bx,ax
	push bx
	pop ax
	cp cx,bp
	const bx,14
	sub cx,bx
	storew ax,cx
;;;;;;;;;; RESTAURATION ALGO ;;;;;;;;;;
	pop dx
	pop cx
	pop bx
	pop ax
	pop bp
	ret
;;;;;;;;;; FIN ALGO ;;;;;;;;;;
;;;;;;;;;;  ;;;;;;;;;;
;;;;;;;;;; RESTAURATION ALGO ;;;;;;;;;;
	pop dx
	pop cx
	pop bx
	pop ax
	pop bp
	ret
;;;;;;;;;; FIN ALGO ;;;;;;;;;;
;;;;;;;;;;  ;;;;;;;;;;
;;;;;;;;;; ALGO ;;;;;;;;;;
;;;;;;;;;; CONFIGURATION ALGO ;;;;;;;;;;
:math_stress
	push bp
	cp bp,sp
	push ax
	push bx
	push cx
	push dx
;;;;;;;;;; DEBUT ALGO ;;;;;;;;;;
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; Lecture du paramètre ;;;;;;;;;;
	cp ax,bp
	const bx,8
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; Lecture du paramètre ;;;;;;;;;;
	cp ax,bp
	const bx,6
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; MULTIPLICATION ;;;;;;;;;;
	pop bx
	pop ax
	mul ax,bx
	push ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,14
	sub cx,bx
	pop ax
	storew ax,cx
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; Lecture du paramètre ;;;;;;;;;;
	cp ax,bp
	const bx,4
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,2
	push ax
;;;;;;;;;; DIVISION ;;;;;;;;;;
	pop bx
	pop ax
	const cx,err0
	div ax,bx
	jmpe cx
	push ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,12
	sub cx,bx
	pop ax
	storew ax,cx
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,14
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,12
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; ADDITION ;;;;;;;;;;
	pop bx
	pop ax
	add ax,bx
	push ax
;;;;;;;;;; Lecture du paramètre ;;;;;;;;;;
	cp ax,bp
	const bx,8
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; SOUSTRACTION ;;;;;;;;;;
	pop bx
	pop ax
	sub ax,bx
	push ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,10
	sub cx,bx
	pop ax
	storew ax,cx
;;;;;;;;;; RETURN ;;;;;;;;;;
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,10
	sub ax,bx
	loadw bx,ax
	push bx
	pop ax
	cp cx,bp
	const bx,16
	sub cx,bx
	storew ax,cx
;;;;;;;;;; RESTAURATION ALGO ;;;;;;;;;;
	pop dx
	pop cx
	pop bx
	pop ax
	pop bp
	ret
;;;;;;;;;; FIN ALGO ;;;;;;;;;;
;;;;;;;;;;  ;;;;;;;;;;
;;;;;;;;;; RESTAURATION ALGO ;;;;;;;;;;
	pop dx
	pop cx
	pop bx
	pop ax
	pop bp
	ret
;;;;;;;;;; FIN ALGO ;;;;;;;;;;
;;;;;;;;;;  ;;;;;;;;;;
;;;;;;;;;; ALGO ;;;;;;;;;;
;;;;;;;;;; CONFIGURATION ALGO ;;;;;;;;;;
:main_algo
	push bp
	cp bp,sp
	push ax
	push bx
	push cx
	push dx
;;;;;;;;;; DEBUT ALGO ;;;;;;;;;;
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; CALL ;;;;;;;;;;
	const ax,0
	push ax
; Réserver espace pour 0 variables locales
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,15
	push ax
;;;;;;;;;; CALL DE L'ALGO ;;;;;;;;;;
	const ax,fibonacci
	call ax
;;;;;;;;;; Restauration variable ;;;;;;;;;;
	pop ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,12
	sub cx,bx
	pop ax
	storew ax,cx
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; CALL ;;;;;;;;;;
	const ax,0
	push ax
; Réserver espace pour 2 variables locales
	push ax
	push ax
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,4
	push ax
;;;;;;;;;; CALL DE L'ALGO ;;;;;;;;;;
	const ax,fact_iter
	call ax
;;;;;;;;;; Restauration variable ;;;;;;;;;;
	pop ax
	pop ax
	pop ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,10
	sub cx,bx
	pop ax
	storew ax,cx
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; CALL ;;;;;;;;;;
	const ax,0
	push ax
; Réserver espace pour 3 variables locales
	push ax
	push ax
	push ax
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,1
	push ax
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,5
	push ax
;;;;;;;;;; CALL DE L'ALGO ;;;;;;;;;;
	const ax,sum_evens
	call ax
;;;;;;;;;; Restauration variable ;;;;;;;;;;
	pop ax
	pop ax
	pop ax
	pop ax
	pop ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,8
	sub cx,bx
	pop ax
	storew ax,cx
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; CALL ;;;;;;;;;;
	const ax,0
	push ax
; Réserver espace pour 3 variables locales
	push ax
	push ax
	push ax
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,12
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,10
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,8
	sub ax,bx
	loadw bx,ax
	push bx
;;;;;;;;;; CALL DE L'ALGO ;;;;;;;;;;
	const ax,math_stress
	call ax
;;;;;;;;;; Restauration variable ;;;;;;;;;;
	pop ax
	pop ax
	pop ax
	pop ax
	pop ax
	pop ax
;;;;;;;;;; Stockage valeur dans la variable locale ;;;;;;;;;;
	cp cx,bp
	const bx,6
	sub cx,bx
	pop ax
	storew ax,cx
;;;;;;;;;; RETURN ;;;;;;;;;;
;;;;;;;;;; Lecture de la variable locale ;;;;;;;;;;
	cp ax,bp
	const bx,12
	sub ax,bx
	loadw bx,ax
	push bx
	pop ax
	cp cx,bp
	const bx,14
	sub cx,bx
	storew ax,cx
;;;;;;;;;; RESTAURATION ALGO ;;;;;;;;;;
	pop dx
	pop cx
	pop bx
	pop ax
	pop bp
	ret
;;;;;;;;;; FIN ALGO ;;;;;;;;;;
;;;;;;;;;;  ;;;;;;;;;;
;;;;;;;;;; RESTAURATION ALGO ;;;;;;;;;;
	pop dx
	pop cx
	pop bx
	pop ax
	pop bp
	ret
;;;;;;;;;; FIN ALGO ;;;;;;;;;;
;;;;;;;;;;  ;;;;;;;;;;
:main
	const bp,stack
	const sp,stack
	const ax,2
	sub sp,ax
;;;;;;;;;; CALL ;;;;;;;;;;
	const ax,0
	push ax
; Réserver espace pour 4 variables locales
	push ax
	push ax
	push ax
	push ax
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,0
	push ax
;;;;;;;;;; CALL DE L'ALGO ;;;;;;;;;;
	const ax,main_algo
	call ax
;;;;;;;;;; Restauration variable ;;;;;;;;;;
	pop ax
	pop ax
	pop ax
	pop ax
	pop ax
;;;;;;;;;;  AFFICHAGE  ;;;;;;;;;;
	cp ax,sp
	callprintfd ax
	end
:stack
@int 0

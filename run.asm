;symboletable_add : func
; Algorithme func ajouté à la table de symboles
;symboletable_add_param : ma_var type : 2
;symboletable_add_param : truc type : 2
;symboletable_add_param : muche type : 2
;symboletable_add_param : bidule type : 2
;symboletable_add_varloc : a type : 0
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
:func
	push bp
	cp bp,sp
	push ax
	push bx
	push cx
	push dx
;;;;;;;;;; DEBUT ALGO ;;;;;;;;;;
;;;;;;;;;; SET ;;;;;;;;;;
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,100
	push ax
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,100
	push ax
;;;;;;;;;; ADDITION ;;;;;;;;;;
	pop bx
	pop ax
	add ax,bx
	push ax
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,50
	push ax
;;;;;;;;;; SOUSTRACTION ;;;;;;;;;;
	pop bx
	pop ax
	sub ax,bx
	push ax
;;;;;;;;;; Stockage valeur dans la variable ;;;;;;;;;;
	cp cx,bp
	const bx,4
	sub cx,bx
	pop ax
	storew ax,cx
;;;;;;;;;; RETURN ;;;;;;;;;;
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,123
	push ax
	pop ax
	cp bx,bp
	const cx,2
	add bx,cx
	storew ax,bx
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
; Réserver espace pour 1 variables locales
	push ax
	const ax,func
	call ax
;;;;;;;;;;  AFFICHAGE  ;;;;;;;;;;
	cp ax,sp
	callprintfd ax
	end
:stack
@int 0

	const ax,main
	jmp ax
:msgerr0
@string "[!] Erreur : Division par 0\n"
:err0
	const ax,msgerr0
	callprintfs ax
	end
:main
	const bp,stack
	const sp,stack
	const ax,2
	sub sp,ax
;symboletable_add : a
;symboletable_add : func
;;;;;;;;;; ALGO ;;;;;;;;;;
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,45
	push ax
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,6
	push ax
;;;;;;;;;; ADDITION ;;;;;;;;;;
	pop bx
	pop ax
	add ax,bx
	push ax
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,3
	push ax
;;;;;;;;;; SOUSTRACTION ;;;;;;;;;;
	pop bx
	pop ax
	sub ax,bx
	push ax
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,45
	push ax
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,6
	push ax
;;;;;;;;;; MULTIPLICATION ;;;;;;;;;;
	pop bx
	pop ax
	mul ax,bx
	push ax
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,3
	push ax
;;;;;;;;;; DIVISION ;;;;;;;;;;
	pop bx
	pop ax
	const cx,err0
	div ax,bx
	jmpe cx
	push ax
;;;;;;;;;; ADDITION ;;;;;;;;;;
	pop bx
	pop ax
	add ax,bx
	push ax
;;;;;;;;;; ENTIER ;;;;;;;;;;
	const ax,45
	push ax
;;;;;;;;;; LEQ ;;;;;;;;;;
	pop bx
	pop ax
	const cx,is_leq_0
	cmp ax,bx
	jmpc cx
	sless ax,bx
	jmpc cx
	const ax,0
	const dx,end_leq_1
	jmp dx
:is_leq_0
	const ax,1
:end_leq_1
	push ax
;;;;;;;;;;  AFFICHAGE  ;;;;;;;;;;
	cp ax,sp
	callprintfd ax
	end
:stack
@int 0

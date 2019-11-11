include macros2.asm 
include number.asm 


.MODEL LARGE
.386
.STACK 200h

MAXTEXTSIZE equ 40

.DATA
	_30	dd	30.0
	z	dd	30
	_1_33	dd	1.33
	_10	dd	10.0
	_Ingrese_Numero	db	'Ingrese_Numero','$', 14 dup (?)
	_3_402823	dd	3.402823
	_65535	dd	65535.0
	_50	dd	50.0
	_IF_AND_ELSE	db	'IF_AND_ELSE','$', 11 dup (?)
	_IF_AND	db	'IF_AND','$', 6 dup (?)
	_100	dd	100.0
	_IF_OR	db	'IF_OR','$', 5 dup (?)
	_20	dd	20.0
	_IF_NOT	db	'IF_NOT','$', 6 dup (?)
	_1	dd	1.0
	_13	dd	13.0
	_B_MAYOR_IGUAL	db	'B_MAYOR_IGUAL','$', 13 dup (?)
	_B_MENOR	db	'B_MENOR','$', 7 dup (?)
	_PruebaY	db	'PruebaY','$', 7 dup (?)
	y	db	'PruebaY','$', 7 dup (?)
	_31_33	dd	31.33
	_5	dd	5.0
	_3	dd	3.0
	_9	dd	9.0
	_2	dd	2.0
	g	db	MAXTEXTSIZE dup (?),'$'
	f	dd	?
	e	dd	?
	d	db	MAXTEXTSIZE dup (?),'$'
	c	dd	?
	b	dd	?
	a	dd	?
	_SUM	dd	?
	_SUB	dd	?
	_MUL	dd	?
	_DIV	dd	?


.CODE
STRLEN PROC
	mov bx,0
STRL01:
	cmp BYTE PTR [SI+BX],'$'
	je STREND
	inc BX
	cmp BX, MAXTEXTSIZE
	jl STRL01
STREND:
	ret
STRLEN ENDP

COPY PROC
	call STRLEN
	cmp bx,MAXTEXTSIZE
	jle COPYSIZEOK
	mov bx,MAXTEXTSIZE
COPYSIZEOK:
	mov cx,bx
	cld
	rep movsb
	mov al,'$'
	mov BYTE PTR [DI],al
	ret
COPY ENDP

	begin: .startup

	; Simple Asignation
	FLD _2
	FSTP b

	; Simple Asignation
	FLD _2
	FSTP c

	; ADD
	FLD _9
	FLD b
	FADD
	FSTP _SUM

	; SUB
	FLD _SUM
	FLD c
	FSUB
	FSTP _SUB

	; DIV
	FLD _SUB
	FLD _3
	FDIV
	FSTP _DIV

	; MUL
	FLD _DIV
	FLD _5
	FMUL
	FSTP a

	; Simple Asignation
	FLD _31_33
	FSTP e

	; PRINT
	displayString y
	newLine 1

	; Condition
	FLD b
	FCOMP a
	FSTSW AX
	SAHF
	JAE IF_0

	; PRINT
	displayString _B_MENOR
	newLine 1

	; Condition
	FLD b
	FCOMP c
	FSTSW AX
	SAHF
	JB IF_1

	; PRINT
	displayString _B_MAYOR_IGUAL
	newLine 1

	; Simple Asignation
	FLD _13
	FSTP b


IF_1:

IF_0:

REPEAT_0:
	; PRINT
	displayFloat b,2
	newLine 1

	; ADD
	FLD _1
	FLD b
	FADD
	FSTP b


REPEAT_1:
	; PRINT
	displayFloat c,2
	newLine 1

	; SUB
	FLD c
	FLD _1
	FSUB
	FSTP c

	; Condition
	FLD c
	FCOMP _2
	FSTSW AX
	SAHF
	JA REPEAT_1

	; Condition
	FLD a
	FCOMP b
	FSTSW AX
	SAHF
	JAE REPEAT_0

	; Condition
	FLD a
	FCOMP b
	FSTSW AX
	SAHF
	JNE IF_2


	; PRINT
	displayString _IF_NOT
	newLine 1

	; Simple Asignation
	FLD _20
	FSTP a


IF_2:
	; Condition
	FLD a
	FCOMP b
	FSTSW AX
	SAHF
	JAE IF_3

	; Condition
	FLD a
	FCOMP c
	FSTSW AX
	SAHF
	JA IF_4


IF_3:
	; PRINT
	displayString _IF_OR
	newLine 1

	; Simple Asignation
	FLD _100
	FSTP b


IF_4:
	; Simple Asignation
	FLD _100
	FSTP a

	; Simple Asignation
	FLD _100
	FSTP b

	; Simple Asignation
	FLD _100
	FSTP c

	; Condition
	FLD a
	FCOMP b
	FSTSW AX
	SAHF
	JNE IF_5

	; Condition
	FLD a
	FCOMP c
	FSTSW AX
	SAHF
	JNE IF_5

	; PRINT
	displayString _IF_AND
	newLine 1

	; Simple Asignation
	FLD _100
	FSTP b

	JMP IF_6

IF_5:
	; PRINT
	displayString _IF_AND_ELSE
	newLine 1

	; Simple Asignation
	FLD _50
	FSTP b


IF_6:
	; Simple Asignation
	FLD _65535
	FSTP a

	; Simple Asignation
	FLD _3_402823
	FSTP e

	; PRINT
	displayString _Ingrese_Numero
	newLine 1


	; READ
	GetFloat a

	; PRINT
	displayFloat a,2
	newLine 1

	; Simple Asignation
	FLD _1
	FSTP a

	; Simple Asignation
	FLD a
	FSTP b

	; Simple Asignation
	FLD _10
	FSTP c

	; Simple Asignation
	FLD _1_33
	FSTP e


	; FREE STACK
	FFREE st(0)
	FFREE st(1)
	FFREE st(2)
	FFREE st(3)
	FFREE st(4)
	FFREE st(5)
	FFREE st(6)
	FFREE st(7)



;END PROGRAM
	mov AX, 4C00h
	int 21h

	END begin
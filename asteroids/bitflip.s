	PRESERVE8			;
						;
	THUMB				;
	AREA 	|.text|, CODE, READONLY		;
	EXPORT flip
	entry
flip	FUNCTION
	PUSH {R1, R2, LR}	; This saves the working registers.
	MOV R2, R0			;
	RBIT R1, R2			;
	LSR R0, R1, #24		;
	POP {R1,R2, LR}		; This restores the previously saved registers.
	BX LR
	ENDFUNC
	END
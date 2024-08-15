	AREA fun, CODE, READONLY

	export function1

function1
	;R0=PlayerID
	;R1=pMorW
	;R2=VorH
	;R3=Y
	;R4=X
	MOV R12, sp
	STMFD sp!,{r4-r8, r10-r11, lr}
	LDR R4, [r12]
	
	LSL R0, R0, #24
	LSL R1, R1, #20
	LSL R2, R2, #16
	LSL R3, R3, #8
	
	ADD R0, R0, R1
	ADD R0, R0, R2
	ADD R0, R0, R3
	ADD R0, R0, R4
	
	LDMFD sp!, {r4-r8, r10-r11, pc}

	END
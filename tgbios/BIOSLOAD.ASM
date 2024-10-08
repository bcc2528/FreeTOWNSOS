						ASSUME	CS:CODE,DS:CODE

						.386p

; device=\TGBIOS.SYS /TGBIOS.BIN
; There should be no space or tab between slash and the file name.


DEVREQ_CMD_INIT			EQU		00h

REQ_LEN					EQU		0
REQ_UNIT				EQU		1
REQ_COMMAND				EQU		2
REQ_STATUS				EQU		3

REQ_INIT_END_OF_RESIDENT_CODE_RETURN	EQU		0Eh
REQ_INIT_COMMAND_PARAMETER				EQU		12H

DEVREQ_STATUS_ERROR		EQU		8000h
DEVREQ_STATUS_NOERROR	EQU		0100h


ONE_TIME_READ_SIZE		EQU		0F000h


CODE					SEGMENT	USE16


CONDEV_HEADER_NEXT			DW		0FFFFh
							DW		0FFFFh
CONDEV_HEADER_ATTRIBWORD	DW		8003h
CONDEV_HEADER_STRATEGY_PTR	DW		STRATEGY
CONDEV_HEADER_INTERRUPT_PTR	DW		INTERRUPT
CONDEV_HEADER_NAME			DB		"TGBIOSLD"


REQ_HEADER				DW		0,0

STRATEGY				PROC
						MOV		CS:[REQ_HEADER],BX
						MOV		CS:[REQ_HEADER+2],ES
						RETF
STRATEGY				ENDP

INTERRUPT				PROC
						PUSHF
						PUSH	ES
						PUSH	DS
						PUSHAD
						LES		BX,DWORD PTR CS:[REQ_HEADER]
						MOV		WORD PTR ES:[BX+REQ_STATUS],DEVREQ_STATUS_NOERROR

						MOV		AL,ES:[BX+REQ_COMMAND]
						OR		AL,AL
						JE		INIT

INTERRUPT_EXIT:
						POPAD
						POP		DS
						POP		ES
						POPF
						RETF
INTERRUPT				ENDP


INIT					PROC
						LDS		SI,ES:[BX+REQ_INIT_COMMAND_PARAMETER]
						CLD

INIT_FIND_SLASH:
						LODSB
						OR		AL,AL
						JE		INIT_SLASH_NOT_FOUND
						CMP		AL,0DH
						JE		INIT_SLASH_NOT_FOUND
						CMP		AL,0AH
						JE		INIT_SLASH_NOT_FOUND

						CMP		AL,'/'
						JNE		INIT_FIND_SLASH

						; DS:SI points to the next letter of /

						PUSH	SI
INIT_MAKE_CSTRING:
						LODSB
						CMP		AL,' '
						JA		INIT_MAKE_CSTRING

						DEC		SI
						MOV		WORD PTR DS:[SI],0
						POP		SI


						MOV		AX,3D00H
						MOV		DX,SI
						INT		21H
						JB		INIT_FILE_NOT_FOUND

						MOV		BX,AX

						MOV		DI,CS
						MOVZX	EDI,DI
						SHL		EDI,4
						MOV		EAX,OFFSET TBIOS_AREA
						ADD		EDI,EAX

INIT_LOAD_LOOP:
						MOV		AH,3FH;
						MOV		CX,ONE_TIME_READ_SIZE

						MOV		EDX,EDI
						SHR		EDX,4
						MOV		DS,DX

						MOV		EDX,EDI
						AND		DX,0Fh	; It is supposed to be always zero because I do ALIGN 16.

						INT		21H

						MOVZX	EAX,AX
						ADD		EDI,EAX

						CMP		AX,ONE_TIME_READ_SIZE
						JAE		INIT_LOAD_LOOP


						MOV		AH,3EH
						INT		21H


						MOV		AX,CS
						SHL		EAX,4
						ADD		EAX,OFFSET TBIOS_AREA

						; TBIOS loaded from EAX to EDI

						MOV		ESI,EDI
						SUB		EDI,EAX

						; Now EDI is the size.   ESI is the last physical address of TBIOS.

						; CMOS 3B90H TBIOS Physical Address
						; CMOS 3B98H TBIOS Size

						MOV		DX,3B90H
						OUT		DX,AL
						ADD		DX,2
						SHR		EAX,8
						OUT		DX,AL
						ADD		DX,2
						SHR		EAX,8
						OUT		DX,AL
						ADD		DX,2
						SHR		EAX,8
						OUT		DX,AL

						MOV		EAX,EDI
						MOV		DX,3B98H
						OUT		DX,AL
						ADD		DX,2
						SHR		EAX,8
						OUT		DX,AL
						ADD		DX,2
						SHR		EAX,8
						OUT		DX,AL
						ADD		DX,2
						SHR		EAX,8
						OUT		DX,AL

						MOV		EDX,ESI
						ADD		EDX,15
						SHR		EDX,4

						LES		BX,DWORD PTR CS:[REQ_HEADER]
						MOV		WORD PTR ES:[BX+REQ_INIT_END_OF_RESIDENT_CODE_RETURN],0
						MOV		ES:[BX+REQ_INIT_END_OF_RESIDENT_CODE_RETURN+2],DX
						MOV		WORD PTR ES:[BX+REQ_STATUS],DEVREQ_STATUS_NOERROR

						JMP		INTERRUPT_EXIT

INIT_FILE_NOT_FOUND:
						LES		BX,DWORD PTR CS:[REQ_HEADER]
						MOV		WORD PTR ES:[BX+REQ_INIT_END_OF_RESIDENT_CODE_RETURN],0
						MOV		ES:[BX+REQ_INIT_END_OF_RESIDENT_CODE_RETURN+2],CS
						MOV		WORD PTR ES:[BX+REQ_STATUS],DEVREQ_STATUS_ERROR
						JMP		INTERRUPT_EXIT

INIT_SLASH_NOT_FOUND:
						LES		BX,DWORD PTR CS:[REQ_HEADER]
						MOV		WORD PTR ES:[BX+REQ_INIT_END_OF_RESIDENT_CODE_RETURN],0
						MOV		ES:[BX+REQ_INIT_END_OF_RESIDENT_CODE_RETURN+2],CS
						MOV		WORD PTR ES:[BX+REQ_STATUS],DEVREQ_STATUS_ERROR
						JMP		INTERRUPT_EXIT

INIT					ENDP

						ALIGN	16
TBIOS_AREA:

MSG_WRONG_PARAMETER		DB		"WRONG PARAMETER$",0
MSG_FILE_NOT_FOUND		DB		"FILE NOT FOUND$",0

CODE					ENDS

						END

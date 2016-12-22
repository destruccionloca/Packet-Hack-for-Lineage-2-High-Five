format PE DLL
include 'win32a.inc'
entry DllMain

;//////////////////////////////////////////////////////////
;//////////////////////////////////////////////////////////

section '.data' data readable writeable
	cMsg db 'By unc1e aka assassins909 for http://tophope.ru', 0

	hookAddr = 0x203D9B55
	retAddr dd hookAddr + 5
	callAddr dd ?
	oldProtect dd ?
	interception dd 0
	msk dd ?
	pck dd 128 dup(0)
	handlerAddr dd ?
	
;//////////////////////////////////////////////////////////
;//////////////////////////////////////////////////////////

section '.code' code executable
proc DllMain, hInstance, fdwReason, lpvReserved
	cmp [fdwReason], DLL_PROCESS_ATTACH
	je @f
	ret

	@@:
	invoke outputdebugstring, cMsg
	invoke disablethreadlibrarycalls, [hInstance]
	invoke createthread, NULL, NULL, workThread, NULL, NULL, NULL
	mov eax, TRUE
	ret
endp

proc workThread
	mov ebx, hookAddr
	mov eax, ebx
	add eax, [eax+1]
	add eax, 5
	mov [callAddr], eax

	invoke virtualprotect, ebx, 5, PAGE_EXECUTE_READWRITE, oldProtect

	mov eax, handler_smart
	sub eax, ebx
	sub eax, 5

	mov BYTE[ebx], 0xe9
	mov [ebx+1], eax
	ret
endp

proc handler_smart
;;;;;;;;;;;;;;change a data
	cmp [interception], 0
	je .handler_smart_end
	push eax
	push esi
	push edx
	push ecx

	mov esi, [esp+24]
	mov [msk], esi
	xor ecx, ecx

	mov eax, esi
	dec eax
	.swMsk:
	inc eax
	cmp byte[eax], 'b'
	je .is_bQ
	cmp byte[eax], 'Q'
	je .is_bQ
	cmp byte[eax], 0x00
	je .swEnd
	mov edx, [esp+28+ecx]
	mov [pck+ecx], edx
	add ecx, 4
	jmp .swMsk

	.is_bQ:
	mov edx, [esp+28+ecx]
	mov [pck+ecx], edx
	add ecx, 4
	mov edx, [esp+28+ecx]
	mov [pck+ecx], edx
	add ecx, 4
	jmp .swMsk

	.swEnd:
	stdcall [handlerAddr]
	pop ecx
	pop edx
	pop esi
	pop eax
;;;;;;;;;;;;;;return to the original function
	.handler_smart_end:
	call [callAddr]
	jmp [retAddr]
endp

proc unc1e_init, arr, addr
	mov eax, [arr]
	mov DWORD[eax], interception
	add eax, 4
	mov DWORD[eax], msk
	add eax, 4
	mov DWORD[eax], pck
	mov eax, [addr]
	mov [handlerAddr], eax
	ret
endp

;//////////////////////////////////////////////////////////
;//////////////////////////////////////////////////////////

section '.idata' import data readable
library kernel32, 'kernel32.dll';dll from Windows

import kernel32,\
	createthread, 'CreateThread',\
	virtualprotect, 'VirtualProtect',\
	outputdebugstring, 'OutputDebugStringA',\
	disablethreadlibrarycalls, 'DisableThreadLibraryCalls',\
	sleep, 'Sleep'
	
;//////////////////////////////////////////////////////////
;//////////////////////////////////////////////////////////

section '.edata' export data readable
export 'unc1e.dll',\
	unc1e_init, 'unc1e_init'
	
;//////////////////////////////////////////////////////////
;//////////////////////////////////////////////////////////

section '.reloc' fixups data readable discardable

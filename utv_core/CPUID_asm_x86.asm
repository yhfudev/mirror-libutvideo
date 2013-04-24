; �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e
; $Id: CPUID_asm_x86.asm 977 2013-04-14 14:56:21Z umezawa $


%include "Common_asm_x86.mac"


section .text


%push
global _cpuid
_cpuid:
	SIMPLE_PROLOGUE 0, result, leaf, subleaf

	mov			eax, dword [esp + %$leaf]
	mov			ecx, dword [esp + %$subleaf]
	mov			edi, dword [esp + %$result]
	cpuid
	mov			dword [edi   ], eax
	mov			dword [edi+ 4], ebx
	mov			dword [edi+ 8], ecx
	mov			dword [edi+12], edx

	SIMPLE_EPILOGUE
%pop


%push
global _xgetbv
_xgetbv:
	SIMPLE_PROLOGUE 0, result, idx

	mov			ecx, dword [esp + %$idx]
	mov			edi, dword [esp + %$result]
	xgetbv
	mov			dword [edi   ], eax
	mov			dword [edi+ 4], edx

	SIMPLE_EPILOGUE
%pop

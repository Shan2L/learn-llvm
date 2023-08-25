	.file	"add.c"
	.text
	.globl	globvar
	.data
	.align 4
	.type	globvar, @object
	.size	globvar, 4
globvar:
	.long	12
	.text
	.globl	_Z3addi
	.type	_Z3addi, @function
_Z3addi:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	globvar(%rip), %edx
	movl	-4(%rbp), %eax
	addl	%edx, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	_Z3addi, .-_Z3addi
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits

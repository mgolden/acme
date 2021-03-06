/* acme block routines for x86-64.
   Copyright (C) 2015 Mitchell Golden.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/* In glibc/sysdeps/x86_64 */
#include <sysdep.h>
#include <jmpbuf-offsets.h>
/* In glibc/sysdeps/generic */
#include <asm-syntax.h>
/* In glibc/include */
#include <stap-probe.h>
/* In glibc-build */
#include <libc-modules.h>
#include <libc-symbols.h>

/* Note that PTR_MANGLE is defined in */
/* glibc/sysdeps/unix/sysv/linux/x86_64/sysdep.h */

#define ACME_BLOCK_INVOKE $1

/* Modeled on setjmp followed by longjmp */
/* Note that the longjump part pops the return address off the stack, */
/* (as we never return to the caller from here), and it doesn't change */
/* the SP, as we don't want to lose the block_caller's stuff on the top */
/* of the stack.  We set rax to the ACME_BLOCK_INVOKE so the block knows */
/* that it is being invoked */

/* extern int block_caller_yield(caller_env c_env, block_env b_env) */

	.text
ENTRY (block_caller_yield)
	/* First the setjmp part */
	/* Save registers into caller_env (register di).  */
	movq %rbx, (JB_RBX*8)(%rdi)
#ifdef PTR_MANGLE
# ifdef __ILP32__
	/* Save the high bits of %rbp first, since PTR_MANGLE will
	   only handle the low bits but we cannot presume %rbp is
	   being used as a pointer and truncate it.  Here we write all
	   of %rbp, but the low bits will be overwritten below.  */
	movq %rbp, (JB_RBP*8)(%rdi)
# endif
	mov %RBP_LP, %RAX_LP
	PTR_MANGLE (%RAX_LP)
	mov %RAX_LP, (JB_RBP*8)(%rdi)
#else
	movq %rbp, (JB_RBP*8)(%rdi)
#endif
	movq %r12, (JB_R12*8)(%rdi)
	movq %r13, (JB_R13*8)(%rdi)
	movq %r14, (JB_R14*8)(%rdi)
	movq %r15, (JB_R15*8)(%rdi)
	lea 8(%rsp), %RDX_LP	/* Save SP as it will be after we return.  */
#ifdef PTR_MANGLE
	PTR_MANGLE (%RDX_LP)
#endif
	movq %rdx, (JB_RSP*8)(%rdi)
	/* Save PC we are returning to now. */
	/* Unlike the original setjmp, we are popping this instead of just taking (%sp), */
	/* because we don't want the return address on the stack anymore */
	popq %RAX_LP
	LIBC_PROBE (setjmp, 3, LP_SIZE@%RDI_LP, -4@%esi, LP_SIZE@%RAX_LP)
#ifdef PTR_MANGLE
	PTR_MANGLE (%RAX_LP)
#endif
	movq %rax, (JB_PC*8)(%rdi)

	
	/* Now the longjmp part */
	/* Restore registers from block_env (register si).  */
	mov (JB_RSP*8)(%rsi),%R8_LP
	mov (JB_RBP*8)(%rsi),%R9_LP
	mov (JB_PC*8)(%rsi),%RDX_LP
#ifdef PTR_DEMANGLE
	PTR_DEMANGLE (%R8_LP)
	PTR_DEMANGLE (%R9_LP)
	PTR_DEMANGLE (%RDX_LP)
# ifdef __ILP32__
	/* We ignored the high bits of the %rbp value because only the low
	   bits are mangled.  But we cannot presume that %rbp is being used
	   as a pointer and truncate it, so recover the high bits.  */
	movl (JB_RBP*8 + 4)(%rsi), %eax
	shlq $32, %rax
	orq %rax, %r9
# endif
#endif
	LIBC_PROBE (longjmp, 3, LP_SIZE@%RDI_LP, -4@%esi, LP_SIZE@%RDX_LP)
	/* We add unwind information for the target here.  */
	cfi_def_cfa(%rsi, 0)
	/* cfi_register(%rsp,%r8) SP is not going to be set by this */
	cfi_register(%rbp,%r9)
	cfi_register(%rip,%rdx)
	cfi_offset(%rbx,JB_RBX*8)
	cfi_offset(%r12,JB_R12*8)
	cfi_offset(%r13,JB_R13*8)
	cfi_offset(%r14,JB_R14*8)
	cfi_offset(%r15,JB_R15*8)
	movq (JB_RBX*8)(%rsi),%rbx
	movq (JB_R12*8)(%rsi),%r12
	movq (JB_R13*8)(%rsi),%r13
	movq (JB_R14*8)(%rsi),%r14
	movq (JB_R15*8)(%rsi),%r15
	/* Set return value for setjmp.  */
	movq ACME_BLOCK_INVOKE, %rax
	/* DO NOT SET THE STACK POINTER, only BP, so NO: mov %R8_LP,%RSP_LP */
	movq %r9,%rbp
	LIBC_PROBE (longjmp_target, 3,
		    LP_SIZE@%RDI_LP, -4@%eax, LP_SIZE@%RDX_LP)
	jmpq *%rdx
END (block_caller_yield)


/* Modeled on setjmp */

/* extern int block_define(block_env b_env) */

ENTRY (block_define)
	/* Save registers.  */
	movq %rbx, (JB_RBX*8)(%rdi)
#ifdef PTR_MANGLE
# ifdef __ILP32__
	/* Save the high bits of %rbp first, since PTR_MANGLE will
	   only handle the low bits but we cannot presume %rbp is
	   being used as a pointer and truncate it.  Here we write all
	   of %rbp, but the low bits will be overwritten below.  */
	movq %rbp, (JB_RBP*8)(%rdi)
# endif
	mov %RBP_LP, %RAX_LP
	PTR_MANGLE (%RAX_LP)
	mov %RAX_LP, (JB_RBP*8)(%rdi)
#else
	movq %rbp, (JB_RBP*8)(%rdi)
#endif
	movq %r12, (JB_R12*8)(%rdi)
	movq %r13, (JB_R13*8)(%rdi)
	movq %r14, (JB_R14*8)(%rdi)
	movq %r15, (JB_R15*8)(%rdi)
	lea 8(%rsp), %RDX_LP	/* Save SP as it will be after we return.  */
#ifdef PTR_MANGLE
	PTR_MANGLE (%RDX_LP)
#endif
	movq %rdx, (JB_RSP*8)(%rdi)
	mov (%rsp), %RAX_LP	/* Save PC we are returning to now.  */
	LIBC_PROBE (setjmp, 3, LP_SIZE@%RDI_LP, -4@%esi, LP_SIZE@%RAX_LP)
#ifdef PTR_MANGLE
	PTR_MANGLE (%RAX_LP)
#endif
	movq %rax, (JB_PC*8)(%rdi)
        /* Return 0 */
	xorq %rax, %rax
	ret
END (block_define)


/* Modeled on longjmp */

/* extern void block_caller_leave(block_env b_env, int ret_type) */

	.text
ENTRY(block_caller_leave)
	/* Restore registers.  */
	mov (JB_RSP*8)(%rdi),%R8_LP
	mov (JB_RBP*8)(%rdi),%R9_LP
	mov (JB_PC*8)(%rdi),%RDX_LP
#ifdef PTR_DEMANGLE
	PTR_DEMANGLE (%R8_LP)
	PTR_DEMANGLE (%R9_LP)
	PTR_DEMANGLE (%RDX_LP)
# ifdef __ILP32__
	/* We ignored the high bits of the %rbp value because only the low
	   bits are mangled.  But we cannot presume that %rbp is being used
	   as a pointer and truncate it, so recover the high bits.  */
	movl (JB_RBP*8 + 4)(%rdi), %eax
	shlq $32, %rax
	orq %rax, %r9
# endif
#endif
	LIBC_PROBE (longjmp, 3, LP_SIZE@%RDI_LP, -4@%esi, LP_SIZE@%RDX_LP)
	/* We add unwind information for the target here.  */
	cfi_def_cfa(%rdi, 0)
	cfi_register(%rsp,%r8)
	cfi_register(%rbp,%r9)
	cfi_register(%rip,%rdx)
	cfi_offset(%rbx,JB_RBX*8)
	cfi_offset(%r12,JB_R12*8)
	cfi_offset(%r13,JB_R13*8)
	cfi_offset(%r14,JB_R14*8)
	cfi_offset(%r15,JB_R15*8)
	movq (JB_RBX*8)(%rdi),%rbx
	movq (JB_R12*8)(%rdi),%r12
	movq (JB_R13*8)(%rdi),%r13
	movq (JB_R14*8)(%rdi),%r14
	movq (JB_R15*8)(%rdi),%r15
	/* Set return value for setjmp.  */
	movl %esi, %eax
        /* Finish registers and go */
	mov %R8_LP,%RSP_LP
	movq %r9,%rbp
	LIBC_PROBE (longjmp_target, 3,
		    LP_SIZE@%RDI_LP, -4@%eax, LP_SIZE@%RDX_LP)
	jmpq *%rdx
END (block_caller_leave)


/* Modeled on longjmp.  Differences */
/* (a) Uses a caller_env, so sets the value of the thing therein */

/* extern void block_leave(caller_env c_env, thing t, int ret_type) */

	.text
ENTRY(block_leave)
	/* Restore registers.  */
	mov (JB_RSP*8)(%rdi),%R8_LP
	mov (JB_RBP*8)(%rdi),%R9_LP
	mov (JB_PC*8)(%rdi),%RDX_LP
#ifdef PTR_DEMANGLE
	PTR_DEMANGLE (%R8_LP)
	PTR_DEMANGLE (%R9_LP)
	PTR_DEMANGLE (%RDX_LP)
# ifdef __ILP32__
	/* We ignored the high bits of the %rbp value because only the low
	   bits are mangled.  But we cannot presume that %rbp is being used
	   as a pointer and truncate it, so recover the high bits.  */
	movl (JB_RBP*8 + 4)(%rdi), %eax
	shlq $32, %rax
	orq %rax, %r9
# endif
#endif
	LIBC_PROBE (longjmp, 3, LP_SIZE@%RDI_LP, -4@%esi, LP_SIZE@%RDX_LP)
	/* We add unwind information for the target here.  */
	cfi_def_cfa(%rdi, 0)
	cfi_register(%rsp,%r8)
	cfi_register(%rbp,%r9)
	cfi_register(%rip,%rdx)
	cfi_offset(%rbx,JB_RBX*8)
	cfi_offset(%r12,JB_R12*8)
	cfi_offset(%r13,JB_R13*8)
	cfi_offset(%r14,JB_R14*8)
	cfi_offset(%r15,JB_R15*8)
	movq (JB_RBX*8)(%rdi),%rbx
	movq (JB_R12*8)(%rdi),%r12
	movq (JB_R13*8)(%rdi),%r13
	movq (JB_R14*8)(%rdi),%r14
	movq (JB_R15*8)(%rdi),%r15
	/* Set return value for setjmp.  */
	movl %ecx, %eax
	/* Stick the thing into the caller_env */
	movq %rsi, (JB_SIZE)(%rdi)
	movq %rdx, (JB_SIZE+8)(%rdi)
        /* Finish registers and go */
	mov %R8_LP,%RSP_LP
	movq %r9,%rbp
	LIBC_PROBE (longjmp_target, 3,
		    LP_SIZE@%RDI_LP, -4@%eax, LP_SIZE@%RDX_LP)
	jmpq *%rdx
END (block_leave)

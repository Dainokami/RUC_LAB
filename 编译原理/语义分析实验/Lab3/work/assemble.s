.LC0:
	.string	"%d"
.LC1:
	.string	"%d\n"
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	movq	%rsp, %rbp
	subq	$4, %rsp
	subq	$4, %rsp
	movl	-4(rbp), %r8d
	movl	-8(rbp), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	r8d, -12(rbp)
	movl	-12(rbp), %edi
	subq	$4, %rsp
	movl	%edi, -16(rbp)
	addq	$16, rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret

	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	movq	%rsp, %rbp
	movl	$3, %eax
	addq	$0, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret

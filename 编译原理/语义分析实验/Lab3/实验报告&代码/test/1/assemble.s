.LC0:
	.string	"%d\n"
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	movq	%rsp, %rbp
	subq	$32, %rsp
	movl	$1, -32(%rbp)
	movl	$2, -28(%rbp)
	movl	$3, -24(%rbp)
	movl	$4, -20(%rbp)
	movl	$0, -16(%rbp)
	movl	$0, -12(%rbp)
	movl	$7, -8(%rbp)
	movl	$0, -4(%rbp)
	subq	$4, %rsp
	movl	$3, %edi
	movl	%edi, -36(%rbp)
	subq	$32, %rsp
	movl	$0, %edi
	movl	%edi, -68(%rbp)
	movl	$0, %edi
	movl	%edi, -64(%rbp)
	movl	$0, %edi
	movl	%edi, -60(%rbp)
	movl	$0, %edi
	movl	%edi, -56(%rbp)
	movl	$0, %edi
	movl	%edi, -52(%rbp)
	movl	$0, %edi
	movl	%edi, -48(%rbp)
	movl	$0, %edi
	movl	%edi, -44(%rbp)
	movl	$0, %edi
	movl	%edi, -40(%rbp)
	subq	$32, %rsp
	movl	$1, %edi
	movl	%edi, -100(%rbp)
	movl	$2, %edi
	movl	%edi, -96(%rbp)
	movl	$3, %edi
	movl	%edi, -92(%rbp)
	movl	$4, %edi
	movl	%edi, -88(%rbp)
	movl	$5, %edi
	movl	%edi, -84(%rbp)
	movl	$6, %edi
	movl	%edi, -80(%rbp)
	movl	$7, %edi
	movl	%edi, -76(%rbp)
	movl	$8, %edi
	movl	%edi, -72(%rbp)
	subq	$32, %rsp
	movl	$1, %edi
	movl	%edi, -132(%rbp)
	movl	$2, %edi
	movl	%edi, -128(%rbp)
	movl	$3, %edi
	movl	%edi, -124(%rbp)
	movl	$0, %edi
	movl	%edi, -120(%rbp)
	movl	$5, %edi
	movl	%edi, -116(%rbp)
	movl	$0, %edi
	movl	%edi, -112(%rbp)
	movl	$7, %edi
	movl	%edi, -108(%rbp)
	movl	$8, %edi
	movl	%edi, -104(%rbp)
	subq	$4, %rsp
	movl	$0, -136(%rbp)
	movl	$1, %r8d
	imull	$1, %r8d
	addl	-136(%rbp), %r8d
	movl	%r8d, -136(%rbp)
	movl	$2, %r8d
	imull	$2, %r8d
	addl	-136(%rbp), %r8d
	movl	%r8d, -136(%rbp)
	subq	$4, %rsp
	movl	$0, -140(%rbp)
	movl	$1, %r8d
	imull	$1, %r8d
	addl	-140(%rbp), %r8d
	movl	%r8d, -140(%rbp)
	movl	$2, %r8d
	imull	$2, %r8d
	addl	-140(%rbp), %r8d
	movl	%r8d, -140(%rbp)
	subq	$32, %rsp
	movl	-136(%rbp), %ebx
	cltq
	movl	-132(%rbp, %rbx, 4), %edi
	movl	%edi, -172(%rbp)
	movl	-140(%rbp), %ebx
	cltq
	movl	-100(%rbp, %rbx, 4), %edi
	movl	%edi, -168(%rbp)
	movl	$3, %edi
	movl	%edi, -164(%rbp)
	movl	$4, %edi
	movl	%edi, -160(%rbp)
	movl	$5, %edi
	movl	%edi, -156(%rbp)
	movl	$6, %edi
	movl	%edi, -152(%rbp)
	movl	$7, %edi
	movl	%edi, -148(%rbp)
	movl	$8, %edi
	movl	%edi, -144(%rbp)
	subq	$4, %rsp
	movl	$0, -176(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-176(%rbp), %r8d
	movl	%r8d, -176(%rbp)
	movl	$1, %r8d
	imull	$1, %r8d
	addl	-176(%rbp), %r8d
	movl	%r8d, -176(%rbp)
	movl	$3, %r8d
	imull	$2, %r8d
	addl	-176(%rbp), %r8d
	movl	%r8d, -176(%rbp)
	subq	$4, %rsp
	movl	$0, -180(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-180(%rbp), %r8d
	movl	%r8d, -180(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-180(%rbp), %r8d
	movl	%r8d, -180(%rbp)
	movl	$0, %r8d
	imull	$2, %r8d
	addl	-180(%rbp), %r8d
	movl	%r8d, -180(%rbp)
	movl	-176(%rbp), %ebx
	cltq
	movl	-172(%rbp, %rbx, 4), %r8d
	movl	-180(%rbp), %ebx
	cltq
	movl	-172(%rbp, %rbx, 4), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -184(%rbp)
	subq	$4, %rsp
	movl	$0, -188(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-188(%rbp), %r8d
	movl	%r8d, -188(%rbp)
	movl	$1, %r8d
	imull	$1, %r8d
	addl	-188(%rbp), %r8d
	movl	%r8d, -188(%rbp)
	movl	$0, %r8d
	imull	$2, %r8d
	addl	-188(%rbp), %r8d
	movl	%r8d, -188(%rbp)
	movl	-184(%rbp), %r8d
	movl	-188(%rbp), %ebx
	cltq
	movl	-172(%rbp, %rbx, 4), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -192(%rbp)
	subq	$4, %rsp
	movl	$0, -196(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-196(%rbp), %r8d
	movl	%r8d, -196(%rbp)
	movl	$3, %r8d
	imull	$2, %r8d
	addl	-196(%rbp), %r8d
	movl	%r8d, -196(%rbp)
	movl	-192(%rbp), %r8d
	movl	-196(%rbp), %ebx
	cltq
	movl	-132(%rbp, %rbx, 4), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -200(%rbp)
	subq	$8, %rsp
	# align stack
	movl	-200(%rbp), %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	addq	$208, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret

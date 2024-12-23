.LC0:
	.string	"%d\n"
	.globl	exgcd
	.type	main, @function
exgcd:
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	movq	%rsp, %rbp
.L1:
	# if ==
	movl	44(%rbp), %r8d
	movl	$0, %r9d
	cmpl	%r9d, %r8d
	je	.L2
	jmp	.L3
.L2:
	# enter stmt
	subq	$4, %rsp
	movl	$0, -4(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-4(%rbp), %r8d
	movl	%r8d, -4(%rbp)
	movl	$1, %r9d
	movl	-4(%rbp), %ebx
	cltq
	movq	48(%rbp), %r10
	movl	%r9d, (%r10, %rbx, 4)
	subq	$4, %rsp
	movl	$0, -8(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-8(%rbp), %r8d
	movl	%r8d, -8(%rbp)
	movl	$0, %r9d
	movl	-8(%rbp), %ebx
	cltq
	movq	56(%rbp), %r10
	movl	%r9d, (%r10, %rbx, 4)
	movl	40(%rbp), %eax
	addq	$8, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
	addq	$8, %rsp
	addq	$0, %rsp
	jmp	.L4
.L3:
	# enter stmt
	movl	40(%rbp), %eax
	movl	44(%rbp), %r9d
	cltd
	idivl %r9d
	subq	$4, %rsp
	movl	%edx, -4(%rbp)
	subq	$12, %rsp
	# align stack
	movq	56(%rbp), %r8
	subq	$8, %rsp
	movq	%r8, -24(%rbp)
	movq	48(%rbp), %r8
	subq	$8, %rsp
	movq	%r8, -32(%rbp)
	movl	-4(%rbp), %r8d
	subq	$4, %rsp
	movl	%r8d, -36(%rbp)
	movl	44(%rbp), %r8d
	subq	$4, %rsp
	movl	%r8d, -40(%rbp)
	subq	$8, %rsp
	# align stack
	call	exgcd
	subq	$4, %rsp
	movl	%eax, -52(%rbp)
	movl	-52(%rbp), %edi
	subq	$4, %rsp
	movl	%edi, -56(%rbp)
	subq	$4, %rsp
	movl	$0, -60(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-60(%rbp), %r8d
	movl	%r8d, -60(%rbp)
	movl	-60(%rbp), %ebx
	cltq
	movq	48(%rbp), %r10
	movl	(%r10, %rbx, 4), %edi
	subq	$4, %rsp
	movl	%edi, -64(%rbp)
	subq	$4, %rsp
	movl	$0, -68(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-68(%rbp), %r8d
	movl	%r8d, -68(%rbp)
	subq	$4, %rsp
	movl	$0, -72(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-72(%rbp), %r8d
	movl	%r8d, -72(%rbp)
	movl	-72(%rbp), %ebx
	cltq
	movq	56(%rbp), %r10
	movl	(%r10, %rbx, 4), %r9d
	movl	-68(%rbp), %ebx
	cltq
	movq	48(%rbp), %r10
	movl	%r9d, (%r10, %rbx, 4)
	subq	$4, %rsp
	movl	$0, -76(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-76(%rbp), %r8d
	movl	%r8d, -76(%rbp)
	movl	40(%rbp), %eax
	movl	44(%rbp), %r9d
	cltd
	idivl	%r9d
	subq	$4, %rsp
	movl	%eax, -80(%rbp)
	subq	$4, %rsp
	movl	$0, -84(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-84(%rbp), %r8d
	movl	%r8d, -84(%rbp)
	movl	-80(%rbp), %r8d
	movl	-84(%rbp), %ebx
	cltq
	movq	56(%rbp), %r10
	movl	(%r10, %rbx, 4), %r9d
	imull	%r8d, %r9d
	subq	$4, %rsp
	movl	%r9d, -88(%rbp)
	movl	-64(%rbp), %r8d
	movl	-88(%rbp), %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -92(%rbp)
	movl	-92(%rbp), %r9d
	movl	-76(%rbp), %ebx
	cltq
	movq	56(%rbp), %r10
	movl	%r9d, (%r10, %rbx, 4)
	movl	-56(%rbp), %eax
	addq	$92, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
	addq	$92, %rsp
	addq	$0, %rsp
.L4:
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	movq	%rsp, %rbp
	movl	$7, %edi
	subq	$4, %rsp
	movl	%edi, -4(%rbp)
	movl	$15, %edi
	subq	$4, %rsp
	movl	%edi, -8(%rbp)
	subq	$4, %rsp
	movl	$1, %edi
	movl	%edi, -12(%rbp)
	subq	$4, %rsp
	movl	$1, %edi
	movl	%edi, -16(%rbp)
	leaq	-16(%rbp), %r8
	subq	$8, %rsp
	movq	%r8, -24(%rbp)
	leaq	-12(%rbp), %r8
	subq	$8, %rsp
	movq	%r8, -32(%rbp)
	movl	-8(%rbp), %r8d
	subq	$4, %rsp
	movl	%r8d, -36(%rbp)
	movl	-4(%rbp), %r8d
	subq	$4, %rsp
	movl	%r8d, -40(%rbp)
	subq	$8, %rsp
	# align stack
	call	exgcd
	subq	$4, %rsp
	movl	%eax, -52(%rbp)
	subq	$4, %rsp
	movl	$0, -56(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-56(%rbp), %r8d
	movl	%r8d, -56(%rbp)
	subq	$4, %rsp
	movl	$0, -60(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-60(%rbp), %r8d
	movl	%r8d, -60(%rbp)
	movl	-60(%rbp), %ebx
	cltq
	movl	-12(%rbp, %rbx, 4), %eax
	movl	-8(%rbp), %r9d
	cltd
	idivl %r9d
	subq	$4, %rsp
	movl	%edx, -64(%rbp)
	movl	-64(%rbp), %r8d
	movl	-8(%rbp), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -68(%rbp)
	movl	-68(%rbp), %eax
	movl	-8(%rbp), %r9d
	cltd
	idivl %r9d
	subq	$4, %rsp
	movl	%edx, -72(%rbp)
	movl	-72(%rbp), %r9d
	movl	-56(%rbp), %ebx
	cltq
	movl	%r9d, -12(%rbp, %rbx, 4)
	subq	$4, %rsp
	movl	$0, -76(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-76(%rbp), %r8d
	movl	%r8d, -76(%rbp)
	subq	$4, %rsp
	# align stack
	movl	-76(%rbp), %ebx
	cltq
	movl	-12(%rbp, %rbx, 4), %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	addq	$80, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret

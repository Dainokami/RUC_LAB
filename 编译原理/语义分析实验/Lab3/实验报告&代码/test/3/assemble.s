.LC0:
	.string	"%d\n"
	.globl	a0
	.data
	.align	4
	.type	a0, @object
	.size	a0, 12
a0:
	.long	0
	.long	0
	.long	0
	.text
	.globl	b0
	.data
	.align	4
	.type	b0, @object
	.size	b0, 16
b0:
	.long	0
	.long	1
	.long	0
	.long	0
	.text
	.globl	c0
	.data
	.align	4
	.type	c0, @object
	.size	c0, 28
c0:
	.long	2
	.long	8
	.long	6
	.long	3
	.long	9
	.long	1
	.long	5
	.text
	.globl	d0
	.data
	.align	4
	.type	d0, @object
	.size	d0, 44
d0:
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.text
	.globl	e0
	.data
	.align	4
	.type	e0, @object
	.size	e0, 8
e0:
	.long	22
	.long	33
	.text
	.globl	f0
	.data
	.align	4
	.type	f0, @object
	.size	f0, 24
f0:
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.text
	.globl	g0
	.data
	.align	4
	.type	g0, @object
	.size	g0, 36
g0:
	.long	85
	.long	0
	.long	1
	.long	29
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.text
	.globl	scj82c9s0j
	.data
	.align	4
	.type	scj82c9s0j, @object
	.size	scj82c9s0j, 4
scj82c9s0j:
	.long	9
	.text
	.globl	a
	.data
	.align	4
	.type	a, @object
	.size	a, 60
a:
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.text
	.globl	b
	.data
	.align	4
	.type	b, @object
	.size	b, 60
b:
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.text
	.globl	c
	.data
	.align	4
	.type	c, @object
	.size	c, 60
c:
	.long	1
	.long	2
	.long	3
	.long	4
	.long	5
	.long	6
	.long	7
	.long	8
	.long	9
	.long	10
	.long	11
	.long	12
	.long	13
	.long	14
	.long	15
	.text
	.globl	d
	.data
	.align	4
	.type	d, @object
	.size	d, 60
d:
	.long	1
	.long	2
	.long	3
	.long	4
	.long	5
	.long	6
	.long	7
	.long	8
	.long	9
	.long	10
	.long	11
	.long	12
	.long	13
	.long	14
	.long	15
	.text
	.globl	e
	.data
	.align	4
	.type	e, @object
	.size	e, 60
e:
	.long	1
	.long	2
	.long	3
	.long	4
	.long	5
	.long	6
	.long	7
	.long	8
	.long	9
	.long	10
	.long	11
	.long	12
	.long	13
	.long	14
	.long	15
	.text
	.globl	f
	.data
	.align	4
	.type	f, @object
	.size	f, 20
f:
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.text
	.globl	g
	.data
	.align	4
	.type	g, @object
	.size	g, 60
g:
	.long	1
	.long	2
	.long	3
	.long	4
	.long	0
	.long	0
	.long	7
	.long	0
	.long	0
	.long	10
	.long	11
	.long	12
	.long	0
	.long	0
	.long	0
	.text
	.globl	h
	.data
	.align	4
	.type	h, @object
	.size	h, 12
h:
	.long	0
	.long	0
	.long	0
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	movq	%rsp, %rbp
	subq	$4, %rsp
	movl	$0, -4(%rbp)
	movl	$2, %r8d
	imull	$1, %r8d
	addl	-4(%rbp), %r8d
	movl	%r8d, -4(%rbp)
	subq	$4, %rsp
	movl	$0, -8(%rbp)
	movl	$3, %r8d
	imull	$1, %r8d
	addl	-8(%rbp), %r8d
	movl	%r8d, -8(%rbp)
	movl	-4(%rbp), %ebx
	cltq
	leaq	0(, %rbx, 4), %rdx
	leaq	c0(%rip), %rbx
	movl	(%rdx, %rbx), %r8d
	movl	-8(%rbp), %ebx
	cltq
	leaq	0(, %rbx, 4), %rdx
	leaq	g0(%rip), %rbx
	movl	(%rdx, %rbx), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -12(%rbp)
	movl	-12(%rbp), %r8d
	movl	scj82c9s0j(%rip), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -16(%rbp)
	movl	-16(%rbp), %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$5, %eax
	addq	$16, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret

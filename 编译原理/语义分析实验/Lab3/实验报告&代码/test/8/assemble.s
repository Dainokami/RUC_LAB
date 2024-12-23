.LC0:
	.string	"%d\n"
.LC1:
	.string	"%d\n"
.LC2:
	.string	"%d\n"
.LC3:
	.string	"%d\n"
	.section	.rodata
	.align	4
	.type	N, @object
	.size	N, 4
N:
	.long	10000
	.text
	.globl	long_array
	.type	main, @function
long_array:
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	movq	%rsp, %rbp
	subq	$40000, %rsp
	subq	$40000, %rsp
	subq	$40000, %rsp
	movl	$0, %edi
	subq	$4, %rsp
	movl	%edi, -120004(%rbp)
.L1:
	# enter while
	# enter stmt
.L2:
	# if <
	movl	-120004(%rbp), %r8d
	movl	$10000, %r9d
	cmpl	%r9d, %r8d
	jl	.L4
	jmp	.L3
.L3:
	# exit while
	addq	$0, %rsp
	jmp	.L5
.L4:
	subq	$4, %rsp
	movl	$0, -120008(%rbp)
	movl	-120004(%rbp), %r8d
	imull	$1, %r8d
	addl	-120008(%rbp), %r8d
	movl	%r8d, -120008(%rbp)
	movl	-120004(%rbp), %r8d
	movl	-120004(%rbp), %r9d
	imull	%r8d, %r9d
	subq	$4, %rsp
	movl	%r9d, -120012(%rbp)
	movl	-120012(%rbp), %eax
	movl	$10, %r9d
	cltd
	idivl %r9d
	subq	$4, %rsp
	movl	%edx, -120016(%rbp)
	movl	-120016(%rbp), %r9d
	movl	-120008(%rbp), %ebx
	cltq
	movl	%r9d, -40000(%rbp, %rbx, 4)
	movl	-120004(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120020(%rbp)
	movl	-120020(%rbp), %r9d
	movl	%r9d, -120004(%rbp)
	addq	$16, %rsp
	addq	$0, %rsp
	jmp	.L1
.L5:
	# while end
	movl	$0, %r9d
	movl	%r9d, -120004(%rbp)
.L6:
	# enter while
	# enter stmt
.L7:
	# if <
	movl	-120004(%rbp), %r8d
	movl	$10000, %r9d
	cmpl	%r9d, %r8d
	jl	.L9
	jmp	.L8
.L8:
	# exit while
	addq	$0, %rsp
	jmp	.L10
.L9:
	subq	$4, %rsp
	movl	$0, -120008(%rbp)
	movl	-120004(%rbp), %r8d
	imull	$1, %r8d
	addl	-120008(%rbp), %r8d
	movl	%r8d, -120008(%rbp)
	subq	$4, %rsp
	movl	$0, -120012(%rbp)
	movl	-120004(%rbp), %r8d
	imull	$1, %r8d
	addl	-120012(%rbp), %r8d
	movl	%r8d, -120012(%rbp)
	subq	$4, %rsp
	movl	$0, -120016(%rbp)
	movl	-120004(%rbp), %r8d
	imull	$1, %r8d
	addl	-120016(%rbp), %r8d
	movl	%r8d, -120016(%rbp)
	movl	-120012(%rbp), %ebx
	cltq
	movl	-40000(%rbp, %rbx, 4), %r8d
	movl	-120016(%rbp), %ebx
	cltq
	movl	-40000(%rbp, %rbx, 4), %r9d
	imull	%r8d, %r9d
	subq	$4, %rsp
	movl	%r9d, -120020(%rbp)
	movl	-120020(%rbp), %eax
	movl	$10, %r9d
	cltd
	idivl %r9d
	subq	$4, %rsp
	movl	%edx, -120024(%rbp)
	movl	-120024(%rbp), %r9d
	movl	-120008(%rbp), %ebx
	cltq
	movl	%r9d, -80000(%rbp, %rbx, 4)
	movl	-120004(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120028(%rbp)
	movl	-120028(%rbp), %r9d
	movl	%r9d, -120004(%rbp)
	addq	$24, %rsp
	addq	$0, %rsp
	jmp	.L6
.L10:
	# while end
	movl	$0, %r9d
	movl	%r9d, -120004(%rbp)
.L11:
	# enter while
	# enter stmt
.L12:
	# if <
	movl	-120004(%rbp), %r8d
	movl	$10000, %r9d
	cmpl	%r9d, %r8d
	jl	.L14
	jmp	.L13
.L13:
	# exit while
	addq	$0, %rsp
	jmp	.L15
.L14:
	subq	$4, %rsp
	movl	$0, -120008(%rbp)
	movl	-120004(%rbp), %r8d
	imull	$1, %r8d
	addl	-120008(%rbp), %r8d
	movl	%r8d, -120008(%rbp)
	subq	$4, %rsp
	movl	$0, -120012(%rbp)
	movl	-120004(%rbp), %r8d
	imull	$1, %r8d
	addl	-120012(%rbp), %r8d
	movl	%r8d, -120012(%rbp)
	subq	$4, %rsp
	movl	$0, -120016(%rbp)
	movl	-120004(%rbp), %r8d
	imull	$1, %r8d
	addl	-120016(%rbp), %r8d
	movl	%r8d, -120016(%rbp)
	movl	-120012(%rbp), %ebx
	cltq
	movl	-80000(%rbp, %rbx, 4), %r8d
	movl	-120016(%rbp), %ebx
	cltq
	movl	-80000(%rbp, %rbx, 4), %r9d
	imull	%r8d, %r9d
	subq	$4, %rsp
	movl	%r9d, -120020(%rbp)
	movl	-120020(%rbp), %eax
	movl	$100, %r9d
	cltd
	idivl %r9d
	subq	$4, %rsp
	movl	%edx, -120024(%rbp)
	subq	$4, %rsp
	movl	$0, -120028(%rbp)
	movl	-120004(%rbp), %r8d
	imull	$1, %r8d
	addl	-120028(%rbp), %r8d
	movl	%r8d, -120028(%rbp)
	movl	-120024(%rbp), %r8d
	movl	-120028(%rbp), %ebx
	cltq
	movl	-40000(%rbp, %rbx, 4), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120032(%rbp)
	movl	-120032(%rbp), %r9d
	movl	-120008(%rbp), %ebx
	cltq
	movl	%r9d, -120000(%rbp, %rbx, 4)
	movl	-120004(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120036(%rbp)
	movl	-120036(%rbp), %r9d
	movl	%r9d, -120004(%rbp)
	addq	$32, %rsp
	addq	$0, %rsp
	jmp	.L11
.L15:
	# while end
	movl	$0, %edi
	subq	$4, %rsp
	movl	%edi, -120008(%rbp)
	movl	$0, %r9d
	movl	%r9d, -120004(%rbp)
.L16:
	# enter while
	# enter stmt
.L17:
	# if <
	movl	-120004(%rbp), %r8d
	movl	$10000, %r9d
	cmpl	%r9d, %r8d
	jl	.L19
	jmp	.L18
.L18:
	# exit while
	addq	$0, %rsp
	jmp	.L46
.L19:
.L20:
	# if <
	movl	-120004(%rbp), %r8d
	movl	$10, %r9d
	cmpl	%r9d, %r8d
	jl	.L21
	jmp	.L22
.L21:
	# enter stmt
	subq	$4, %rsp
	movl	$0, -120012(%rbp)
	movl	-120004(%rbp), %r8d
	imull	$1, %r8d
	addl	-120012(%rbp), %r8d
	movl	%r8d, -120012(%rbp)
	movl	-120008(%rbp), %r8d
	movl	-120012(%rbp), %ebx
	cltq
	movl	-120000(%rbp, %rbx, 4), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120016(%rbp)
	movl	-120016(%rbp), %eax
	movl	$1333, %r9d
	cltd
	idivl %r9d
	subq	$4, %rsp
	movl	%edx, -120020(%rbp)
	movl	-120020(%rbp), %r9d
	movl	%r9d, -120008(%rbp)
	subq	$12, %rsp
	# align stack
	movl	-120008(%rbp), %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addq	$24, %rsp
	addq	$0, %rsp
	jmp	.L45
.L22:
	# enter stmt
.L23:
	# if <
	movl	-120004(%rbp), %r8d
	movl	$20, %r9d
	cmpl	%r9d, %r8d
	jl	.L24
	jmp	.L30
.L24:
	# enter stmt
	movl	$5000, %edi
	subq	$4, %rsp
	movl	%edi, -120012(%rbp)
.L25:
	# enter while
	# enter stmt
.L26:
	# if <
	movl	-120012(%rbp), %r8d
	movl	$10000, %r9d
	cmpl	%r9d, %r8d
	jl	.L28
	jmp	.L27
.L27:
	# exit while
	addq	$0, %rsp
	jmp	.L29
.L28:
	subq	$4, %rsp
	movl	$0, -120016(%rbp)
	movl	-120004(%rbp), %r8d
	imull	$1, %r8d
	addl	-120016(%rbp), %r8d
	movl	%r8d, -120016(%rbp)
	movl	-120008(%rbp), %r8d
	movl	-120016(%rbp), %ebx
	cltq
	movl	-120000(%rbp, %rbx, 4), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120020(%rbp)
	subq	$4, %rsp
	movl	$0, -120024(%rbp)
	movl	-120012(%rbp), %r8d
	imull	$1, %r8d
	addl	-120024(%rbp), %r8d
	movl	%r8d, -120024(%rbp)
	movl	-120020(%rbp), %r8d
	movl	-120024(%rbp), %ebx
	cltq
	movl	-40000(%rbp, %rbx, 4), %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120028(%rbp)
	movl	-120028(%rbp), %r9d
	movl	%r9d, -120008(%rbp)
	movl	-120012(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120032(%rbp)
	movl	-120032(%rbp), %r9d
	movl	%r9d, -120012(%rbp)
	addq	$20, %rsp
	addq	$0, %rsp
	jmp	.L25
.L29:
	# while end
	subq	$4, %rsp
	# align stack
	movl	-120008(%rbp), %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addq	$8, %rsp
	addq	$0, %rsp
	jmp	.L44
.L30:
	# enter stmt
.L31:
	# if <
	movl	-120004(%rbp), %r8d
	movl	$30, %r9d
	cmpl	%r9d, %r8d
	jl	.L32
	jmp	.L42
.L32:
	# enter stmt
	movl	$5000, %edi
	subq	$4, %rsp
	movl	%edi, -120012(%rbp)
.L33:
	# enter while
	# enter stmt
.L34:
	# if <
	movl	-120012(%rbp), %r8d
	movl	$10000, %r9d
	cmpl	%r9d, %r8d
	jl	.L36
	jmp	.L35
.L35:
	# exit while
	addq	$0, %rsp
	jmp	.L41
.L36:
.L37:
	# if >
	movl	-120012(%rbp), %r8d
	movl	$2233, %r9d
	cmpl	%r9d, %r8d
	jg	.L38
	jmp	.L39
.L38:
	# enter stmt
	subq	$4, %rsp
	movl	$0, -120016(%rbp)
	movl	-120004(%rbp), %r8d
	imull	$1, %r8d
	addl	-120016(%rbp), %r8d
	movl	%r8d, -120016(%rbp)
	movl	-120008(%rbp), %r8d
	movl	-120016(%rbp), %ebx
	cltq
	movl	-80000(%rbp, %rbx, 4), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120020(%rbp)
	subq	$4, %rsp
	movl	$0, -120024(%rbp)
	movl	-120012(%rbp), %r8d
	imull	$1, %r8d
	addl	-120024(%rbp), %r8d
	movl	%r8d, -120024(%rbp)
	movl	-120020(%rbp), %r8d
	movl	-120024(%rbp), %ebx
	cltq
	movl	-40000(%rbp, %rbx, 4), %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120028(%rbp)
	movl	-120028(%rbp), %r9d
	movl	%r9d, -120008(%rbp)
	movl	-120012(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120032(%rbp)
	movl	-120032(%rbp), %r9d
	movl	%r9d, -120012(%rbp)
	addq	$20, %rsp
	addq	$0, %rsp
	jmp	.L40
.L39:
	# enter stmt
	subq	$4, %rsp
	movl	$0, -120016(%rbp)
	movl	-120004(%rbp), %r8d
	imull	$1, %r8d
	addl	-120016(%rbp), %r8d
	movl	%r8d, -120016(%rbp)
	movl	-120008(%rbp), %r8d
	movl	-120016(%rbp), %ebx
	cltq
	movl	-40000(%rbp, %rbx, 4), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120020(%rbp)
	subq	$4, %rsp
	movl	$0, -120024(%rbp)
	movl	-120012(%rbp), %r8d
	imull	$1, %r8d
	addl	-120024(%rbp), %r8d
	movl	%r8d, -120024(%rbp)
	movl	-120020(%rbp), %r8d
	movl	-120024(%rbp), %ebx
	cltq
	movl	-120000(%rbp, %rbx, 4), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120028(%rbp)
	movl	-120028(%rbp), %eax
	movl	$13333, %r9d
	cltd
	idivl %r9d
	subq	$4, %rsp
	movl	%edx, -120032(%rbp)
	movl	-120032(%rbp), %r9d
	movl	%r9d, -120008(%rbp)
	movl	-120012(%rbp), %r8d
	movl	$2, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120036(%rbp)
	movl	-120036(%rbp), %r9d
	movl	%r9d, -120012(%rbp)
	addq	$24, %rsp
	addq	$0, %rsp
.L40:
	addq	$0, %rsp
	addq	$0, %rsp
	jmp	.L33
.L41:
	# while end
	subq	$4, %rsp
	# align stack
	movl	-120008(%rbp), %esi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	addq	$8, %rsp
	addq	$0, %rsp
	jmp	.L43
.L42:
	# enter stmt
	subq	$4, %rsp
	movl	$0, -120012(%rbp)
	movl	-120004(%rbp), %r8d
	imull	$1, %r8d
	addl	-120012(%rbp), %r8d
	movl	%r8d, -120012(%rbp)
	movl	-120012(%rbp), %ebx
	cltq
	movl	-120000(%rbp, %rbx, 4), %r8d
	movl	44(%rbp), %r9d
	imull	%r8d, %r9d
	subq	$4, %rsp
	movl	%r9d, -120016(%rbp)
	movl	-120008(%rbp), %r8d
	movl	-120016(%rbp), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120020(%rbp)
	movl	-120020(%rbp), %eax
	movl	$99988, %r9d
	cltd
	idivl %r9d
	subq	$4, %rsp
	movl	%edx, -120024(%rbp)
	movl	-120024(%rbp), %r9d
	movl	%r9d, -120008(%rbp)
	addq	$16, %rsp
	addq	$0, %rsp
.L43:
	addq	$0, %rsp
.L44:
	addq	$0, %rsp
.L45:
	movl	-120004(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -120012(%rbp)
	movl	-120012(%rbp), %r9d
	movl	%r9d, -120004(%rbp)
	addq	$4, %rsp
	addq	$0, %rsp
	jmp	.L16
.L46:
	# while end
	movl	-120008(%rbp), %eax
	addq	$120008, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	movq	%rsp, %rbp
	movl	$9, %r8d
	subq	$4, %rsp
	movl	%r8d, -4(%rbp)
	subq	$12, %rsp
	# align stack
	call	long_array
	subq	$4, %rsp
	movl	%eax, -20(%rbp)
	subq	$12, %rsp
	# align stack
	movl	-20(%rbp), %esi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	addq	$32, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret

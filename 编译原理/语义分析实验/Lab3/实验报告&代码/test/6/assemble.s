.LC0:
	.string	"%d\n"
.LC1:
	.string	"%d\n"
	.globl	MAX
	.type	main, @function
MAX:
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	movq	%rsp, %rbp
.L1:
	# if ==
	movl	40(%rbp), %r8d
	movl	44(%rbp), %r9d
	cmpl	%r9d, %r8d
	je	.L2
	jmp	.L3
.L2:
	# enter stmt
	movl	40(%rbp), %eax
	addq	$0, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
	addq	$0, %rsp
	jmp	.L8
.L3:
	# enter stmt
.L4:
	# if >
	movl	40(%rbp), %r8d
	movl	44(%rbp), %r9d
	cmpl	%r9d, %r8d
	jg	.L5
	jmp	.L6
.L5:
	# enter stmt
	movl	40(%rbp), %eax
	addq	$0, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
	addq	$0, %rsp
	jmp	.L7
.L6:
	# enter stmt
	movl	44(%rbp), %eax
	addq	$0, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
	addq	$0, %rsp
.L7:
	addq	$0, %rsp
.L8:
	.globl	max_sum_nonadjacent
	.type	main, @function
max_sum_nonadjacent:
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	movq	%rsp, %rbp
	subq	$64, %rsp
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
	movl	$0, %edi
	movl	%edi, -36(%rbp)
	movl	$0, %edi
	movl	%edi, -32(%rbp)
	movl	$0, %edi
	movl	%edi, -28(%rbp)
	movl	$0, %edi
	movl	%edi, -24(%rbp)
	movl	$0, %edi
	movl	%edi, -20(%rbp)
	movl	$0, %edi
	movl	%edi, -16(%rbp)
	movl	$0, %edi
	movl	%edi, -12(%rbp)
	movl	$0, %edi
	movl	%edi, -8(%rbp)
	movl	$0, %edi
	movl	%edi, -4(%rbp)
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
	movq	36(%rbp), %r10
	movl	(%r10, %rbx, 4), %r9d
	movl	-68(%rbp), %ebx
	cltq
	movl	%r9d, -64(%rbp, %rbx, 4)
	subq	$4, %rsp
	movl	$0, -76(%rbp)
	movl	$1, %r8d
	imull	$1, %r8d
	addl	-76(%rbp), %r8d
	movl	%r8d, -76(%rbp)
	subq	$4, %rsp
	movl	$0, -80(%rbp)
	movl	$0, %r8d
	imull	$1, %r8d
	addl	-80(%rbp), %r8d
	movl	%r8d, -80(%rbp)
	subq	$4, %rsp
	movl	$0, -84(%rbp)
	movl	$1, %r8d
	imull	$1, %r8d
	addl	-84(%rbp), %r8d
	movl	%r8d, -84(%rbp)
	subq	$12, %rsp
	# align stack
	movl	-84(%rbp), %ebx
	cltq
	movq	36(%rbp), %r10
	movl	(%r10, %rbx, 4), %r8d
	subq	$4, %rsp
	movl	%r8d, -100(%rbp)
	movl	-80(%rbp), %ebx
	cltq
	movq	36(%rbp), %r10
	movl	(%r10, %rbx, 4), %r8d
	subq	$4, %rsp
	movl	%r8d, -104(%rbp)
	subq	$8, %rsp
	# align stack
	call	MAX
	subq	$4, %rsp
	movl	%eax, -116(%rbp)
	movl	-116(%rbp), %r9d
	movl	-76(%rbp), %ebx
	cltq
	movl	%r9d, -64(%rbp, %rbx, 4)
	movl	$2, %edi
	subq	$4, %rsp
	movl	%edi, -120(%rbp)
.L9:
	# enter while
	# enter stmt
.L10:
	# if <
	movl	-120(%rbp), %r8d
	movl	44(%rbp), %r9d
	cmpl	%r9d, %r8d
	jl	.L12
	jmp	.L11
.L11:
	# exit while
	addq	$0, %rsp
	jmp	.L13
.L12:
	subq	$4, %rsp
	movl	$0, -124(%rbp)
	movl	-120(%rbp), %r8d
	imull	$1, %r8d
	addl	-124(%rbp), %r8d
	movl	%r8d, -124(%rbp)
	movl	-120(%rbp), %r8d
	movl	$2, %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -128(%rbp)
	subq	$4, %rsp
	movl	$0, -132(%rbp)
	movl	-128(%rbp), %r8d
	imull	$1, %r8d
	addl	-132(%rbp), %r8d
	movl	%r8d, -132(%rbp)
	subq	$4, %rsp
	movl	$0, -136(%rbp)
	movl	-120(%rbp), %r8d
	imull	$1, %r8d
	addl	-136(%rbp), %r8d
	movl	%r8d, -136(%rbp)
	movl	-132(%rbp), %ebx
	cltq
	movl	-64(%rbp, %rbx, 4), %r8d
	movl	-136(%rbp), %ebx
	cltq
	movq	36(%rbp), %r10
	movl	(%r10, %rbx, 4), %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -140(%rbp)
	movl	-120(%rbp), %r8d
	movl	$1, %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -144(%rbp)
	subq	$4, %rsp
	movl	$0, -148(%rbp)
	movl	-144(%rbp), %r8d
	imull	$1, %r8d
	addl	-148(%rbp), %r8d
	movl	%r8d, -148(%rbp)
	subq	$12, %rsp
	# align stack
	movl	-148(%rbp), %ebx
	cltq
	movl	-64(%rbp, %rbx, 4), %r8d
	subq	$4, %rsp
	movl	%r8d, -164(%rbp)
	movl	-140(%rbp), %r8d
	subq	$4, %rsp
	movl	%r8d, -168(%rbp)
	subq	$8, %rsp
	# align stack
	call	MAX
	subq	$4, %rsp
	movl	%eax, -180(%rbp)
	movl	-180(%rbp), %r9d
	movl	-124(%rbp), %ebx
	cltq
	movl	%r9d, -64(%rbp, %rbx, 4)
	movl	-120(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -184(%rbp)
	movl	-184(%rbp), %r9d
	movl	%r9d, -120(%rbp)
	addq	$64, %rsp
	addq	$0, %rsp
	jmp	.L9
.L13:
	# while end
	movl	44(%rbp), %r8d
	movl	$1, %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -124(%rbp)
	subq	$4, %rsp
	movl	$0, -128(%rbp)
	movl	-124(%rbp), %r8d
	imull	$1, %r8d
	addl	-128(%rbp), %r8d
	movl	%r8d, -128(%rbp)
	movl	-128(%rbp), %ebx
	cltq
	movl	-64(%rbp, %rbx, 4), %eax
	addq	$128, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret
	.globl	longest_common_subseq
	.type	main, @function
longest_common_subseq:
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	movq	%rsp, %rbp
	subq	$1024, %rsp
	movl	$0, %edi
	movl	%edi, -1024(%rbp)
	movl	$0, %edi
	movl	%edi, -1020(%rbp)
	movl	$0, %edi
	movl	%edi, -1016(%rbp)
	movl	$0, %edi
	movl	%edi, -1012(%rbp)
	movl	$0, %edi
	movl	%edi, -1008(%rbp)
	movl	$0, %edi
	movl	%edi, -1004(%rbp)
	movl	$0, %edi
	movl	%edi, -1000(%rbp)
	movl	$0, %edi
	movl	%edi, -996(%rbp)
	movl	$0, %edi
	movl	%edi, -992(%rbp)
	movl	$0, %edi
	movl	%edi, -988(%rbp)
	movl	$0, %edi
	movl	%edi, -984(%rbp)
	movl	$0, %edi
	movl	%edi, -980(%rbp)
	movl	$0, %edi
	movl	%edi, -976(%rbp)
	movl	$0, %edi
	movl	%edi, -972(%rbp)
	movl	$0, %edi
	movl	%edi, -968(%rbp)
	movl	$0, %edi
	movl	%edi, -964(%rbp)
	movl	$0, %edi
	movl	%edi, -960(%rbp)
	movl	$0, %edi
	movl	%edi, -956(%rbp)
	movl	$0, %edi
	movl	%edi, -952(%rbp)
	movl	$0, %edi
	movl	%edi, -948(%rbp)
	movl	$0, %edi
	movl	%edi, -944(%rbp)
	movl	$0, %edi
	movl	%edi, -940(%rbp)
	movl	$0, %edi
	movl	%edi, -936(%rbp)
	movl	$0, %edi
	movl	%edi, -932(%rbp)
	movl	$0, %edi
	movl	%edi, -928(%rbp)
	movl	$0, %edi
	movl	%edi, -924(%rbp)
	movl	$0, %edi
	movl	%edi, -920(%rbp)
	movl	$0, %edi
	movl	%edi, -916(%rbp)
	movl	$0, %edi
	movl	%edi, -912(%rbp)
	movl	$0, %edi
	movl	%edi, -908(%rbp)
	movl	$0, %edi
	movl	%edi, -904(%rbp)
	movl	$0, %edi
	movl	%edi, -900(%rbp)
	movl	$0, %edi
	movl	%edi, -896(%rbp)
	movl	$0, %edi
	movl	%edi, -892(%rbp)
	movl	$0, %edi
	movl	%edi, -888(%rbp)
	movl	$0, %edi
	movl	%edi, -884(%rbp)
	movl	$0, %edi
	movl	%edi, -880(%rbp)
	movl	$0, %edi
	movl	%edi, -876(%rbp)
	movl	$0, %edi
	movl	%edi, -872(%rbp)
	movl	$0, %edi
	movl	%edi, -868(%rbp)
	movl	$0, %edi
	movl	%edi, -864(%rbp)
	movl	$0, %edi
	movl	%edi, -860(%rbp)
	movl	$0, %edi
	movl	%edi, -856(%rbp)
	movl	$0, %edi
	movl	%edi, -852(%rbp)
	movl	$0, %edi
	movl	%edi, -848(%rbp)
	movl	$0, %edi
	movl	%edi, -844(%rbp)
	movl	$0, %edi
	movl	%edi, -840(%rbp)
	movl	$0, %edi
	movl	%edi, -836(%rbp)
	movl	$0, %edi
	movl	%edi, -832(%rbp)
	movl	$0, %edi
	movl	%edi, -828(%rbp)
	movl	$0, %edi
	movl	%edi, -824(%rbp)
	movl	$0, %edi
	movl	%edi, -820(%rbp)
	movl	$0, %edi
	movl	%edi, -816(%rbp)
	movl	$0, %edi
	movl	%edi, -812(%rbp)
	movl	$0, %edi
	movl	%edi, -808(%rbp)
	movl	$0, %edi
	movl	%edi, -804(%rbp)
	movl	$0, %edi
	movl	%edi, -800(%rbp)
	movl	$0, %edi
	movl	%edi, -796(%rbp)
	movl	$0, %edi
	movl	%edi, -792(%rbp)
	movl	$0, %edi
	movl	%edi, -788(%rbp)
	movl	$0, %edi
	movl	%edi, -784(%rbp)
	movl	$0, %edi
	movl	%edi, -780(%rbp)
	movl	$0, %edi
	movl	%edi, -776(%rbp)
	movl	$0, %edi
	movl	%edi, -772(%rbp)
	movl	$0, %edi
	movl	%edi, -768(%rbp)
	movl	$0, %edi
	movl	%edi, -764(%rbp)
	movl	$0, %edi
	movl	%edi, -760(%rbp)
	movl	$0, %edi
	movl	%edi, -756(%rbp)
	movl	$0, %edi
	movl	%edi, -752(%rbp)
	movl	$0, %edi
	movl	%edi, -748(%rbp)
	movl	$0, %edi
	movl	%edi, -744(%rbp)
	movl	$0, %edi
	movl	%edi, -740(%rbp)
	movl	$0, %edi
	movl	%edi, -736(%rbp)
	movl	$0, %edi
	movl	%edi, -732(%rbp)
	movl	$0, %edi
	movl	%edi, -728(%rbp)
	movl	$0, %edi
	movl	%edi, -724(%rbp)
	movl	$0, %edi
	movl	%edi, -720(%rbp)
	movl	$0, %edi
	movl	%edi, -716(%rbp)
	movl	$0, %edi
	movl	%edi, -712(%rbp)
	movl	$0, %edi
	movl	%edi, -708(%rbp)
	movl	$0, %edi
	movl	%edi, -704(%rbp)
	movl	$0, %edi
	movl	%edi, -700(%rbp)
	movl	$0, %edi
	movl	%edi, -696(%rbp)
	movl	$0, %edi
	movl	%edi, -692(%rbp)
	movl	$0, %edi
	movl	%edi, -688(%rbp)
	movl	$0, %edi
	movl	%edi, -684(%rbp)
	movl	$0, %edi
	movl	%edi, -680(%rbp)
	movl	$0, %edi
	movl	%edi, -676(%rbp)
	movl	$0, %edi
	movl	%edi, -672(%rbp)
	movl	$0, %edi
	movl	%edi, -668(%rbp)
	movl	$0, %edi
	movl	%edi, -664(%rbp)
	movl	$0, %edi
	movl	%edi, -660(%rbp)
	movl	$0, %edi
	movl	%edi, -656(%rbp)
	movl	$0, %edi
	movl	%edi, -652(%rbp)
	movl	$0, %edi
	movl	%edi, -648(%rbp)
	movl	$0, %edi
	movl	%edi, -644(%rbp)
	movl	$0, %edi
	movl	%edi, -640(%rbp)
	movl	$0, %edi
	movl	%edi, -636(%rbp)
	movl	$0, %edi
	movl	%edi, -632(%rbp)
	movl	$0, %edi
	movl	%edi, -628(%rbp)
	movl	$0, %edi
	movl	%edi, -624(%rbp)
	movl	$0, %edi
	movl	%edi, -620(%rbp)
	movl	$0, %edi
	movl	%edi, -616(%rbp)
	movl	$0, %edi
	movl	%edi, -612(%rbp)
	movl	$0, %edi
	movl	%edi, -608(%rbp)
	movl	$0, %edi
	movl	%edi, -604(%rbp)
	movl	$0, %edi
	movl	%edi, -600(%rbp)
	movl	$0, %edi
	movl	%edi, -596(%rbp)
	movl	$0, %edi
	movl	%edi, -592(%rbp)
	movl	$0, %edi
	movl	%edi, -588(%rbp)
	movl	$0, %edi
	movl	%edi, -584(%rbp)
	movl	$0, %edi
	movl	%edi, -580(%rbp)
	movl	$0, %edi
	movl	%edi, -576(%rbp)
	movl	$0, %edi
	movl	%edi, -572(%rbp)
	movl	$0, %edi
	movl	%edi, -568(%rbp)
	movl	$0, %edi
	movl	%edi, -564(%rbp)
	movl	$0, %edi
	movl	%edi, -560(%rbp)
	movl	$0, %edi
	movl	%edi, -556(%rbp)
	movl	$0, %edi
	movl	%edi, -552(%rbp)
	movl	$0, %edi
	movl	%edi, -548(%rbp)
	movl	$0, %edi
	movl	%edi, -544(%rbp)
	movl	$0, %edi
	movl	%edi, -540(%rbp)
	movl	$0, %edi
	movl	%edi, -536(%rbp)
	movl	$0, %edi
	movl	%edi, -532(%rbp)
	movl	$0, %edi
	movl	%edi, -528(%rbp)
	movl	$0, %edi
	movl	%edi, -524(%rbp)
	movl	$0, %edi
	movl	%edi, -520(%rbp)
	movl	$0, %edi
	movl	%edi, -516(%rbp)
	movl	$0, %edi
	movl	%edi, -512(%rbp)
	movl	$0, %edi
	movl	%edi, -508(%rbp)
	movl	$0, %edi
	movl	%edi, -504(%rbp)
	movl	$0, %edi
	movl	%edi, -500(%rbp)
	movl	$0, %edi
	movl	%edi, -496(%rbp)
	movl	$0, %edi
	movl	%edi, -492(%rbp)
	movl	$0, %edi
	movl	%edi, -488(%rbp)
	movl	$0, %edi
	movl	%edi, -484(%rbp)
	movl	$0, %edi
	movl	%edi, -480(%rbp)
	movl	$0, %edi
	movl	%edi, -476(%rbp)
	movl	$0, %edi
	movl	%edi, -472(%rbp)
	movl	$0, %edi
	movl	%edi, -468(%rbp)
	movl	$0, %edi
	movl	%edi, -464(%rbp)
	movl	$0, %edi
	movl	%edi, -460(%rbp)
	movl	$0, %edi
	movl	%edi, -456(%rbp)
	movl	$0, %edi
	movl	%edi, -452(%rbp)
	movl	$0, %edi
	movl	%edi, -448(%rbp)
	movl	$0, %edi
	movl	%edi, -444(%rbp)
	movl	$0, %edi
	movl	%edi, -440(%rbp)
	movl	$0, %edi
	movl	%edi, -436(%rbp)
	movl	$0, %edi
	movl	%edi, -432(%rbp)
	movl	$0, %edi
	movl	%edi, -428(%rbp)
	movl	$0, %edi
	movl	%edi, -424(%rbp)
	movl	$0, %edi
	movl	%edi, -420(%rbp)
	movl	$0, %edi
	movl	%edi, -416(%rbp)
	movl	$0, %edi
	movl	%edi, -412(%rbp)
	movl	$0, %edi
	movl	%edi, -408(%rbp)
	movl	$0, %edi
	movl	%edi, -404(%rbp)
	movl	$0, %edi
	movl	%edi, -400(%rbp)
	movl	$0, %edi
	movl	%edi, -396(%rbp)
	movl	$0, %edi
	movl	%edi, -392(%rbp)
	movl	$0, %edi
	movl	%edi, -388(%rbp)
	movl	$0, %edi
	movl	%edi, -384(%rbp)
	movl	$0, %edi
	movl	%edi, -380(%rbp)
	movl	$0, %edi
	movl	%edi, -376(%rbp)
	movl	$0, %edi
	movl	%edi, -372(%rbp)
	movl	$0, %edi
	movl	%edi, -368(%rbp)
	movl	$0, %edi
	movl	%edi, -364(%rbp)
	movl	$0, %edi
	movl	%edi, -360(%rbp)
	movl	$0, %edi
	movl	%edi, -356(%rbp)
	movl	$0, %edi
	movl	%edi, -352(%rbp)
	movl	$0, %edi
	movl	%edi, -348(%rbp)
	movl	$0, %edi
	movl	%edi, -344(%rbp)
	movl	$0, %edi
	movl	%edi, -340(%rbp)
	movl	$0, %edi
	movl	%edi, -336(%rbp)
	movl	$0, %edi
	movl	%edi, -332(%rbp)
	movl	$0, %edi
	movl	%edi, -328(%rbp)
	movl	$0, %edi
	movl	%edi, -324(%rbp)
	movl	$0, %edi
	movl	%edi, -320(%rbp)
	movl	$0, %edi
	movl	%edi, -316(%rbp)
	movl	$0, %edi
	movl	%edi, -312(%rbp)
	movl	$0, %edi
	movl	%edi, -308(%rbp)
	movl	$0, %edi
	movl	%edi, -304(%rbp)
	movl	$0, %edi
	movl	%edi, -300(%rbp)
	movl	$0, %edi
	movl	%edi, -296(%rbp)
	movl	$0, %edi
	movl	%edi, -292(%rbp)
	movl	$0, %edi
	movl	%edi, -288(%rbp)
	movl	$0, %edi
	movl	%edi, -284(%rbp)
	movl	$0, %edi
	movl	%edi, -280(%rbp)
	movl	$0, %edi
	movl	%edi, -276(%rbp)
	movl	$0, %edi
	movl	%edi, -272(%rbp)
	movl	$0, %edi
	movl	%edi, -268(%rbp)
	movl	$0, %edi
	movl	%edi, -264(%rbp)
	movl	$0, %edi
	movl	%edi, -260(%rbp)
	movl	$0, %edi
	movl	%edi, -256(%rbp)
	movl	$0, %edi
	movl	%edi, -252(%rbp)
	movl	$0, %edi
	movl	%edi, -248(%rbp)
	movl	$0, %edi
	movl	%edi, -244(%rbp)
	movl	$0, %edi
	movl	%edi, -240(%rbp)
	movl	$0, %edi
	movl	%edi, -236(%rbp)
	movl	$0, %edi
	movl	%edi, -232(%rbp)
	movl	$0, %edi
	movl	%edi, -228(%rbp)
	movl	$0, %edi
	movl	%edi, -224(%rbp)
	movl	$0, %edi
	movl	%edi, -220(%rbp)
	movl	$0, %edi
	movl	%edi, -216(%rbp)
	movl	$0, %edi
	movl	%edi, -212(%rbp)
	movl	$0, %edi
	movl	%edi, -208(%rbp)
	movl	$0, %edi
	movl	%edi, -204(%rbp)
	movl	$0, %edi
	movl	%edi, -200(%rbp)
	movl	$0, %edi
	movl	%edi, -196(%rbp)
	movl	$0, %edi
	movl	%edi, -192(%rbp)
	movl	$0, %edi
	movl	%edi, -188(%rbp)
	movl	$0, %edi
	movl	%edi, -184(%rbp)
	movl	$0, %edi
	movl	%edi, -180(%rbp)
	movl	$0, %edi
	movl	%edi, -176(%rbp)
	movl	$0, %edi
	movl	%edi, -172(%rbp)
	movl	$0, %edi
	movl	%edi, -168(%rbp)
	movl	$0, %edi
	movl	%edi, -164(%rbp)
	movl	$0, %edi
	movl	%edi, -160(%rbp)
	movl	$0, %edi
	movl	%edi, -156(%rbp)
	movl	$0, %edi
	movl	%edi, -152(%rbp)
	movl	$0, %edi
	movl	%edi, -148(%rbp)
	movl	$0, %edi
	movl	%edi, -144(%rbp)
	movl	$0, %edi
	movl	%edi, -140(%rbp)
	movl	$0, %edi
	movl	%edi, -136(%rbp)
	movl	$0, %edi
	movl	%edi, -132(%rbp)
	movl	$0, %edi
	movl	%edi, -128(%rbp)
	movl	$0, %edi
	movl	%edi, -124(%rbp)
	movl	$0, %edi
	movl	%edi, -120(%rbp)
	movl	$0, %edi
	movl	%edi, -116(%rbp)
	movl	$0, %edi
	movl	%edi, -112(%rbp)
	movl	$0, %edi
	movl	%edi, -108(%rbp)
	movl	$0, %edi
	movl	%edi, -104(%rbp)
	movl	$0, %edi
	movl	%edi, -100(%rbp)
	movl	$0, %edi
	movl	%edi, -96(%rbp)
	movl	$0, %edi
	movl	%edi, -92(%rbp)
	movl	$0, %edi
	movl	%edi, -88(%rbp)
	movl	$0, %edi
	movl	%edi, -84(%rbp)
	movl	$0, %edi
	movl	%edi, -80(%rbp)
	movl	$0, %edi
	movl	%edi, -76(%rbp)
	movl	$0, %edi
	movl	%edi, -72(%rbp)
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
	movl	$0, %edi
	movl	%edi, -36(%rbp)
	movl	$0, %edi
	movl	%edi, -32(%rbp)
	movl	$0, %edi
	movl	%edi, -28(%rbp)
	movl	$0, %edi
	movl	%edi, -24(%rbp)
	movl	$0, %edi
	movl	%edi, -20(%rbp)
	movl	$0, %edi
	movl	%edi, -16(%rbp)
	movl	$0, %edi
	movl	%edi, -12(%rbp)
	movl	$0, %edi
	movl	%edi, -8(%rbp)
	movl	$0, %edi
	movl	%edi, -4(%rbp)
	subq	$4, %rsp
	subq	$4, %rsp
	movl	$1, %r9d
	movl	%r9d, -1028(%rbp)
.L14:
	# enter while
	# enter stmt
.L15:
	# if <=
	movl	-1028(%rbp), %r8d
	movl	48(%rbp), %r9d
	cmpl	%r9d, %r8d
	jle	.L17
	jmp	.L16
.L16:
	# exit while
	addq	$0, %rsp
	jmp	.L27
.L17:
	movl	$1, %r9d
	movl	%r9d, -1032(%rbp)
.L18:
	# enter while
	# enter stmt
.L19:
	# if <=
	movl	-1032(%rbp), %r8d
	movl	60(%rbp), %r9d
	cmpl	%r9d, %r8d
	jle	.L21
	jmp	.L20
.L20:
	# exit while
	addq	$0, %rsp
	jmp	.L26
.L21:
	movl	-1028(%rbp), %r8d
	movl	$1, %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -1036(%rbp)
	subq	$4, %rsp
	movl	$0, -1040(%rbp)
	movl	-1036(%rbp), %r8d
	imull	$1, %r8d
	addl	-1040(%rbp), %r8d
	movl	%r8d, -1040(%rbp)
	movl	-1032(%rbp), %r8d
	movl	$1, %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -1044(%rbp)
	subq	$4, %rsp
	movl	$0, -1048(%rbp)
	movl	-1044(%rbp), %r8d
	imull	$1, %r8d
	addl	-1048(%rbp), %r8d
	movl	%r8d, -1048(%rbp)
.L22:
	# if ==
	movl	-1040(%rbp), %ebx
	cltq
	movq	40(%rbp), %r10
	movl	(%r10, %rbx, 4), %r8d
	movl	-1048(%rbp), %ebx
	cltq
	movq	52(%rbp), %r10
	movl	(%r10, %rbx, 4), %r9d
	cmpl	%r9d, %r8d
	je	.L23
	jmp	.L24
.L23:
	# enter stmt
	subq	$4, %rsp
	movl	$0, -1052(%rbp)
	movl	-1032(%rbp), %r8d
	imull	$1, %r8d
	addl	-1052(%rbp), %r8d
	movl	%r8d, -1052(%rbp)
	movl	-1028(%rbp), %r8d
	imull	$16, %r8d
	addl	-1052(%rbp), %r8d
	movl	%r8d, -1052(%rbp)
	movl	-1028(%rbp), %r8d
	movl	$1, %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -1056(%rbp)
	movl	-1032(%rbp), %r8d
	movl	$1, %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -1060(%rbp)
	subq	$4, %rsp
	movl	$0, -1064(%rbp)
	movl	-1060(%rbp), %r8d
	imull	$1, %r8d
	addl	-1064(%rbp), %r8d
	movl	%r8d, -1064(%rbp)
	movl	-1056(%rbp), %r8d
	imull	$16, %r8d
	addl	-1064(%rbp), %r8d
	movl	%r8d, -1064(%rbp)
	movl	-1064(%rbp), %ebx
	cltq
	movl	-1024(%rbp, %rbx, 4), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -1068(%rbp)
	movl	-1068(%rbp), %r9d
	movl	-1052(%rbp), %ebx
	cltq
	movl	%r9d, -1024(%rbp, %rbx, 4)
	addq	$20, %rsp
	addq	$0, %rsp
	jmp	.L25
.L24:
	# enter stmt
	subq	$4, %rsp
	movl	$0, -1052(%rbp)
	movl	-1032(%rbp), %r8d
	imull	$1, %r8d
	addl	-1052(%rbp), %r8d
	movl	%r8d, -1052(%rbp)
	movl	-1028(%rbp), %r8d
	imull	$16, %r8d
	addl	-1052(%rbp), %r8d
	movl	%r8d, -1052(%rbp)
	movl	-1028(%rbp), %r8d
	movl	$1, %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -1056(%rbp)
	subq	$4, %rsp
	movl	$0, -1060(%rbp)
	movl	-1032(%rbp), %r8d
	imull	$1, %r8d
	addl	-1060(%rbp), %r8d
	movl	%r8d, -1060(%rbp)
	movl	-1056(%rbp), %r8d
	imull	$16, %r8d
	addl	-1060(%rbp), %r8d
	movl	%r8d, -1060(%rbp)
	movl	-1032(%rbp), %r8d
	movl	$1, %r9d
	subl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -1064(%rbp)
	subq	$4, %rsp
	movl	$0, -1068(%rbp)
	movl	-1064(%rbp), %r8d
	imull	$1, %r8d
	addl	-1068(%rbp), %r8d
	movl	%r8d, -1068(%rbp)
	movl	-1028(%rbp), %r8d
	imull	$16, %r8d
	addl	-1068(%rbp), %r8d
	movl	%r8d, -1068(%rbp)
	subq	$4, %rsp
	# align stack
	movl	-1068(%rbp), %ebx
	cltq
	movl	-1024(%rbp, %rbx, 4), %r8d
	subq	$4, %rsp
	movl	%r8d, -1076(%rbp)
	movl	-1060(%rbp), %ebx
	cltq
	movl	-1024(%rbp, %rbx, 4), %r8d
	subq	$4, %rsp
	movl	%r8d, -1080(%rbp)
	subq	$8, %rsp
	# align stack
	call	MAX
	subq	$4, %rsp
	movl	%eax, -1092(%rbp)
	movl	-1092(%rbp), %r9d
	movl	-1052(%rbp), %ebx
	cltq
	movl	%r9d, -1024(%rbp, %rbx, 4)
	addq	$44, %rsp
	addq	$0, %rsp
.L25:
	movl	-1032(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -1052(%rbp)
	movl	-1052(%rbp), %r9d
	movl	%r9d, -1032(%rbp)
	addq	$20, %rsp
	addq	$0, %rsp
	jmp	.L18
.L26:
	# while end
	movl	-1028(%rbp), %r8d
	movl	$1, %r9d
	addl	%r9d, %r8d
	subq	$4, %rsp
	movl	%r8d, -1036(%rbp)
	movl	-1036(%rbp), %r9d
	movl	%r9d, -1028(%rbp)
	addq	$4, %rsp
	addq	$0, %rsp
	jmp	.L14
.L27:
	# while end
	subq	$4, %rsp
	movl	$0, -1036(%rbp)
	movl	60(%rbp), %r8d
	imull	$1, %r8d
	addl	-1036(%rbp), %r8d
	movl	%r8d, -1036(%rbp)
	movl	48(%rbp), %r8d
	imull	$16, %r8d
	addl	-1036(%rbp), %r8d
	movl	%r8d, -1036(%rbp)
	movl	-1036(%rbp), %ebx
	cltq
	movl	-1024(%rbp, %rbx, 4), %eax
	addq	$1036, %rsp
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
	subq	$60, %rsp
	movl	$8, %edi
	movl	%edi, -60(%rbp)
	movl	$7, %edi
	movl	%edi, -56(%rbp)
	movl	$4, %edi
	movl	%edi, -52(%rbp)
	movl	$1, %edi
	movl	%edi, -48(%rbp)
	movl	$2, %edi
	movl	%edi, -44(%rbp)
	movl	$7, %edi
	movl	%edi, -40(%rbp)
	movl	$0, %edi
	movl	%edi, -36(%rbp)
	movl	$1, %edi
	movl	%edi, -32(%rbp)
	movl	$9, %edi
	movl	%edi, -28(%rbp)
	movl	$3, %edi
	movl	%edi, -24(%rbp)
	movl	$4, %edi
	movl	%edi, -20(%rbp)
	movl	$8, %edi
	movl	%edi, -16(%rbp)
	movl	$3, %edi
	movl	%edi, -12(%rbp)
	movl	$7, %edi
	movl	%edi, -8(%rbp)
	movl	$0, %edi
	movl	%edi, -4(%rbp)
	subq	$52, %rsp
	movl	$3, %edi
	movl	%edi, -112(%rbp)
	movl	$9, %edi
	movl	%edi, -108(%rbp)
	movl	$7, %edi
	movl	%edi, -104(%rbp)
	movl	$1, %edi
	movl	%edi, -100(%rbp)
	movl	$4, %edi
	movl	%edi, -96(%rbp)
	movl	$2, %edi
	movl	%edi, -92(%rbp)
	movl	$4, %edi
	movl	%edi, -88(%rbp)
	movl	$3, %edi
	movl	%edi, -84(%rbp)
	movl	$6, %edi
	movl	%edi, -80(%rbp)
	movl	$8, %edi
	movl	%edi, -76(%rbp)
	movl	$0, %edi
	movl	%edi, -72(%rbp)
	movl	$1, %edi
	movl	%edi, -68(%rbp)
	movl	$5, %edi
	movl	%edi, -64(%rbp)
	movl	$15, %r8d
	subq	$4, %rsp
	movl	%r8d, -116(%rbp)
	leaq	-60(%rbp), %r8
	subq	$8, %rsp
	movq	%r8, -124(%rbp)
	subq	$4, %rsp
	# align stack
	call	max_sum_nonadjacent
	subq	$4, %rsp
	movl	%eax, -132(%rbp)
	subq	$12, %rsp
	# align stack
	movl	-132(%rbp), %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$13, %r8d
	subq	$4, %rsp
	movl	%r8d, -148(%rbp)
	leaq	-112(%rbp), %r8
	subq	$8, %rsp
	movq	%r8, -156(%rbp)
	movl	$15, %r8d
	subq	$4, %rsp
	movl	%r8d, -160(%rbp)
	leaq	-60(%rbp), %r8
	subq	$8, %rsp
	movq	%r8, -168(%rbp)
	subq	$8, %rsp
	# align stack
	call	longest_common_subseq
	subq	$4, %rsp
	movl	%eax, -180(%rbp)
	subq	$12, %rsp
	# align stack
	movl	-180(%rbp), %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	addq	$192, %rsp
	popq	%r9
	popq	%r8
	popq	%rbp
	ret

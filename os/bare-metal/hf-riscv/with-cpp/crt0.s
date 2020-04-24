	.text
	.align 2

	.global RESET_handler
	.global _entry
_entry:
	la	a3, _bss_start
	la	a2, _end
	la	gp, _gp
	la	sp, _stack
	la	tp, _end + 63
	and	tp, tp, -64

	# jump to main
	jal	ra, RESET_handler

	li	s10, 0xe0000000		# this will interrupt the simulation (assertion)
	sw	a0,0(s10)
L1:
	beq	zero, zero, L1


	.global   setjmp
setjmp:
	sw    s0, 0(a0)
	sw    s1, 4(a0)
	sw    s2, 8(a0)
	sw    s3, 12(a0)
	sw    s4, 16(a0)
	sw    s5, 20(a0)
	sw    s6, 24(a0)
	sw    s7, 28(a0)
	sw    s8, 32(a0)
	sw    s9, 36(a0)
#	sw    gp, 40(a0)
	sw    tp, 44(a0)
	sw    sp, 48(a0)
	sw    ra, 52(a0)
	ori  a0, zero, 0
	ret

	.global   longjmp
longjmp:
	lw    s0, 0(a0)
	lw    s1, 4(a0)
	lw    s2, 8(a0)
	lw    s3, 12(a0)
	lw    s4, 16(a0)
	lw    s5, 20(a0)
	lw    s6, 24(a0)
	lw    s7, 28(a0)
	lw    s8, 32(a0)
	lw    s9, 36(a0)
#	lw    gp, 40(a0)
	lw    tp, 44(a0)
	lw    sp, 48(a0)
	lw    ra, 52(a0)
	ori  a0, a1, 0
	ret

# system call interface: syscall(service, arg0, arg1, arg2)
	.global syscall
syscall:
	addi	a7, a0, 0
	ecall
	ret

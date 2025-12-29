.reset_vector:
	li sp, 0xFFFF0
	call .main
.main:
	addi sp,sp,-40000016
	sw   ra,40000012(sp)
	sw   s0,40000008(sp)
	addi s0,sp,40000016
	li   t1,0
	sw   t1,-40000012(s0)
	j    .L0
.L0:
	lw t1,-40000012(s0)
	slli t0,t1,2
	addi t6,s0,-40000008
	add  t0,t0,t6
	lw   t1,-40000012(s0)
	sw   t1,0(t0)
.L1:
	lw t1,-40000012(s0)
	addi t1,t1,1
	sw   t1,-40000012(s0)
	lw   t1,-40000012(s0)
	li   t6,10000000
	blt  t1,t6,.L0
.L2:
	lw ra,40000012(sp)
	lw s0,40000008(sp)
	addi sp,sp,40000016
	jr ra

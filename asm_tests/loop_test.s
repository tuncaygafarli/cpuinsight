addi x2,x0,10000
loop:
	addi x1,x1,1
	beq  x1,x2,end
	beq x0,x0,loop
end:
	add x0,x0,x0
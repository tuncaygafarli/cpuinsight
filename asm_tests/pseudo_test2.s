# Setup: x10 will be our "success counter"
# x1 = 0, x2 = 5, x3 = 10
li x10, 0
li x1, 0
li x2, 5
li x3, 10

test_1:
    beqz x1, pass_1     # Should jump because x1 is 0
    j fail
pass_1:
    addi x10, x10, 1

test_2:
    bnez x2, pass_2     # Should jump because x2 is not 0
    j fail
pass_2:
    addi x10, x10, 1

test_3:
    blez x1, pass_3     # Should jump (0 <= 0)
    j fail
pass_3:
    blez x2, fail        # Should NOT jump (5 is not <= 0)
    addi x10, x10, 1

test_4:
    bgez x1, pass_4     # Should jump (0 >= 0)
    j fail
pass_4:
    bgez x2, pass_4b    # Should jump (5 >= 0)
    j fail
pass_4b:
    addi x10, x10, 1

test_5:
    bltz x1, fail       # Should NOT jump (0 is not < 0)
    li x4, -1
    bltz x4, pass_5     # Should jump (-1 < 0)
    j fail
pass_5:
    addi x10, x10, 1

test_6:
    bgtz x2, pass_6     # Should jump (5 > 0)
    j fail
pass_6:
    addi x10, x10, 1

test_7:
    ble x2, x3, pass_7  # Should jump (5 <= 10)
    j fail
pass_7:
    addi x10, x10, 1

test_8:
    bgt x3, x2, pass_8  # Should jump (10 > 5)
    j fail
pass_8:
    addi x10, x10, 1

test_9:
    # Testing Unsigned: 0xFFFFFFFF is -1 signed, but huge unsigned
    li x5, -1 
    bleu x2, x5, pass_9 # Should jump (5 <= 4294967295)
    j fail
pass_9:
    addi x10, x10, 1

test_10:
    bgtu x5, x2, pass_10 # Should jump (4294967295 > 5)
    j fail
pass_10:
    addi x10, x10, 1

# If x10 == 10, then every single branch type worked
done:
    j end

fail:
    li x10, 0xDEAD      
    j done
end:
	nop
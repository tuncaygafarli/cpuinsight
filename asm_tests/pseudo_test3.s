#----------------------------------------------------------
# Test Case: Nested Function Linkage
# Objective: Verify 'call' and 'ret' pseudo-instruction 
#            correctness and register stability.
#----------------------------------------------------------

li x10, 0          # Initialize success counter to 0

main:
    call function_a    # First level call: Sets x1 (ra)
    # Return point 1
    addi x10, x10, 1   # Final increment: x10 should reach 4
    j test_complete    # Exit test

function_a:
    # Save the original return address (x1) to x2 before nesting
    # This prevents the subsequent 'call' from losing the main return path
    mv x2, x1          
    
    addi x10, x10, 1   # Counter: 1
    call function_b    # Second level call: Overwrites x1 (ra)
    
    # Return point 2
    addi x10, x10, 1   # Counter: 3
    mv x1, x2          # Restore original return address to x1
    ret                # Return to main

function_b:
    addi x10, x10, 1   # Counter: 2
    ret                # Return to function_a

test_complete:
    nop             # Or use a loop: j test_complete
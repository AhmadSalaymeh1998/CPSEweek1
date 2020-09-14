.global toggle
.cpu cortex-m0 

.text
.align 2
toggle:
	push {r0,lr}
	sub r0, r0, #65
	BGE makelower
	pop {r0}
	b done	

makelower:
	sub r0, r0, #25
	BGT case1
	pop {r0}
	add r0, #32
	b done

case1:
	sub r0, r0, #6
	BGT makeupper
	pop {r0}
	b done
	
makeupper:
	sub r0, r0, #26
	BGT case2
	pop {r0}
	sub r0, r0, #32
	b done

case2:
	pop {r0}

done:
	pop { pc }
	









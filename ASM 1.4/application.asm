.global application
.cpu cortex-m0 

.text
.align 2
application:
	push { lr }
	ldr r0, =helloworld
	bl print_asciz
	pop { pc }

.data
helloworld:
	.asciz "Hello world, the ANSWER is 42! @[]`{}~\n"

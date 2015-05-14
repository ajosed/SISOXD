	.extern _main

entry start

start: 
	call _main

Loop: 
	jmp Loop

	.data
operand:	.word	120,54
	.text
	addi	$1,$0,0	
	add	$10,$0,$0
	add	$11,$0,$0
	lw 	$2,0($1)
	lw 	$3,4($1)
	add	$5,$0,$0
	add	$4,$0,$0
	add	$10,$0,$0
	add	$11,$0,$0
loop:	add 	$4,$4,$2
	addi	$5,$5,1	
	add	$10,$0,$0
	add	$11,$0,$0
	bne	$5,$3,loop
	add	$10,$0,$0
	add	$11,$0,$0
	add	$12,$0,$0
	halt


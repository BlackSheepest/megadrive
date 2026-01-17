## MAKEFILE for tools




all: ascii hexa binhex instr

instr: src/instr.c
	gcc -o instr src/instr.c

ascii: src/ascii.c
	gcc -o ascii src/ascii.c

hexa: src/hexa.c
	gcc -o hexa src/hexa.c

binhex: src/binhex.c
	gcc -o binhex src/binhex.c

test: instr
	./test_instr.sh

clean:
	rm -f instr
	rm -f ascii
	rm -f hexa
	rm -f binhex
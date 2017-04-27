percufa: percufa.c 
	gcc -O3 -Wall -o percufa.e percufa.c -lm
	./percufa.e 5 0.3419 0.5919 100 30000 7 8 9
	./percufa.e 5 0.3919 0.5919 100 30000 10 11 12 13 14 15
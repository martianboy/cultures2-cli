extract:
	gcc -I ./include src/extract.c src/utils/sort.c -o bin/extract -Wall

cif:
	gcc -I ./include src/cif.c -o bin/cif -Wall

bmd:
	gcc -I ./include src/bmd.c -o bin/bmd -Wall

pcx:
	gcc -I ./include src/pcx.c -o bin/pcx -Wall

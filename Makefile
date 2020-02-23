extract:
	gcc -I ./include src/bin/extract.c src/utils/sort.c -o bin/extract -Wall

cif:
	gcc -I ./include src/bin/cif.c -o bin/cif -Wall

bmd:
	gcc -I ./include src/bin/bmd.c src/bmd.c -o bin/bmd -Wall

bmd_convert:
	gcc -I ./include src/bin/bmd_convert.c src/pcx.c src/bmd.c -o bin/bmd_convert -Wall

pcx:
	gcc -I ./include src/bin/pcx.c src/pcx.c -o bin/pcx -Wall

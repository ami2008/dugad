CFLAGS=-lnetpbm -lm -I/usr/include -L/usr/lib/

all:
	gcc ${CFLAGS} embed.c   -o embed
	gcc ${CFLAGS} detect.c  -o detect
	gcc ${CFLAGS} compare.c -o compare

clean:
	rm -rf embed detect compare images/compare.pgm images/wm.pgm

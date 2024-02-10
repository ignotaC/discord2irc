CC=cc -Wall -Wextra -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L -O2

.PHONY: make clean clear

#make: smallsoft/bin/gethostipv scandiscod/logdisc writeirc/ircbot
make: scandiscord/logdisc writeirc/ircbot libigf_read.a libigf_write.a\
          libigt_sleep.a
	${CC} -c d2i.c -o d2i.o
	${CC} d2i.o -L. -ligt_sleep -ligf_write -ligf_read -o d2i
	@printf "Building finished\n"

writeirc/ircbot:
	make -C writeirc

scandiscord/logdisc:
	make -C scandiscord

smallsoft/bin/gethostipv: smallsoft
	make -C smallsoft

smallsoft:
	git clone 'https://github.com/ignotaC/smallsoft.git'

libigf_read.a: writeirc/ircbot
	cp writeirc/libigf_read.a libigf_read.a

libigf_write.a: writeirc/ircbot
	cp writeirc/libigf_write.a libigf_write.a

libigt_sleep.a: writeirc/ircbot
	cp writeirc/libigt_sleep.a libigt_sleep.a


clean clear:
	rm -f d2i d2i.o
	rm -rf smallsoft
	rm -f libigf_read.a libigf_write.a libigt_sleep.a
	make -C scandiscord clean
	make -C writeirc clean




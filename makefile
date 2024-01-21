
.PHONY: make clean clear

#make: smallsoft/bin/gethostipv scandiscod/logdisc writeirc/ircbot
make: scandiscord/logdisc writeirc/ircbot

writeirc/ircbot:
	make -C writeirc

scandiscord/logdisc:
	make -C scandiscord

smallsoft/bin/gethostipv: smallsoft
	make -C smallsoft

smallsoft:
	git clone 'https://github.com/ignotaC/smallsoft.git'

clean clear:
	rm -rf smallsoft
	make -C scandiscord clean
	make -C writeirc clean




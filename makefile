compile:
	g++ -Wall board.cpp paperSoccer.cpp main.cpp -o paperSoccer `pkg-config --cflags --libs gtk+-3.0`
debug:
	g++ -Wall -g board.cpp paperSoccer.cpp main.cpp -o paperSoccer `pkg-config --cflags --libs gtk+-3.0`
run:
	./paperSoccer
comprun:
	g++ -Wall board.cpp paperSoccer.cpp main.cpp -o paperSoccer `pkg-config --cflags --libs gtk+-3.0` && ./paperSoccer

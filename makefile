compile:
	g++ board.cpp paperSoccer.cpp main.cpp -o paperSoccer `pkg-config --cflags --libs gtk+-3.0`
debug:
	g++ -g board.cpp paperSoccer.cpp main.cpp -o paperSoccer `pkg-config --cflags --libs gtk+-3.0`
run:
	./paperSoccer
comprun:
	g++ board.cpp paperSoccer.cpp main.cpp -o paperSoccer `pkg-config --cflags --libs gtk+-3.0` && ./paperSoccer

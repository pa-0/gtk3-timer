run: build
	./timer 10

build: build/main.o
		gcc build/main.o build/gtk_simplified.o `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -o timer 

build/main.o: src/main.c
		gcc -c src/main.c `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -o build/main.o

clean:
	rm build/* percentage_calculator

a:
	g++ -c main.cpp -I ./include
	g++ main.o -o app -L./ -lsfml-graphics-2 -lsfml-window-2 -lsfml-system-2
	./app <zadani.txt >reseni.txt
	./app >zadani1.txt <reseni.txt
	make

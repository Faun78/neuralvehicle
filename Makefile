a:
	g++ -c main.cpp -I ./include
	g++ main.o -o app -L./include -lsfml-graphics-2 -lsfml-window-2 -lsfml-system-2
	app.exe <zadani1.txt >reseni.txt
	app.exe >zadani1.txt <reseni.txt
	make

	
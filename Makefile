filesystem : ./src/main.cpp control.o
	g++ -o ./bin/filesystem ./src/main.cpp ./obj/control.o

	
control.o : ./src/control.cpp
	g++ -c ./src/control.cpp -o ./obj/control.o


test :
	g++ -o test test.cpp

clean :
	rm -f  ./bin/filesystem test ./obj/* ./bin/*

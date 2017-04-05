filesystem : ./src/main.cpp control.o
	g++ -o ./bin/filesystem ./src/main.cpp ./obj/control.o


control.o : ./src/control.cpp
	g++ -c ./src/control.cpp -o ./obj/control.o

test : buffer.o test.cpp superblock.o
	g++ -o test test.cpp ./obj/buffer.o ./obj/superblock.o

buffer.o : ./src/buffer.cpp
	g++ -c ./src/buffer.cpp -o ./obj/buffer.o

superblock.o : ./src/superblock.cpp
	g++ -c ./src/superblock.cpp -o ./obj/superblock.o

clean :
	rm -f  ./bin/filesystem test ./obj/* ./bin/*

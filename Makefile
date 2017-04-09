filesystem : ./src/main.cpp control.o buffer.o superblock.o sector.o inode.o
	g++ -o ./bin/filesystem ./src/main.cpp ./obj/control.o ./obj/buffer.o \
	./obj/superblock.o ./obj/sector.o ./obj/inode.o

inode.o : ./src/inode.cpp
	g++ -c ./src/inode.cpp -o ./obj/inode.o

control.o : ./src/control.cpp
	g++ -c ./src/control.cpp -o ./obj/control.o

test : buffer.o test.cpp superblock.o
	g++ -o test test.cpp ./obj/buffer.o ./obj/superblock.o

sector.o : ./src/sector.cpp
	g++ -c ./src/sector.cpp -o ./obj/sector.o

buffer.o : ./src/buffer.cpp
	g++ -c ./src/buffer.cpp -o ./obj/buffer.o

superblock.o : ./src/superblock.cpp
	g++ -c ./src/superblock.cpp -o ./obj/superblock.o

clean :
	rm -f  ./bin/filesystem test ./obj/* ./bin/* p1-1.png

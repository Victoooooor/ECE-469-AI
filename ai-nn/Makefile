main: train.exe test.exe temp.exe
	rm -f *.o *.stackdump *~
	
train.exe: train.o NN.o
	g++ -o train.exe train.o NN.o

test.exe: test.o NN.o
	g++ -o test.exe test.o NN.o

temp.exe: temp.o NN.o
	g++ -o temp.exe temp.o NN.o

train.o: train.cpp
	g++ -c train.cpp

test.o: test.cpp
	g++ -c test.cpp

temp.o: temp.cpp
	g++ -c temp.cpp

NN.o: NN.cpp NN.h
	g++ -c NN.cpp 

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups

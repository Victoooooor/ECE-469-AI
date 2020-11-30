Checker.exe: main.o MyFrame.o checker.o
	g++ -o Checker.exe main.o MyFrame.o checker.o `wx-config --cxxflags` `wx-config --libs`

test.exe: test.o checker.o
	g++ -o test.exe test.o checker.o

main.o: main.cpp
	g++ -c main.cpp `wx-config --cxxflags` `wx-config --libs`

MyFrame.o: MyFrame.cpp MyFrame.h
	g++ -c MyFrame.cpp `wx-config --cxxflags` `wx-config --libs`

test.o: test.cpp
	g++ -c test.cpp

checker.o: checker.cpp checker.h
	g++ -c checker.cpp 

debug:
	g++ -g -o DijkstraDebug.exe Dijkstra.cpp dxta.cpp heap.cpp hash.cpp `wx-config --cxxflags` `wx-config --libs`

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
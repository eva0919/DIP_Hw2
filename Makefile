all:hw2p1.o hw2p2.o hw2p3a.o hw2p3b.o hw2p3c.o hw2p3d.o
	g++ -o hw2p1 hw2p1.o
	g++ -o hw2p2 hw2p2.o
	g++ -o hw2p3a hw2p3a.o
	g++ -o hw2p3b hw2p3b.o
	g++ -o hw2p3c hw2p3c.o
	g++ -o hw2p3d hw2p3d.o
	rm hw2p1.o
	rm hw2p2.o
	rm hw2p3a.o
	rm hw2p3b.o
	rm hw2p3c.o
	rm hw2p3d.o

hw2p1.o:hw2p1.cpp
	g++ -c hw2p1.cpp
hw2p2.o:hw2p2.cpp
	g++ -c hw2p2.cpp
hw2p3a.o:hw2p3a.cpp
	g++ -c hw2p3a.cpp
hw2p3b.o:hw2p3b.cpp
	g++ -c hw2p3b.cpp
hw2p3c.o:hw2p3c.cpp
	g++ -c hw2p3c.cpp
hw2p3d.o:hw2p3d.cpp
	g++ -c hw2p3d.cpp

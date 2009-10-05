
all: CursorPos

CursorPos:  CursorPos.o ../../IComm/ICommClient.o ../../../common/stat.o
	gcc -o CursorPos \
	 CursorPos.o ../../IComm/ICommClient.o ../../../common/stat.o \
	 -lbe -ltracker

CursorPos.o:	CursorPos.cpp CursorPos.fdh CursorPos.h ../../../common/basics.h \
		../../IComm/ICommClient.h ../../IComm/IComm.h
	gcc -g -c CursorPos.cpp -o CursorPos.o

../../IComm/ICommClient.o:	../../IComm/ICommClient.cpp ../../IComm/ICommClient.fdh ../../IComm/IComm.h ../../IComm/ICommClient.h
	gcc -g -c ../../IComm/ICommClient.cpp -o ../../IComm/ICommClient.o

../../../common/stat.o:	../../../common/stat.cpp ../../../common/stat.fdh ../../../common/basics.h
	gcc -g -c ../../../common/stat.cpp -o ../../../common/stat.o


clean:
	rm -f CursorPos.o
	rm -f ../../IComm/ICommClient.o
	rm -f ../../../common/stat.o

cleanfdh:
	rm -f CursorPos.fdh
	rm -f ../../IComm/ICommClient.fdh
	rm -f ../../../common/stat.fdh

cleanall: clean cleanfdh


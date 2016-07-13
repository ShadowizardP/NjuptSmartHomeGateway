	objs=SmartHome.o CameraController.o httpRequest.o MessageController.o OutlierTest.o SDT.o Simulate.o SimulatProgress.o support.o
SmartHome:$(objs)
	g++ -o SmartHome $(objs) -lvlc -lcurl -lpthread -std=c++11 -pthread
httpRequest.o:httpRequest.cpp httpRequest.h
	g++ -c httpRequest.cpp -I/usr/local/include -L/usr/local/lib -lcurl
SmartHome.o:SmartHome.h SmartHome.cpp
	g++ -c SmartHome.cpp -lpthread -std=c++11 -pthread
CameraController.o:CameraController.cpp httpRequest.h SmartHome.h
	g++ -c CameraController.cpp -I/usr/include -L/usr/lib -lvlc
MessageController.o:MessageController.cpp SmartHome.h support.h httpRequest.h
	g++ -c MessageController.cpp
OutlierTest.o:OutlierTest.cpp SmartHome.h support.h
	g++ -c OutlierTest.cpp
SDT.o:SDT.cpp support.h SmartHome.h
	g++ -c SDT.cpp
SimulatProgress.o:SimulatProgress.cpp Simulate.h
	g++ -c SimulatProgress.cpp
Simulate.o:Simulate.cpp Simulate.h SmartHome.h
	g++ -c Simulate.cpp
support.o:support.cpp support.h
	g++ -c support.cpp
clean:
	rm SmartHome $(objs)

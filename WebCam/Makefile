objects = main.o socketCode.o HTMLProcess.o WebCam.o

edit : $(objects)
	cc -o edit $(objects)
	
main.o : main.h
socketCode.o : socketCode.h
HTMLProcess.o : HTMLProcess.h
WebCam.o : WebCam.h

.PHONY : clean
clean :
	rm edit $(objects)
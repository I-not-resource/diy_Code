objects = main.o BPfree.o BPBase.objects

edit : $(objects)
	cc -o edit ${objects}
	
main.o : BPfree.h time.h
BPfree.O : BPfree.h
BPBase.O : BPBase.h

.PHONY : clean
clean
	rm edit $(objects)
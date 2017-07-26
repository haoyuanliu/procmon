object=procmon.o CycleBuffer.o StatData.o httpResponse.o TimeStamp.o plot.o
target=procmon dummyload
CXXFLAGS=-std=c++11 -g
GCC=g++ -std=c++11 -g
IFCGI=-I /usr/local/include
LFCGI=/usr/local/lib/libfcgi.a
all:$(target)
procmon : $(object)
	$(GCC) -o $@ $^ $(IFCGI) -lpthread -lgd $(LFCGI)
dummyload : dummyload.cpp TimeStamp.o
	$(GCC) -o $@ $^ -lpthread
clean:
	rm -rf $(target) $(object)

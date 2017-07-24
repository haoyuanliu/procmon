object=procmon.o CycleBuffer.o StatData.o httpResponse.o TimeStamp.o plot.o
target=procmon
CXXFLAGS=-std=c++11 -g
GCC=g++ -std=c++11 -g
IFCGI=-I /usr/local/include
LFCGI=/usr/local/lib/libfcgi.a
$(target) : $(object)
	$(GCC) -o $@ $^ $(IFCGI) -lpthread -lgd $(LFCGI)
clean:
	rm -rf $(target) $(object)

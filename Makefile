object=procmon.o CycleBuffer.o StatData.o httpResponse.o TimeStamp.o
target=procmon
CXXFLAGS=-g
GCC=g++ -g
IFCGI=-I /usr/local/include
LFCGI=/usr/local/lib/libfcgi.a
$(target) : $(object)
	$(GCC) -o $@ $^ $(IFCGI) -lpthread $(LFCGI)
clean:
	rm -rf $(target) $(object)

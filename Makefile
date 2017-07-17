target=procmon
object=procmon.o
CXXFLAGS=-g
CPP=g++ -g -o
&(targer):$(object)
    $(CPP) $@ $^ -lpthread
clean:
      rm -rf procmon.o procmon

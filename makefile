

CXX=g++
APP=img_proc_filters


$(APP): ImageBW.o ImageRGBInterleaved.o ProcessBW.o ProcessRGBInterleaved.o Loader.o Main.o 
	$(CXX) Main.o ImageBW.o ImageRGBInterleaved.o ProcessRGBInterleaved.o ProcessBW.o Loader.o -o $(APP)
	@echo "Application compilated!"

Main.o: Main.cpp
	$(CXX) -c Main.cpp -o Main.o

ImageBW.o: ImageBW.cpp ImageBW.h
	$(CXX) -c ImageBW.cpp -o ImageBW.o

ImageRGBInterleaved.o: ImageRGBInterleaved.cpp ImageRGBInterleaved.h
	$(CXX) -c ImageRGBInterleaved.cpp -o ImageRGBInterleaved.o

ProcessRGBInterleaved.o: ProcessRGBInterleaved.cpp ProcessRGBInterleaved.h
	$(CXX) -c ProcessRGBInterleaved.cpp -o ProcessRGBInterleaved.o

ProcessBW.o: ProcessBW.cpp ProcessBW.h
	$(CXX) -c ProcessBW.cpp -o ProcessBW.o

Loader.o: Loader.cpp Loader.h
	$(CXX) -c Loader.cpp -o Loader.o

removeAll: clean remove
	@echo "All removed!"

clean:
	rm -rf *.o
	@echo "All object files removed!"

remove:
	rm -rf $(APP)*
	@echo "Application removed!"



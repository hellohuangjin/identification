CPPFLAG = -I/usr/local/opt/opencv3/include
LDFLAGS = -L/usr/local/opt/opencv3/lib -O2
LIBS = -lopencv_imgcodecs -lopencv_core -lopencv_imgproc -lopencv_highgui -ltesseract

objects = detector.o main.o

idFind:$(objects) 
	clang++ $(objects) -o $@ $(LDFLAGS) $(LIBS)

main.o:main.cpp detector.h
	clang++ -c $< $(CPPFLAG)

detector.o:detector.cpp detector.h
	clang++ -c $< $(CPPFLAG)

.PHONY:clean

clean:
	rm -f idFind $(objects)
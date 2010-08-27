#Compilation/Linking/Loading flags
DEFINES = -g
CXXFLAGS = $(DEFINES)
CPPFLAGS = 
LD = g++
LDFLAGS = -lglut


#Rules
first_glut: first_glut.o
	$(LD) $(LDFLAGS) first_glut.o -o first_glut

first_glut.o: first_glut.cpp

clean:
	rm *.o
	rm first_glut

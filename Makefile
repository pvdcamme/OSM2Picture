EXEC=OSM2Picture
OBJ=main.o
LIB= -lexpat
CPPFLAGS= -Wall


build: ${OBJ}
	g++ ${CPPFLAGS} ${OBJ} ${LIB} -o ${EXEC}
.cpp.o:
	g++ -c ${CPPFLAGS} $<
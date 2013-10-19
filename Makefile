EXEC=OSM2Picture
OBJ=main.o
LIB= -lexpat
CPPFLAGS= -Wall


build: ${OBJ}
	g++ ${LIB} ${CPPFLAGS} ${OBJ} -o ${EXEC}
.cpp.o:
	g++ -c ${CPPFLAGS} $<

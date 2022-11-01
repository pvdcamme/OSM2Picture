EXEC=OSM2Picture
OBJ=main.o ImageMap.o OSMData.o
# For LibOsmium
LIB= -lpthread -lz 
CPPFLAGS= -Wall -g -O3


build: ${OBJ}
	g++ ${CPPFLAGS} ${OBJ} ${LIB} -o ${EXEC}
.cpp.o:
	g++ -c ${CPPFLAGS} $<
clean: 
	rm -- ${OBJ} ${EXEC}

EXEC=OSM2Picture.so
OBJ=main.o ImageMap.o OSMData.o
# For LibOsmium
LIB= -lpthread -lz 
CPPFLAGS= -Wall -g -O3 -fPIC


build: ${OBJ}
	g++ ${CPPFLAGS} ${OBJ} ${LIB} -shared -o ${EXEC}
.cpp.o:
	g++ -c ${CPPFLAGS} $<
clean: 
	rm -- ${OBJ} ${EXEC}

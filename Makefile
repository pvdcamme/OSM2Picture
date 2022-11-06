EXEC=OSM2Picture.so
OBJ=main.o ImageMap.o 
# For LibOsmium
LIB= -lpthread -lz 
CPPFLAGS= -Wall -g -O2 -fPIC -flto


build: ${OBJ}
	g++ ${CPPFLAGS} ${OBJ} ${LIB} -shared -o ${EXEC}
.cpp.o:
	g++ -c ${CPPFLAGS} $<
clean: 
	rm -- ${OBJ} ${EXEC}

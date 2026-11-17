# Compilation de Cesichi avec g++ et SFML 3
# "make" compile le jeu, "make run" le lance, "make clean" supprime l'exécutable.

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LIBS     = -lsfml-graphics -lsfml-window -lsfml-system

SOURCES  = $(wildcard src/*.cpp)

cesichi: $(SOURCES) $(wildcard src/*.hpp)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o cesichi $(LIBS)

run: cesichi
	./cesichi

clean:
	rm -f cesichi cesichi.exe

FLAGS=-O3 -std=c++17
DEBUGFLAGS=#-fsanitize=address -g

build: main.o frame.o linalg.o render2d.o scene2d.o shapes.o
	g++ main.o frame.o linalg.o render2d.o scene2d.o shapes.o $(FLAGS) $(DEBUGFLAGS) -pthread -lpng -o RenderDemo
	ar rvs render.a frame.o linalg.o render2d.o scene2d.o shapes.o

main.o: main.cpp render2d.h shader.h scene2d.h linalg.h shapes.h frame.h exampleshaders.h
	g++ -c main.cpp $(FLAGS) $(DEBUGFLAGS)

frame.o: frame.cpp frame.h shader.h
	g++ -c frame.cpp $(FLAGS) $(DEBUGFLAGS)

linalg.o: linalg.cpp linalg.h
	g++ -c linalg.cpp $(FLAGS) $(DEBUGFLAGS)

render2d.o: render2d.cpp render2d.h frame.h scene2d.h
	g++ -c render2d.cpp $(FLAGS) $(DEBUGFLAGS)

scene2d.o: scene2d.cpp scene2d.h shapes.h
	g++ -c scene2d.cpp $(FLAGS) $(DEBUGFLAGS)

shapes.o: shapes.cpp shapes.h linalg.h shader.h
	g++ -c shapes.cpp $(FLAGS) $(DEBUGFLAGS)

clean:
	rm *.o

MAKEFLAGS += -j6

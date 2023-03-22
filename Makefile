FLAGS=-O3 -std=c++17

a.out: main.o frame.o linalg.o render2d.o scene2d.o shapes.o
	g++ main.o frame.o linalg.o render2d.o scene2d.o shapes.o $(FLAGS) -pthread -lpng

lib: frame.o linalg.o render2d.o scene2d.o shapes.o
	ar rvs render.a frame.o linalg.o render2d.o scene2d.o shapes.o

main.o: main.cpp render2d.h shader.h scene2d.h linalg.h shapes.h frame.h exampleshaders.h
	g++ -c main.cpp $(FLAGS)

frame.o: frame.cpp frame.h shader.h
	g++ -c frame.cpp $(FLAGS)

linalg.o: linalg.cpp linalg.h
	g++ -c linalg.cpp $(FLAGS)

render2d.o: render2d.cpp render2d.h frame.h scene2d.h
	g++ -c render2d.cpp $(FLAGS) 

scene2d.o: scene2d.cpp scene2d.h shapes.h
	g++ -c scene2d.cpp $(FLAGS)

shapes.o: shapes.cpp shapes.h linalg.h shader.h
	g++ -c shapes.cpp $(FLAGS)

clean:
	rm *.o *.out

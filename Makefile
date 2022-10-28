FLAGS=-O3 -g -fsanitize=address

debug: main.o frame.o render.o scene2d.o scene3d.o shapes.o
	g++ main.o frame.o render.o scene2d.o scene3d.o shapes.o $(FLAGS)

main.o: main.cpp render.h
	g++ -c main.cpp $(FLAGS)

frame.o: frame.cpp frame.h shader.h
	g++ -c frame.cpp $(FLAGS)

render.o: render.cpp render.h frame.h scene2d.h scene3d.h
	g++ -c render.cpp $(FLAGS)

scene2d.o: scene2d.cpp scene2d.h shapes.h
	g++ -c scene2d.cpp $(FLAGS)

scene3d.o: scene3d.cpp scene3d.h shapes.h
	g++ -c scene3d.cpp $(FLAGS)

shapes.o: shapes.cpp shapes.h linalg.h
	g++ -c shapes.cpp $(FLAGS)

clean:
	rm *.o *.out

FLAGS=-O3

a.out: main.o frame.o linalg.o render.o scene2d.o scene3d.o shapes.o
	g++ main.o frame.o linalg.o render.o scene2d.o scene3d.o shapes.o $(FLAGS) -pthread

main.o: main.cpp render.h shader.h scene2d.h scene3d.h linalg.h shapes.h frame.h exampleshaders.h
	g++ -c main.cpp $(FLAGS)

frame.o: frame.cpp frame.h shader.h
	g++ -c frame.cpp $(FLAGS)

linalg.o: linalg.cpp linalg.h
	g++ -c linalg.cpp $(FLAGS)

render.o: render.cpp render.h frame.h scene2d.h scene3d.h
	g++ -c render.cpp $(FLAGS) -Drender_threads=24

scene2d.o: scene2d.cpp scene2d.h shapes.h
	g++ -c scene2d.cpp $(FLAGS)

scene3d.o: scene3d.cpp scene3d.h shapes.h
	g++ -c scene3d.cpp $(FLAGS)

shapes.o: shapes.cpp shapes.h linalg.h
	g++ -c shapes.cpp $(FLAGS)

clean:
	rm *.o *.out *.mp4 *.png

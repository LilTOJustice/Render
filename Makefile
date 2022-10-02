FLAGS=-O3 -g -fsanitize=address

a.out: obj
	g++ $(wildcard *.o) $(FLAGS)

obj: $(wildcard *.cpp) $(wildcard *.h)
	g++ -c $(wildcard *.cpp) $(FLAGS)

clean:
	rm *.o *.out

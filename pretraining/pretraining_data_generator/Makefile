start: rouge main semantic_sim
	g++ -std=c++11 rouge.o main.o semantic_sim.o -O3 -funroll-loops -o run.out -L/usr/local/lib/ -lboost_filesystem -lboost_system -fopenmp

rouge: rouge/rouge.hpp rouge/rouge.cpp FORCE
	g++ -std=c++11 -c rouge/rouge.cpp -O3 -funroll-loops -o rouge.o -fopenmp

main: main.cpp rouge
	g++ -std=c++11 -c main.cpp -O3 -funroll-loops -o main.o -fopenmp

semantic_sim: semantic_similarity/SemanticSimilarity.hpp
	g++ -std=c++11 -c semantic_similarity/SemanticSimilarity.cpp -O3 -funroll-loops -o semantic_sim.o -fopenmp
	
clean:
	rm -rf *.out *.o

FORCE: ;
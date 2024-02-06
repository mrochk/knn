FLAGS = -Wall -lm

all: build

build: 
	@mkdir -p bin
	@gcc src/main.c src/datapoint/datapoint.c src/knn/knn.c $(FLAGS) -o bin/knn

run: build
	@./bin/knn

memcheck: build
	@valgrind -s --leak-check=full ./bin/knn

clean:
	@rm -r bin
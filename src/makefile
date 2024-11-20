CC = clang++
FILE = heavy.cpp
EXE = main
RAY = -lraylib -lGL -lpthread -ldl -lrt 
STD = -lm

all: $(EXE)

$(EXE): $(FILE)
		$(CC) $(FILE) -o $(EXE) $(RAY) $(STD)

clean:
	rm -f $(EXE)

all: simulation

CC = g++
CC_FLAGS=-w

NODE_FILES= Node.cpp
NODE_OBJ_FILES= Node.o

SIMULATION_FILES = Simulation.cpp
SIMULATION_OBJ_FILES = Simulation.o

compile_simulation: $(NODE_FILES) Simulation.cpp
	$(CC) $(CC_FLAGS) -c $(NODE_FILES) Simulation.cpp 

simulation: compile_simulation
	$(CC) $(CC_FLAGS) -o simulation $(NODE_OBJ_FILES) $(SIMULATION_OBJ_FILES)

clean:
	rm -f $(NODE_OBJ_FILES) $(SIMULATION_OBJ_FILES) simulation
 



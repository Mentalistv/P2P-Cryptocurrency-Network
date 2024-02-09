all: simulation

CC = g++
CC_FLAGS=-w

NODE_FILES= Node.cpp network/GenerateNetwork.cpp
NODE_OBJ_FILES= Node.o GenerateNetwork.o

UTILITY_FILES= utility/RandomGenerator.cpp
UTILITY_OBJ_FILES= RandomGenerator.o

SIMULATION_FILES = Simulation.cpp
SIMULATION_OBJ_FILES = Simulation.o

MODEL_FILES = model/Event.cpp
MODEL_OBJ_FILES = Event.o


compile_simulation: $(NODE_FILES) $(UTILITY_FILES) $(MODEL_FILES) Simulation.cpp
	$(CC) $(CC_FLAGS) -c $(NODE_FILES) $(UTILITY_FILES) $(MODEL_FILES) Simulation.cpp 

simulation: compile_simulation
	$(CC) $(CC_FLAGS) -o simulation $(NODE_OBJ_FILES) $(UTILITY_OBJ_FILES) $(SIMULATION_OBJ_FILES) $(MODEL_OBJ_FILES)

clean:
	rm -f $(NODE_OBJ_FILES) $(SIMULATION_OBJ_FILES) $(UTILITY_OBJ_FILES) $(MODEL_OBJ_FILES) simulation
 



all: simulation

INCLUDE_DIR = headers

CC = g++
CC_FLAGS=-w -I $(INCLUDE_DIR)

NODE_FILES= Node.cpp network/GenerateNetwork.cpp
NODE_OBJ_FILES= Node.o GenerateNetwork.o

UTILITY_FILES= utility/RandomGenerator.cpp
UTILITY_OBJ_FILES= RandomGenerator.o

SIMULATION_FILES = Simulation.cpp
SIMULATION_OBJ_FILES = Simulation.o

MODEL_FILES = model/Block.cpp model/Transaction.cpp
MODEL_OBJ_FILES = Block.o Transaction.o

EVENT_FILES = events/ReceiveBlock.cpp events/GenerateTransaction.cpp events/ReceiveTransaction.cpp
EVENT_OBJ_FILES = ReceiveBlock.o GenerateTransaction.o ReceiveTransaction.o


ALL_CPP_FILES = $(NODE_FILES) $(UTILITY_FILES) $(MODEL_FILES) $(EVENT_FILES) $(SIMULATION_FILES)
ALL_OBJ_FILES = $(NODE_OBJ_FILES) $(UTILITY_OBJ_FILES) $(SIMULATION_OBJ_FILES) $(MODEL_OBJ_FILES) $(EVENT_OBJ_FILES)


compile_simulation: $(ALL_CPP_FILES)
	$(CC) $(CC_FLAGS) -c $(ALL_CPP_FILES) 

simulation: compile_simulation
	$(CC) $(CC_FLAGS) -o simulation $(ALL_OBJ_FILES) 

clean:
	rm -f $(ALL_OBJ_FILES) simulation
 



# all: simulation

# INCLUDE_DIR = headers

# CC = g++
# CC_FLAGS=-w -I $(INCLUDE_DIR)

# ROOT_DIR = .
# EVENTS_DIR = events
# MODEL_DIR = model
# NETWORK_DIR = network
# UTILITY_DIR = utility
# BUILD_DIR = build


# NODE_FILES= $(wildcard $(NETWORK_DIR)/*.cpp)
# NODE_OBJ_FILES= $(patsubst $(NETWORK_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(NETWORK_FILES))

# UTILITY_FILES= $(wildcard $(UTILITY_DIR)/*.cpp)
# UTILITY_OBJ_FILES= $(patsubst $(UTILITY_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(UTILITY_FILES))

# SIMULATION_FILES = $(wildcard $(ROOT_DIR)/*.cpp)
# SIMULATION_OBJ_FILES = $(patsubst $(ROOT_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SIMULATION_FILES))

# MODEL_FILES = $(wildcard $(MODEL_DIR)/*.cpp)
# MODEL_OBJ_FILES = $(patsubst $(MODEL)/%.cpp, $(BUILD_DIR)/%.o, $(MODEL_FILES))

# EVENT_FILES = $(wildcard $(EVENTS_DIR)/*.cpp)
# EVENT_OBJ_FILES = $(patsubst $(EVENTS_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(EVENT_FILES))


# ALL_CPP_FILES = $(NODE_FILES) $(UTILITY_FILES) $(MODEL_FILES) $(EVENT_FILES) $(SIMULATION_FILES)
# ALL_OBJ_FILES = $(NODE_OBJ_FILES) $(UTILITY_OBJ_FILES) $(MODEL_OBJ_FILES) $(EVENT_OBJ_FILES) $(SIMULATION_OBJ_FILES)


# $(BUILD_DIR)/%.o: $(ALL_CPP_FILES)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # compile_simulation: $(ALL_CPP_FILES)
# # 	$(CC) $(CC_FLAGS) -o $(ALL_OBJ_FILES) -c $(ALL_CPP_FILES) 

# simulation: $(ALL_OBJ_FILES)
# 	$(CC) $(CC_FLAGS) -o simulation $(ALL_OBJ_FILES) 

# clean:
# 	rm -f $(ALL_OBJ_FILES) simulation



all: simulation

INCLUDE_DIR = headers

CC = g++
CC_FLAGS=-w -I $(INCLUDE_DIR)

NODE_FILES= network/GenerateNetwork.cpp
NODE_OBJ_FILES= GenerateNetwork.o

UTILITY_FILES= utility/RandomGenerator.cpp
UTILITY_OBJ_FILES= RandomGenerator.o

SIMULATION_FILES = Simulation.cpp
SIMULATION_OBJ_FILES = Simulation.o

MODEL_FILES = model/Block.cpp model/Transaction.cpp model/Node.cpp model/SelfishNode.cpp
MODEL_OBJ_FILES = Block.o Transaction.o Node.o SelfishNode.o

EVENT_FILES = events/ReceiveBlock.cpp events/GenerateTransaction.cpp events/ReceiveTransaction.cpp events/Event.cpp events/MineBlock.cpp
EVENT_OBJ_FILES = ReceiveBlock.o GenerateTransaction.o ReceiveTransaction.o Event.o MineBlock.o


ALL_CPP_FILES = $(NODE_FILES) $(UTILITY_FILES) $(MODEL_FILES) $(EVENT_FILES) $(SIMULATION_FILES)
ALL_OBJ_FILES = $(NODE_OBJ_FILES) $(UTILITY_OBJ_FILES) $(SIMULATION_OBJ_FILES) $(MODEL_OBJ_FILES) $(EVENT_OBJ_FILES)


compile_simulation: $(ALL_CPP_FILES)
	$(CC) $(CC_FLAGS) -c $(ALL_CPP_FILES) 

simulation: compile_simulation
	$(CC) $(CC_FLAGS) -o simulation $(ALL_OBJ_FILES) 

clean:
	rm -f $(ALL_OBJ_FILES) simulation
 



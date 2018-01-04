

SOURCE_DIR := src
OBJ_DIR := result\obj
DEPEND_DIR := result\depend
HEADER_DIR := include
BIN_DIR := result\bin

DEBUG := $(BIN_DIR)\Debug\csv_plot
RELEASE := $(BIN_DIR)\Release\csv_plot

SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp)
HEADERS := $(wildcard $(HEADER_DIR)/*.h)

SOURCE_NAME := $(notdir $(SOURCES))
OBJS := $(addprefix $(OBJ_DIR)/,$(SOURCE_NAME:.cpp=.o))
DEPENDS := $(addprefix $(DEPEND_DIR)/,$(SOURCE_NAME:.cpp=.depend))
CC := g++
CFLAGS :=

.PHONY: all
all: $(DEPENDS) $(DEBUG)
debug: $(DEPENDS) $(DEBUG)
debug: CFLAGS += -g -Wall -DDEBUG -O0
release: $(DEPENDS) $(RELEASE)
release: CFLAGS += -Wall -DNDEBUG -O2

$(DEBUG): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(DEBUG)

$(RELEASE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(RELEASE)

$(DEPEND_DIR)/%.depend: $(SOURCE_DIR)/%.cpp $(HEADERS)
	@echo "generating $@"
	@$(CC) -I$(HEADER_DIR) -MM $< > $@

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $^ -o $@

ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPENDS)
endif

.PHONY : clean
clean:
	del $(DEBUG).exe $(RELEASE).exe $(DEPEND_DIR)\*.depend $(OBJ_DIR)\*.o

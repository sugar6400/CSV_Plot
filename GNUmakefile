

SOURCE_DIR := src
OBJ_DIR := result/obj
DEPEND_DIR := result/depend
HEADER_DIR := include
BIN_DIR := result/bin

DEBUG := $(BIN_DIR)/Debug/csv_plot
RELEASE := $(BIN_DIR)/Release/csv_plot

SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp)
HEADERS := $(wildcard $(HEADER_DIR)/*.h)

SOURCE_NAME := $(notdir $(SOURCES))
OBJS := $(addprefix $(OBJ_DIR)/,$(SOURCE_NAME:.cpp=.o))
DEPENDS := $(addprefix $(DEPEND_DIR)/,$(SOURCE_NAME:.cpp=.depend))
CC := g++
CFLAGS := -Wall -O2

.PHONY: all
all: $(DEPENDS) $(DEBUG)
debug: $(DEPENDS) $(DEBUG)
release: $(DEPENDS) $(RELEASE)
$(DEBUG): $(OBJS)
	$(CC) $(CFLAGS) -g $^ -o $(DEBUG)

$(RELEASE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(RELEASE)

$(DEPEND_DIR)/%.depend: $(SOURCE_DIR)/%.cpp $(HEADERS)
	@echo "generating $@"
	@$(CC) $(CFLAGS) -I$(HEADER_DIR) -MM $< > $@

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $^ -o $@


ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPENDS)
endif

.PHONY : clean
clean:
	rm -f $(BIN_DIR)/Debug
	rm -f $(BIN_DIR)/Release
	rm -fr $(OBJS)
	rm -fr $(DEPENDS)

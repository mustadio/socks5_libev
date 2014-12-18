MOD_NAME = $(shell basename $(CURDIR))

INC_DIR = ./include ../common/include
SRC_DIR = ./src

CUR_OBJ_DIR = $(OBJ_DIR)/$(MOD_NAME)


SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst %.c,$(CUR_OBJ_DIR)/%.o,$(notdir $(SRC_FILES)))
DEP_FILES = $(patsubst %.c,$(CUR_OBJ_DIR)/%.d,$(notdir $(SRC_FILES)))


TARGET = $(OBJ_DIR)/$(MOD_NAME)$(MOD_EXT)

include $(TOP_DIR)/build/config.mak

.PHONY: all $(CUR_OBJ_DIR) clean

all: $(CUR_OBJ_DIR) $(TARGET)


$(CUR_OBJ_DIR):
	$(MKDIR_P) $(CUR_OBJ_DIR)

$(TARGET): $(DEP_FILES) $(OBJ_FILES)
	$(AR) $(AR_FLAGS) $@ $(OBJ_FILES)

$(DEP_FILES): $(CUR_OBJ_DIR)/%.d: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -MM $^ -MF $@
	@sed -i "s,^.*o:,$(CUR_OBJ_DIR)/&," $@
	@echo -e '\t' $(CC) $(CFLAGS) -c  $< -o $(patsubst %.d,%.o, $@) >> $@

-include $(DEP_FILES)

clean:
	@-rm $(TARGET) $(OBJ_FILES) $(DEP_FILES)

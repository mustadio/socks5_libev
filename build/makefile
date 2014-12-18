SUBMODS = echo_server common
SUB_DIRS = $(addprefix ../, $(SUBMODS))


TOP_DIR = $(CURDIR)/..
OBJ_DIR = $(CURDIR)/obj


INC_DIR = $(addsuffix /include, $(SUB_DIRS))
SRC_DIR = ..


SRC_FILES = $(wildcard ${SRC_DIR}/*.c)
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRC_FILES)))
DEP_FILES = $(patsubst %.c,$(OBJ_DIR)/%.d,$(notdir $(SRC_FILES)))

DEPEND_FILE = $(OBJ_DIR)/.depend

LIB_FLAGS = $(addsuffix $(MOD_EXT), $(addprefix $(OBJ_DIR)/, $(SUBMODS)))
LIB_FLAGS += -lev

export TOP_DIR
export OBJ_DIR

TARGET = socks5

include config.mak


.PHONY: all clean $(SUB_DIRS) $(OBJ_DIR)


all: $(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	$(MKDIR_P) $(OBJ_DIR)

$(TARGET): $(SUB_DIRS) $(DEP_FILES) $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(LIB_FLAGS) -o $@

$(DEP_FILES): $(OBJ_DIR)/%.d: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -MM $^ -MF $@
	@sed -i "s,^.*o:,$(OBJ_DIR)/&," $@
	@echo -e '\t' $(CC) $(CFLAGS) -c  $< -o $(patsubst %.d,%.o, $@) >> $@

-include $(DEP_FILES)


$(SUB_DIRS):
	@$(MAKE) -C $@ $(MAKECMDGOALS)

clean: $(SUB_DIRS)
	@-rm -rf $(TARGET) $(OBJ_FILES) $(DEP_FILES)










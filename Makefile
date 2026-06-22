# 操作系统库
ifeq ($(OS),)
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        OS := Mac
    else ifeq ($(UNAME_S),Linux)
        OS := Linux
    else ifeq ($(UNAME_S),Windows_NT)
        OS := Windows
    else
        OS := Unknown
    endif
else ifeq ($(OS),Windows_NT)
	OS := Windows
endif

# 编译工具
CROSS_COMPILE  := 
CC              := $(CROSS_COMPILE)gcc
LD              := $(CROSS_COMPILE)ld
OBJCOPY         := $(CROSS_COMPILE)objcopy
NM              := $(CROSS_COMPILE)nm
BIN_SUFFIX      := .out

# 文件目录
SOURCE_DIR      := src
LIB_DIR         := lib
INCLUDE_DIR     := include
TEST_DIR        := test
OUTPUT_DIR      := output
RES_DIR      	:= res
OUTPUT_DEP_DIR  := $(OUTPUT_DIR)/.dep
OUTPUT_OBJ_DIR  := $(OUTPUT_DIR)/.obj
MODULES_DIR     := modules

# 确保必要目录存在
$(shell mkdir -p $(SOURCE_DIR) $(LIB_DIR)/$(OS) $(INCLUDE_DIR) $(TEST_DIR) $(OUTPUT_DEP_DIR) $(OUTPUT_OBJ_DIR) $(MODULES_DIR) $(RES_DIR)) 
	
# 源文件收集
SRCS            := $(shell find $(SOURCE_DIR) -type f -name "*.c")
OBJS            := $(patsubst %.c,$(OUTPUT_OBJ_DIR)/%.o,$(SRCS))
DEPS            := $(patsubst %.c,$(OUTPUT_DEP_DIR)/%.d,$(SRCS))

SRCS_NO_MAIN    := $(filter-out $(SOURCE_DIR)/main.c, $(SRCS))
OBJS_NO_MAIN    := $(patsubst %.c,$(OUTPUT_OBJ_DIR)/%.o,$(SRCS_NO_MAIN))
DEPS_NO_MAIN    := $(patsubst %.c,$(OUTPUT_DEP_DIR)/%.d,$(SRCS_NO_MAIN))

TSET_SRCS       := $(shell find $(TEST_DIR) -type f -name "*.c")
TEST_OBJS       := $(patsubst %.c,$(OUTPUT_OBJ_DIR)/%.o,$(TSET_SRCS))
TEST_DEPS       := $(patsubst %.c,$(OUTPUT_DEP_DIR)/%.d,$(TSET_SRCS))
TEST_OUT        := $(patsubst $(TEST_DIR)/%.c,$(OUTPUT_DIR)/%$(BIN_SUFFIX),$(TSET_SRCS))

TARGET_NAME 	?= demo

-include $(DEPS)
-include $(DEPS_NO_MAIN)
-include $(TEST_DEPS)

ifeq ($(OS), Mac)
	OS_LIBS := -framework Cocoa -framework AppKit -framework Foundation \
			  -framework CoreFoundation -framework IOKit -framework CoreVideo \
			  -framework OpenGL -framework QuartzCore
else ifeq ($(OS), Linux)
  	OS_LIBS := -ldl -lpthread -lm
else ifeq ($(OS), Windows)
	OS_LIBS := -lgdi32 -luser32 -lkernel32 -lshell32 -lopengl32 -lwinmm
else
  	$(error Unsupported OS: $(OS))
endif

# 子模块收集
MODULES_INC 	:= $(patsubst %,-I%,$(wildcard $(MODULES_DIR)/*/$(INCLUDE_DIR)))
MODULES_OBJ	 	:= $(patsubst %,-L%,$(wildcard $(MODULES_DIR)/*/$(OUTPUT_DIR)))
MODULES_PATH 	:= $(foreach d,$(wildcard $(MODULES_DIR)/*/$(OUTPUT_DIR)) $(wildcard $(MODULES_DIR)/*/$(OUTPUT_DIR)),-Wl,-rpath,$(d))
MODULES_FILE    := $(patsubst lib%,-l%,$(basename $(notdir $(filter %.so %.lib %.a, $(wildcard $(MODULES_DIR)/*/$(OUTPUT_DIR)/*)))))

# 库文件收集
LIB_FILES 		:= $(shell find $(LIB_DIR)/$(OS) -type f  -name "*.so" -o -name "*.lib" -o -name "*.dll" -o -name "*.a")
LIBS_DIR		:= $(patsubst %,-L%,$(wildcard $(LIB_DIR)/$(OS)/*/))
LIBS 			:= $(foreach f,$(notdir $(LIB_FILES)),-l$(basename $(patsubst lib%,%,$(f))))

# 编译标志
CFLAGS          := -Wall -Wextra -g -Wno-unused-variable -Wno-missing-braces -DOUTPUT_DIR=\"$(OUTPUT_DIR)\" -DRES_DIR=\"$(RES_DIR)\"
INCLUDE         := -I$(INCLUDE_DIR) $(MODULES_INC)
LIB             := -L$(LIB_DIR)/$(OS) $(LIBS_DIR) $(MODULES_OBJ) $(MODULES_FILE) -Wl,-rpath=$(LIB_DIR)/$(OS) $(MODULES_PATH)

# 创建目录的规则
define MAKE_DIR
    @mkdir -p $1
endef

# 编译命令
$(OUTPUT_OBJ_DIR)/%.o : %.c
	$(call MAKE_DIR,$(dir $@))
	$(call MAKE_DIR,$(OUTPUT_DEP_DIR)/$(dir $*))
	$(CC) $(CFLAGS) $(INCLUDE) -MMD -MP -MF $(OUTPUT_DEP_DIR)/$*.d -MT $@ -c $< -o $@

# 默认目标
all:  $(TEST_OUT)

# 测试目标
$(OUTPUT_DIR)/%.out: $(OUTPUT_OBJ_DIR)/$(TEST_DIR)/%.o $(OBJS_NO_MAIN) | $(LIB_FILES) $(OUTPUT_OBJ_DIR)/$(TEST_DIR)/%.o
	$(call MAKE_DIR,$(dir $@))
	$(CC) $^ $(LIB) $(LIBS) $(OS_LIBS) $(MODULES_FILE) -o $@

# 打印目标
print-%:
	@echo $($*)

# 清理目标
clean:
	rm -rf $(OUTPUT_DIR)

.PHONY: all clean

# 静态库目标
static: $(OBJS_NO_MAIN) | $(LIB_FILES)
	ar rcs $(OUTPUT_DIR)/lib$(TARGET_NAME).a $^

# 动态库目标
dynamic: $(OBJS_NO_MAIN) | $(LIB_FILES)
	$(CC) $(CFLAGS) $(INCLUDE) -fPIC -shared -o $(OUTPUT_DIR)/lib$(TARGET_NAME).so $^ $(LIB) $(LIBS) $(OS_LIBS)

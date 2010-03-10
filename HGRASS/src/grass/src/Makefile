########################################################################
# A generic Makefile for OpenGL and GTK+(libglade).
#
# Originally Created by Li Ji <liji@ic.sjtu.edu.cn>
# June 25, 2009
########################################################################
#
# Usage:
#
# Synopsis:
#
# make [GUI2D=0] [PARALLEL=0] [DEBUG=0]
#
# Options:
#
# GUI2D=1:  Enable the GTK+ 2D GUI instead of the default one,
#           3D GUI implemented with OpenGL. Set GUI2D=1 will force PARALLEL=0.
#
# PARALLEL=1: Enable the parallel mode (OpenMP parallel mode requires gcc version > 4.1).
#
# DEBUG=1:  Build a debug version.
#
# Supported Platform: MINGW32, CYGWIN, Linux
#
########################################################################

# System information
SUPPORTED_PLATFORMS := Linux, CYGWIN, MINGW32
export PLATFORM := $(findstring $(shell uname -s | sed 's,_.*,,'),$(SUPPORTED_PLATFORMS))
ifeq ($(PLATFORM),"")
$(error Unknown platform: $(shell uname -s))
endif
# target name. Usually, only TARGET_NAME need to be changed
TARGET_NAME := grass
# include path. Default: .
export INCLUDE_DIRS := . ../pccts/h ../inc
# target path. Default: .
TARGET_PATH := .
# C++ compiler
ifeq ($(PLATFORM),CYGWIN)
export CC := g++-4
else
export CC := g++
endif
# Archieve utility
AR := ar
ARFLAGS := rus

# debug flag. Note: comment out the line below for the release version
ifeq ($(DEBUG),1)
CONFIG := debug
export DEBUG_FLAG := -O0 -ggdb  -DNTRACE -DDEBUG -DNDEBUG1
#DEBUG_FLAG += -pg
else
CONFIG := release
export DEBUG_FLAG := -O3
endif

# libraries
LDFLAGS :=

# GUI option: 3D OpenGL (default, -DGUI3D) or 2D GTK+ (empty)
# Will generate different GUI_LIB.
ifeq ($(GUI2D),1)
GUI := GUI2D
GUI_LIB_DIR := gtk_gui
export GUI_LIB := libgtkgui.a
LDFLAGS += `pkg-config libglade-2.0 --libs`
MAIN_GUI2D_CFLAGS := `pkg-config libglade-2.0 --cflags`
# GUI2D does not support parallel computation
PARALLEL := 0
else
GUI := GUI3D
GUI_LIB_DIR := opengl_gui
export GUI_LIB := libopenglgui.a
ifeq ($(PLATFORM),MINGW32)
LDFLAGS += -mwindows $(GUI_LIB_DIR)/glut32.lib -lopengl32 -lglu32
else
LDFLAGS += -lglut -lGLU -lGL
endif
endif

# OpenMP Support, gcc version > 4.0 is required
ifeq ($(PARALLEL),1)
OPENMP := -fopenmp
DEBUG_FLAG += -DPARALLEL_COMPUTING
CONFIG_PARALLEL := Parallel
# OpenMP Support on MINGW32
ifeq ($(PLATFORM),MINGW32)
LDFLAGS += -lgomp -lpthread
endif
else
OPENMP :=
CONFIG_PARALLEL := Serial
endif
LDFLAGS += $(OPENMP)

INCLUDE_DIRS += $(GUI_LIB_DIR)

# all compiler flags
export CFLAGS := -Wall -Wno-unused-variable $(DEBUG_FLAG) -D$(GUI) $(OPENMP) 

# The excutable has ".exe" suffix on the CYGWIN and MINGW32 platforms.
ifeq ($(PLATFORM),Linux)
TARGET_SUFFIX :=
else
TARGET_SUFFIX := .exe
endif
# construct the full target name
TARGET := $(TARGET_PATH)/$(TARGET_NAME)-$(GUI)-$(PLATFORM)-$(CONFIG)-$(CONFIG_PARALLEL)$(TARGET_SUFFIX)

# the prerequisite of each source file will be generated automatically
# the suffix of source files
export SRCS_SUFFIX := .cpp
# the suffix of rerequisite files
export PREREQ_SUFFIX := .d

SRCS := $(wildcard *$(SRCS_SUFFIX))
OBJS := $(patsubst %$(SRCS_SUFFIX),%.o,$(wildcard *$(SRCS_SUFFIX)))
PREREQS := $(SRCS:%$(SRCS_SUFFIX)=%$(PREREQ_SUFFIX))

.PHONY: gui clean distclean

all: $(TARGET)

ifneq "$(MAKECMDGOALS)" "clean"
ifneq "$(MAKECMDGOALS)" "distclean"
sinclude $(PREREQS)
endif
endif

$(TARGET): $(OBJS) $(GUI_LIB_DIR)/$(GUI_LIB)
	@echo Building $(TARGET) ...; echo -e "\n"
	$(CC) -o $@ $^ $(LDFLAGS) 
	@echo -e "\n"; echo "Build $(TARGET) successfully"

$(GUI_LIB_DIR)/$(GUI_LIB): gui
	$(MAKE) --directory=$(GUI_LIB_DIR)

%$(PREREQ_SUFFIX): %$(SRCS_SUFFIX)
	$(CC) -MM $< $(CFLAGS) $(addprefix -I, $(INCLUDE_DIRS)) > $@.$$$$; \
	sed 's,\($*\)\.o[:]*,\1.o $@:,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

ifeq ($(GUI2D),1)
main.o: main$(SRCS_SUFFIX)
	$(CC) -c $< -o $@ $(CFLAGS) \
		$(MAIN_GUI2D_CFLAGS) $(addprefix -I,$(INCLUDE_DIRS))
endif

%.o: %$(SRCS_SUFFIX)
	$(CC) -c $< -o $@ $(CFLAGS) $(addprefix -I,$(INCLUDE_DIRS))

clean:
	$(MAKE) --directory=$(GUI_LIB_DIR) $@
	$(RM) $(OBJS) $(PREREQS) $(GUI_LIB_DIR)/$(GUI_LIB)

distclean: clean
	$(MAKE) --directory=$(GUI_LIB_DIR) $@
	$(RM) $(TARGET_NAME)-*-$(PLATFORM)-*$(TARGET_SUFFIX)


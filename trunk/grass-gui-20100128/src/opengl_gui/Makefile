########################################################################
# Sub-Makefile for gtk+ gui
# A generic Makefile for GTK+(libglade).
#
# Originally Created by Li Ji <liji@ic.sjtu.edu.cn>
# Nov 4, 2009
########################################################################
CC := $(if $(CC),$(CC),g++)
AR := $(if $(AR),$(AR),ar)
ARFLAGS := $(if $(ARFLAGS),$(ARFLAGS),rus)

SRCS_SUFFIX := $(if $(SRCS_SUFFIX),$(SRCS_SUFFIX),.cpp)
SRCS := $(wildcard *$(SRCS_SUFFIX))
OBJS := $(patsubst %$(SRCS_SUFFIX),%.o,$(wildcard *$(SRCS_SUFFIX)))
PREREQ_SUFFIX := $(if $(PREREQ_SUFFIX),$(PREREQ_SUFFIX),.d)
PREREQS := $(SRCS:%$(SRCS_SUFFIX)=%$(PREREQ_SUFFIX))
OPENGLGUI_LIB := $(if $(GUI_LIB),$(GUI_LIB),libopenglgui.a)

all: $(OPENGLGUI_LIB)

ifneq "$(MAKECMDGOALS)" "clean"
ifneq "$(MAKECMDGOALS)" "distclean"
sinclude $(PREREQS)
endif
endif

$(OPENGLGUI_LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%$(PREREQ_SUFFIX): %$(SRCS_SUFFIX)
	$(CC) -MM $< $(addprefix -I../,$(INCLUDE_DIRS)) > $@.$$$$; \
	sed 's,\($*\)\.o[:]*,\1.o $@:,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

%.o: %$(SRCS_SUFFIX)
	$(CC) -c $< -o $@ $(CFLAGS) $(addprefix -I../,$(INCLUDE_DIRS))

.PHONY: clean distclean

clean:
	$(RM) $(OBJS) $(PREREQS)

distclean: clean
	$(RM) $(OPENGLGUI_LIB) 

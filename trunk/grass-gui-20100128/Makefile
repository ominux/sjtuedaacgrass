# make [PARALLEL=0] [DEBUG=0]
# PARALLEL=1: Enable the parallel mode (OpenMP parallel mode requires g++ version > 4.1).
# DEBUG=1:  Build a debug version.

DOXYGEN := doxygen
DOXYFILE := Doxyfile

all:
	$(MAKE) --directory=src GUI2D=1 $(MAKECMDGOALS)
	$(MAKE) --directory=src GUI2D=1 clean
	$(MAKE) --directory=src GUI2D=0 $(MAKECMDGOALS)
	$(MAKE) --directory=src GUI2D=0 clean

.PHONY: distclean doc

distclean:
	$(MAKE) --directory=src GUI2D=0 $@; \
	$(MAKE) --directory=src GUI2D=1 $@

doc:
	$(DOXYGEN) $(DOXYFILE)

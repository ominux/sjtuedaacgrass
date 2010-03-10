ROOT = $(PWD)
SRC = $(ROOT)/src
GRASS = $(SRC)/grass/src
SPADE = $(SRC)/spade
SED = $(SRC)/sed
SCRIPT = $(ROOT)/script
RPT = $(ROOT)/rpt
DOC = $(ROOT)/doc
CASES = $(ROOT)/examples

DIRS = $(GRASS) $(SPADE)

GRASS3D = $(GRASS)/grass-GUI3D-Linux-release-Serial
GRASS2D = $(GRASS)/grass-GUI2D-Linux-release-Serial
GRASS3DSim = $(GRASS)/grass3D
GRASS2DSim = $(GRASS)/grass2D
SPADEBIN = $(SPADE)/SPADE

PROGRAM = SPADE

all:$(DIRS)
	#GRASS Make
	@echo Building GRASS Binary ...;
	$(MAKE) -C $(GRASS) clean >/dev/null
	$(MAKE) -C $(GRASS) GUI2D=0 >/dev/null
	$(MAKE) -C $(GRASS) clean >/dev/null
	$(MAKE) -C $(GRASS) GUI2D=1 >/dev/null
	mv $(GRASS3D) $(GRASS3DSim); mv $(GRASS2D) $(GRASS2DSim)
	@echo GRASS Binary Building Finished!;
	#End of GRASS Make
	#SPADE Make
	@echo Building SPADE Binary ...;
	$(MAKE) -C $(SPADE) clean >/dev/null
	$(MAKE) -C $(SPADE) >/dev/null
	mv $(SPADEBIN) $(ROOT)
	@echo SPADE Binary Building Finished!;
	#End of SPADE Make
	#SED Make
	#End of SED Make
clean:
	#GRASS Clean
	$(MAKE) -C $(GRASS) clean >/dev/null
	#End of GRASS Clean
	#SPADE Clean
	$(MAKE) -C $(SPADE) clean >/dev/null
	#End of SPADE Clean
	#SED Clean
	#End of SED Clean
	rm -f $(PROGRAM) *.o

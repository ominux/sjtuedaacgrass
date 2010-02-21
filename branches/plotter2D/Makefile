# Top Makefile for Qt 4 projects on windows NT (32 bit)
MAKE := mingw32-make
QMAKE := qmake
# Qt4 Makefile
MAKEFILE_QT = Makefile.qt

.PHONY: qt debug release clean distclean

all: qt
	$(MAKE) -f$(MAKEFILE_QT) -j2

debug: qt
	$(MAKE) $@ -f$(MAKEFILE_QT) -j2

release: qt
	$(MAKE) $@ -f$(MAKEFILE_QT) -j2

qt: 
	$(QMAKE) -project
	$(QMAKE) -win32 -o $(MAKEFILE_QT)

clean:
	$(MINGW32_MAKE) -f$(MAKEFILE_QT) $@
	$(RM) $(MAKEFILE_QT)

distclean:
	$(MINGW32_MAKE) -f$(MAKEFILE_QT) $@
	$(RM) $(MAKEFILE_QT)

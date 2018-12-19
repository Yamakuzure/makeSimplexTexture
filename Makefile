# ------------------------------------
# Version
# ------------------------------------
VERSION := 0.8.6

# ------------------------------------
# Tools and Flags
# ------------------------------------
CXX      ?= $(shell which g++)
CXXFLAGS += -std=c++17 -Wall -Wextra -Wpedantic -fexceptions -O2
CPPFLAGS += $(shell pkg-config --cflags pwxlib) $(shell pkg-config --cflags sfml-graphics) -DVERSION=\"${VERSION}\"
INSTALL  := $(shell which install)
LDFLAGS  += $(shell pkg-config --libs pwxlib) $(shell pkg-config --libs sfml-graphics)
RM       := $(shell which rm) -f
SED      := $(shell which sed)
TARGET   := mkst

# Use the compiler as the linker.
LD := $(CXX)

# ------------------------------------
# Install and target directories
# ------------------------------------
PREFIX     ?= /usr
DESTDIR    ?=
BINPREFIX  ?= $(PREFIX)
BINDIR     ?= ${BINPREFIX}/bin
DOCPREFIX  ?= $(PREFIX)
DOCDIR     ?= $(DOCPREFIX)/share/doc/makeSimplexTexture-$(VERSION)

# ------------------------------------
# Source files, objects, deps, doc
# ------------------------------------
SOURCES  := $(sort $(wildcard src/*.cpp))
MODULES  := $(addprefix obj/,$(notdir $(SOURCES:.cpp=.o)))
DEPENDS  := $(addprefix dep/,$(notdir $(SOURCES:.cpp=.d)))
DOCFILES := \
AUTHORS ChangeLog code_of_conduct.md CONTRIBUTING.md \
INSTALL.md LICENSE NEWS.md README.md TODO.md


# ------------------------------------
# Rules
# ------------------------------------
.Phony: all clean install
.SUFFIXES: .cpp

# ------------------------------------
# Create dependencies
# ------------------------------------
dep/%.d: src/%.cpp
	@set -e; $(RM) $@; \
	$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,obj/\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

# ------------------------------------
# Compile modules
# ------------------------------------
obj/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

# ------------------------------------
# Default target
# ------------------------------------
all: $(TARGET)

# ------------------------------------
# Regular targets
# ------------------------------------
clean:
	@echo "Cleaning makeSimplexTexture"
	@rm -rf $(TARGET) $(MODULES)

$(TARGET): $(MODULES)
	$(CXX) -o $(TARGET) $(MODULES) $(CPPFLAGS) $(LDFLAGS) $(CXXFLAGS)

install: $(TARGET)
	$(INSTALL) -d $(DESTDIR)${BINDIR}
	$(INSTALL) -m 755 $(TARGET) $(DESTDIR)${BINDIR}
	$(INSTALL) -d $(DESTDIR)${DOCDIR}
	$(INSTALL) -m 644 $(DOCFILES) $(DESTDIR)${DOCDIR}

# ------------------------------------
# Include all dependency files
# ------------------------------------
ifeq (,$(findstring clean,$(MAKECMDGOALS)))
  -include $(DEPENDS)
endif

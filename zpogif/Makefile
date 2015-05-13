LIBRARY = libzpogif.so.1.0.0
LDFLAGS += -Wl,-soname,libzpogif.so.1
SOURCES = common.cpp gif-save.cpp main.cpp streamops.cpp zpogif.cpp
HEADERS = common.hpp gif-load.hpp streamops.hpp zpogif.h zpogif.hpp zpogif_error.h

HEADERDIR = src
SRCDIR = src
OBJDIR = obj

CXXFLAGS += -g -std=c++11 -Wall -pedantic -I$(HEADERDIR)

OBJSFULL = $(addprefix $(OBJDIR)/,$(addsuffix .o, $(SOURCES)))
SOURCESFULL = $(addprefix $(SRCDIR)/,$(SOURCES))
HEADERSFULL = $(addprefix $(HEADERDIR)/,$(HEADERS))

all: $(LIBRARY)

clean:
	$(RM) -r $(OBJDIR)
	$(RM) $(LIBRARY)

$(LIBRARY): $(OBJSFULL)
	$(CXX) -shared $(CXXFLAGS) $(LDLIBS) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.cpp.o: $(SRCDIR)/%.cpp $(HEADERSFULL)
	mkdir -p $(@D)
	$(CXX) -fPIC $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

.PHONY: all clean


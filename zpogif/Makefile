LIBRARY = libzpogif.so.1.0.0
LDFLAGS += -Wl,-soname,libzpogif.so.1
SOURCES = common.cpp gif-save.cpp main.cpp streamops.cpp zpogif.cpp
PUBHEADERS = zpogif.hpp zpogif_error.h zpogif_exceptions.hpp
PRIVHEADERS = common.hpp gif-load.hpp gif-save.hpp streamops.hpp 

PUBHEADERDIR = include
PRIVHEADERDIR = src
SRCDIR = src
OBJDIR = obj

CXXFLAGS += -g -std=c++11 -Wall -pedantic -I$(PUBHEADERDIR) -I$(PRIVHEADERDIR)

OBJSFULL = $(addprefix $(OBJDIR)/,$(addsuffix .o, $(SOURCES)))
SOURCESFULL = $(addprefix $(SRCDIR)/,$(SOURCES))
PUBHEADERSFULL = $(addprefix $(PUBHEADERDIR)/,$(PUBHEADERS))
PRIVHEADERSFULL = $(addprefix $(PRIVHEADERDIR)/,$(PRIVHEADERS))

all: $(LIBRARY)

clean:
	$(RM) -r $(OBJDIR)
	$(RM) $(LIBRARY)

$(LIBRARY): $(OBJSFULL)
	$(CXX) -shared $(CXXFLAGS) $(LDLIBS) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.cpp.o: $(SRCDIR)/%.cpp $(PUBHEADERSFULL) $(PRIVHEADERSFULL)
	mkdir -p $(@D)
	$(CXX) -fPIC $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

.PHONY: all clean


LIBRARY = libzpopcx.so
LDFLAGS += -Wl,-soname,libzpopcx.so
SOURCES = pcx.cpp zpopcx.cpp
PUBHEADERS = zpopcx.h
PRIVHEADERS = pcx.hpp

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


.PHONY: all clean compile
CXX=g++
CXXFLAGS=-Wall -pedantic -std=c++17 -fsanitize=address -fsanitize=undefined -g
SRCFOLDER=src
OFOLDER=compiled
CCAL=CCalendar
CERPT=CEventRepeat

SRCS := $(wildcard $(SRCFOLDER)/$(CERPT)/*.cpp) $(wildcard $(SRCFOLDER)/$(CCAL)/*.cpp) $(wildcard $(SRCFOLDER)/*.cpp)
OBJS := $(patsubst $(SRCFOLDER)/%.cpp, $(OFOLDER)/%.o, $(SRCS))
DIRS := $(sort $(dir $(OBJS)))

all: folder kaspafil doc 

kaspafil: $(OBJS)
	$(CXX) $(OBJS) -o $@ $(CXXFLAGS) -lstdc++fs

$(OFOLDER)/%.o: $(SRCFOLDER)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

doc:
	doxygen Doxyfile

compile: folder kaspafil

run:
	./kaspafil

clean:
	rm -f kaspafil
	rm -rf $(OFOLDER)
	rm -rf doc

folder:
	mkdir -p $(OFOLDER)
SOURCES := $(shell find ./src -type f -name "*.cpp")

OBJECTS := $(addprefix build/,$(SOURCES:%=%.o))

CXX := mpicxx
CXXFLAGS := -std=c++17 -O3 -Wall -Wextra -MMD -MP -march=native -mtune=native

LIBS := -Wl,-Bstatic -lkissat -L kissat-inc/build -I kissat-inc/ \
	    -llstech -L lstech_maple/build/release/lib -I lstech_maple/lstech/ \
	    -lm4ri -L m4ri-20140914/.libs -I m4ri-20140914/ \
        -pthread -lz -lm -lboost_thread -lboost_date_time -lboost_system \
		-Wl,-Bdynamic -lmpi_cxx -lmpi 

PRS-distributed: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

build/%.o: % | build
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)

-include $(OBJECTS:.o=.d)

build:
	mkdir -p $(sort $(dir $(OBJECTS)))

clean:
	rm -rf build $(OBJECTS)

.PHONY: clean all
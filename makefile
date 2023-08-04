SRCS = $(shell find . -name "*.cpp") 
OBJS = $(addsuffix .o, $(basename $(SRCS)))
EXEC = PRS
STATIC_LIB = libprs.a
LIBS = -lkissat -Lsolvers/kissat-inc/build \
	   -lm4ri -Lpreprocess/m4ri-20140914/.libs \
       -lpthread -pthread -lz -lm -lboost_thread -lboost_date_time -lboost_system

CXXFLAGS = -Isolvers/kissat-inc \
		   -Ipreprocess/m4ri-20140914 \
		   -std=c++11 -O3 -g

# $(EXEC): $(OBJS)
# 	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

$(STATIC_LIB): $(OBJS)
	ar rcs $@ $^

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(OBJS) $(EXEC) $(STATIC_LIB)
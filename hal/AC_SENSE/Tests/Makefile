# Add CPP source files here
CPP_SRC =  ../AC_SENSE.cpp\
	   ../POWER.cpp\
	   ../RMS.cpp\
	   mocks/AC_IP.cpp\
	   Tests.cpp\
	   Main.cpp\
	   Sine.cpp\

# Add include paths here
INC_PATH = mocks

INC_PARAMS=$(foreach d, $(INC_PATH), -I$d)

CPP_OBJS = $(patsubst %.cpp, %.o, $(CPP_SRC))

a11: ac_param_calc.exe

ac_param_calc.exe: $(CPP_OBJS)
	g++ -std=c++11 -o ac_param_calc.exe $^

%.o: %.cpp
	g++ -std=c++11 -v -g -o $@ -c $< $(INC_PARAMS)

clean:
	rm $(CPP_OBJS)
	rm *.exe

.PHONY: run

run:
	./ac_param_calc.exe

.PHONY: every

every:
	make clean
	make all
	male run

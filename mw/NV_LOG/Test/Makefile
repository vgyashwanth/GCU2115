# Add CPP source files here
CPP_SRC =  ../NVlog.cpp\
		   mocks/FLASH/FLASH.cpp\
		   mocks/NV_LOG_Interface/NV_LOG_Interface.cpp\
		   mocks/CRC/UTILS_Crc.cpp\
		   main.cpp\
		   Test.cpp

# Add include paths here
INC_PATH = mocks/json/include\
		   mocks/FLASH\
		   mocks/CRC\
		   mocks/C_QUEUE\
		   mocks\
		   ../\
		   .

INC_PARAMS=$(foreach d, $(INC_PATH), -I$d)

CPP_OBJS = $(patsubst %.cpp, %.o, $(CPP_SRC))

a11: flash.exe

flash.exe: $(CPP_OBJS)
	g++ -std=c++11 -o flash.exe $^

%.o: %.cpp
	g++ -std=c++11 -v -g -o $@ -c $< $(INC_PARAMS)

clean:
	rm $(CPP_OBJS)
	rm *.exe

.PHONY: run

run:
	./flash.exe

.PHONY: every

every:
	make clean
	make all
	male run

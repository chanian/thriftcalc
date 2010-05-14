CC=g++ -O2 -lthrift
CFLAGS=-g -I./ -I/usr/local/include/thrift/ -c

all: thrift calculator_server

thrift:
	thrift -gen cpp ../calculator.thrift
	cp gen-cpp/*.cpp .
	cp gen-cpp/*.h .

calculator_server: calculator_constants.o calculator_types.o Calculator.o CalculatorHandler.o 
	$(CC) CalculatorHandler.o Calculator.o calculator_constants.o calculator_types.o -o calculator_server

calculator_constants.o: calculator_constants.cpp
	$(CC) $(CFLAGS) calculator_constants.cpp

calculator_types.o: calculator_types.cpp
	$(CC) $(CFLAGS) calculator_types.cpp

Calculator.o: Calculator.cpp
	$(CC) $(CFLAGS) Calculator.cpp

CalculatorHandler.o: CalculatorHandler.cpp
	$(CC) $(CFLAGS) CalculatorHandler.cpp

clean:
	rm -rf *o calculator_server
	mv CalculatorHandler.cpp CalculatorHandler.tmp
	rm *.cpp
	rm *.h
	cp CalculatorHandler.tmp CalculatorHandler.cpp
	rm CalculatorHandler.tmp
	rm -rf gen-cpp/

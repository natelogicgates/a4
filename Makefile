# CXX Make variable for compiler
CC=g++
# -std=c++11  C/C++ variant to use, e.g. C++ 2011
# -Wall       show the necessary warning files
# -g3         include information for symbolic debugger e.g. gdb 
CCFLAGS=-std=c++11 -Wall -g3 -c

# object files
OBJS = producer.o consumer.o tradecrypto.o report.o

# Program name
PROGRAM = tradecrypto

# The program depends upon its object files
$(PROGRAM) : $(OBJS)
	$(CC) -pthread -o $(PROGRAM) $(OBJS)

producer.o : producer.cpp producer.h
	$(CC) $(CCFLAGS) producer.cpp

consumer.o: consumer.cpp consumer.h
	$(CC) $(CCFLAGS) consumer.cpp

report.o : report.c report.h
	$(CC) $(CCFLAGS) producer.c
    
tradecrypto.o: tradecrypto.cpp tradecrypto.h
	$(CC) $(CCFLAGS) tradecrypto.cpp

# Once things work, people frequently delete their object files.
# If you use "make clean", this will do it for you.
# As we use gnuemacs which leaves auto save files termintating
# with ~, we will delete those as well.
clean :
	rm -f *.o *~ $(PROGRAM)

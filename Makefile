CC = g++
exe_file = state_facts
$(exe_file): state_facts.o run_facts.o 
	$(CC) state_facts.cpp run_facts.cpp -o $(exe_file)
state_facts.o: state_facts.cpp
	$(CC) -c state_facts.cpp
run_facts.o: run_facts.cpp
	$(CC) -c run_facts.cpp
remove:
	rm -f *.o *.out $(exe_file) 

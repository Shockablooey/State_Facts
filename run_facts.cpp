#include "state_facts.h" 

int main(int argc, char** argv){
	int numStates;
	state* states;
	char filename[256];

	if (valid_args(argv, argc)){ // if 5 args and -s and -f exist
		numStates = get_num_states(argv); // set numStates using argv or user input 
		cout << "Number of states: " << numStates << endl;

		ifstream input;
		get_good_file(argv, input, filename); // makes sure the file is good and gets another if not
		cout << "Name of file: " << filename << endl;

		states = create_states(numStates);
		input.open(filename); // open input file
		get_state_data(states, numStates, input);
		input.close();

	//	show_array(states, numStates); // used for debugging
		print_information(states, numStates);
		delete_info(&states, numStates);
	}

	else 
		return 1; // if bad args


	return 0;
} // end of main function


/********************************************************************* 
 ** Program Filename: shrubbery 
 ** Author: Chase Stubblefield
 ** Date: 16 April 2017
 ** Description: receives county and state information from an external file,
 ** interprets it, and outputs the new information to either the screen 
 ** or a file
 ** Input: 
 ** Output:
 *********************************************************************/

#include <iostream>
#include <string> // c++ strings
#include <cstring> // c style strings
#include <fstream> // for file io
#include <cstdlib>
#include <algorithm> // for swap() command
 
using namespace std;

struct state {
       string name; 
       struct county* counties; // struct within a struct 
       int numCounties;
       int population; 
};
struct county {
        string name; 
        string* cities; // array of city names 
        int numCities;
        int population; 
        float avgIncome;  //avg household income
        float avgHouseCost;  //avg cost of a house in county
};

/*--- FUNCTION DECLARATIONS ------------------------------------------------------*/
bool valid_args(char** argv, int argc);
void crash_and_die();
int get_num_states(char** argv);
void get_good_file(char** argv, ifstream &, char[]);
state* create_states(int);
county* create_counties(int);
string* create_cities(int);
void get_state_data(state* , int, ifstream &);
void print_information(state* , int);
void largest_populations(state* , int, bool, ofstream&);
void analysis(state* , int, bool, ofstream&);
void show_array(state* states, int numStates);
void delete_info(state**, int);
void income_check(state* , int, bool, ofstream&);
void house_costs(state* states, int numStates, bool, ofstream&);
void alphabetical_states(state* states, int numStates, bool, ofstream&);
void sort(state* states, int i, int letter, bool* done);
void alphabetical_counties(state* states, int numStates, bool, ofstream&);
void sort_counties(state* states, int i, int j, int letter, bool* done);
void population(state* states, int numStates, bool file, ofstream& output);
void population_counties(state* states, int numStates, bool file, ofstream& output);
/*--------------------------------------------------------------------------------*/ 


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


/*--------------------------------------------------------------------------------*/ 
// Additions:
// 
//
// 
/*--------------------------------------------------------------------------------*/ 

/********************************************************************* 
 ** Function: analysis
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void analysis(state* states, int numStates, bool file, ofstream &output){
 
	income_check(states, numStates, file, output);
	largest_populations(states, numStates, file, output);
	house_costs(states, numStates, file, output);
	alphabetical_states(states, numStates, file, output);
	alphabetical_counties(states, numStates, file, output);
	population(states, numStates, file, output);
	population_counties(states, numStates, file, output);

}
/********************************************************************* 
 ** Function: print_information
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void print_information(state* states, int numStates){
	bool file;
	char userFileName[256];
	ofstream output;

	cout << "| Would you like to export the data to the console or an external file? |" << endl;
	cout << "| (0) console | (1) file 						|";
	cin >> file; // user choice
	if (file == 0){ // output to the console
		analysis(states, numStates, file, output);
	}
	else{ // output to a file
		cout << "Name of output file? (if does not exist, will be created):";
		cin >> userFileName;
		output.open(userFileName);
		analysis(states, numStates, file, output);
	}
}
/********************************************************************* 
 ** Function: population
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void population(state* states, int numStates, bool file, ofstream& output){
	for(int c=0; c < 25; c++) // go through 25 times becuase im too lazy to do the bool stuff
		for(int i=0; i < (numStates - 1); i++){ //go through all the states minus the last one
			if(states[i].population < states[i+1].population) 
				swap(states[i],states[i+1]);
		}
	if(file == false){
		cout << "----------------------------------------------------------------------------------------------------" << endl;
		cout << "The states sorted by population:" << endl;
		for(int j=0; j < numStates; j++)
			cout << "| " << states[j].name << " (" << states[j].population << ") "; // print out the states
	}
	else{
		output << "----------------------------------------------------------------------------------------------------" << endl;
		output << "The states sorted by population:" << endl;
		for(int j=0; j < numStates; j++)
			output << "| " << states[j].name << " (" << states[j].population << ") "; // print out the states
	}
}
/********************************************************************* 
 ** Function: population_counties
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void population_counties(state* states, int numStates, bool file, ofstream& output){

	if(file == false){	
		cout << endl << "----------------------------------------------------------------------------------------------------";
		for(int i=0; i < numStates; i++){
			cout << endl;
			cout << "Counties in " << states[i].name << " sorted by population: " << endl;
			for(int c=0; c < 10; c++)
				for(int j=0; j < states[i].numCounties - 1; j++){ // go through all the counties in a state minus the last
					if(states[i].counties[j].population < states[i].counties[j+1].population)
						swap(states[i].counties[j], states[i].counties[j+1]);
				}
			for(int k=0; k < states[i].numCounties; k++) // print information for a state
				cout << "| " << states[i].counties[k].name << " (" << states[i].counties[k].population << ") ";
		}
		cout << endl;
	}
	else{
		output << endl << "----------------------------------------------------------------------------------------------------";
		for(int i=0; i < numStates; i++){
			output << endl;
			output << "Counties in " << states[i].name << " sorted by population: " << endl;
			for(int c=0; c < 10; c++)
				for(int j=0; j < states[i].numCounties - 1; j++){ // go through all the counties in a state minus the last
					if(states[i].counties[j].population < states[i].counties[j+1].population)
						swap(states[i].counties[j], states[i].counties[j+1]);
				}
			for(int k=0; k < states[i].numCounties; k++) // print information for a state
				output << "| " << states[i].counties[k].name << " (" << states[i].counties[k].population << ") ";
		}
		output << endl;
	}

}
/********************************************************************* 
 ** Function: alphabetical_counties
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void alphabetical_counties(state* states, int numStates, bool file, ofstream& output){
	bool done;

	cout << "----------------------------------------------------------------------------------------------------" << endl;
	for(int i=0; i < numStates; i++){
		if(file == false)
			cout << "Counties of " << states[i].name << " in alphabetical order:";
		else{
			if(i == 0) // nice formatting
				output << "----------------------------------------------------------------------------------------------------" << endl;
			output << "Counties of " << states[i].name << " in alphabetical order:";

		}
		do{ // repeatedly goes through this loop until no switches are made 
			done = true; // assumes true
			for(int j=0; j < (states[i].numCounties - 1); j++){ // go through the entire 1D struct array minus the last
				sort_counties(states, i, j, 0, &done); // sort a pair, using recursion if same letter
			}
		}while(done == false);

		for(int k=0; k < states[i].numCounties; k++){
			if(file == 0)
				cout << " " << states[i].counties[k].name;
			else
				output << " " << states[i].counties[k].name;
		}
		if(file == 0)
			cout << endl;
		else
			output << endl;
	}
}
/********************************************************************* 
 ** Function: sort_counties
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void sort_counties(state* states, int i, int j, int letter, bool* done) {
	if(states[i].counties[j].name.at(letter) > states[i].counties[j+1].name.at(letter)){
		swap(states[i].counties[j], states[i].counties[j+1]); // swap the two elements
		*done = false; // make sure we go through the whole array at least one more time
	}
	else if(states[i].counties[j].name.at(letter) < states[i].counties[j+1].name.at(letter)) {
		return; // they are in the correct order, so return
	}
	else {
		sort_counties(states, i, j, letter + 1, done); // recursively call with the next letter
	}
}
/********************************************************************* 
 ** Function: alphabetical_states
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void alphabetical_states(state* states, int numStates, bool file, ofstream& output){
	bool done = true;

	if(file == false){
		cout << "----------------------------------------------------------------------------------------------------" << endl;
		cout << "Order of the states array pre program run:" << endl;	
		for(int c=0; c < numStates; c++)
			cout << states[c].name << " ";
		cout << endl; 

		do{ // repeatedly goes through this loop until no switches are made 
			done = true; // assumes true
			for(int i=0; i < (numStates - 1); i++){ // go through the entire 1D struct array minus the last
				sort(states, i, 0, &done); // sort a pair, using recursion if same letter
			}
		}while(done == false);

		cout << "Order of the states array once alphabetized:" << endl;
		for(int r=0; r < numStates; r++)
			cout << states[r].name << " ";
		cout << endl;
	}
	else{
		output << "----------------------------------------------------------------------------------------------------" << endl;
		output << "Order of the states array pre program run:" << endl;	
		for(int c=0; c < numStates; c++)
			output << states[c].name << " ";
		output << endl; 

		do{ // repeatedly goes through this loop until no switches are made 
			done = true; // assumes true
			for(int i=0; i < (numStates - 1); i++){ // go through the entire 1D struct array minus the last
				sort(states, i, 0, &done); // sort a pair, using recursion if same letter
			}
		}while(done == false);

		output << "Order of the states array once alphabetized:" << endl;
		for(int r=0; r < numStates; r++)
			output << states[r].name << " ";
		output << endl;
	}
}

/********************************************************************* 
 ** Function: sort
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void sort(state* states, int i, int letter, bool* done) {
	if(states[i].name.at(letter) > states[i+1].name.at(letter)){
		swap(states[i], states[i+1]); // swap the two elements
		*done = false; // make sure we go through the whole array at least one more time
	}
	else if(states[i].name.at(letter) < states[i + 1].name.at(letter)) {
		return; // they are in the correct order, so return
	}
	else {
		sort(states, i, letter + 1, done); // recursively call with the next letter
	}
}
/********************************************************************* 
 ** Function: house_costs
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void house_costs(state* states, int numStates, bool file, ofstream& output){
	float totalCounties = 0; // we will increment this and then divide by it
	float totalHouseCost = 0; // we will add to this each time we get a county
	if(file == false){
		cout << "----------------------------------------------------------------------------------------------------";
		for(int i=0; i < numStates; i++){
			totalHouseCost = 0; // reset these values for each state
			totalCounties = 0;
			for(int j=0; j < states[i].numCounties; j++){
				totalHouseCost += states[i].counties[j].avgHouseCost;
				totalCounties++;
			}
			cout << endl << "The average cost of a house in " << states[i].name <<  " is " << totalHouseCost/totalCounties;
		}
		cout << endl;
	}
	else{
		output << "----------------------------------------------------------------------------------------------------";
		for(int i=0; i < numStates; i++){
			totalHouseCost = 0; // reset these values for each state
			totalCounties = 0;
			for(int j=0; j < states[i].numCounties; j++){
				totalHouseCost += states[i].counties[j].avgHouseCost;
				totalCounties++;
			}
			output << endl << "The average cost of a house in " << states[i].name <<  " is " << totalHouseCost/totalCounties;
		}
		output << endl;
	}
}
/********************************************************************* 
 ** Function: income_check
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void income_check(state* states, int numStates, bool file, ofstream& output){
	int standard; // the user input

	if(file == false){
		cout << "|----------------------------------------------------------------------------------------------------|" << endl;
		cout << "| Value for finding counties with average income above a certain amount? ";
		cin >> standard;
		cout << "The counties with an income level above " << standard << " are:" << endl;

		for(int i=0; i < numStates; i++)
			for(int j=0; j < states[i].numCounties; j++){
				if(states[i].counties[j].avgIncome >= standard){
					cout << "| " << states[i].counties[j].name << " (" << states[i].counties[j].avgIncome << ") ";
				}
			}
		cout << endl;
	}		
	else{
		output << "----------------------------------------------------------------------------------------------------" << endl;
		cout << "| Value for finding counties with average income above a certain amount? ";
		cin >> standard;
		output << "The counties with an income level above " << standard << " are:" << endl;

		for(int i=0; i < numStates; i++)
			for(int j=0; j < states[i].numCounties; j++){
				if(states[i].counties[j].avgIncome >= standard){
					output << "| " << states[i].counties[j].name << " (" << states[i].counties[j].avgIncome << ") ";
				}
			}
		output << endl;
	}		
}
/********************************************************************* 
 ** Function: delete_info
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void delete_info(state** states, int numStates){
	for(int i=0; i < numStates; i++)
		for(int j=0; j < (*states)[i].numCounties; j++)
			delete[](*states)[i].counties[j].cities; // deletes each cities array
	for(int c=0; c < numStates; c++)
		delete[](*states)[c].counties; // deletes each counties array
	delete[](*states); // deletes states array


}
/********************************************************************* 
 ** Function: show_array 
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void show_array(state* states, int numStates){
	for(int i = 0; i < numStates; i++){
		cout << endl;
		cout << states[i].name << " ";
		cout << states[i].population << " ";
		cout << states[i].numCounties;
		for(int j = 0; j < states[i].numCounties; j++){
			cout << endl << states[i].counties[j].name << " ";
			cout << states[i].counties[j].population << " ";
			cout << states[i].counties[j].avgIncome << " ";
			cout << states[i].counties[j].avgHouseCost << " ";
			cout << states[i].counties[j].numCities << " ";
			for(int k = 0; k < states[i].counties[j].numCities; k++)
				cout << states[i].counties[j].cities[k] << " ";
		}
	}
	cout << endl;
}
/********************************************************************* 
 ** Function: largest_populations
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void largest_populations(state* states, int numStates, bool file, ofstream& output){
	int best = 0; // set it to the first value
	int best2 = 0;
	int best3 = 0;

	for(int i=0; i < numStates; i++){ // loops up until the value before the last
		if(states[i].population > states[best].population)
			best = i; // we've found a new highest value
	}
	if(file == false){
		cout << "----------------------------------------------------------------------------------------------------" << endl;
		cout << "The state with the highest population is " << states[best].name;
		cout << ". (" << states[best].population << ")" << endl;
	}
	else{
		output << "----------------------------------------------------------------------------------------------------" << endl;
		output << "The state with the highest population is " << states[best].name;
		output << ". (" << states[best].population << ")" << endl;
	}

	for(int j=0; j < numStates; j++)
		for (int k=0; k < states[j].numCounties; k++){
			if(states[j].counties[k].population > states[best2].counties[best3].population){
				best2 = j;
				best3 = k; // we've found a new highest value 
			}
		}
	if(file == false){
		cout << "The county with the highest population is " << states[best2].counties[best3].name;
		cout << ". (" << states[best2].counties[best3].population << ")" << endl;
	}
	else{
		output << "The county with the highest population is " << states[best2].counties[best3].name;
		output << ". (" << states[best2].counties[best3].population << ")" << endl;
	}		
}
/********************************************************************* 
 ** Function: get_state_data
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void get_state_data(state* states, int numStates, ifstream &input){
	for(int i = 0; i < numStates; i++){
		input >> states[i].name;
		input >> states[i].population;
		input >> states[i].numCounties;
		states[i].counties = create_counties(states[i].numCounties); // make counties struct array
		for(int j = 0; j < states[i].numCounties; j++){
			input >> states[i].counties[j].name;
			input >> states[i].counties[j].population;
			input >> states[i].counties[j].avgIncome;
			input >> states[i].counties[j].avgHouseCost;
			input >> states[i].counties[j].numCities;
			states[i].counties[j].cities = create_cities(states[i].counties[j].numCities); // make cities string array
			for(int k = 0; k < states[i].counties[j].numCities; k++)
				input >> states[i].counties[j].cities[k];
		}
	}
}

/********************************************************************* 
 ** Function: get_good_file
 ** Description:
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
void get_good_file(char** argv, ifstream &input, char temp[256] ){
	bool bad = false; // assume good unless a bad input is given

	for (int c = 0; c < 5; c++){
		if (strcmp(argv[c], "-f") == 0)
			strcpy(temp, argv[c+1]); // finds the proper arg and stores it in a temporary 1D array
	}
	input.open(temp);
	if (input.fail()){
		do{
			bad = false; // reset bool "bad" for the loop
			input.close();
			cout << "File could not be opened.  Please supply another: ";
			cin.getline(temp,256);
			input.open(temp);
			if (input.fail())
				bad = true;	
		}while(bad == true);
	}
	input.close();
	return;
}
/********************************************************************* 
 ** Function: create_counties
 ** Description: makes the struct counties array
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
county* create_counties(int numCounties){
	county* pointer = new county[numCounties];
	return pointer;
}
/********************************************************************* 
 ** Function: create_states
 ** Description: makes the struct states array
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
state* create_states(int numStates){
	state* pointer = new state[numStates];	
	return pointer;
}
/********************************************************************* 
 ** Function: create_cities
 ** Description: makes the cities array
 ** Parameters:
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
string* create_cities(int numCities){
	string* pointer = new string[numCities];
	return pointer;
}
/********************************************************************* 
 ** Function: pull_arg_data 
 ** Description: takes a positive nonzero integer and good file name
 ** Parameters: argv and numStates 
 ** Pre-Conditions:
 ** Post-Conditions:
 ** Return:
 *********************************************************************/
int get_num_states(char** argv){
	char temp[256]; // where we will store the user input 
	int tempInt; // the int we will continually alter until it is good and then return
	bool bad = false; // is assumed good until bad input is given

	for (int c = 0; c < 5; c++){
		if (strcmp(argv[c], "-s") == 0)
			strcpy( temp, argv[c+1] ); // finds the proper arg and stores it in a temporary 1D array
	}

	tempInt = atoi(temp); // stores the result of atoi on temporary array in tempInt
	if (tempInt < 1 || tempInt > 50){ // if the command line argument is bad
		do{ // loop until we get a good integer input
			cout << "Input is invalid.  Please enter a new input: ";
			cin.getline(temp, 256); // get user input 
			tempInt = atoi(temp); // perform atoi on the new input
			if (tempInt < 1 || tempInt > 50)
				bad = true; // get a new input if the integer is not within bounds
			else 
				bad = false; // if it is within bounds, exit the loop and return
		}while(bad == true);
	}
	return tempInt;
}
/********************************************************************* 
 ** Function: check_args
 ** Description: checks the commmand line arguments for 5 args and for a -s and -f
 ** Parameters: the command line arguments
 ** Pre-Conditions: command line arguments have been entered and the program run properly
 ** Post-Conditions: no variables are altered
 ** Return: either true or false
 *********************************************************************/
bool valid_args(char** argv, int argc){
	if (argc != 5){
		crash_and_die();
		return false; // we do not have 5 args
	}
	else if ( strcmp(argv[1] , "-s") != 0  && strcmp(argv[1] , "-f") != 0 ){ 
		crash_and_die();
		return false; // our second arg is not -s of -f
	}
	else if ( strcmp(argv[3] , "-s") != 0  && strcmp(argv[3] , "-f") != 0 ){ 
		crash_and_die();
		return false; // our fourth arg is not -s or -f
	}
	else 
		return true;
}
/********************************************************************* 
 ** Function: crash_and_die 
 ** Description: displays an error message
 ** Parameters: none
 ** Pre-Conditions: none
 ** Post-Conditions: no variables are altered
 ** Return: none
 *********************************************************************/
void crash_and_die(){
	cout << "|----------------------------------------------------------------------------------------------------|" << endl;
	cout << "|												     |" << endl;	
	cout << "|		You have entered invalid command line arguments.				     |" << endl;
	cout << "|              Example run: ./state_facts -s (1-8) -f file     				     |" << endl;
	cout << "|												     |" << endl;
	cout << "|----------------------------------------------------------------------------------------------------|" << endl;
}

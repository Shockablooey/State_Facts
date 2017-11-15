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


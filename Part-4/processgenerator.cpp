// Faris Hawamdeh
// CSCE 4600
// Homework 2 - Problem 3

#include <iostream>
#include <random>
#include <fstream>
#include <stdlib.h>
#include "four.h"

using namespace std;

// Example of Structure to Store Process Information
// NOT ACTUALLY USED IN THIS PROGRAM.
struct process {
    int id;
    int cycles;
    int memory;
};

int main( int argc, char* argv[] )
{	
	initialize();//added this 
	double numbercycles, numbermemory;//added this
	int numProcs = 0;
	int i = 0;
	ofstream outputFile;

	// Check if Arguments are correct.
	if ( argc > 2 )
	{
		numProcs = atoi( argv[1] );
	}
	else
	{
		cout << "Not Enough Arguments! Restate as ./exe *Number of Processes* *Output File Name*";
		return 0;
	}

	// Sets Up Normal Distribution.
	std::default_random_engine generator(time(0));
	std::normal_distribution<double> cycles( 25000000.0, 10000000.0 );
	std::normal_distribution<double> memory( 5000000.0, 4000000.0 );

	// Attempt to Open File
	outputFile.open( argv[2], ios::trunc );
	if (	outputFile.is_open()	)
	{
		// Prints Heading for Displaying Values.
		//cout << "ProcessID\tNumberOfCycles (Millions)\t\tMemory ( kb )\n";
		

		for( i = 0; i < numProcs; i++ )
		{
			// Prints Process Number to Screen.
			// Process Number Not Printed to File.
			//cout << i << "\t\t";

			// Infinite While Loops to Generate Valid Values.
         while(1)
         {
			// Generate Number of Cycles and Check If Positive
			numbercycles = cycles(generator);
            if ( numbercycles > 10.0 && numbercycles < 50000000.0 )
				{
					// Print Number of Cycles to Screen and Write to File.
					//cout << int( numbercycles ) << "\t\t\t\t";
					outputFile << int( numbercycles ) << " ";
					break;
				}
         }

			// Infinite While Loops to Generate Valid Values.
		 while(1)
         {
				// Generate Number of Cycles and Check If Positive and Within Bound
			numbermemory = memory(generator);
            if ( numbermemory > 250.0 && numbermemory < 5000000.0 )
				{
					// Print Memory Footprint to Screen and Write to File.
					//cout << int( numbermemory ) << "\n";
					outputFile << int( numbermemory ) << "\n";
					break;
				}
         }
         add(i, numbercycles, numbermemory);//added this 
		}
	}
	outputFile.close();
	sort();//added this
	printP();//added this
	return 0;        
}

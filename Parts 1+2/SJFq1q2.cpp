#include <iostream>
#include <iomanip>
#include <random>
#include <climits>
#include <fstream>
#include <stdlib.h>
#include <vector>

using namespace std;

struct process {
	int id;
    int cycles;
    int memory;
    int waitTime;
};

struct processor {
	vector<process> procList;
	int totalCycleCount;
	int memoryAvailable;
};

vector<process> processList;
vector<processor> processorList;

void generateProcesses()
{
	double curProcCycle, curProcMemory;
	int numProcs = 100;
	int i = 0;
	ofstream outputFile;

	// Sets Up Normal Distribution.
	default_random_engine generator(time(0));
	normal_distribution<double> cycles(25000000.0, 10000000.0);
	normal_distribution<double> memory(5000000.0, 4000000.0);

	// Attempt to Open File
	outputFile.open("processes.txt", ios::trunc );
	if (	outputFile.is_open()	)
	{
		for(i = 0; i < numProcs; i++)
		{
			process currentProcess;
			currentProcess.id = (1000 + i);

	        while(1)
	        {
				// Generate Number of Cycles and Check If Positive
				curProcCycle = cycles(generator);
	            if (curProcCycle > 10.0 && curProcCycle < 50000000.0)
				{

					outputFile << int(curProcCycle) << " ";
					currentProcess.cycles = curProcCycle;
					break;
				}
	        }

			while(1)
	        {
				// Generate Number of Cycles and Check If Positive and Within Bound
				curProcMemory = memory(generator);
	            if (curProcMemory > 250.0 && curProcMemory < 5000000.0)
				{
					outputFile << int(curProcMemory) << "\n";
					currentProcess.memory = curProcMemory;
					break;
				}
	        }
	        processList.push_back(currentProcess);
		}
	}

	outputFile.close();
}

// Reads Process Data from Input File Passed as Argument
void readFile(char *fileName)
{
	int curProcCycle, curProcMemory;
	int i = 0;
	ifstream inputFile;
	
	// Attempt to Open File
	inputFile.open(fileName, ios::in );
	
	if (inputFile.is_open())
	{
		// Attempt to Read Data From File
		while ( inputFile >> curProcCycle >> curProcMemory )
		{
			process currentProcess;
			currentProcess.id = i;
			currentProcess.cycles = curProcCycle;
			currentProcess.memory = curProcMemory;
			processList.push_back(currentProcess);
			i++;
		}
		inputFile.close();
	}
}

//generates 5 processors with identical properties
//each processor has 8GB memory
void generateIdenticalProcessors()
{
	int numOfProcessors = 5;
	int cntr;

	for(cntr = 0; cntr < numOfProcessors; cntr++)
	{
		processor currentProcessor;
		currentProcessor.totalCycleCount = 0;
		currentProcessor.memoryAvailable = 8000000;
		processorList.push_back(currentProcessor);
	}
}

//generates 5 processors with different memory properties
//processor 1+2 have 2GB
//processor 3+4 have 4GB
//processor 5 has 8GB
void generateDiffMemProcessors()
{
	int cntr;

	for(cntr = 0; cntr < 2; cntr++)
	{
		processor currentProcessor;
		currentProcessor.totalCycleCount = 0;
		currentProcessor.memoryAvailable = 2000000;
		processorList.push_back(currentProcessor);
	}

	for(cntr = 0; cntr < 2; cntr++)
	{
		processor currentProcessor;
		currentProcessor.totalCycleCount = 0;
		currentProcessor.memoryAvailable = 4000000;
		processorList.push_back(currentProcessor);
	}

	for(cntr = 0; cntr < 1; cntr++)
	{
		processor currentProcessor;
		currentProcessor.totalCycleCount = 0;
		currentProcessor.memoryAvailable = 8000000;
		processorList.push_back(currentProcessor);
	}
}

//schedules processes to processors
//operates by SJF
void scheduleProcesses()
{
	//while ready queue isnt empty
	while(!processList.empty())
	{
		vector<process>::iterator processListIterator;
		vector<processor>::iterator processorListIterator;
		int processChosenIndex, processChosenCycle = 1000000000, processChosenMemory;
		int processorChosenIndex, processorChosenCycleCount = 1000000000;

		//find shortest job
		for(processListIterator = processList.begin(); processListIterator != processList.end(); ++processListIterator)
		{
			if((*processListIterator).cycles < processChosenCycle)
			{
				processChosenCycle = (*processListIterator).cycles;
				processChosenMemory = (*processListIterator).memory;
				processChosenIndex = (processListIterator - processList.begin());
			}
		}

		//find processor with least load by cycle count
		for(processorListIterator = processorList.begin(); processorListIterator != processorList.end(); ++processorListIterator)
		{
			if((*processorListIterator).totalCycleCount < processorChosenCycleCount && (*processorListIterator).memoryAvailable >= processChosenMemory)
			{
				processorChosenCycleCount = (*processorListIterator).totalCycleCount;
				processorChosenIndex = (processorListIterator - processorList.begin());
			}
		}

		//add job to processor queue
		(processList.at(processChosenIndex)).waitTime = (processorList.at(processorChosenIndex)).totalCycleCount;
		(processorList.at(processorChosenIndex).procList).push_back(processList.at(processChosenIndex));
		(processorList.at(processorChosenIndex).totalCycleCount) += (processList.at(processChosenIndex)).cycles;


		//remove job from ready queue
		processList.erase(processList.begin()+processChosenIndex);
	}
}

//prints the process queues of each processor
void printProcessorQueues()
{
	vector<processor>::iterator processorListIterator;
	vector<process>::iterator processListIterator;
	unsigned long long int totalWaitTime = 0;	//used to hold total wait time

	cout << endl;

	//iterate through each processor
	for(processorListIterator = processorList.begin(); processorListIterator != processorList.end(); ++processorListIterator)
	{
		cout << "Current processor #: " << (processorListIterator - processorList.begin()) << endl << endl;
		cout << "Memory Available: " << (*processorListIterator).memoryAvailable << endl;
		cout << "Processor Total Cycle Count: " << (*processorListIterator).totalCycleCount << endl;
		cout << "Process List" << endl;
		cout << "ID               Cycles               Memory               Wait Time" << endl;

		//iterate through each processor process queue
		for(processListIterator = ((*processorListIterator).procList).begin(); processListIterator != ((*processorListIterator).procList).end(); ++processListIterator)
		{
			totalWaitTime += (*processListIterator).waitTime;	//add time to total wait time
			cout << setw(17) << setiosflags(ios::left) << (*processListIterator).id << setw(21) << (*processListIterator).cycles << setw(21) << (*processListIterator).memory << (*processListIterator).waitTime << endl;
		}

		cout << endl << endl;
	}

	cout << "Total Wait Time: " << totalWaitTime << endl;
	cout << "Average Wait Time: " << (totalWaitTime / 100) << endl;
}

//print menu of program
void printProcessorMenu()
{
	cout << "Processor Menu" << endl;
	cout << "1. Identical Processors" << endl;
	cout << "2. Different Memory Processors" << endl;
	cout << "What kind of processors would you like to use: ";
}

int main( int argc, char* argv[] )
{
	int choice;
	bool valid;

	if(argc == 1)
	{
		generateProcesses();
	}
	else if(argc == 2)
	{
		readFile(argv[1]);
	}
	else
	{
		cout << "Too Many Arguments!" << endl;
		return 0;
	}

	do
	{
		printProcessorMenu();

		if(!(std::cin >> choice))
		{
			std::cout << "PLEASE ENTER A VALID OPTION" << std::endl;
			valid = false;
		}
		
		else
		{
			if(choice > 0 && choice < 3)
				valid = true;
			
			else
			{
				std::cout << "PLEASE ENTER A VALID OPTION" << std::endl;
				valid = false;
			}

		}
		
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
	}while(!valid);

	if(choice == 1)
	{
		generateIdenticalProcessors();
	}

	else
	{
		generateDiffMemProcessors();
	}


	scheduleProcesses();
	printProcessorQueues();

	return 0;        
}

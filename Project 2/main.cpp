// CSCE 4600
// Project 2 - main.cpp
// Faris Hawamdeh

#include "buddy.h"

#include <iostream>
#include <iomanip>
#include <random>
#include <climits>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <sys/time.h>

using namespace std;

struct process {
	int id;
    int cycles;
    int memory;
    int finishTime;
    void *data;
};

vector<process> waitingList;
vector<process> runningList;

void generateProcesses()
{
   double curProcCycle, curProcMemory;
   int numProcs = 100;
   int i = 0;
   ofstream outputFile;

   // Sets Up Normal Distribution.
   default_random_engine generator(time(0));
   normal_distribution<double> cycles(5000.0, 2500.0);
   normal_distribution<double> memory(1000.0, 250.0);

   // Attempt to Open File
   outputFile.open("processes.txt", ios::trunc );
   if ( outputFile.is_open() )
   {
      for(i = 0; i < numProcs; i++)
      {
         process currentProcess;
         currentProcess.id = (1000 + i);
         outputFile << int(currentProcess.id) << " ";

         while(1)
         {
            // Generate Number of Cycles and Check If Positive
            curProcCycle = cycles(generator);
            if (curProcCycle > 100.0 && curProcCycle < 10000.0)
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
            if (curProcMemory > 250.0 && curProcMemory < 10000.0)
            {
               outputFile << int(curProcMemory) << "\n";
               currentProcess.memory = curProcMemory;
               break;
            }
         }

         waitingList.push_back(currentProcess);
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
         waitingList.push_back(currentProcess);
         i++;
      }

      inputFile.close();
   }
}

// Runs the Malloc Experiment
// Uses stdlib.h malloc()
void mallocExperiment()
{
   int cycleCount = 0;
   vector<process>::iterator processListIterator;

   processListIterator = waitingList.begin();

   (*processListIterator).data = malloc( (*processListIterator).memory );
   (*processListIterator).finishTime = cycleCount + (*processListIterator).cycles;

   cout << "Process " << processListIterator -> id << " allocated " << processListIterator -> memory << " bytes." << endl;

   runningList.push_back(waitingList.at(0));
   waitingList.erase(waitingList.begin());

   cycleCount++;

   // While The Waiting List Is Not Empty.
   while( !waitingList.empty() )
   {
      // Check If There Is A Process Entering The System.
      if( cycleCount % 50 == 0 )
      {
         processListIterator = waitingList.begin();

         (*processListIterator).data = malloc( (*processListIterator).memory );
         (*processListIterator).finishTime = cycleCount + (*processListIterator).cycles;

         cout << "Process " << processListIterator -> id << " allocated " << processListIterator -> memory << " bytes." << endl;

         runningList.push_back(waitingList.at(0));
         waitingList.erase(waitingList.begin());
      }

      // Look Through Running List To Find a Process That Has Completed.
      for( processListIterator = runningList.begin(); processListIterator != runningList.end(); processListIterator++ )
      {
         if( (*processListIterator).finishTime < cycleCount )
         {
            cout << "Process " << processListIterator -> id << " freed " << processListIterator -> memory << " bytes." << endl;
            free( (*processListIterator).data );
            runningList.erase( runningList.begin() + (processListIterator - runningList.begin()) );
         }
      }

      cycleCount++;
   }

   while(!runningList.empty())
   {
      // Look Through Running List To Find a Process That Has Completed.
      for( processListIterator = runningList.begin(); processListIterator != runningList.end(); processListIterator++ )
      {
         if((*processListIterator).finishTime < cycleCount)
         {
            cout << "Process " << processListIterator -> id << " freed " << processListIterator -> memory << " bytes." << endl;
            free( (*processListIterator).data );

            runningList.erase( runningList.begin() + (processListIterator - runningList.begin()) );
            runningList.shrink_to_fit();
				break;
         }
      }

      cycleCount++;
   }
}

// Runs the Buddy System Experiment
// Uses myMalloc()
void buddyExperiment()
{
   InitializeBuddyMemoryManager(10240);

   int cycleCount = 0;
   vector<process>::iterator processListIterator;

   processListIterator = waitingList.begin();

   (*processListIterator).data = malloc( (*processListIterator).memory );
   (*processListIterator).finishTime = cycleCount + (*processListIterator).cycles;

   cout << "Process " << processListIterator -> id << " allocated " << processListIterator -> memory << " bytes." << endl;

   runningList.push_back(waitingList.at(0));
   waitingList.erase(waitingList.begin());

   cycleCount++;

   // While The Waiting List Is Not Empty.
   while( !waitingList.empty() )
   {
      // Check If There Is A Process Entering The System.
      if( cycleCount % 50 == 0 )
      {
         processListIterator = waitingList.begin();

         (*processListIterator).data = myMalloc( (*processListIterator).memory );

         if( (*processListIterator).data != NULL )
         {
            (*processListIterator).finishTime = cycleCount + (*processListIterator).cycles;
            cout << "Process " << processListIterator -> id << " allocated " << processListIterator -> memory << " bytes." << endl;

            runningList.push_back(waitingList.front());
            waitingList.erase(waitingList.begin());
         }
      }

      // Look Through Running List To Find a Process That Has Completed.
      for( processListIterator = runningList.begin(); processListIterator != runningList.end(); processListIterator++ )
      {
         if( (*processListIterator).finishTime < cycleCount )
         {
            cout << "Process " << processListIterator -> id << " freed " << processListIterator -> memory << " bytes." << endl;
            myFree( (*processListIterator).data, (*processListIterator).memory );

            runningList.erase(processListIterator);
            runningList.shrink_to_fit();
				break;
         }
      }

      cycleCount++;
   }

   while(runningList.size())
   {
      // Look Through Running List To Find a Process That Has Completed.
      for( processListIterator = runningList.begin(); processListIterator != runningList.end(); processListIterator++ )
      {
         if((*processListIterator).finishTime < cycleCount)
         {
            cout << "Process " << processListIterator -> id << " freed " << processListIterator -> memory << " bytes." << endl;
            myFree( (*processListIterator).data, (*processListIterator).memory );

            runningList.erase(processListIterator);
            runningList.shrink_to_fit();
				break;
         }
      }

      cycleCount++;
   }

   CleanupBuddyMemoryManager();
   return;
}

//print menu of program
void printProgramMenu()
{
   cout << "Program Menu" << endl;
   cout << "1. Malloc" << endl;
   cout << "2. MyMalloc" << endl;
   cout << "Which experiment would you like to run?" << endl;
}

int main( int argc, char* argv[] )
{
   int choice;
   bool valid;
   struct timeval start, end;

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
      printProgramMenu();

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
      gettimeofday(&start, NULL);
      mallocExperiment();
      gettimeofday(&end, NULL);

      cout << endl << "Total Processing time: " << ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) << endl;
   }
   else
   {
      gettimeofday(&start, NULL);
      buddyExperiment();
      gettimeofday(&end, NULL);

      cout << endl << "Total Processing time: " << ((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) << endl;
   }


   return 0;
}

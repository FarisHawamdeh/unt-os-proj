// The data structure I used is vector<vector<type>* > table;
// I assume the processes come 1 second at a time 
// The way I chose to distribute each process (after the first 5) is by calculating the total burst time each processor
// has and giving the new job to the least busy processor. 
// I have not seen a cycle being generated lower than 2000
#include <iostream>
#include <vector>
#include <algorithm>

#define PROCESSORS 5
#define PROCESSOR_SPEED 2000 //2ghz = 2000* 10^6
using namespace std;

struct processBlock{
	int id;
	double cycles;
	int memory;
	double avgcycles;
	int arrivaltime;
	double bursttime;
	double finishtime; //cycles/rate
	double turnaround;
};

struct sort_cycles{//used for std::sort(), to by sort shortest burst time
	inline bool operator() (const processBlock &struct1, const processBlock &struct2){
		return (struct1.cycles < struct2.cycles);
	}
};

void add(int id, double cycles, int memory);
void initialize();
void printP();
void sort();
vector<vector<processBlock>* > Processor; //vector of vector pointers type struct

void initialize(){//creates vector[i][0]
	for(int i = 0; i<PROCESSORS; i++)
		Processor.push_back(new vector <processBlock>);
}
void add(int id, double cycles, int memory){//used to fill the structure with data
	int lowPosition=0;
	double low,avgcycl;
	processBlock next_process;
	
	next_process.id = id;
	next_process.cycles = cycles;
	next_process.memory = memory;

	if( Processor.at(4)->empty()){ // first round just hands each process to each processor e.g. P1-p1,..,P5-p5
		next_process.avgcycles = cycles;
		next_process.arrivaltime = 0;
		next_process.bursttime = cycles/PROCESSOR_SPEED;
		next_process.finishtime = cycles/PROCESSOR_SPEED;
		next_process.turnaround = cycles/PROCESSOR_SPEED;
		Processor.at(id)->push_back(next_process);//add next_process to the end of vector[i]
	}

	else{// all subsequent processes are distributed here
		low=Processor.at(0)->at(Processor[lowPosition]->size()-1).avgcycles;
		for(int i=0; i<5; i++){// find the processor that has the lowest total cyclecount
			if( Processor.at(i)->at(Processor[i]->size()-1).avgcycles < low){
				low = Processor.at(i)->at(Processor[i]->size()-1).avgcycles;
				lowPosition = i;
			}
		}
		avgcycl = 0;
		for (int i=0; i < Processor[lowPosition]->size(); i++){
			avgcycl = avgcycl + Processor.at(lowPosition)->at(i).cycles;
		}
		avgcycl += cycles; //new cycle sum
		next_process.avgcycles = avgcycl;
		next_process.arrivaltime = id - Processor.at(lowPosition)->at(0).id;
		next_process.bursttime = cycles/PROCESSOR_SPEED;
		Processor.at(lowPosition)->push_back(next_process);//add next_process to the end of vector[i]
	}	
}
void printP(){//prints information about each processor and each process
	double avgTurnaround;
	int j;
	for(int i=0; i < PROCESSORS; i++){
		cout << "----Processor: "<< i+1 <<"----*all times in seconds*" << endl;
		cout << "Process ID\t"<<"Arrival Time\t"<<"Burst Time\t"<<"Finish Time\t"<<"Turnaround Time"<<endl;
		for(j=0; j< Processor.at(i)->size(); j++){
			cout << Processor.at(i)->at(j).id << "\t\t"<< Processor.at(i)->at(j).arrivaltime << 
			"\t\t" << int(Processor.at(i)->at(j).bursttime) << "\t\t" <<
			int(Processor.at(i)->at(j).finishtime) <<"\t\t"<< int(Processor.at(i)->at(j).turnaround) << endl;
			avgTurnaround += Processor.at(i)->at(j).turnaround;
		}
		avgTurnaround = avgTurnaround / Processor.at(i)->size();//find Average turnaround time
		cout << "Average Turnaround time: " << int(avgTurnaround)<<endl;
		cout << endl;
		avgTurnaround = 0;
	}
}
void sort(){//sorts the processes in each processor by shortest burst time
	for(int i=0; i<PROCESSORS; i++){
		std::sort(Processor.at(i)->begin()+1, Processor.at(i)->end(),sort_cycles());
		for (int j = 1; j < Processor.at(i)->size(); j++){
			Processor.at(i)->at(j).finishtime =  Processor.at(i)->at(j).bursttime + Processor.at(i)->at(j-1).finishtime;// calculate finish time
			Processor.at(i)->at(j).turnaround =  Processor.at(i)->at(j).finishtime + Processor.at(i)->at(j).arrivaltime;// calculate arrival time
		}
	}
}

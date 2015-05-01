// CSCE 4600
// Project 2 - buddy.h
// Faris Hawamdeh

#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>

// Used To Generate Powers Of Two
int powerOfTwo(int);

// Initialize The Memory To Be Used By The Buddy Memory Manager.
void * InitializeBuddyMemoryManager(unsigned int);

// Frees The Memory Used By The Buddy Memory Manager
void CleanupBuddyMemoryManager();

// The malloc() Equivalent For The Buddy Memory Manager
void * myMalloc(short);

// The free() Equivalent For The Buddy Memory Manager
bool myFree(void *, short);

// Checks To See If The Requested Memory Can Be Placed In The Location In The BuddyArray.
bool CanInsertAtLocation(int);

// Marks All Divided Parent Nodes.
void DivideUP (int);

// Log base 2 Function For Integers.
int Log2 (int);
// CSCE 4600
// Project 2 - buddy.cpp
// Faris Hawamdeh

#include "buddy.h"

#define MIN_BLOCK_SIZE 8

short * BuddyArray;
void * AllocatedSpace = NULL;

size_t BASize=0;
size_t AllocatedSpaceSize=0;

// Initialize The Memory Block To Be Used.
void * InitializeBuddyMemoryManager(unsigned int RequestedSize)
{

   BASize = powerOfTwo(Log2((int)RequestedSize)- Log2((int)MIN_BLOCK_SIZE)+1);
   AllocatedSpaceSize = RequestedSize;

   // Allocate The Amount of Memory In Bytes
   AllocatedSpace = malloc((size_t)AllocatedSpaceSize);

   // Allocate The Array For The Tree.
   // Max Nodes Should Be: RequestedSize / MIN_BLOCK_SIZE.
   BuddyArray=(short* ) malloc((size_t) sizeof(short)*(BASize));
  
   // If Allocation Failed Return NULL
   if (AllocatedSpace == NULL || BuddyArray == NULL )
   {
      BASize = 0;
      AllocatedSpaceSize = 0;
      return NULL;
   }

   // Initialize Memory Block to All 0s.
   for(unsigned int i=0; i<BASize; i++)
   {
      BuddyArray[i]=(short)0;
   }

   // Return Address of Memory Space.
   return AllocatedSpace;

}

// Frees Memory Allocated For Memory Manager;
void CleanupBuddyMemoryManager()
{
  
   BASize = 0;
   AllocatedSpaceSize = 0;

   free (AllocatedSpace);
   free (BuddyArray);
}

// The Function That Returns The Address Of The Memory Block
// Of The Size That Was Requested.
void * myMalloc(short MemRequest)
{
	int TreeLevel = 0;
	int BlockSizeNeeded = AllocatedSpaceSize;
   int BALoc;

   // Return NULL If Request Is Larger Than Total Memory
	if((size_t) MemRequest > AllocatedSpaceSize)
   {
      return NULL;
   }

   // If Request Less Than Minimum Block Size,
   // Force Minimum Block Size Rule.
   if (MemRequest < MIN_BLOCK_SIZE)
   {
      MemRequest = MIN_BLOCK_SIZE;
   }

   // Determine What Tree Level The Memory Request Will Need
	while(1)
   {
		if( (MemRequest <= BlockSizeNeeded) && (MemRequest > (BlockSizeNeeded/2)) )
      {
			break;
      }
      else
      {
			BlockSizeNeeded /= 2;
			TreeLevel++;
		}
	}

   // Check The Blocks On The Tree To Find One That Can Be Used.
	for( BALoc = powerOfTwo(TreeLevel)-1; BALoc <= (powerOfTwo(TreeLevel+1)-2); BALoc++ )
   {
		if(BuddyArray[BALoc] == 0 && CanInsertAtLocation(BALoc))
      {
			BuddyArray[BALoc]=(short)MemRequest;
			DivideUP(BALoc);
			break;
		}
   }

   // If No Free Block Available, Return Null.
	if(BALoc == powerOfTwo(TreeLevel+1)-1)
   {
      return NULL;
   }

   // Return the Address Of The Allocated Space With The Offset.
   return (void*) (reinterpret_cast<char*>(AllocatedSpace) + ( ( BALoc + 1 - powerOfTwo(TreeLevel)) * BlockSizeNeeded ) );
 

}

// The Function That Performs The Equivalent of Free() For The Buddy System
bool myFree(void * AddressPtr,short MemSize){

   int BlockSizeNeeded = AllocatedSpaceSize;
   int TreeLevel = 0;

   // Get The Offset From The Address Given By Using the AllocatedSpace Address.
   short offset = (short)( reinterpret_cast<char*>(AddressPtr) - reinterpret_cast<char*>(AllocatedSpace));

   if (offset < 0 )
   { 
      return false;
   }

   if (MemSize < MIN_BLOCK_SIZE) {MemSize = MIN_BLOCK_SIZE;}

   //Determine what treelevel the MemSize will land on 
	while(1)
   {
	   if( (MemSize<=BlockSizeNeeded) && ( MemSize>(BlockSizeNeeded/2)))
      {
			break;
      }
      else
      {
			BlockSizeNeeded/=2;
         TreeLevel++;
		}
	}

   int BALoc = (powerOfTwo(TreeLevel)) + (offset / BlockSizeNeeded) - 1;

	BuddyArray[BALoc] = 0;
	while(BALoc != 0)
	{
		if( BuddyArray[BALoc % 2 == 0 ? BALoc - 1: BALoc + 1] == 0 && BuddyArray[BALoc] == 0)
		{
			BuddyArray[BALoc % 2 == 0 ? (BALoc - 1) / 2 : BALoc / 2] = 0;
			BALoc = BALoc % 2 == 0 ? (BALoc - 1) / 2 : BALoc / 2;
		}
		else break;
	}

   return true;
}

// Check To See If The Requested Memory Can Be Placed In Location In The BuddyArray.
bool CanInsertAtLocation(int Location)
{
   while(Location != 0)
   {
      if(Location % 2 == 0)
      {
	      Location = (Location - 1) / 2;
      }
      else 
      {
         Location = Location / 2;
      }

      if(BuddyArray[Location] > 1)
      {
	      return false;
      }
	}

	return true;
}

//Marks All Divided Parent Nodes.
void DivideUP (int CurrentNode)
{

   // Start At Bottom and Move Up The Parent Nodes.
	while(CurrentNode != 0)
	{
      if ((CurrentNode % 2) == 0)
      {
         CurrentNode = (CurrentNode - 1) / 2;
      }
      else 
      {
         CurrentNode /= 2;
      }

		BuddyArray[CurrentNode] = 1;
	}
}

int powerOfTwo(int exp)
{
   return (1 << (exp));
}

int Log2 (int Value)
{
  //This code based on code from 
  //http://graphics.stanford.edu/~seander/bithacks.html#IntegerLog 

  const unsigned int RangeMask[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
  const unsigned int Shift[] = {1, 2, 4, 8, 16};
  register unsigned int ret = 0; // result of log2(v) will go here

  for (int i = 4; i >= 0; i--) // unroll for speed...
  {
     if (Value & RangeMask[i])
     {
        Value >>= Shift[i];
        ret |= Shift[i];
     } 
   }

   return ret;
}

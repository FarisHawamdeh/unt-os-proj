// CSCE 4600
// Project 2 - buddy.cpp
// Faris Hawamdeh, Dustin Fennessy, Eric Guzman

#include "buddy.h"

#define MIN_BLOCK_SIZE 8

short * BuddyArray;
void * AllocatedSpace = NULL;

size_t BASize = 0;
size_t AllocatedSpaceSize = 0;

// Initialize The Memory Block To Be Used.
void * initializeBuddy( unsigned int RequestedSize )
{
	// Calculate the Number of Nodes Needed for the Binary Tree
   BASize = powerOfTwo( log2( ( int ) RequestedSize ) - log2( ( int ) MIN_BLOCK_SIZE ) + 1 );
   AllocatedSpaceSize = RequestedSize;

   // Allocate The Amount of Memory In Bytes
   AllocatedSpace = malloc( (size_t) AllocatedSpaceSize );

   // Allocate The Array For The Tree.
   // Max Nodes Should Be: RequestedSize / MIN_BLOCK_SIZE.
   BuddyArray = ( short* ) malloc( ( size_t ) sizeof( short ) * ( BASize ) );
  
   // If Allocation Failed Return NULL
   if ( AllocatedSpace == NULL || BuddyArray == NULL )
   {
      BASize = 0;
      AllocatedSpaceSize = 0;
      return NULL;
   }

   // Initialize Memory Block to All 0s.
   for( unsigned int i = 0; i < BASize; i++ )
   {
      BuddyArray[ i ] = ( short ) 0;
   }

   // Return Address of Memory Block.
   return AllocatedSpace;
}

// Frees Memory Allocated For Memory Manager;
void freeBuddy()
{
  
   BASize = 0;
   AllocatedSpaceSize = 0;

   free ( AllocatedSpace );
   free ( BuddyArray );
}

// The Function That Returns The Address Of The Memory Block
// Of The Size That Was Requested.
void * myMalloc( short memorySize )
{
	int treeLevel = 0;
	int blockSizeNeeded = AllocatedSpaceSize;
   int arrayLocation;

   // Return NULL If Request Is Larger Than Total Memory
	if( ( size_t ) memorySize > AllocatedSpaceSize )
   {
      return NULL;
   }

   // If Request Less Than Minimum Block Size,
   // Force Minimum Block Size Rule.
   if ( memorySize < MIN_BLOCK_SIZE )
   {
      memorySize = MIN_BLOCK_SIZE;
   }

   // Determine What Tree Level The Memory Request Will Need
	while( 1 )
   {
		if( ( memorySize <= blockSizeNeeded ) && ( memorySize > ( blockSizeNeeded / 2 ) ) )
      {
			break;
      }
      else
      {
			blockSizeNeeded /= 2;
			treeLevel++;
		}
	}

   // Check The Blocks On The Tree To Find One That Can Be Used.
	for( arrayLocation = powerOfTwo( treeLevel ) - 1; arrayLocation <= ( powerOfTwo( treeLevel + 1 ) - 2 ); arrayLocation++ )
   {
		if( BuddyArray[ arrayLocation ] == 0 && canInsertAtLocation( arrayLocation ) )
      {
			BuddyArray[ arrayLocation ] = ( short ) memorySize;
			markDivided( arrayLocation );
			break;
		}
   }

   // If No Free Block Available, Return Null.
	if( arrayLocation == powerOfTwo( treeLevel + 1 ) - 1 )
   {
      return NULL;
   }

   // Return the Address Of The Allocated Space With The Offset.
   return ( void * ) ( reinterpret_cast<char*>( AllocatedSpace ) + ( ( arrayLocation + 1 - powerOfTwo(treeLevel) ) * blockSizeNeeded ) );
}

// The Function That Performs The Equivalent of Free() For The Buddy System
bool myFree( void * addressPtr, short memorySize )
{
   int blockSizeNeeded = AllocatedSpaceSize;
   int treeLevel = 0;

   // Get The Offset From The Address Given By Using the AllocatedSpace Address.
   short offset = ( short )( reinterpret_cast<char*>( addressPtr ) - reinterpret_cast<char*>( AllocatedSpace ) );

	// If Offset Is Negative Return false as Error
   if ( offset < 0 )
   { 
      return false;
   }

   // If Request Less Than Minimum Block Size,
   // Force Minimum Block Size Rule.
   if ( memorySize < MIN_BLOCK_SIZE )
	{
		memorySize = MIN_BLOCK_SIZE;
	}

   //Determine what tree level the memory block is on.
	while( 1 )
   {
	   if( ( memorySize <= blockSizeNeeded ) && ( memorySize > ( blockSizeNeeded / 2 ) ) )
      {
			break;
      }
      else
      {
			blockSizeNeeded /= 2;
         treeLevel++;
		}
	}

	// Calculate which node the memory block is assigned.
   int arrayLocation = ( powerOfTwo( treeLevel ) ) + ( offset / blockSizeNeeded ) - 1;

	// Mark Block as Unused.
	BuddyArray[ arrayLocation ] = 0;

	// Merge Nodes
	markMerged( arrayLocation );

   return true;
}

// Check To See If The Requested Memory Can Be Placed In the Location In The BuddyArray.
bool canInsertAtLocation( int currentNode )
{
   // Start At Bottom and Move Up The Parent Nodes.
   while( currentNode != 0 )
   {
		// Move Up One Level
      if( currentNode % 2 == 0 )
      {
	      currentNode = ( currentNode - 1 ) / 2;
      }
      else 
      {
         currentNode = currentNode / 2;
      }

		// Check If Block Is Used
      if( BuddyArray[ currentNode ] > 1 )
      {
	      return false;
      }
	}

	return true;
}

// Merges Nodes back together if unused.
void markMerged( int currentNode )
{
	// Start At currentNode and Move Up The Parent Nodes.
	while( currentNode != 0 )
	{
		// Check If Left or Right Node
		if( currentNode % 2 == 0 )
		{
			// Check If Left and Right Unused
			if( BuddyArray[ currentNode - 1 ] == 0 && BuddyArray[ currentNode ] == 0 )
			{
				// Mark Parent As Merged, ie UnDivided
				BuddyArray[ ( currentNode - 1 ) / 2 ] = 0;

				// Move Up One Level
				currentNode = ( currentNode - 1 ) / 2;
			}
			else 
			{
				break;
			}
		}
		else
		{
			// Check If Left and Right Unused
			if( BuddyArray[ currentNode + 1 ] == 0 && BuddyArray[ currentNode ] == 0 )
			{
				// Mark Parent As Merged, ie UnDivided
				BuddyArray[ currentNode / 2 ] = 0;

				// Move Up One Level
				currentNode = currentNode / 2;
			}
			else 
			{
				break;
			}
		}
	}
}

// Marks All Parent Nodes As Divided.
void markDivided( int currentNode )
{
   // Start At currentNode and Move Up The Parent Nodes.
	while( currentNode != 0 )
	{
		// Move Up One Level.
      if ( ( currentNode % 2 ) == 0 )
      {
         currentNode = ( currentNode - 1 ) / 2;
      }
      else 
      {
         currentNode /= 2;
      }

		// Mark As Divided.
		BuddyArray[ currentNode ] = 1;
	}
}

int powerOfTwo( int exp )
{
   return ( 1 << ( exp ) );
}

int log2 (int Value)
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

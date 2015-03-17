Compile the ProcessGenerator with -std=c++0x on the cse machines.
The G++ compiler on the cse machines will throw an error when trying to compile.
Use the flag to override the warning and compile.
It works file on the cse machines when compiled with the flag.

The arguments for ProcessGenerateor are ./exe *Number of Processes* *Output File Name*
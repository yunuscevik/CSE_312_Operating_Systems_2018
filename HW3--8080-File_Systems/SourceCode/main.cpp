#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "memory.h"



// This is just a sample main function, you should rewrite this file to handle problems 
// with new multitasking and virtual memory additions.
int main (int argc, char**argv)
{
	if (argc != 4){
            std::cerr << "Usage: prog exeFile datFile debugOption\n";
            exit(1); 
	}
	int DEBUG = atoi(argv[3]);
    
	memory mem;
	CPU8080 theCPU(&mem);
	GTUOS	theOS(argv[2]);
	uint64_t totalCycle = 0;
        int cycle = 0;

             
	theCPU.ReadFileIntoMemoryAt(argv[1], 0x0000);	
        
	do	
	{
            
            cycle += theCPU.Emulate8080p(DEBUG);
            totalCycle += cycle;
            if(theCPU.isSystemCall()){
                cycle += theOS.handleCall(theCPU, totalCycle);
                totalCycle += cycle;
                if(DEBUG == 1)
                    theOS.Debug1Print();
            }
            theOS.robin(cycle,theCPU);
            
            if(DEBUG == 2)
                std::cin.get();
            else if(DEBUG == 3)
                theOS.Debug3_Print();
            
        }while (!theCPU.isHalted());
        std::cout<< "\n\nThere are "<< theOS.getFileTable().size() << " files in the " << argv[2] << " file." << std::endl;
        theOS.endOfSimulation();
        std::cout << "Total value of Cycle: " << (int)totalCycle << std::endl;
        theOS.writeFileSystemDat(argv[2]);
        theOS.WriteToFileFromMemory(theCPU);
        std::cout << "---------------------------------------------------------" << std::endl;
        
        
        
	return 0;
}


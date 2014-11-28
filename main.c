// Tether Control of Uav
// Ground Station
//
// BS.c. Project 
// Danish Technical University
// Peter Savnik
// 2014-2015

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <phidget21.h>

#include "global.h"
#include "phidget.h"
#include "display.h"


int main(int argc, char** argv)
{
    printf("Ground station started\n");
    stop = 0;
    

    pthread_t *threadData; 
    pthread_t *threadDisplay;
    
    char key;
    dataRate = 100; // [ms] [8:1000]
    dataFilter = 0; // see updateAngle
    
    
    int threadData_check = pthread_create(&threadData, NULL, GetAngle, NULL);
    if(threadData_check != 0) puts("ERROR: pthread Data (getAngle) failed");
    
    int threadDisplay_check = pthread_create(&threadDisplay, NULL, display, NULL);
    if(threadDisplay_check != 0) puts("ERROR: pthread Display (terminal interface) failed");
    
    while(stop == 0){
      key = '1';
      
      if(key == 'c'){ 
	stop == 1;
      
      }
    }
    
    stop = 1;
    
    sleep(1); // Magic number ensures phreads has time to stop
    pthread_join(&threadData, NULL);
    pthread_join(&threadDisplay, NULL);
    puts("INFO: pthread finished");
    
  
    return 0;
}

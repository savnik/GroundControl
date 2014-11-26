#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <ncurses.h>
#include <menu.h>

#include "global.h"


void init_display(void){
  
  initscr();

  //printf("\033[2J\033[1;1H");
  printw(" ______________________________________________________________________________\n");
  printw("|                                                                              |\n");
  printw("|    Ground Control                                                            |\n");
  printw("|    Menu: (c) Calibration                                                     |\n");
  printw("|______________________________________________________________________________|\n");
  printw("|                                                                              |\n");
  printw("|    ForceX:                                                                   |\n");
  printw("|    ForceY:                                                                   |\n");
  printw("|    Total force:                                                              |\n");
  printw("|______________________________________________________________________________|\n");
  printw("|                                                                              |\n");
  printw("|   <Backspace> : Back to Menu                                                 |\n");
  printw("|   <r>         : Reset Statistics                                             |\n");
  printw("|   <Esc> / <s> : Stop Program                                                 |\n");
  printw("|______________________________________________________________________________|\n");

  refresh();
  
  
}


void update_display(void){
    time_t current_time;
    char* c_time_string;
    current_time = time(NULL);
    c_time_string = ctime(&current_time);
  
    mvprintw(2,40,"%s",c_time_string);
    mvprintw(6,27,"%lf", forceX);
    mvprintw(7,27,"%lf", forceY);
    mvprintw(8,27,"%lf", forceX+forceY);
    //mvprintw("\033[16;1H");
    
    refresh();
}

void display(void){
   init_display();
 
 
  while(stop == 0){
    update_display(); 
    sleep(1); 
   }
   
   endwin();
  
}
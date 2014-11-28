#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#include <ncurses.h>
#include <menu.h>

#include "global.h"
//#include "phidget.h"

int kbhit(void)
{
    timeout(1);
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

void init_display(void){
  
  initscr();

  //printf("\033[2J\033[1;1H");
  printw(" ______________________________________________________________________________\n");
  printw("|                                                                              |\n");
  printw("|    Ground Control                                                            |\n");
  printw("|    Menu: (c) Calibration                                                     |\n");
  printw("|______________________________________________________________________________|\n");
  printw("|                                                                              |\n");
  printw("|    Phidget:                                                                  |\n");
  printw("|    UAV:                                                                      |\n");
  printw("|______________________________________________________________________________|\n");
  printw("|                                                                              |\n");
  printw("|    ForceX:                                                                   |\n");
  printw("|    ForceY:                                                                   |\n");
  printw("|    Total force:                                                              |\n");
  printw("|    Angle:                                                                    |\n");
  printw("|______________________________________________________________________________|\n");
  printw("|                                                                              |\n");
  printw("|   <Backspace> : Back to Menu                                                 |\n");
  printw("|   <r>         : Reset Statistics                                             |\n");
  printw("|   <Esc> / <s> : Stop Program                                                 |\n");
  printw("|______________________________________________________________________________|\n");

  mvprintw(23,10,"");
  refresh();
  
  
}

void display_calibration(void){
  
  double noLoadX,noLoadY, loadX, loadY, load;
  
  load = 0.5; // calibration load
  
  // clear screen
  clear();
  
    // No load
  mvprintw(0,0," ______________________________________________________________________________\n");
  printw("|                                                                              |\n");
  printw("|    Ground Control                                                            |\n");
  printw("|    CALIBRATION                                                               |\n");
  printw("|______________________________________________________________________________|\n");
  printw("|                                                                              |\n");
  printw("|    ForceX:                                                                   |\n");
  printw("|    ForceY:                                                                   |\n");
  printw("|    Total force:                                                              |\n");
  printw("|______________________________________________________________________________|\n");
  printw("|                                                                              |\n");
  printw("|    1. No load                                                                |\n");
  printw("|       Remove all load from the cable and winch the cable all in.             |\n");
  printw("|       Hit any key when ready                                                 |\n");
  printw("|______________________________________________________________________________|\n");
  
  mvprintw(6,27,"%lf", forceX);
  mvprintw(7,27,"%lf", forceY);
  mvprintw(8,27,"%lf", forceX+forceY);

  refresh();

  while(!kbhit()){
    mvprintw(6,27,"%lf", forceX);
    mvprintw(7,27,"%lf", forceY);
    mvprintw(8,27,"%lf", forceX+forceY);
    refresh();
    
  }
  // clear buffer
  getch();
  
 noLoadX = loadcellX.input;
 noLoadY = loadcellY.input;

   // Known load X
  
  mvprintw(11,0,"|    2. X load                                                                 |\n");
  printw("|       Put 0.5kg load on X-loadcell in positiv direction.                     |\n");
  printw("|       Hit any key when ready                                                 |\n");
  printw("|______________________________________________________________________________|\n");
  
  refresh();
 
  while(!kbhit()){
    mvprintw(6,27,"%lf", forceX);
    mvprintw(7,27,"%lf", forceY);
    mvprintw(8,27,"%lf", forceX+forceY);
    refresh();
    
  }
  // clear buffer
  getch();
  
  // update X gain
  loadX = loadcellX.input;
  
  
  // Known load Y
  mvprintw(11,0,"|    3. Y load                                                                 |\n");
  printw("|       Put 0.5kg load on Y-loadcell in positiv direction.                     |\n");
  printw("|       Hit any key when ready                                                 |\n");
  printw("|______________________________________________________________________________|\n");
  
  refresh();
 
  while(!kbhit()){
    mvprintw(6,27,"%lf", forceX);
    mvprintw(7,27,"%lf", forceY);
    mvprintw(8,27,"%lf", forceX+forceY);
    refresh();
    
  }
  
  // clear buffer
  getch();
  
  // update gain Y
  loadY = loadcellY.input;
  
  // clear screen
  clear();
  
  // return
  
  Calibration(noLoadX, noLoadY, loadX, loadY, load);
  
}


void update_display(void){
    time_t current_time;
    char* c_time_string;
    current_time = time(NULL);
    c_time_string = ctime(&current_time);
  
    mvprintw(2,40,"%s",c_time_string);
    mvprintw(6,27,"%s", MsgPhidgets);
    mvprintw(10,27,"%lf\t%lf", loadcellX.input, loadcellX.force);
    mvprintw(11,27,"%lf\t%lf", loadcellY.input, loadcellY.force);
    mvprintw(12,27,"%lf", loadcellX.input+loadcellY.input);
    //mvprintw("\033[16;1H");
   
    
    refresh();
}

void display(void){
   init_display();
   char cmd;
 
 
  while(stop == 0){
    
    if(kbhit()){
      cmd = getch();
      
      // Calibration
      if(cmd == 'c'){
	display_calibration();
	init_display();
      }
    } // end kbhit()
    
    
    update_display(); 
    sleep(1); 
  }
   endwin();
  
}
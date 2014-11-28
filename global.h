// Global Vars

typedef struct{
  double K, offset;
  double input; // Sensor value
  double force; 
  
} loadcell;

int stop;

double forceX, forceY;


loadcell loadcellX;
loadcell loadcellY;

int dataRate, dataFilter;


// Phidgets
char MsgPhidgets[256];
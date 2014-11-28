


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <phidget21.h>
#include "global.h"
#include "phidget.h"






int CCONV AttachHandler(CPhidgetHandle phid, void *userptr)
{
	CPhidgetBridgeHandle bridge = (CPhidgetBridgeHandle)phid;

	CPhidgetBridge_setEnabled(bridge, 0, PTRUE);
	CPhidgetBridge_setEnabled(bridge, 1, PTRUE);
	CPhidgetBridge_setEnabled(bridge, 2, PFALSE);
	CPhidgetBridge_setEnabled(bridge, 3, PFALSE);

	CPhidgetBridge_setGain(bridge, 0, PHIDGET_BRIDGE_GAIN_128);
	CPhidgetBridge_setGain(bridge, 1, PHIDGET_BRIDGE_GAIN_16);
	CPhidgetBridge_setGain(bridge, 2, PHIDGET_BRIDGE_GAIN_32);
	CPhidgetBridge_setGain(bridge, 3, PHIDGET_BRIDGE_GAIN_64);
	CPhidgetBridge_setDataRate(bridge, 1000);

	strcpy(MsgPhidgets, "Attach handler ran!"); // set status MSG
	return 0;
}

int CCONV DetachHandler(CPhidgetHandle phid, void *userptr)
{
	//MsgPhidgets = "Detach handler ran!\n";
	return 0;
}

int CCONV ErrorHandler(CPhidgetHandle phid, void *userptr, int ErrorCode, const char *errorStr)
{
	//printf("Error event: %s\n",errorStr);
	strcpy(MsgPhidgets, "Error event Phidget"); // Set status MSG
	return 0;
}



void display_generic_properties(CPhidgetHandle phid)
{
	int sernum, version;
	const char *deviceptr;
	CPhidget_getDeviceType(phid, &deviceptr);
	CPhidget_getSerialNumber(phid, &sernum);
	CPhidget_getDeviceVersion(phid, &version);

	//printf("%s\n", deviceptr);
	//printf("Version: %8d SerialNumber: %10d\n", version, sernum);
	return;
}

int CCONV UpdateAngle(CPhidgetBridgeHandle phid, void *userPtr, int index, double val)
{
	CPhidgetBridgeHandle bridge = (CPhidgetBridgeHandle)phid;
	double f, ms;
	int i;

	// Force X
	if(index == 0){
	  // No filter
	  if(dataFilter == 0){
	    forceX = val;
	  }
	  // Lowpass moving avarage filter
	  else if(dataFilter == 1){
	    forceX = (val+forceX)/2;
	  }
	  // Lowpass complementary filter
	  else if(dataFilter == 2){
	    forceX = val*0.1+forceX*0.9;
	  }
	}
	//Force Y
	if(index == 1){
	  // No filter
	  if(dataFilter == 0){
	    forceY = val;
	  }
	  // Lowpass moving avarage filter
	  else if(dataFilter == 1){
	    forceY = (val+forceY)/2;
	  }
	  // Lowpass complementary filter
	  else if(dataFilter == 2){
	    forceY = val*0.1+forceY*0.9;
	  }
	  
	}
	
	// store to struct
	loadcellX.input = forceX;
	loadcellY.input = forceY;
	loadcellX.force = loadcellX.K*(forceX-loadcellX.offset);
	loadcellY.force = loadcellY.K*(forceY-loadcellY.offset);
	
	//printf("Data Event ForceX: %lf\t ForceY: %lf\t Total force: %lf\n",forceX,forceY,forceX+forceY);
	
	/*if(val < 0)
		printf("Data Event (%d) - -0x%06x\n",index,(int)-val);
	else
		printf("Data Event (%d) - +0x%06x\n",index,(int)val);*/

	return 0;
}



void GetAngle(void)
{
    const char *err;	
	int result;
	CPhidgetBridgeHandle bridge;
	//CPhidget_enableLogging(PHIDGET_LOG_VERBOSE, NULL);

	//printf("Creating messurement thread\n");
	strcpy(MsgPhidgets, "Creating messurement thread"); // set status MSG
	
	// Initial condition
	forceX = 0;
	forceY = 0;
	loadcellX.offset = -0.05;
	loadcellX.K = 1;
	loadcellY.offset = -0.05;
	loadcellY.K = 1;
	
	
	
	CPhidgetBridge_create(&bridge);

	CPhidget_set_OnAttach_Handler((CPhidgetHandle)bridge, AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)bridge, DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)bridge, ErrorHandler, NULL);

	CPhidgetBridge_set_OnBridgeData_Handler(bridge, UpdateAngle, NULL);

	CPhidget_open((CPhidgetHandle)bridge, -1);

	//Wait for 10 seconds, otherwise exit
	if(result = CPhidget_waitForAttachment((CPhidgetHandle)bridge, dataRate))
	{

		CPhidget_getErrorDescription(result, &err);
		//printf("Problem waiting for attachment: %s\n", err);
		strcpy(MsgPhidgets, "Problem waiting for attachment"); // Set status MSG
		return;
	}

	// display_generic_properties((CPhidgetHandle)bridge);

	//Wait for stop
	while(stop == 0){
	    // wait for stop signal
	}

	//printf("Closing...\n");
	strcpy(MsgPhidgets,"Closing..."); // Set status MSG
	
	CPhidget_close((CPhidgetHandle)bridge);
	CPhidget_delete((CPhidgetHandle)bridge);

	return;
}


void Calibration(double noLoadX, double noLoadY, double loadX, double loadY, double load){
  
  // Record the output of the load cell at rest on a flat surface with no force on it. The mv/V output measured by the PhidgetBridge is the offset.
  loadcellX.offset = noLoadX;
  loadcellY.offset = noLoadY;
  
  // Once you’ve found the offset, measure something with a known weight and solve the equation for K. You can also calibrate the load cell at multiple known weights and use these points to model a linear function. 
  // Expected Force or Weight = K * (Measured mV/V - Offset)
  
  loadcellX.K = load/(loadX-loadcellX.offset);
  loadcellY.K = load/(loadY-loadcellY.offset);
  
  
}


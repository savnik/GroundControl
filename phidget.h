
#define FREQS_SIZE 20

int CCONV DetachHandler(CPhidgetHandle phid, void *userptr);
int CCONV ErrorHandler(CPhidgetHandle phid, void *userptr, int ErrorCode, const char *errorStr);
void display_generic_properties(CPhidgetHandle phid);
int CCONV UpdateAngle(CPhidgetBridgeHandle phid, void *userPtr, int index, double val);
void getAngle(void);

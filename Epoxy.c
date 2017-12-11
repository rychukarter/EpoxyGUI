#include <utility.h>
#include <rs232.h>
#include <cvirte.h>		
#include <userint.h>
#include <stdio.h>
#include "Epoxy.h"
#include <string.h>
#include <ctype.h>

static int panelHandle, ctrlarray;
static int openedComPort = 0;
static char gEventChar = 'T';
int frequency, timeBetween, channelNumber, measurementNumber, calibResistance, calibChannel = 0;
float calibPhase = 0;
void ComPortReciveCallback(int portNumber, int eventMask, void *callbackdata);
static int openedComPortStatus = 0;
float values_m[16] = {0};
float values_p[16] = {0};


FILE *outFile; // declaring a FILE type pointer to handle the file


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Epoxy.uir", PANEL)) < 0)
		return -1;
	ctrlarray = GetCtrlArrayFromResourceID (panelHandle, CTRLARRAY);
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK connectToPort (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int selectedComPort = 0;
	int comPortError = 0;
	switch (event)
	{
		case EVENT_COMMIT:

			GetCtrlVal (PANEL, PANEL_COMPORTLIST, &selectedComPort);
			comPortError = OpenComConfig (selectedComPort, "", 256000, 0, 8, 1, 512, 512);
			if(comPortError >= 0){
				SetCtrlVal (PANEL, PANEL_MESSAGELOG, "COM Port Opened\r\n");
				openedComPort = selectedComPort;
				openedComPortStatus = 1;
				InstallComCallback (openedComPort, (LWRS_RECEIVE | LWRS_ERR), 13, (int)gEventChar, ComPortReciveCallback, NULL);
				SetCtrlAttribute (PANEL, PANEL_CONNECTBUTTON, ATTR_DIMMED, 1);
				SetCtrlAttribute (PANEL, PANEL_DISCONNECTBUTTON, ATTR_DIMMED, 0);
				SetCtrlAttribute (PANEL, PANEL_CALIBBUTTON, ATTR_DIMMED, 0);
			}
			else{
				SetCtrlVal (PANEL, PANEL_MESSAGELOG, "COM Port not Opened\r\n");
				CloseCom (selectedComPort);
			}
			break;
	}
	return 0;
}

int CVICALLBACK disconnectFromPort (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	
	int comPortError = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			comPortError = CloseCom (openedComPort);
			if(comPortError >= 0){
				SetCtrlVal (PANEL, PANEL_MESSAGELOG, "COM Port Closed\r\n");
				SetCtrlAttribute (PANEL, PANEL_CONNECTBUTTON, ATTR_DIMMED, 0);
				SetCtrlAttribute (PANEL, PANEL_DISCONNECTBUTTON, ATTR_DIMMED, 1);
				SetCtrlAttribute (PANEL, PANEL_STARTBUTTON, ATTR_DIMMED, 1);
				SetCtrlAttribute (PANEL, PANEL_STOPBUTTON, ATTR_DIMMED, 1);
				SetCtrlAttribute (PANEL, PANEL_CALIBBUTTON, ATTR_DIMMED, 1);
				openedComPortStatus = 0;
				
			}
			else{
				SetCtrlVal (PANEL, PANEL_MESSAGELOG, "COM Port not Closed\r\n");
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK quitApp (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}


int CVICALLBACK startMeasurment (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			char sendBuffer[40] = {0};
			
			int sendBufferLenght=0;
			if(openedComPortStatus>0){ 
				GetCtrlVal(PANEL,PANEL_FREQUENCY,&frequency);
				GetCtrlVal(PANEL,PANEL_MEASURMENTTIME,&timeBetween);
				GetCtrlVal(PANEL,PANEL_MEASURMENTNUMBER,&measurementNumber);
				GetCtrlVal(PANEL,PANEL_NUMBEROFCHANNELS,&channelNumber);	
					
				sprintf(sendBuffer, "F%dT%dM%dC%dX\n", frequency,timeBetween,measurementNumber,channelNumber);

				sendBufferLenght=strlen(sendBuffer);
				
				SetCtrlAttribute (PANEL, PANEL_STARTBUTTON, ATTR_DIMMED, 1);
				SetCtrlAttribute (PANEL, PANEL_STOPBUTTON, ATTR_DIMMED, 0);
				SetCtrlAttribute (PANEL, PANEL_CALIBBUTTON, ATTR_DIMMED, 1);
				SetCtrlAttribute (PANEL, PANEL_MODULUSSTRIP, ATTR_POINTS_PER_SCREEN, measurementNumber);
				SetCtrlAttribute (PANEL, PANEL_PHASESTRIP, ATTR_POINTS_PER_SCREEN, measurementNumber);
				SetCtrlAttribute (PANEL, PANEL_MODULUSSTRIP, ATTR_XAXIS_GAIN, (double)timeBetween);
				SetCtrlAttribute (PANEL, PANEL_PHASESTRIP, ATTR_XAXIS_GAIN, (double)timeBetween);
				
				ComWrt(openedComPort,sendBuffer,sendBufferLenght);
				
				//SetCtrlVal (PANEL, PANEL_MESSAGELOG, sendBuffer);
			}
			break;
	}
	return 0;
}

int CVICALLBACK stopMeasurment (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			char sendBuffer[40] = {0};
			
			int sendBufferLenght=0;
			if(openedComPortStatus>0){ 
	
				sprintf(sendBuffer, "SX\n");

				sendBufferLenght=strlen(sendBuffer);
				ComWrt(openedComPort,sendBuffer,sendBufferLenght);
			}
			SetCtrlAttribute (PANEL, PANEL_STARTBUTTON, ATTR_DIMMED, 0);
			SetCtrlAttribute (PANEL, PANEL_STOPBUTTON, ATTR_DIMMED, 1);
			SetCtrlAttribute (PANEL, PANEL_CALIBBUTTON, ATTR_DIMMED, 0);
			break;
	}
	return 0;
}

int CVICALLBACK calibrateMeasurment (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			char sendBuffer[40] = {0};
			
			int sendBufferLenght=0;
			if(openedComPortStatus>0){ 
				GetCtrlVal(PANEL,PANEL_CALIBCHANNEL,&calibChannel);
				GetCtrlVal(PANEL,PANEL_CALIBRESISTANCE,&calibResistance);
				GetCtrlVal(PANEL,PANEL_FREQUENCY,&frequency);
				GetCtrlVal(PANEL,PANEL_CALIBPHASE,&calibPhase); 
	
				sprintf(sendBuffer, "R%dA%0.2fK%dX\n", calibResistance,calibPhase,calibChannel);

				sendBufferLenght=strlen(sendBuffer);
				ComWrt(openedComPort,sendBuffer,sendBufferLenght);
				
				//SetCtrlVal (PANEL, PANEL_MESSAGELOG, sendBuffer);
				SetCtrlAttribute (PANEL, PANEL_STARTBUTTON, ATTR_DIMMED, 0);
			}
			break;
	}
	return 0;
}

void CVICALLBACK ComPortReciveCallback(int portNumber, int eventMask, void *callbackdata){
	char readBuf[256] = {0};
	int strLen;
	char temp[20]  = {0};
	int reciveErr = 0;
	int k =0;
	int channel = 0;
	
	switch (eventMask){
		case LWRS_RECEIVE:
			strLen = GetInQLen (portNumber);

			reciveErr = ComRd (portNumber, readBuf, strLen);
			if(readBuf[0] == 'M')
			{
				//SetCtrlVal (PANEL, PANEL_MESSAGELOG, readBuf);
				for(int i = 0; i<=strLen; i++){
					if(readBuf[i] == 'C')
					{
						for(int j = i+1; isdigit(readBuf[j]); j++)
						{
							temp[k] = readBuf[j];
							k++;
						}
						sscanf(temp,"%d",&channel);
						memset(&temp[0], 0, sizeof(temp));
						k=0;
					}
				}
				for(int i = 0; i<=strLen; i++)
				{
					if(readBuf[i] == 'M')
					{
						for(int j = i+1; isdigit(readBuf[j]) || readBuf[j] == '.'; j++){
							temp[k] = readBuf[j];
							k++;
						}
						sscanf(temp,"%f",&values_m[channel]);
						memset(&temp[0], 0, sizeof(temp));
						i=i+k;
						k=0;
					}
					if(readBuf[i] == 'P'){
						for(int j = i+1; isdigit(readBuf[j]) || readBuf[j] == '-' || readBuf[j] == '.'; j++){
							temp[k] = readBuf[j];
							k++;
						}
						sscanf(temp,"%f",&values_p[channel]);
						memset(&temp[0], 0, sizeof(temp));
						i=i+k;
						k=0;
					}								
				}
				if(channel == (channelNumber-1)){
					PlotStripChart (PANEL, PANEL_MODULUSSTRIP, values_m,16,0,0,VAL_FLOAT);
					PlotStripChart (PANEL, PANEL_PHASESTRIP, values_p,16,0,0,VAL_FLOAT);
					outFile=fopen("data.txt","a");
					for (int i=0;i<channelNumber;i++)
						fprintf(outFile,"%d\t%0.2f\t%0.2f\n",i,values_m[i],values_p[i]);
					fclose(outFile);
				}
			}
			else{
				SetCtrlVal (PANEL, PANEL_MESSAGELOG, readBuf);
				FlushInQ(portNumber);
			}
			break;
		}

	return;
}

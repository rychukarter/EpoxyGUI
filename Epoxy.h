/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_CONNECTBUTTON              2       /* control type: command, callback function: connectToPort */
#define  PANEL_DISCONNECTBUTTON           3       /* control type: command, callback function: disconnectFromPort */
#define  PANEL_QUITBUTTON                 4       /* control type: command, callback function: quitApp */
#define  PANEL_COMPORTLIST                5       /* control type: listBox, callback function: (none) */
#define  PANEL_MESSAGELOG                 6       /* control type: textBox, callback function: (none) */
#define  PANEL_MODULUSSTRIP               7       /* control type: strip, callback function: (none) */
#define  PANEL_STOPBUTTON                 8       /* control type: command, callback function: stopMeasurment */
#define  PANEL_CALIBBUTTON                9       /* control type: command, callback function: calibrateMeasurment */
#define  PANEL_STARTBUTTON                10      /* control type: command, callback function: startMeasurment */
#define  PANEL_PHASESTRIP                 11      /* control type: strip, callback function: (none) */
#define  PANEL_DECORATION_2               12      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION                 13      /* control type: deco, callback function: (none) */
#define  PANEL_SERIALMESSAGE              14      /* control type: textMsg, callback function: (none) */
#define  PANEL_MEASURMENTMESSAGE          15      /* control type: textMsg, callback function: (none) */
#define  PANEL_DECORATION_3               16      /* control type: deco, callback function: (none) */
#define  PANEL_MEASURMENTNUMBER           17      /* control type: numeric, callback function: (none) */
#define  PANEL_CALIBPHASE                 18      /* control type: numeric, callback function: (none) */
#define  PANEL_CALIBCHANNEL               19      /* control type: numeric, callback function: (none) */
#define  PANEL_CALIBRESISTANCE            20      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMBEROFCHANNELS           21      /* control type: numeric, callback function: (none) */
#define  PANEL_MEASURMENTTIME             22      /* control type: numeric, callback function: (none) */
#define  PANEL_CALIBMESSAGE               23      /* control type: textMsg, callback function: (none) */
#define  PANEL_FREQUENCY                  24      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK calibrateMeasurment(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK connectToPort(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK disconnectFromPort(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK quitApp(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK startMeasurment(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK stopMeasurment(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif

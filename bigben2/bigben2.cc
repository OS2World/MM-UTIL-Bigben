//////////////////////////////////////////////
// bigben2.cc -- bigben Version 2 (for MMPM)
//
// This plays the bigben tune every hour
//
//
// Marc E.E. van Woerkom, 13.12.93     [MvW]
//


#define OS2EMX_PLAIN_CHAR

#define INCL_DOSPROCESS   // DosBeep()
#define INCL_DOSDATETIME  // DosGetDateTime()
#define INCL_WIN
#define INCL_REXXSAA      // use REXX interpreter

#include <os2.h>


//
// call REXX procedure "dingdong.cmd"
//

void dingdong()
{
    LONG ReturnCode;

    char Resultbuf[80];
    RXSTRING Result;
    MAKERXSTRING(Result, Resultbuf, 80);  // initialize RXSTRING

    RexxStart(0,
              0,
              "dingdong",
              0,
              0,
              RXCOMMAND,
              0,
              (SHORT*)&ReturnCode,  // cast due to incorrect emx header
              &Result);
}


const ULONG ID_TIMER = 1;

HAB  hab;
HWND hwndClient;

int  first_sync;
int  sync;
int  min_event;
int  sec_event;


//
// PM client window procedure
//

extern "C"
MRESULT EXPENTRY ClientWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    switch (msg) {

    case WM_CREATE:         // tunes shamelessly cloned from Petzold :-)
        DosBeep(261, 100);
        DosBeep(330, 100);
        DosBeep(392, 100);
        DosBeep(523, 500);
        return 0;


    // Use minute ticks except at start and at hh.59.ss

    case WM_TIMER:
        DATETIME DateTime;                     // get the time
        DosGetDateTime(&DateTime);

        if (first_sync) {                      // get into sync with full minutes
            // DosBeep(5000, 50);

            if (DateTime.seconds == 0) {
                first_sync = 0;                // at hh.mm.00 -> first sync performed
                sec_event = 0;

                if (DateTime.minutes == 59) {  // at hh.59.00 -> 1s steps
                    sync = 1;
                    min_event = 59;
                }
                else { 
                    sync = 0;                  // slow down
                    WinStartTimer(hab,         // input anchor block
                                  hwndClient,  // input window handle
                                  ID_TIMER,    // timer id
                                  60000);      // intervall in milliseconds

                    if (DateTime.minutes == 0) // if started later than hh.59.00
                        dingdong();
                }
            }
        }
        else {
            if (sync) {                        // sync is on (at hh.59.00)
                // DosBeep(3000, 50);
                sec_event++;                   // count second steps

                if (sec_event >= 59) {         // ok, now slow down (at hh.59.59)
                    sync = 0;                  // sync off
                    WinStartTimer(hab,         // input anchor block
                                  hwndClient,  // input window handle
                                  ID_TIMER,    // timer id
                                  60000);      // intervall in milliseconds
                    dingdong();
                }
            }
            else {                             // sync is off (at hh.00.00 ... hh.58.00)
                // DosBeep(400, 100);
                min_event = DateTime.minutes;

                if (min_event >= 59) {         // accelerate to second steps
                    sync = 1;                  // sync again
                    sec_event = DateTime.seconds;

                    WinStartTimer(hab,         // input anchor block
                                  hwndClient,  // input window handle
                                  ID_TIMER,    // timer id
                                  1000);       // intervall in milliseconds
                }
            }
        }

        return 0;

    case WM_DESTROY:        // thanks again, Charles!
        DosBeep(523, 100);
        DosBeep(392, 100);
        DosBeep(330, 100);
        DosBeep(261, 500);
        return 0;
    }

    return WinDefWindowProc(hwnd, msg, mp1, mp2);  // default processing
}


//
// main function
//

main()
{
    hab = WinInitialize(0);                   // handle: anchor block

    HMQ hmq = WinCreateMsgQueue(hab, 0);      // handle: message queue


    // register ClientWndProc

    CHAR ClientClass[] = "bigben2";

    WinRegisterClass(hab,           
                     ClientClass,    // name of class being registered
                     ClientWndProc,  // window procedure for class
                     0,              // class style
                     0);             // extra memory to reserve
    

    // create parent window

    ULONG FrameFlags = FCF_TASKLIST;  // show up in tasklist

    HWND hwndFrame = WinCreateStdWindow(HWND_DESKTOP,  // parent
                                        0,             // style
                                        &FrameFlags,   // control data
                                        ClientClass,   // client name
                                        "Burp :-)",    // title bar text
                                        0,             // client style
                                        0,             // resource handle
                                        0,             // resource id
                                        &hwndClient);  // client pointer

    // quit if no timer is available

    if (WinQuerySysValue(HWND_DESKTOP, SV_CTIMERS) == 0) {

        WinMessageBox(HWND_DESKTOP,                                        // parent handle
                      hwndClient,                                          // owner handle
                      "No more timer available.\nCan't continue. Sorry.",  // message
                      "bigben2",                                           // title
                      0,
                      MB_OK |                                              // OK button
                      MB_ICONEXCLAMATION);                                 // "!" icon
    }
    else {

        // start timer

        first_sync = 1;  // start in 1s steps to synchronize to full minutes

        WinStartTimer(hab,         // input anchor block
                      hwndClient,  // input window handle
                      ID_TIMER,    // timer id
                      1000);       // intervall in milliseconds


        // it's not a trick, it's a message loop!

        QMSG qmsg;  // queue: message queue

        while(WinGetMsg(hab, &qmsg, 0, 0, 0))
            WinDispatchMsg(hab, &qmsg);


        // stop timer

        WinStopTimer(hab,         // input anchor block
                     hwndClient,  // input window handle
                     ID_TIMER);   // timer id
    }


    WinDestroyWindow(hwndFrame);  // kill window
    WinDestroyMsgQueue(hmq);      // kill message queue
    WinTerminate(hab);            // that's all!

    return 0;
}


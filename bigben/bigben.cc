/////////////////////////////////////////
//
// bigben.cc -- Marc van Woerkom, 12/92
//
// Version 1.0
//


extern "C" {

#define INCL_DOSPROCESS
#define INCL_DOSDATETIME

#include <os2.h>

}


#include <iostream.h>



char* HourStr[12] = { "twelve", "one",  "two", "three",
                      "four",   "five", "six", "seven",
                      "eight",  "nine", "ten", "eleven" };

char* AmPmStr[2]  = { "am", "pm" };



//
// SlowDown: Slow down to idle priority
//

void SlowDown()
{

    APIRET rc = DosSetPriority(PRTYS_PROCESSTREE,
                               PRTYC_IDLETIME,
                               PRTYD_MINIMUM,
                               0);

    if (rc) {
        cerr << "DosSetPriority Error: " << rc << "!\n";
    }
 
}


//
// GetHour: Get the current hour
//

UCHAR GetHour()
{
    DATETIME DateTime;

    APIRET rc = DosGetDateTime(&DateTime);

    if (rc) {
        cerr << "DosGetDateTime Error: " << rc << "!\n";
    }

    return DateTime.hours;
}


//
// HourName: Return the name for the hour
//

char* HourName(UCHAR hour)
{
    return HourStr[hour % 12];
}


//
// AmPm: Return if it's am or pm
//

char* AmPm(UCHAR hour)
{
   if (hour < 12) return AmPmStr[0];
   else return AmPmStr[1];
}


//
// DoTheBigBen: Start "player.exe" if hour changed
//

void DoTheBigBen()
{
    cout << "Yeah! Do the Big Ben, man! :-)\n";

    CHAR LoadError[100];  // contains error

    PSZ Args = PSZ("player\0bigben.voc\0");
    PSZ Envs = 0;

    RESULTCODES ReturnCodes;


    PSZ Pgm = PSZ("player.exe");

    APIRET rc = DosExecPgm(LoadError,
                           sizeof LoadError,
                           EXEC_BACKGROUND,
                           Args,
                           Envs,
                           &ReturnCodes,
                           Pgm);

    if (rc) {
        cerr << "Oh shit. DosExecPgm Error: " << rc << "!\n";
    }
}


//
// main: Here it all starts...
//

main()
{
    cout << "\nBigBen --Every hour: Sound made in England-- 12/92 [MvW]\n";

    SlowDown();

    int hour = GetHour();

    for (;;) {

        if (GetHour() != hour) {
            hour = GetHour();

            cout << "\nIt\'s "   << HourName(hour)
                 << " o\'clock " << AmPm(hour)
                 << " now.  ";

            DoTheBigBen();
        }
    }

    cout << "\nThe End. (But how did we get along here???)\n";

    return 0;
}


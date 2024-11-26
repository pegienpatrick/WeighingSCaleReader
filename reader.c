
/**************************************************

file: demo_rx.c
purpose: simple demo that receives characters from
the serial port and print them on the screen,
exit the program by pressing Ctrl-C

compile with the command: gcc demo_rx.c rs232.c -Wall -Wextra -o2 -o test_rx

**************************************************/

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"


int connected;
int main()
{
    connected=0;
    if(1<3)
    {
        FILE *fpt;
        fpt=fopen("data.pat","w");
        fprintf(fpt,"ST,GS,    0.0KG ");
        fclose(fpt);
    }


    int i, n,
        cport_nr=2,        /* /dev/ttyS0 (COM1 on windows) */
        bdrate=9600;       /* 9600 baud */

    unsigned char buf[4096];

    char mode[]={'8','N','1',0};

    int a=0;
    while(2>1)
    {
        //connected=0;
        if(RS232_OpenComport(cport_nr, bdrate, mode, 0))
        {
            printf("try to open comport\n");
            printf("opened failed");
            FILE *fptr;
            fptr=fopen("data.pat","w");
            fprintf(fptr,"ST,  0.0KG");
            fclose(fptr);
        }
        else
        {
            printf("serial port opening\n");

        }
        a=0;
        while(a<20)
        {

            n = RS232_PollComport(cport_nr, buf, 4095);
            a++;
            if(n > 0)
            {
                buf[n] = 0;   /* always put a "null" at the end of a string! */
                for(i=0; i < n; i++)
                {
                    if(buf[i] < 32)  /* replace unreadable control-codes by dots */
                    {
                        buf[i] = ' ';
                    }
                }

                printf("received %i bytes: %s\n", n, (char *)buf);
                FILE *fptr;
                fptr=fopen("data.pat","w");
                fprintf(fptr,"%s",(char *)buf);
                fclose(fptr);
            }

#ifdef _WIN32
            Sleep(100);
#else
            usleep(100000);  /* sleep for 100 milliSeconds */
#endif
        }
        if(n<1)
        {
            printf("Can not open comport\n");
            if(connected==1)
            {
                system("hstart -hide \"java closed.java\"");
            }
            connected=0;

            FILE *fptr;
            fptr=fopen("data.pat","w");
            fprintf(fptr,"ST,  0.0KG");
            fclose(fptr);
        }
        else
        {
            printf("serial opened successfully");
            if(connected==0)
            {
                system("hstart -hide \"java open.java\"");
            }

            connected=1;
        }
        RS232_CloseComport(cport_nr);
        printf(" Closing Port");
    }




  return(0);
}


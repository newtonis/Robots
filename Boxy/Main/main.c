
#include <xc.h>
#include "config.h"

void configIO(){
    TRISDbits.TRISD2 = OUTPUT;
    TRISDbits.TRISD3 = OUTPUT;
    TRISDbits.TRISD4 = OUTPUT;
    
    TRISBbits.TRISB0 = INPUT;
    TRISBbits.TRISB1 = INPUT;
    TRISBbits.TRISB2 = INPUT;
    
    ///Infrared
    TRISDbits.TRISD5 = INPUT;  
    TRISBbits.TRISB6 = INPUT;

    //Motor
    TRISDbits.TRISD0 = OUTPUT;
    TRISCbits.TRISC0 = OUTPUT;
    //PWM
    TRISCbits.TRISC1 = OUTPUT;
    TRISCbits.TRISC2 = OUTPUT;
}

char R,Y,O;
char PR , PY , PO;

int fa , ma;
int fb , mb;
int d1 , d2;

void main(void){
    R = 0;
    Y = 0;
    O = 0;
    PR = 0;
    PY = 0;
    PO = 0;

    fa = 0;
    fb = 0;
    ma = 0;
    mb = 0;
    d1 = 0;
    d2 = 0;

    InitTIMERS();
    configurations_init();

    MotorsPWM();
    configIO();
    //L_RED = 1;
    //L_YELLOW = 0;
    //L_ORANGE = 1;
   
    while (1){
        //MotorsSpeed(500 ,500);
        /*if (IR0 == 0 ){

            L_RED = 1;
            L_YELLOW = 1;
            L_ORANGE = 0;
        
        }else{
            L_RED = 0;
            L_YELLOW = 0;
            L_ORANGE = 1;
        }*/
        if (not B_RED and not ma){
            ma = 1;
            d1 = d1 == 10 ? -10 : (d1 + 1);
        }
        if (not B_ORANGE and not mb){
            mb = 1;
            d2 = d2 == 10 ? -10 : (d2 + 1);
        }
        if (B_RED) ma = 0;
        if (B_ORANGE) mb = 0;

        L_RED = d1 != 0;
        L_YELLOW = 1;
        L_ORANGE = d2 != 0;
        if (TIME > 10000){
            TIME = 0;
        }
        MotorsSpeed(d1 * 100, d2*100);
        //MotorsSpeed(TIME / 10 ,TIME / 10);
    }
        
    
}
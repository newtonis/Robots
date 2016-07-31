/* 
 * File:   main.c
 * Author: newtonis
 *
 * Created on August 16, 2014, 11:51 AM
 */

#include <xc.h>

#pragma config BOREN = OFF, CPD = OFF, FOSC = INTOSCIO, MCLRE = OFF, WDTE = OFF, CP = OFF, LVP = OFF, PWRTE = OFF
#define _XTAL_FREQ 4000000

#define INICIO  0
#define PAUSA   1
#define LEER    2
#define APAGADO 3

#define OUTPUT 0
#define INPUT  1

#define not !

#define IN_IR0 PORTBbits.RB2
#define IN_IR1 PORTBbits.RB1
#define IN_IR2 PORTBbits.RB0
#define IN_IR3 PORTAbits.RA4
#define IN_IR4 PORTAbits.RA3

#define OUT_IR0 PORTAbits.RA1
#define OUT_IR1 PORTAbits.RA0
#define OUT_IR2 PORTAbits.RA7
#define OUT_IR3 PORTAbits.RA6
#define OUT_IR4 PORTBbits.RB5

//#define IN_DIS PORTAbits.RA0

#define CVALORES 5

#define CERCA 0x04
#define LEJOS 0x10

char estado;
char contador;
char ciclos;

char sensor[CVALORES];

char valores[CVALORES] = {1,2,6,8,12};
char actual;

void init(void);
void configurar_pwm(void);
void configurar_IO(void);
void configurar_timer0(void);
void configurar_timer1(void);
void sensores(void);
void read();
void actualizar_salidas(void);
void DUTYSet(int value);

int main(int argc, char** argv) {

    init();
    configurar_IO();
    configurar_pwm();
    configurar_timer1();
    configurar_timer0();
    while (1){
        sensores();
    }
}


void init(void){
    CMCON = 0x07;
    contador = 0;
    estado = INICIO;
    ciclos = 0;
    actual = 0;
    
    ///// 0000
}
void configurar_pwm(void){
    T2CONbits.TMR2ON=1; // timer2 prendido
    T2CONbits.TOUTPS=0; // hasta 15
    T2CONbits.T2CKPS=0; // 00=1:1 01=1:4 1x=1:16
    TRISBbits.TRISB3=0; // pwm salida
    PR2=17;//26; // periodo 30Khz
    CCPR1L=0; //apagado
    CCP1CON=12;
}
void configurar_timer1(void){
    TMR1H=0xFC;
    TMR1L=0x18;

    PIE1bits.TMR1IE     = 0; //desabilita interrupcion
    PIR1bits.TMR1IF     = 0; //desborde

    T1CONbits.T1CKPS    = 0; //prescaler

    T1CONbits.nT1SYNC   = 1;
    T1CONbits.T1OSCEN   = 0;
    T1CONbits.TMR1CS    = 0;
    T1CONbits.TMR1ON    = 0; //on-off
    T1CONbits.TMR1ON    = 1; //prender timer
}
void configurar_timer0(void){
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA  = 0;
    OPTION_REGbits.PS   = 1;

    INTCONbits.T0IF     = 0;
    INTCONbits.T0IE     = 1;
    INTCONbits.GIE      = 1;
}
void configurar_IO(void){
    TRISB3 = OUTPUT;  //pwm
   
    TRISB2 = INPUT;
    TRISB1 = INPUT;
    TRISB0 = INPUT;
    TRISA4 = INPUT;
    TRISA3 = INPUT;
    
    TRISA1 = OUTPUT;
    TRISA0 = OUTPUT;
    TRISA7 = OUTPUT;
    TRISA6 = OUTPUT;
    TRISB5 = OUTPUT;
}
void interrupt t0_int(void){
    INTCONbits.T0IF=0;
    contador ++;
}
void sensores(){
    switch (estado){
        case INICIO:
            CCPR1L = 0x01; //IN_DIS ? CERCA : LEJOS;
            //CCPR1L= valores[actual]; //prender pwm
            T1CONbits.TMR1ON=1; //prender timer
            estado = PAUSA;
        break;
        case PAUSA:
            if(PIR1bits.TMR1IF==1){  //desborde
                PIR1bits.TMR1IF=0; //borro desborde
                T1CONbits.TMR1ON=0; //apagar timer
                TMR1H=0xFC;
                TMR1L=0x17;
                estado = LEER;
            }
        break;
        case LEER:
            read();
            contador = 0;
            CCPR1L=0; //apagado pwm
            estado = APAGADO;
        break;
        case APAGADO:
            if(contador >= 100){
                estado = INICIO;
            }
        break;
    }
}
void read(){
    OUT_IR0 = not IN_IR0;
    OUT_IR1 = not IN_IR1;
    OUT_IR2 = not IN_IR2;
    OUT_IR3 = not IN_IR3;
    OUT_IR4 = not IN_IR4;
}
void DUTYSet(int value){
    CCPR1L = value / 4;
    CCP1CON = value % 4;
}

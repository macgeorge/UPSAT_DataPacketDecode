//
//  main.c
//  Program to decode or encode QB50 DataSet of Whole Orbit Data Packet for the EPS
//
//  Created by George Christidis on 4/22/15.
//  Copyright (c) 2015 George Christidis. All rights reserved.
//

#include <stdio.h>

#define inputformat 1   //0: binary, 1: hex
#define action 1        //0: word->data, 1: data->word

int Measurements[13], Temperature[2];
long int GenerateWOD(void);

int main(){
    
    long int binaryNumber,hexadecimalNumber=0,j=1,remainder;
    int values[8];
    double measurements[7];
    long int WOD;

    if (action==0) { //word->data
    while(1){
        
        if (inputformat==0) {
            printf("Enter dataset in binary format, -1 to exit: ");
            scanf("%ld",&binaryNumber);
            if(binaryNumber==-1) break;
            
            while(binaryNumber!=0){
                remainder=binaryNumber%10;
                hexadecimalNumber=hexadecimalNumber+remainder*j;
                j=j*2;
                binaryNumber=binaryNumber/10;
            }
            
            
            printf("Equivalent hexadecimal value: 0x%lX\n",hexadecimalNumber);
        }
        else {
            printf("Enter dataset in hex format, -1 to exit: ");
            scanf("%lX",&hexadecimalNumber);
            if (hexadecimalNumber==-1) break;
            printf("Value: 0x%lX\n\n",hexadecimalNumber);
        }
        
        values[0]=(hexadecimalNumber&0x100000000000000)>>56;
        values[1]=(hexadecimalNumber&0x0FF000000000000)>>48;
        values[2]=(hexadecimalNumber&0x000FF0000000000)>>40;
        values[3]=(hexadecimalNumber&0x00000FF00000000)>>32;
        values[4]=(hexadecimalNumber&0x0000000FF000000)>>24;
        values[5]=(hexadecimalNumber&0x000000000FF0000)>>16;
        values[6]=(hexadecimalNumber&0x00000000000FF00)>>8;
        values[7]=(hexadecimalNumber&0x0000000000000FF)>>0;
        
        
        if (values[0]==0) printf("Mode: Normal\n");
        else printf("Mode: Safe\n");
        
        measurements[0]=values[1]*0.05+3.;
        measurements[1]=values[2]*7.87451-1000.;
        measurements[2]=values[3]*25;
        measurements[3]=values[4]*25;
        measurements[4]=values[5]*0.25-15;
        measurements[5]=values[6]*0.25-15;
        measurements[6]=values[7]*0.25-15;
        
        printf("Battery Voltage: %2.2fV\n",measurements[0]);
        printf("Battery Current: %4.2fmA\n",measurements[1]);
        printf("3.3V Current: %4.2fmA\n",measurements[2]);
        printf("5V Current: %4.2fmA\n",measurements[3]);
        printf("Temperature Comm: %2.2fC\n",measurements[4]);
        printf("Temperature EPS: %2.2fC\n",measurements[5]);
        printf("Temperature Battery: %2.2fC\n\n",measurements[6]);
    }
    printf("Exiting\n");
    return 0;
    }


    if (action==1) { //data->word
        //example//
        Measurements[8]=650;    //Battery current positive [mA]
        Measurements[9]=0;      //Battery current negative [mA]
        Measurements[10]=8900;  //Battery voltage [mV]
        Measurements[11]=500;   //3.3V rail current [mA]
        Measurements[12]=120;   //5V rail current [mA]
        Temperature[0]=14.8;    //First temperature [C, [int only]
        Temperature[1]=41.5;    //Second temperature [C], [int only]
        
        WOD=GenerateWOD();
        printf("WOD: 0x%lX\n", WOD);
    }
}


long int GenerateWOD(void)
{
    long int WOD=0;
    uint8_t data;
    
    //battery voltage
    data=0;
    if (Measurements[10]<3000) data=0;
    else if (Measurements[10]>15750) data=0xFF;
    else data=(uint8_t)(((Measurements[10]*20)-60000)/1000.);
    printf("Battery Voltage: 0x%X\n", data);
    WOD=WOD|((long)data<<48);
    
    //battery current
    data=0;
    if ((Measurements[8]-Measurements[9])<-1000) data=0;
    else if ((Measurements[8]-Measurements[9])>1008) data=0xFF;
    else data=(uint8_t)((((Measurements[8]-Measurements[9])*127)+127000)/1000.);
    printf("Battery Current: 0x%X\n", data);
    WOD=WOD|((long)data<<40);
    
    //3.3V current
    if (Measurements[11]<0) data=0;
    else if (Measurements[11]>6375) data=0xFF;
    else data=(uint8_t)((Measurements[11]*40)/1000.);
    printf("3.3V Current: 0x%X\n", data);
    WOD=WOD|((long)data<<32);
    
    //5V current
    if (Measurements[12]<0) data=0;
    else if (Measurements[12]>6375) data=0xFF;
    else data=(uint8_t)((Measurements[12]*40)/1000.);
    printf("5V Current: 0x%X\n", data);
    WOD=WOD|((long)data<<24);
    
    //Temperature EPS
    if (Temperature[0]<-15) data=0;
    else if (Temperature[0]>49) data=0xFF;
    else data=(uint8_t)(Temperature[0]*4+60);
    printf("Temperature EPS: 0x%X\n", data);
    WOD=WOD|((long)data<<8);
    
    //Temperature Battery
    if (Temperature[1]<-15) data=0;
    else if (Temperature[1]>49) data=0xFF;
    else data=(uint8_t)(Temperature[1]*4+60);
    printf("Temperature Battery: 0x%X\n", data);
    WOD=WOD|data;
    
    return WOD;
}
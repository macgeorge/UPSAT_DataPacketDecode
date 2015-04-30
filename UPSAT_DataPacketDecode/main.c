//
//  main.c
//  Program to decode QB50 DataSet of Whole Orbit Data Packet
//
//  Created by George Christidis on 4/22/15.
//  Copyright (c) 2015 George Christidis. All rights reserved.
//

#include <stdio.h>

#define inputformat 1 //0: binary, 1: hex

int main(){
    
    long int binaryNumber,hexadecimalNumber=0,j=1,remainder;
    int values[8];
    double measurements[7];
    
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
        printf("Temperature Battery: %2.2fC\n",measurements[6]);
    }
    printf("Exiting\n");
    return 0;
}
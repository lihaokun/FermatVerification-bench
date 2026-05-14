/* run.config
 DEPS: @PTEST_DEPS@ unit_bitwise.h
   OPT:
*/
/* run.config_qualif
 DEPS: @PTEST_DEPS@ unit_bitwise.h
   OPT: -wp-prop="-ko"
   OPT: -wp-prop="ko"
*/

#include "unit_bitwise.h"
//===============================================
//-- int
//===============================================


int band_int(int a,int b) { 
  
  
  return a & b ; 
}


int bor_int(int a,int b) { return a | b ; }


int bxor_int(int a,int b) { return a ^ b ; }


int bnot_int(int a) { return ~a ; }


int lshift_int(int a,int n) { return a << n ; }


int rshift_int(int a,int n) { return a >> n ; }

//===============================================
//-- unsigned int 
//===============================================
typedef unsigned int uint;

void band1_uint(uint a) {
   uint b = a & 1;
   
   
}


uint band_uint(uint a,uint b) { return a & b ; }


uint bor_uint(uint a,uint b) { return a | b ; }


uint bxor_uint(uint a,uint b) { return a ^ b ; }


uint bnot_uint(uint a) { return ~a ; }


uint lshift_uint(uint a,uint n) { return a << n ; }


uint rshift_uint(uint a,uint n) { return a >> n ; }

//===============================================
//-- char
//===============================================


char band_char(char a,char b) { return a & b ; }


char bor_char(char a,char b) { return a | b ; }


char bxor_char(char a,char b) { return a ^ b ; }


char bnot_char(char a) { return ~a ; }


char lshift_char(char a,char n) { return a << n ; }


char rshift_char(char a,char n) { return a >> n ; }

//===============================================
//-- unsigned char
//===============================================
typedef unsigned char uchar;

void band1_uchar(uchar a) {
   uchar b = a & 1;
   
   
}


uchar band_uchar(uchar a,uchar b) { return a & b ; }


uchar bor_uchar(uchar a,uchar b) { return a | b ; }


uchar bxor_uchar(uchar a,uchar b) { return a ^ b ; }


uchar bnot_uchar(uchar a) { return ~a ; }


uchar lshift_uchar(uchar a,uchar n) { return a << n ; }


uchar rshift_uchar(uchar a,uchar n) { return a >> n ; }

//===============================================
//-- Logic
//===============================================

//===============================================

void band1_ushort(unsigned short a) {
   unsigned short b = a & 1;
   
   
}
void band1_ulong(unsigned long a) {
   unsigned long b = a & 1;
   
   
}


uchar cast(uchar c,uint i,long long j) { 
  
  
  

  
  
  

  

  

  return (uint) c ; 
}


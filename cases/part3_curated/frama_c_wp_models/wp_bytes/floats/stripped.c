typedef unsigned char      uint8 ;

void float_(float f){
  uint8 buffer[sizeof(float)] ;
  *((float*) buffer) = f ;
  float read = *((float*) buffer) ;
  
}

void double_(double d){
  uint8 buffer[sizeof(double)] ;
  *((double*) buffer) = d ;
  double read = *((double*) buffer) ;
  
}

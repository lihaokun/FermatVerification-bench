typedef unsigned long long uint64 ;
typedef unsigned           uint32 ;
typedef unsigned char      uint8 ;

typedef int                int32 ;

struct X {
  uint32 u ;
  int32 i ;
} ;

union U {
  struct X x ;
  uint64   u ;
  uint8    a[sizeof(struct X)] ;
  uint32   s ;
} ;


void union_1(union U* u){
  u->u = 0x00000000FFFFFFFF ;

  
  

  
  

  
}


void union_2(union U* u){
  u->a[0] = 0xFF ;
  u->a[1] = 0xFF ;
  u->a[2] = 0xFF ;
  u->a[3] = 0xFF ;
  u->a[4] = 0x00 ;
  u->a[5] = 0x00 ;
  u->a[6] = 0x00 ;
  u->a[7] = 0x00 ;

  

  
  

  
}


void union_3(union U* u){
  u->u = 0x00000000FFFFFFFF ;
  u->s = 0xFFFFFFFF ;

  

  
  

  
  
}


void union_4(union U* u){
  struct X x = { .u = 0xFFFFFFFFu, .i = 0x00000000 } ;
  u->x = x ;

  

  
  

  
}

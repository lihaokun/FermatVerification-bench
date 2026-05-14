int triangle(int a, int b, int c)
{
  int __retres;
  
  
  
  
  if ((a + b) + c == 180) {
    if (a > 0) {
      if (b > 0) {
        if (c > 0) {
          __retres = 1;
          goto return_label;
        }
        else goto _LAND_1;
      }
      else goto _LAND_1;
    }
    else goto _LAND_1;
  }
  else {
    _LAND_1: {
               __retres = 0;
               goto return_label;
             }
  }
  return_label: return __retres;
}



int abs(int val)
{
  int __retres;
  if (val < 0) {
    
    __retres = - val;
    goto return_label;
  }
  __retres = val;
  return_label: return __retres;
}



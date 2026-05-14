int max(int x, int y)
{
  int __retres;
  if (x >= y) {
    __retres = x;
    goto return_label;
  }
  __retres = y;
  return_label: return __retres;
}






long long strlen(const char *s) {
  const char *ss = s;
  
  while (*ss)
    ss++;

  
  return ss - s;
}

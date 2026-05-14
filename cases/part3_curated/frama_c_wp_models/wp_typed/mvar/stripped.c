extern char A[20];




extern void Write(char *p, int n);



void Job(void)
{
  char DataWrite;
  DataWrite = 1 ;
  Write((& DataWrite),1);
  return;
}

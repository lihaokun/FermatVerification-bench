



int equal(const int* a, int n, const int* b)
{
  
  for (int i = 0; i < n; i++)
     if (a[i] != b[i])
       return 0;

  return 1;
}


int find(const int* a, int n, int val)
{
  
  for (int i = 0; i < n; i++)
    if (a[i] == val)
      return i;

  return n;
}



int find2(const int* a, int n, int val)
{
  
  for (int i = 0; i < n; i++)
    if (a[i] == val)
      return i;

  return n;
}



int find_first_of(const int* a, int m, const int* b, int n)
{
  
  for(int i = 0; i < m; i++)
     if (find(b, n, a[i]) < n)
          return i;

  return m;
}


int max_element(const int* a, int n)
{
  if (n == 0) return 0;
  int max = 0;
  
  for (int i = 0; i < n; i++)
     if (a[max] < a[i])
       max = i;

  return max;
}



int max_seq(const int* p, int n)
{
  return p[max_element(p, n)];
}




int count(const int* a, int n, int val)
{
  int cnt = 0;
  
  for (int i = 0; i < n; i++)
     if (a[i] == val)
       cnt++;

  return cnt;
}


void swap(int* p, int* q)
{
  int const save = *p;
  *p = *q;
  *q = save;
}


void swap_ranges(int* a, int n, int* b)
{
  
  for (int i = 0; i < n; i++)
     swap(&a[i], &b[i]);
}


void fill(int* a, int n, int val)
{
  
  for (int i = 0; i < n; i++)
     a[i] = val;
}


void copy(const int* a, int n, int* b)
{
  
  for (int i = 0; i < n; ++i)
     b[i] = a[i];
}


int replace_copy(const int* a, int n, int* b, int old_val, int
    new_val)
{
  
  for (int i = 0; i < n; ++i)
     b[i] = (a[i] == old_val ? new_val : a[i]);

  return n;
}


int remove_copy(const int* a, int n, int* b, int val)
{
  int j = 0;
  
  for (int i = 0; i < n; ++i)
     if (a[i] != val)
       b[j++] = a[i];
  return j;
}


void iota(int* a, int n, int val)
{
  
  for(int i = 0; i < n; ++i)
     a[i] = val + i;
}




int adjacent_find(int* a, int n)
{
  if (0 == n) return n;

  
  for (int i = 0; i < n-1; i++)
     if (a[i] == a[i+1])
       return i;

  return n;
}


int min_element(int* a, int n)
{
  if (0 == n) return n;

  int min = 0;
  
  for (int i = 0; i < n; i++)
     if (a[i] < a[min])
       min = i;

  return min;
}

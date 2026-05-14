struct S {
    int f;
    int g;
};





void f (struct S *p, struct S *q) {
  (p->f)++;
  (q->f)--;
  short *r = &(q->g);
  r[0] = -1;
  r[1] = -1;
  
  (q->g)++;
}

#include <assert.h>
int unknown1();
int unknown2();
int unknown3();
int unknown4();
int main() {
  int w = 1;
  int z = 0;
  int x = 0;
  int y = 0;
  /*@
  loop invariant z % 2 == 0 ==> y == z;
  loop invariant x <= 1;
  loop invariant w % 2 == 1 ==> x == w - 1;
  loop invariant z % 2 == y;
  loop invariant y == z;
  loop invariant w == z || w == z + 1;
  loop invariant w == 2 * x || w == 2 * x + 1;
  loop invariant 1 <= w;
  loop assigns x,y,w,z;
  */
  while (unknown2()) {
    if (w % 2 == 1) {
      x++;
      w++;
    }
    if (z % 2 == 0) {
      y++;
      z++;
    }
  }
  //@ assert x <= 1;
  return 0;
}
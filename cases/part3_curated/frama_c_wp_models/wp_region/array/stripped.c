struct S {
    int len;
    int content[10];
};




void add_first4(struct S * a , struct S * b )
{
    a->content[0] += b->content[0];
    a->content[1] += b->content[1];
    a->content[2] += b->content[2];
    a->content[3] += b->content[3];
}

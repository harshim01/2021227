#ifndef BIGINTEGER_H
#define BIGINTEGER_H
struct node
{
    int data;
    struct node *next;
    struct node *prev;
};
struct BigInteger
{
    struct node *head;
    int length, sign;
};
struct BigInteger add(struct BigInteger a, struct BigInteger b);
struct BigInteger sub(struct BigInteger a, struct BigInteger b);
struct BigInteger mul(struct BigInteger a, struct BigInteger b);
struct BigInteger div1(struct BigInteger a, struct BigInteger b);
struct BigInteger mod(struct BigInteger a, struct BigInteger b);
void InsertAtBegin(struct BigInteger *bi, int data);
void printBigInteger(struct BigInteger bi);
void truncate(struct BigInteger *bi);
struct BigInteger initialize(char *s);
int comp(struct BigInteger a, struct BigInteger b);

#endif
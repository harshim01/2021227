// BigInteger.h
// BigInteger.c
#include "BigInteger.h"
#include <stdio.h>
#include <stdlib.h>

void InsertAtBegin(struct BigInteger *bi, int data)
{
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->data = data;
    newNode->next = bi->head;
    newNode->prev = NULL;
    if (bi->head != NULL)
    {
        bi->head->prev = newNode;
    }
    bi->head = newNode;
    bi->length++;
}

// Function to print a big integer
void printBigInteger(struct BigInteger bi)
{
    if (bi.sign)
    {
        printf("-");
    }

    struct node *current = bi.head;

    if (current == NULL)
    {
        printf("0\n");
        return;
    }

    while (current != NULL)
    {
        printf("%d", current->data);
        current = current->next;
    }

    printf("\n");
}

// Function to remove leading zeros
void truncate(struct BigInteger *bi)
{
    struct node *current = bi->head;
    while (current != NULL && current->data == 0)
    {
        struct node *temp = current;
        current = current->next;
        if (current != NULL)
        {
            current->prev = NULL;
        }
        free(temp);
    }
    bi->head = current;
    if (current == NULL)
    {
        bi->length = 0;
    }
}

// Function to initialize a big integer from a string
struct BigInteger initialize(char *s)
{
    struct BigInteger temp;
    temp.length = 0;
    temp.sign = 0;
    int i = 0;

    if (s[i] == '-')
    {
        temp.sign = 1;
        i++;
    }

    while (s[i] != '\0')
    {
        temp.length++;
        InsertAtBegin(&temp, s[i] - '0');
        i++;
    }

    return temp;
}

// Function to compare two big integers
int comp(struct BigInteger a, struct BigInteger b)
{
    truncate(&a);
    truncate(&b);

    if (a.length > b.length)
    {
        return 1;
    }
    else if (a.length < b.length)
    {
        return -1;
    }

    struct node *curA = a.head;
    struct node *curB = b.head;

    while (curA != NULL && curB != NULL)
    {
        if (curA->data > curB->data)
        {
            return 1;
        }
        else if (curA->data < curB->data)
        {
            return -1;
        }
        curA = curA->next;
        curB = curB->next;
    }

    return 0;
}

// Function to perform addition (a + b)
struct BigInteger add(struct BigInteger a, struct BigInteger b)
{
    struct BigInteger result;

    // Handle the signs of 'a' and 'b'
    if (a.sign && !b.sign)
    {
        a.sign = 0;
        return sub(b, a);
    }
    else if (!a.sign && b.sign)
    {
        b.sign = 0;
        return sub(a, b);
    }
    else if (a.sign && b.sign)
    {
        a.sign = 0;
        b.sign = 0;
        result = add(a, b);
        result.sign = 1; // Result is negative
    }
    else
    {
        struct node *curA = a.head;
        struct node *curB = b.head;
        int carry = 0;

        result.head = NULL;
        struct node *tail = NULL;

        while (curA != NULL || curB != NULL)
        {
            int digitA = (curA != NULL) ? curA->data : 0;
            int digitB = (curB != NULL) ? curB->data : 0;

            int sum = digitA + digitB + carry;
            carry = sum / 10;

            struct node *newNode = (struct node *)malloc(sizeof(struct node));
            newNode->data = sum % 10;
            newNode->next = NULL;
            newNode->prev = tail;

            if (tail != NULL)
            {
                tail->next = newNode;
            }
            else
            {
                result.head = newNode;
            }

            tail = newNode;

            if (curA != NULL)
                curA = curA->next;
            if (curB != NULL)
                curB = curB->next;
        }

        if (carry > 0)
        {
            struct node *newNode = (struct node *)malloc(sizeof(struct node));
            newNode->data = carry;
            newNode->next = result.head;
            newNode->prev = NULL;
            result.head = newNode;
        }

        truncate(&result);
    }
    return result;
}

// Function to perform subtraction (a - b)
struct BigInteger sub(struct BigInteger a, struct BigInteger b)
{
    struct BigInteger result;

    // Handle the signs of 'a' and 'b'
    if (a.sign && !b.sign)
    {
        a.sign = 0;
        result = add(a, b);
        result.sign = 1; // Result is negative
    }
    else if (!a.sign && b.sign)
    {
        b.sign = 0;
        return add(a, b);
    }
    else if (a.sign && b.sign)
    {
        a.sign = 0;
        b.sign = 0;
        return sub(b, a);
    }
    else
    {
        // Compare a and b to determine the sign of the result
        int compareResult = comp(a, b);
        if (compareResult == 0)
        {
            result.head = NULL;
            result.length = 0;
            return result;
        }
        else if (compareResult < 0)
        {
            struct BigInteger temp = a;
            a = b;
            b = temp;
            result.sign = 1; // The result will be negative
        }

        struct node *curA = a.head;
        struct node *curB = b.head;
        int borrow = 0;

        result.head = NULL;
        struct node *tail = NULL;

        while (curA != NULL || curB != NULL)
        {
            int digitA = (curA != NULL) ? curA->data : 0;
            int digitB = (curB != NULL) ? curB->data : 0;

            int difference = digitA - digitB - borrow;

            if (difference < 0)
            {
                difference += 10;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }

            struct node *newNode = (struct node *)malloc(sizeof(struct node));
            newNode->data = difference;
            newNode->next = NULL;
            newNode->prev = tail;

            if (tail != NULL)
            {
                tail->next = newNode;
            }
            else
            {
                result.head = newNode;
            }

            tail = newNode;

            if (curA != NULL)
                curA = curA->next;
            if (curB != NULL)
                curB = curB->next;
        }

        truncate(&result);
    }
    return result;
}

// Function to perform multiplication (a * b)
struct BigInteger mul(struct BigInteger a, struct BigInteger b)
{
    struct BigInteger result;
    result.sign = a.sign ^ b.sign;

    result.head = NULL;
    result.length = 0;

    if (a.length == 0 || b.length == 0)
    {
        result.head = (struct node *)malloc(sizeof(struct node));
        result.head->data = 0;
        result.head->next = NULL;
        result.head->prev = NULL;
        result.length = 1;
        return result;
    }

    struct node *curB = b.head;

    while (curB != NULL)
    {
        int digitB = curB->data;
        struct BigInteger partialProduct;
        partialProduct.head = NULL;

        int carry = 0;

        struct node *curA = a.head;
        while (curA != NULL)
        {
            int digitA = curA->data;
            int product = (digitA * digitB) + carry;
            carry = product / 10;

            struct node *newNode = (struct node *)malloc(sizeof(struct node));
            newNode->data = product % 10;
            newNode->next = NULL;
            newNode->prev = NULL;

            if (partialProduct.head == NULL)
            {
                partialProduct.head = newNode;
            }
            else
            {
                struct node *tail = partialProduct.head;
                while (tail->next != NULL)
                {
                    tail = tail->next;
                }
                tail->next = newNode;
                newNode->prev = tail;
            }

            curA = curA->next;
        }

        if (carry > 0)
        {
            struct node *newNode = (struct node *)malloc(sizeof(struct node));
            newNode->data = carry;
            newNode->next = NULL;
            newNode->prev = NULL;

            if (partialProduct.head == NULL)
            {
                partialProduct.head = newNode;
            }
            else
            {
                struct node *tail = partialProduct.head;
                while (tail->next != NULL)
                {
                    tail = tail->next;
                }
                tail->next = newNode;
                newNode->prev = tail;
            }
        }

        result = add(result, partialProduct);

        curB = curB->next;
    }

    return result;
}

// Function to perform division (a / b)
struct BigInteger div1(struct BigInteger a, struct BigInteger b)
{
    struct BigInteger result;

    // Handle division by zero
    if (b.length == 0)
    {
        printf("Error: Division by zero.\n");
        result.head = NULL;
        result.length = 0;
        result.sign = 0;
        return result;
    }

    // Handle the case where 'a' is zero
    if (a.length == 0)
    {
        result.head = (struct node *)malloc(sizeof(struct node));
        result.head->data = 0;
        result.head->next = NULL;
        result.head->prev = NULL;
        result.length = 1;
        result.sign = 0;
        return result;
    }

    result.sign = a.sign ^ b.sign; // XOR of signs, result is negative if only one is negative

    result.head = NULL;
    result.length = 0;

    struct BigInteger quotient;
    quotient.head = NULL;
    quotient.length = 0;

    struct BigInteger dividend = a;
    struct node *curB = b.head;

    while (comp(dividend, b) >= 0)
    {
        struct BigInteger currentQuotient;
        currentQuotient.head = NULL;
        currentQuotient.length = 0;

        struct node *curCopyB = curB;

        int count = 0;
        while (comp(dividend, currentQuotient) >= 0)
        {
            currentQuotient = add(currentQuotient, b);
            count++;
        }

        struct BigInteger one;
        one.head = (struct node *)malloc(sizeof(struct node));
        one.head->data = 1;
        one.head->next = NULL;
        one.head->prev = NULL;
        one.length = 1;

        currentQuotient = sub(currentQuotient, one);
        free(one.head);

        result = add(result, currentQuotient);

        struct BigInteger product = mul(currentQuotient, b);

        dividend = sub(dividend, product);

        if (curB != NULL)
        {
            curB = curB->next;
        }
    }

    truncate(&result);
    return result;
}

// Function to perform modulus (a % b)
struct BigInteger mod(struct BigInteger a, struct BigInteger b)
{
    struct BigInteger result;

    if (b.length == 0)
    {
        printf("Error: Modulus by zero.\n");
        result.head = NULL;
        result.length = 0;
        result.sign = 0;
        return result;
    }

    result.sign = a.sign;
    result.head = NULL;
    result.length = 0;

    struct node *curB = b.head;

    while (comp(a, b) >= 0)
    {
        struct BigInteger currentQuotient;
        currentQuotient.head = NULL;
        currentQuotient.length = 0;

        while (comp(a, currentQuotient) >= 0)
        {
            a = sub(a, b);
            currentQuotient = add(currentQuotient, b);
        }
    }

    result = a;
    truncate(&result);
    return result;
}
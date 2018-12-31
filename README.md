**Arbitrary Integer In C**

This is a C struct which can contain arbitrary integer. 

This struct provides add, substract, assign, and many elementary operations.
Besides, this struct can be constructed from string or long long int. 

It can also be transfered to long long int and string.


**The format that it can accepted**

+ 0, +0, -0, +1, -1, 1.

+ extra zeros like 0000, 001 and so on will cause an error directly.

**The implementation**
+ ```struct arb_int```

    ```C
    char sign;
    char * digit;
    unsigned int length;
    ```
    Sign is used to record its sign. Char array digit is used to store every digits.
    Length is used to store this number's length of digits.

+ ```void arb_free (arb_int_t * i)```

    This function firstly free the char array digit, then free the pointer to pointer to struct arb_int.
    
+ ```int arb_duplicate (arb_int_t * new, const arb_int_t original)```

    >return 0: successful
    
    >return 1: failed

    This function will allocate a new space for new. Then copy every content of original to new. It uses strcpy for copying.

+ ```int arb_from_string (arb_int_t * i, const char * s)```

    >return 0: successful
    
    >return 1: failed
    
    It will check the first char of s to see if it is a sign. Then decide the position to start to copy digits.
    
    It uses strcpy and strlen to copy and get the length.

+ ```int arb_from_int (arb_int_t * i, signed long long int source)```

    >return 0: successful
    
    >return 1: failed
    
    This function will use while loop, / and % to get every digit of source and the number of digits. 
    
    The sign will be dicided by if the source is positive or negative.

+ ```int arb_to_string (const arb_int_t i, char * buf, size_t max)```

    >return 0: successful
    
    >return 1: failed
    
    First, it will check if the buffer is not enough by comparing the length of i with max.
    
    If buffer can contain it, it will copy the char array digit to the buffer. 
    
    At last, add an '\0' to the end.
    
+ ```int arb_to_int (const arb_int_t i, long long int * out)```

    >return 0: successful
    
    >return 1: failed
    
    If i is too long (>LLONG_MAX OR <LLONG_MIN), out can't contain i. Then return error.
    
    If i can be transfered to a long long integer, then use while loop to add/substract every digit (1, 10, 100, ...) to out.
    
    It will return an erro when there is an overflow in while loop.
    
+ ```int arb_assign (arb_int_t x, const arb_int_t y)```

    >return 0: successful
    
    >return 1: failed
    
    Unlike duplicate function, x will be created by caller before. So This function only copy every content of y to x.
    
+ ```int arb_add (arb_int_t x, const arb_int_t y)```

    >return 0: successful

    >return 1: failed
    
    This funciton will seperate the input into some different cases. And the basical case is same signs. Other cases will be transfered to subtract format.
    
    It will use two pointers to add every digit and calculate the carry to deal with the base case.

+ ```int arb_subtract (arb_int_t x, const arb_int_t y)```

    >return 0: successful

    >return 1: failed
    
    This funciton will seperate the input into some different cases. And the basical case is a bigger positive number subtract the smaller positive number. 
    
    Other cases will be transfered to subtract or add format.
    
    It will use two pointers to add every digit and calculate the carry to deal with the base case.

+ ```int arb_multiply (arb_int_t x, const arb_int_t y)```

    >return 0: successful
    
    >return 1: failed
    
    The function will first determine the sign.
    
    Then, this function uses the digit mapping to get every digit multiplying result. And add then to get the final result.
    
+ ```int arb_divide (arb_int_t x, const arb_int_t y)```

    >return 0: successful
    
    >return 1: failed
    
    The function will first determine the sign.
    
    Then it will call subtract function iteratively to get the final result. 
    
+ ```int arb_digit_count (const arb_int_t x)```

    >return the count of digits
    
    This function uses strlen() to get the char* length and add 1 if the sign is '-';

**Test**

*Please see the arb_test.c. There are detailed explainations and test cases.*

**Unit Test**

*Please run "make check". Make sure there is required cunit library.* 

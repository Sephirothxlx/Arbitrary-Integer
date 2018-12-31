#include "arb_int.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

void testWithoutArg();

/**
* Below is an example program.
* This program should compile and link with your code
* *WITHOUT MODIFICATIONS* and
* should not show any memory leaks or other warnings when
* running under vlagrind.
*/
int main(int argc, char **argv){
	int error=0;
	if(argc==1){
		testWithoutArg();
	}else if(argc==4){
		if(strlen(argv[2])!=1||(*(argv[2])!='+'&&*(argv[2])!='-'&&*(argv[2])!='*'&&*(argv[2])!='/')){
			fprintf(stderr,"%s\n","Operator is wrong!");
			error=1;
		}else{
			arb_int_t a;
			arb_int_t b;
			if(arb_from_string(&a,argv[1])==1){
				fprintf(stderr,"%s\n","Number is wrong!");
				error=1;
			}else if(arb_from_string(&b,argv[3])==1){
				fprintf(stderr,"%s\n","Number is wrong!");
				arb_free(&a);
				error=1;
			}else{
				if(*(argv[2])=='+'){
					if(arb_add(a,b)==1)
						error=1;
				}else if(*(argv[2])=='-'){
					if(arb_subtract(a,b)==1)
						error=1;
				}else if(*(argv[2])=='*'){
					if(arb_multiply(a,b)==1)
						error=1;
				}else if(*(argv[2])=='/'){
					if(arb_divide(a,b)==1){
						error=1;
						printf("Divide has an error.\n");
					}
				}
				if(error!=1){
					int number=arb_digit_count(a);
					char buff[number+1];
					if(arb_to_string(a,buff,number+1)==1){
						error=1;
					}else{
						printf("%s\n",buff);
					}
				}
				arb_free(&a);
				arb_free(&b);
			}
		}
	}else{
		error=1;
	}
	if(error==0)
		return EXIT_SUCCESS;
	else{
		fprintf(stderr, "%s", "Error, please input number operation number\n");
		fprintf(stderr, "%s", "Number is an arbitrary precision integer and operation should be one of '+', '-', '*' or '/'.\n");
		return EXIT_FAILURE;
	}
}

void testWithoutArg(){

  /**
    example tests with some printed results
  **/

  //the example codes
  arb_int_t a;
  arb_from_string(&a, "12345123451234512345999");
  arb_int_t b;
  arb_duplicate (&b, a);
  if(arb_compare(b,a)==0)
    printf("Test compare: They are the same.\n");
  else 
    printf("Test compare: They are not the same.\n");

  //my codes
  //test arb_int_string
  char output[100];
  arb_to_string(b, output, sizeof(output));
  printf("Test arb_to_string: b is %s.\n", output);

  arb_free (&a);
  arb_free (&b);

  //the example codes
  arb_from_string(&a, "1234567890123456789012345678901234567890");
  arb_from_string(&b, "1");
  arb_assign(b,a); // b = a

  // you should validate the result is 0
  if(arb_compare(b,a)==0)
    printf("Test compare: They are the same.\n");
  else
    printf("Test compare: They are not the same.\n");


  /**
    All my tests
  **/

  //test arb_to_string
  memset(output,'\0',100);
  arb_to_string(b, output, sizeof(output));
  // you should validate the number matches the number above
  printf("Test arb_to_string:b is %s.\n", output);
  

  //test arb_to_int if the arb_int is too long
  long long int n;
  if(arb_to_int(b,&n)==0){
    printf("Test too long number convert: The number is %lld.\n", n);
  }else{
    printf("Test too long number convert: The number is too big! arb_to_int can't work.\n");
  }
  
  //test arb_from_int
  //and test arb_to_int
  n=LLONG_MAX;
  arb_free (&b);
  arb_from_int(&b,n);
  if(arb_to_int(b,&n)==0){
    printf("Test LLONG_MAX: The number is %lld.\n", n);
  }else{
    printf("Test LLONG_MAX: The number is too big! arb_to_int can't work.\n");
  }
 
  n=LLONG_MIN;
  arb_free (&b);
  arb_from_int(&b,n);
  if(arb_to_int(b,&n)==0){
    printf("Test LLONG_MIN: The number is %lld.\n", n);
  }else{
    printf("Test LLONG_MIN: The number is too big! arb_to_int can't work.\n");
  }

  n=0;
  arb_free (&b);
  arb_from_int(&b,n);
  if(arb_to_int(b,&n)==0){
    printf("Test 0: The number is %lld.\n", n);
  }else{
    printf("Test 0: The number is too big! arb_to_int can't work.\n");
  }

  n=23232;
  arb_free (&b);
  arb_from_int(&b,n);
  if(arb_to_int(b,&n)==0){
    printf("Test 23232: The number is %lld.\n", n);
  }else{
    printf("Test 23232: The number is too big! arb_to_int can't work.\n");
  }

  n=-23232;
  arb_free (&b);
  arb_from_int(&b,n);
  if(arb_to_int(b,&n)==0){
    printf("Test -23232: The number is %lld.\n", n);
  }else{
    printf("Test -23232: The number is too big! arb_to_int can't work.\n");
  }
 
  //test arb_from_string
  arb_free (&b);
  arb_from_string(&b,"0");
  if(arb_to_int(b,&n)==0){
    printf("Test 0: The number is %lld.\n", n);
  }else{
    printf("Test 0: The number is too big! arb_to_int can't work.\n");
  }
   
  arb_free (&b);
  arb_from_string(&b,"+0");
  if(arb_to_int(b,&n)==0){
    printf("Test +0: The number is %lld.\n", n);
  }else{
    printf("Test +0: The number is too big! arb_to_int can't work.\n");
  }

  arb_free (&b);
  arb_from_string(&b,"-0");
  if(arb_to_int(b,&n)==0){
    printf("Test -0: The number is %lld.\n", n);
  }else{
    printf("Test -0: The number is too big! arb_to_int can't work.\n");
  }
  
  arb_free (&b);
  arb_from_string(&b,"+23232");
  if(arb_to_int(b,&n)==0){
    printf("Test +23232: The number is %lld.\n", n);
  }else{
    printf("Test +23232: The number is too big! arb_to_int can't work.\n");
  }

  arb_free (&b);
  arb_from_string(&b,"-23232");
  if(arb_to_int(b,&n)==0){
    printf("Test -23232: The number is %lld.\n", n);
  }else{
    printf("Test -23232: The number is too big! arb_to_int can't work.\n");
  }
 
  arb_free (&b);
  arb_from_string(&b,"23232");
  if(arb_to_int(b,&n)==0){
    printf("Test 23232: The number is %lld.\n", n);
  }else{
    printf("Test 23232: The number is too big! arb_to_int can't work.\n");
  }
  
  //add test
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"1");
  arb_from_string(&b,"-1");
  if(arb_add(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test add 1+(-1): The number is %lld.\n", n);
  }else{
    printf("Test add 1+(-1): error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"-1");
  arb_from_string(&b,"1");
  if(arb_add(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test add -1+1: The number is %lld.\n", n);
  }else{
    printf("Test add -1+1: error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"123");
  arb_from_string(&b,"99");
  if(arb_add(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test add 123+99: The number is %lld.\n", n);
  }else{
    printf("Test add 123+99: error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"-123");
  arb_from_string(&b,"-99");
  if(arb_add(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test add -123+(-99): The number is %lld.\n", n);
  }else{
    printf("Test add -123+(-99): error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"123");
  arb_from_string(&b,"-999");
  if(arb_add(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test add 123+(-999): The number is %lld.\n", n);
  }else{
    printf("Test add 123+(-999): error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"-123");
  arb_from_string(&b,"999");
  if(arb_add(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test add -123+999: The number is %lld.\n", n);
  }else{
    printf("Test add -123+999: error.\n");
  }
  
  //subtract test
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"1");
  arb_from_string(&b,"1");
  if(arb_subtract(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test subtract 1-1: The number is %lld.\n", n);
  }else{
    printf("Test subtract 1-1: error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"-1");
  arb_from_string(&b,"-1");
  if(arb_subtract(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test subtract -1-(-1): The number is %lld.\n", n);
  }else{
    printf("Test subtract -1-(-1): error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"123");
  arb_from_string(&b,"99");
  if(arb_subtract(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test subtract 123-99: The number is %lld.\n", n);
  }else{
    printf("Test subtract 123-99: error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"-123");
  arb_from_string(&b,"-99");
  if(arb_subtract(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test subtract -123-(-99): The number is %lld.\n", n);
  }else{
    printf("Test subtract -123-(-99): error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"123");
  arb_from_string(&b,"-999");
  if(arb_subtract(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test subtract 123-(-999): The number is %lld.\n", n);
  }else{
    printf("Test subtract 123-(-999): error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"-123");
  arb_from_string(&b,"999");
  if(arb_subtract(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test subtract -123-999: The number is %lld.\n", n);
  }else{
    printf("Test subtract -123-999: error.\n");
  }
  
  //multiply test
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"0");
  arb_from_string(&b,"-1");
  if(arb_multiply(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test multiply 0*(-1): The number is %lld.\n", n);
  }else{
    printf("Test multiply 0*(-1): error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"-1");
  arb_from_string(&b,"-1");
  if(arb_multiply(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test multiply (-1)*(-1): The number is %lld.\n", n);
  }else{
    printf("Test multiply (-1)*(-1): error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"123");
  arb_from_string(&b,"99");
  if(arb_multiply(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test multiply 123*99: The number is %lld.\n", n);
  }else{
    printf("Test multiply 123*99: error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"-123");
  arb_from_string(&b,"-99");
  if(arb_multiply(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test multiply -123*(-99): The number is %lld.\n", n);
  }else{
    printf("Test multiply -123*(-99): error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"123");
  arb_from_string(&b,"-999");
  if(arb_multiply(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test multiply 123*(-999): The number is %lld.\n", n);
  }else{
    printf("Test multiply 123*(-999): error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"-123");
  arb_from_string(&b,"999");
  if(arb_multiply(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test multiply (-123)*999: The number is %lld.\n", n);
  }else{
    printf("Test multiply (-123)*999: error.\n");
  }
  
  //divide test
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"1");
  arb_from_string(&b,"0");
  if(arb_divide(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test divide 1/0: The number is %lld.\n", n);
  }else{
    printf("Test divide 1/0: error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"-1");
  arb_from_string(&b,"1");
  if(arb_divide(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test divide -1/1: The number is %lld.\n", n);
  }else{
    printf("Test divide -1/1: error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"99999");
  arb_from_string(&b,"99");
  if(arb_divide(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test divide 99999/99: The number is %lld.\n", n);
  }else{
    printf("Test divide 99999/99: error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"-99998");
  arb_from_string(&b,"-99");
  if(arb_divide(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test divide (-99998)/(-99): The number is %lld.\n", n);
  }else{
    printf("Test divide (-99998)/(-99): error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"12300");
  arb_from_string(&b,"-999");
  if(arb_divide(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test divide 12300/(-999): The number is %lld.\n", n);
  }else{
    printf("Test divide 12300/(-999): error.\n");
  }
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"-12300");
  arb_from_string(&b,"999");
  if(arb_divide(a,b)==0){
  	arb_to_int(a,&n);
    printf("Test divide -12300/999: The number is %lld.\n", n);
  }else{
    printf("Test divide -12300/999: error.\n");
  }
  
  //test count function
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"123");
  arb_from_string(&b,"-123");
  printf("Test count function: 123 has %d digits\n",arb_digit_count(a));
  printf("Test count function: -123 has %d digits\n",arb_digit_count(b));
  
  //test compare function
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"123");
  arb_from_string(&b,"-123");
  if(arb_compare(a,b)==1)
  	printf("Test compare function: 123 is bigger than -123\n");
  else if(arb_compare(a,b)==-1)
  	printf("Test compare function: 123 is smaller than -123\n");
  else
  	printf("Test compare function: 123 is equal to -123\n");
  	
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"123");
  arb_from_string(&b,"12");
  if(arb_compare(a,b)==1)
  	printf("Test compare function: 123 is bigger than 12\n");
  else if(arb_compare(a,b)==-1)
  	printf("Test compare function: 123 is smaller than 12\n");
  else
  	printf("Test compare function: 123 is equal to 12\n");
  
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"-123");
  arb_from_string(&b,"-12");
  if(arb_compare(a,b)==1)
  	printf("Test compare function: -123 is bigger than -12\n");
  else if(arb_compare(a,b)==-1)
  	printf("Test compare function: -123 is smaller than -12\n");
  else
  	printf("Test compare function: -123 is equal to -12\n");
  	
  arb_free(&a);
  arb_free(&b);
  arb_from_string(&a,"-123");
  arb_from_string(&b,"-123");
  if(arb_compare(a,b)==1)
  	printf("Test compare function: -123 is bigger than -123\n");
  else if(arb_compare(a,b)==-1)
  	printf("Test compare function: -123 is smaller than -123\n");
  else
  	printf("Test compare function: -123 is equal to -123\n");
  
  arb_free(&a);
  arb_free(&b);
}



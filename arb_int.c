#include "arb_int.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

struct arb_int{
  char sign;
  char * digit;
  unsigned int length;
};

void arb_free (arb_int_t * i){
  free((*i)->digit);
  free(*i);
}

//new should be freed by caller
int arb_duplicate (arb_int_t * new, const arb_int_t original){
  //if it can not duplicate
  if(original==NULL||original->digit==NULL)
    return 1;
  //allocate a new space
  *new=(arb_int_t)malloc(sizeof(arb_int));
  //includes '\0'
  (*new)->digit=(char *)malloc(sizeof(char)*(strlen(original->digit)+1));
  //copy sign
  (*new)->sign=original->sign;
  //copy char*
  strcpy((*new)->digit,original->digit);
  //copy length
  (*new)->length=original->length;
  return 0;
}

//the i should be freed by the program, not this function
int arb_from_string (arb_int_t * i, const char * s){
  if(s==NULL)
    return 1;
  //create a new struct
  //if this function failed, the new struct will not be assigned to i,
  //and  it will be freed by this function
  arb_int_t temp=(arb_int_t)malloc(sizeof(arb_int));

  //use begin to record the first digit
  //sometimes, the first digit may be sign
  int begin=0;
  if(s[0]=='-'||s[0]=='+'){
    if(strlen(s)==1){
       free(temp);
       return 1;
    }else{
      temp->sign=s[0];
      begin=1;
    }
  }else if(isdigit(s[0])){
    temp->sign='+';
  }else{
    free(temp);
    return 1;
  }
  //if there are ineffective zeros, return error
  if(strlen(s+begin)!=1&&s[begin]=='0'&&s[begin+1]=='0'){
    free(temp);
    return 1;
  }
  //allocate a new space
  temp->digit=(char*)malloc(sizeof(char)*(strlen(s)-begin+1));
  int j=0;
  char c=0;
  for(;j<strlen(s)-begin;j++){
    c=s[begin+j];
    if(isdigit(c)){
      *(temp->digit+j)=c;
    }else{
      //call arb_free
      arb_free(&temp);
      return 1;
    }
  }
  *(temp->digit+j)='\0';
  temp->length=strlen(s)-begin;
  if(*(temp->digit)=='0'){
		temp->sign='+';
	}
  *i=temp;
  return 0;
}

//i should be freed before by caller
int arb_from_int (arb_int_t * i, signed long long int source){
  //allocate a new space
  *i=(arb_int_t)malloc(sizeof(arb_int));
  if(source>=0){
    (*i)->sign='+';
  }else{
    (*i)->sign='-';
  }

  //count the length of digits
  int l=1;
  long long int temp=source;
  if(temp!=0){
    l=0;
    while(temp!=0){
      temp=temp/10;
      l++;
    }
  }

  //allocate new space
  (*i)->digit=(char*)malloc(sizeof(char)*(l+1));
  (*i)->length=l;
  int j=l;
  *((*i)->digit+j)='\0';
  j--;
  //copy every digit
  for(;j>=0;j--){
    *((*i)->digit+j)='0'+abs(source%10);
    source=source/10;
  }
  return 0;
}

int arb_to_string (const arb_int_t i, char * buf, size_t max){
  if(i->sign=='+'){
    //if exceed max
    if(i->length+1>(unsigned int)max){
      return 1;
    }else{
      strcpy(buf,i->digit);
      return 0;
    }
  }else{
    if(i->length+2>(unsigned int)max){
      return 1;
    }else if(*(i->digit)=='0'){
      strcpy(buf,i->digit);
      return 0;
    }else{
      //first char is '-'
      *buf='-';
      strcpy(buf+1,i->digit);
      return 0;
    }
  }
}

int arb_to_int (const arb_int_t i, long long int * out){
  if(i->sign=='+'){
    //count the max length
    long long int l=LLONG_MAX;
    int len=0;
    while(l!=0){
      l=l/10;
      len++;
    }
    if(len<i->length){
      return 1;
    }else{
      *out=0;
      long long int c=1;
      long long int temp;
      for(int j=i->length-1;j>=0;j--){
        temp=*out+(*(i->digit+j)-'0')*c;
        //if overflow, return
        if(temp<*out)
          return 1;
        *out=temp;
        c=c*10;
      }
    }
  }else{
    //count the max length
    long long int l=LLONG_MIN;
    int len=0;
    while(l!=0){
      l=l/10;
      len++;
    }
    if(len<i->length){
      return 1;
    }else{
      *out=0;
      long long int c=1;
      long long int temp;
      for(int j=i->length-1;j>=0;j--){
        temp=*out-(*(i->digit+j)-'0')*c;
        //if overflow, return
        if(temp>*out)
          return 1;
        *out=temp;
        c=c*10;
      }
    }
  }
  return 0;
}

//x and y should be created before
int arb_assign (arb_int_t x, const arb_int_t y){
  if(x==NULL||y==NULL)
    return 1;
  if(x->digit==NULL||y->digit==NULL)
    return 1;
  x->length=y->length;
  x->sign=y->sign;
  //free the digit
  free(x->digit);
  x->digit=(char*)malloc(x->length+1);
  strcpy(x->digit,y->digit);
  return 0;
}

int arb_compare (const arb_int_t x, const arb_int_t y){
  //firstly, compare their sign
  if(*(x->digit)=='0'&&*(y->digit)=='0')
    return 0;
  else if(x->sign=='+'&&y->sign=='-')
    return 1;
  else if(x->sign=='-'&&y->sign=='+')
    return -1;
  else if(x->sign=='+'&&y->sign=='+'){
    //secondly, compare their length
    if(x->length>y->length)
      return 1;
    else if(x->length<y->length)
      return -1;
    else {
      //thirdly, compare their digits
      for(int i=0;i<x->length;i++){
        if(*(x->digit+i)-'0'>*(y->digit+i)-'0'){
          return 1;
        }else if(*(x->digit+i)-'0'<*(y->digit+i)-'0'){
          return -1;
        }
      }
    }
  }else if(x->sign=='-'&&y->sign=='-'){
    //secondly, compare their length
    if(x->length>y->length)
      return -1;
    else if(x->length<y->length)
      return 1;
    else {
      //thirdly, compare their digits
      for(int i=0;i<x->length;i++){
        if(*(x->digit+i)-'0'>*(y->digit+i)-'0'){
          return -1;
        }else if(*(x->digit+i)-'0'<*(y->digit+i)-'0'){
          return 1;
        }
      }
    }
  }
  //otherwise, they are equal
  return 0;
}

int arb_add (arb_int_t x, const arb_int_t y){
  if(x==NULL||y==NULL)
    return 1;
	if(x->digit==NULL||y->digit==NULL)
		return 1;
		
	//same sign, which is the base case
	if(x->sign==y->sign){
		int lx=x->length;
		int ly=y->length;
		int len=lx>ly?lx:ly;
		char*c=(char*)malloc(sizeof(char)*(len+1));
		memset(c,'0',len+1);
		int carry=0,remain=0,a=0,b=0;
		lx--;
		ly--;
		int m=len;
		//use two pointer
		//add every digit, calculate the carry
		while(lx>=0||ly>=0){
			if(lx<0){
				b=*(y->digit+ly)-'0';
				remain=(b+carry)%10;
				carry=(b+carry)/10;
				*(c+m)=remain+'0';
				m--;
				ly--;
			}else if(ly<0){
				a=*(x->digit+lx)-'0';
				remain=(a+carry)%10;
				carry=(a+carry)/10;
				*(c+m)=remain+'0';
				m--;
				lx--;
			}else{
				a=*(x->digit+lx)-'0';
				b=*(y->digit+ly)-'0';
				remain=(a+b+carry)%10;
				carry=(a+b+carry)/10;
				*(c+m)=remain+'0';
				m--;
				lx--;
				ly--;
			}
		}	
		//if there is a carry
		if(carry!=0){
			*(c+m)=carry+'0';
			m--;
		}
		
		free(x->digit);
		x->digit=(char*)malloc(sizeof(char)*(len-m+1));
		memcpy(x->digit,c+m+1,len-m);
		*(x->digit+len-m)='\0';
		free(c);
		x->length=len-m;
	}else if(x->sign=='+'){
	//there cases can be transfered to subtract
		x->sign='-';
		if(arb_compare(x,y)!=0){
			arb_int_t new;
			x->sign='+';
			arb_duplicate(&new,y);
			new->sign='+';
			arb_subtract(x,new);
			arb_free(&new);
		}else{
			x->sign='+';
			x->length=1;
			free(x->digit);
			x->digit=(char*)malloc(sizeof(char)*2);
			*(x->digit)='0';
			*(x->digit+1)='\0';
		}
	}else if(y->sign=='+'){
	//there cases can be transfered to subtract
		x->sign='+';
		if(arb_compare(x,y)!=0){
			arb_int_t new;
			arb_duplicate(&new,y);
			arb_subtract(x,new);
			arb_free(&new);
			x->sign=x->sign=='+'?'-':'+';
		}else{
			x->sign='+';
			x->length=1;
			free(x->digit);
			x->digit=(char*)malloc(sizeof(char)*2);
			*(x->digit)='0';
			*(x->digit+1)='\0';
		}
	}
	return 0;
}

int arb_subtract (arb_int_t x, const arb_int_t y){
	if(x==NULL||y==NULL)
    return 1;
	if(x->digit==NULL||y->digit==NULL)
		return 1;
	
	//the base case
	if(x->sign=='+'&&y->sign=='+'){
		if(arb_compare(x,y)>0){
			char* c=(char*)malloc(sizeof(char)*(x->length));
			memset(c,'0',x->length);
			int lx=x->length-1;
			int ly=y->length-1;
			int a=0,b=0,n=0,carry=0,m=x->length-1;
			//use two pointers
			//subtract every digit
			while(lx>=0){
				if(ly<0){
					a=*(x->digit+lx)-'0';
					n=a-carry;
					if(n<0){
						carry=1;
						*(c+m)=n+10+'0';
					}else{
						carry=0;
						*(c+m)=n+'0';
					}
					m--;
					lx--;
				}else{
					a=*(x->digit+lx)-'0';
					b=*(y->digit+ly)-'0';
					n=a-b-carry;
					if(n<0){
						carry=1;
						*(c+m)=n+10+'0';
					}else{
						carry=0;
						*(c+m)=n+'0';
					}
					m--;
					lx--;
					ly--;
				}
			}
			//find the first non-zero digit
			int i=0;
			for(;i<x->length;i++){
				if(*(c+i)!='0')
					break;
			}
			free(x->digit);
			x->digit=(char*)malloc(sizeof(char)*(x->length-i+1));
			memcpy(x->digit,c+i,x->length-i);
			*(x->digit+x->length-i)='\0';
			free(c);
			x->length=x->length-i;
		}else if(arb_compare(x,y)<0){
		//these cases can be transfered to base case
			arb_int_t new;
			arb_duplicate(&new,y);
			arb_subtract(new,x);
			arb_assign(x,new);
			arb_free(&new);
			x->sign='-';
		}else{
			x->sign='+';
			x->length=1;
			free(x->digit);
			x->digit=(char*)malloc(sizeof(char)*2);
			*(x->digit)='0';
			*(x->digit+1)='\0';
		}
	}else if(x->sign=='+'&&y->sign=='-'){
	//these cases can be transfered to add
		arb_int_t new;
		arb_duplicate(&new,y);
		new->sign='+';
		arb_add(x,new);
		arb_free(&new);
	}else if(x->sign=='-'&&y->sign=='+'){
	//these cases can be transfered to add
		arb_int_t new;
		arb_duplicate(&new,y);
		new->sign='-';
		arb_add(x,new);
		arb_free(&new);
		if(*(x->digit)!='0')
			x->sign='-';
	}else{
	//these cases can be transfered to base case
		if(arb_compare(x,y)>0){
			arb_int_t new;
			arb_duplicate(&new,y);
			new->sign='+';
			x->sign='+';
			arb_subtract(new,x);
			arb_assign(x,new);
			arb_free(&new);
		}else if(arb_compare(x,y)<0){
			arb_int_t new;
			arb_duplicate(&new,y);
			new->sign='+';
			x->sign='+';
			arb_subtract(x,new);
			arb_free(&new);
			x->sign='-';
		}else{
			x->sign='+';
			x->length=1;
			free(x->digit);
			x->digit=(char*)malloc(sizeof(char)*2);
			*(x->digit)='0';
			*(x->digit+1)='\0';
		}
	}
	if(*(x->digit)=='0')
		x->sign='+';
	return 0;
}

int arb_multiply (arb_int_t x, const arb_int_t y){
	if(x==NULL||y==NULL)
		return 1;
	if(x->digit==NULL||y->digit==NULL)
		return 1;
	if(*(x->digit)=='0'){
		x->sign='+';
		return 0;
	}
	//0 cases
	if(*(y->digit)=='0'){
		x->sign='+';
		free(x->digit);
		x->length=1;
		x->digit=(char*)malloc(sizeof(char)*2);
		*(x->digit)='0';
		*(x->digit+1)='\0';
		return 0;
	}
	if(x->sign==y->sign){
		x->sign='+';
	}else{
		x->sign='-';
	}
	//find the longer one
	int bl=x->length>y->length?x->length:y->length;
	int sl=x->length<y->length?x->length:y->length;
	char*c=(char*)malloc(sizeof(char)*(bl+sl));
	memset(c,'0',bl+sl);
	int p=0,q=0,temp=0,a=0,b=0;
	//use the digit mapping to calculate the result
	for(int i=bl-1;i>=0;i--){
		for(int j=sl-1;j>=0;j--){
			p=i+j;
			q=i+j+1;
			if(x->length>y->length)
				temp=(*(x->digit+i)-'0')*(*(y->digit+j)-'0');
			else
				temp=(*(x->digit+j)-'0')*(*(y->digit+i)-'0');
			a=temp%10+(*(c+q)-'0');
			b=a/10+temp/10+(*(c+p)-'0');
			*(c+q)=a%10+'0';
			*(c+p)=b+'0';
		}
	}
	//find the first non-zero fidit
	int i=0;
	for(;i<bl+sl;i++){
		if(*(c+i)!='0')
			break;
	}
	free(x->digit);
	x->digit=(char*)malloc(sizeof(char)*(bl+sl-i+1));
	memcpy(x->digit,c+i,bl+sl-i);
	*(x->digit+bl+sl-i)='\0';
	free(c);
	x->length=bl+sl-i;
	return 0;
}

int arb_divide (arb_int_t x, const arb_int_t y){
	if(x==NULL||y==NULL)
		return 1;
	if(x->digit==NULL||y->digit==NULL)
		return 1;
	if(*(y->digit)=='0')
		return 1;
	if(*(x->digit)=='0'){
		x->sign='+';
		return 0;
	}
	arb_int_t tempy;
	arb_duplicate(&tempy,y);
	arb_int_t temp1;
	arb_from_string(&temp1,"1");
	arb_int_t temp;
	arb_from_string(&temp,"0");
	char s=0;
	if(x->sign==y->sign){
		s='+';
	}else{
		s='-';
	}
	x->sign='+';
	tempy->sign='+';
	//use subtract to get the result
	while(arb_compare(x,tempy)>=0){
		arb_subtract(x,tempy);
		arb_add(temp,temp1);
	}
	arb_assign(x,temp);
	x->sign=s;
	arb_free(&temp);
	arb_free(&tempy);
	arb_free(&temp1);
	return 0;
}

int arb_digit_count (const arb_int_t x){
	int c=x->sign=='+'?0:1;
	c=c+strlen(x->digit);
	return c;
}

#include<stdio.h>

int sum_to_div(int num){

int sum=0;
int i;

for(i=1;i<=num-1;i++){
	if(num%i==0){
		
		sum = sum+i;
	// end if	
	}
	 // end for
	
	 
}
  return sum;
}
   
int main()
{
	int n,i,a,b;
	int total_pair;
	
  	printf("Enter the  no n");
	scanf("%d",&n);
	printf("The no is %d\n",n);
	
	for(i=0;i<=n;i++){
		a=i+1;
		b=sum_to_div(a);
		if(b>a && b<=n){
			
		if(sum_to_div(b)==a){
			
			printf("(%d ,%d)\n",a,b);
			total_pair=total_pair+1;
		}
		}
		
	}
	printf("Total_pair %d",total_pair);
	return 0;
}

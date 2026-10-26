#include<stdio.h>
int main(){
	int n;
	printf("Enter the no ");
	scanf("%d",&n);
	printf("The no is :%d\n ",n);
	
	int i,num_div,sum=0;
	for(i=1;i<=n/2;i++){
		
	if(n%i==0){
	
	sum=sum+i;	
}
	}
	printf("The sum is %d\n",sum);
}

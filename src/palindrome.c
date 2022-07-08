#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);
	
	int tmp=n, rev=0;
	while(tmp>0)
	{
		rev=rev*10+tmp%10;
		tmp/=10;
	}

	if(rev==n){
		printf("Y");
	}else{
		printf("N");
	}
	return 0;
}

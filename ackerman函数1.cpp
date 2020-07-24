#include "stdio.h"
int ack(int m, int n )
{
	int y;
	if( m==0 ){
	
		y = n+1;
	
		}
		
	else if( n==0 )
	{
		y = ack( m-1, 1);
		
		}
	else 
	y = ack( m - 1, ack( m , n - 1) );
	
	return y;		
		
}
int main(void)
{
	int m, n, y;
	scanf("%d %d", &m, &n);
      y = ack( m, n ); 
      printf("%d", y);  
	
	return 0;
	
}

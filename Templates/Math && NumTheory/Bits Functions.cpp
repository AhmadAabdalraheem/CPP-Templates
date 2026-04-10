//set ith bit
int setbit(int x , int i )
{
	return x | (1<<i);
}
//unset ith bit
int removebit(int x , int i )
{
	return x & (~(1<<i));
}
//check the existence of the ith bit
bool checkbit(int x , int i )
{
	return (x>>i) & 1;
}

//flip the bit
int flipbite(int x, int i )
{
	return x ^ (1<<i);
}
//get the first set bit from right    least significant bit
int LSB(int x)
{
	return x & (-x);
}
//get the first set bit from left    most significant bit
int MSB(int x) //O(LOG(N))
{
	int msb = 0;
	while(x)
	{
		 x>>=1; //  x/=2;
		msb++;
	}
	return msb-1;

}
bool ispoweroftwo(int x)
{
	return !( x & (x-1));
}

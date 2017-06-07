#include "deploy.h"
#include <stdio.h>

int n, l, m;
int Cost;
char output[300000]; int cnt;
int Son[505]; int Cap[505];

void P(int x)
{
	if(x == -1)
	{
		output[cnt++] = 10; return;
	}else if(x == -2)
	{
		output[cnt++] = 32; return;
	}else if(x == 0)
	{
		output[cnt++] = '0'; return;
	}
	int a[10]; int cn = 0;
	while(x)
	 {
		a[cn++] = x%10;
		x /= 10;
	}
	for(int i = cn-1; i >= 0; --i) output[cnt++] = a[i]+'0';
}

void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
	int routecnt = 0, usecnt = 0;
	n = 0; m = 0; l = 0;
	Cost = 0;
	cnt = 0;

	for(int i = 0; i < line_num; ++i)
	{
		int tmp = 0; int fl = 0;
		if(!n)
		{
			for(int j = 0; topo[i][j]; ++j)
			{
				if(topo[i][j] >= '0' && topo[i][j] <= '9')
				{
					tmp = tmp*10 + topo[i][j] - '0'; fl = 1;
				}else if(fl) {
					if(!n) n = tmp;
					else if(!l) l = tmp;
					else m = tmp;
					tmp = 0; fl = 0;
				}
			}
		}else if(!Cost)
		{
			for(int j = 0; topo[i][j]; ++j)
			{
				if(topo[i][j] >= '0' && topo[i][j] <= '9') {
					tmp = tmp*10 + topo[i][j] - '0'; fl = 1;
				}else if(fl)
				{
					Cost = tmp;
					tmp = 0; fl = 0;
				}
			}
		}else if(routecnt < l)
		{
			int fr=-1,to=-1,cap=-1,cost=-1;
			for(int j = 0; topo[i][j]; ++j)
			{
				if(topo[i][j] >= '0' && topo[i][j] <= '9')
				{
					tmp = tmp*10 + topo[i][j] - '0'; fl = 1;
				}else if(fl)
				{
					if(fr == -1) fr = tmp;
					else if(to == -1) to = tmp;
					else if(cap == -1) cap = tmp;
					else cost = tmp;
					tmp = 0; fl = 0;
				}
			}
			if(fr != -1)
			 {
				printf("%d %d %d %d\n",fr,to,cap,cost);
				routecnt ++;
			}
		}else if(usecnt < m)
		{
			int fr=-1,to=-1,cap=-1;
			for(int j = 0; topo[i][j]; ++j)
			 {
				if(topo[i][j] >= '0' && topo[i][j] <= '9')
				{
					tmp = tmp*10 + topo[i][j] - '0'; fl = 1;
				}else if(fl)
				{
					if(fr == -1) fr = tmp;
					else if(to == -1) to = tmp;
					else if(cap == -1) cap = tmp;
					tmp = 0; fl = 0;
				}
			}
			if(fr != -1)
			{
				fr ++; to ++;
				usecnt ++;
				Son[fr] = to; Cap[fr] = cap;
			}
		}else break;
	}
//	printf("%d %d %d\n",n,m,l);
//	for(int i = 1; i <= m; ++i) printf("%d ",Son[i]);
	int ans = Cost * m;
	P(ans); P(-1);
	for(int i = 1; i <= m; ++i)
	{
		P(-1);
    P(Son[i]-1); P(-2); P(i-1); P(-2); P(Cap[i]);
	}

	output[cnt] = 0;
	char * topo_file = output;
	write_result(topo_file, filename);
}

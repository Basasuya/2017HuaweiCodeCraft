#include "deploy.h"
#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <queue>
#include <cmath>
#include <ctime>
#include <set>
using namespace std;

const int INF = 0x3f3f3f3f;
typedef long long ll;
const int N = 1e4+5;
const int M = 6e5+5;

/*************you can change this here *******/
double EPS = 1e-8;
double T = 3000;
double DELTA = 0.98;
int timelimit = 88;
double tem;
clock_t stTime, edTime;

int netNum, routeNum, useNum, netLevel;
int Levelcost[15], Levelflow[15];
int netCost[N];
int Son[N]; int Dad[N]; int Need[N];
int netSt[N], useEd[N];
int s,t;
int Maxflow;

int cdn[N];
struct Edge{
	int to,next,cap,flow,cost;
}edge[M], E[M]; // edge 用于测试 E 用于 最后答案
int head[N],tol;
int anscdn[N];
int old[N];

struct Aode{
	int pos,flow,cost;
	Aode(int a=0, int b=0, int c=0):pos(a), flow(b), cost(c){}
	Aode& operator=(const Aode &T) {
		pos = T.pos;
		flow = T.flow;
		cost = T.cost;
		return *this;
	}
}tmpAns;


int cur[N],dis[N];
int vis[N];

int testcdn[N];
pair<double, int> randch[N];
double prefix[N];

vector<int> tmp; // 最后需要输出的答案
char output[500000];
int Ans;
vector<vector<int> > PathAns;

int LevelOfNodes[N];
int maxLevel;

#define LEVEL 7

void Init() {
	netLevel = 0;
	srand(time(NULL));
	stTime = clock();
	Maxflow = 0;
	memset(cdn,0,sizeof(cdn));
	memset(head, -1, sizeof(head)); tol = 0;
	Ans = INF;
	PathAns.clear();
	tmp.clear();
}

void Addedge(int u,int v,int cap,int cost){
	edge[tol].to = v;
	edge[tol].cap = cap;
	edge[tol].cost = cost;
	edge[tol].flow = 0;
	edge[tol].next = head[u];
	head[u] = tol++;
	edge[tol].to = u;
	edge[tol].cap = 0;
	edge[tol].cost = -cost;
	edge[tol].flow = 0;
	edge[tol].next = head[v];
	head[v] = tol++;
}
int aug(int u, int flow) {
	if(u == t) return flow;
	vis[u] = true;
	for(int i = cur[u]; ~i; i = edge[i].next) {
		int v = edge[i].to;
		if(edge[i].cap > edge[i].flow && !vis[v] && dis[u] == dis[v]+edge[i].cost) {
			int tmp = aug(v, min(flow, edge[i].cap - edge[i].flow));
			edge[i].flow += tmp;
			edge[i^1].flow -= tmp;
			cur[u] = i;
			if(tmp) return tmp;
		}
	}
	return 0;
}
bool modify_label() {
	int d = INF;
	for(int u = s; u <= t; ++u) {
		if(vis[u]) {
			for(int i = head[u]; ~i; i = edge[i].next) {
				int v = edge[i].to;
				if(edge[i].cap > edge[i].flow && !vis[v])
					d = min(d, dis[v]+edge[i].cost-dis[u]);
			}
		}
	}
	if(d == INF) return false;
	for(int i = s; i <= t; ++i) {
//    dis[i] = vis[i] == true? dis[i]+d : dis[i];
//    vis[i] = false;

		if(vis[i]) {
			vis[i] = false;
			dis[i] += d;
		}
	}
	return true;
}

int MCMF_TIMES = 0;

void MCMF(int s,int t){
	MCMF_TIMES ++;
	for(int i = 1; i <= netNum; ++i) {
		if(cdn[i]) {
			int idx = netSt[i];
			edge[idx].cap = Levelflow[LEVEL];
		}
		else edge[netSt[i]].cap = 0;
	}
	for(int i = 0; i < tol; ++i) edge[i].flow = 0;

	int flow = 0; int cost = 0;
	for(int i = s; i <= t; ++i) dis[i] = 0;
	while(1) {
		for(int i = s; i <= t; ++i) cur[i] = head[i];
		while(1) {
			for(int i = s; i <= t; ++i) vis[i] = 0;
			int tmp = aug(s, INF);
			if(tmp == 0) break;
			flow += tmp;
			cost += tmp*dis[s];
		}
		if(!modify_label()) break;
	}

	for(int i = 1; i <= netNum; ++i) {
		if(edge[netSt[i]].flow > 0) {
			for(int j = 1; j <= netLevel; ++j) {
				if(Levelflow[j] >= edge[netSt[i]].flow) {
					cost += Levelcost[j]; cost += netCost[i];
					cdn[i] = j;
					break;
				}
			}
		}else cdn[i] = 0;
	}

	int chpos = 0; int chflow = 0;
	for(int i = 1; i <= useNum; ++i) {
		int tmp = edge[useEd[i]].cap - edge[useEd[i]].flow;
		if(tmp > chflow) {
			chflow = tmp; chpos = Dad[i];
		}
	}

	tmpAns = Aode(chpos, flow, cost);
	if(tmpAns.flow != Maxflow) {
		tmpAns.cost += INF;
	}else {
		if(Ans > tmpAns.cost) {
			Ans = tmpAns.cost;
			for(int i = 0; i < tol; ++i) {
				E[i].to = edge[i].to; E[i].next = edge[i].next; E[i].cap = edge[i].cap; E[i].flow = edge[i].flow; E[i].cost = edge[i].cost;
			}
			for(int i = 1; i <= netNum; ++i) anscdn[i] = cdn[i];
		}
	}
}

void findpath(int x, int pre, int flow, int level) {
	if(x == t) {
		PathAns.push_back(tmp);
		return;
	}
	for(int i = head[x]; ~i; i = E[i].next) {
		if( E[i].cost > 0 &&  E[i].to != pre && E[i].flow > 0) {
			int tt = min(E[i].flow, flow);
			E[i].flow -= tt; flow -= tt;
			tmp.push_back(E[i].to-1);
			findpath(E[i].to, x, tt, level);
			tmp.erase(--tmp.end());
		}else if(E[i].to == t) {
			int tt = min(E[i].flow, flow);
			E[i].flow -= tt; flow -= tt;
			tmp.push_back(Son[x]-1);
			tmp.push_back(tt);
			tmp.push_back(level);
			findpath(E[i].to, x, tt, level);
			tmp.erase(--tmp.end());
			tmp.erase(--tmp.end());
			tmp.erase(--tmp.end());
		}
		if(!flow) break;
	}
}

void GetLevelOfNodes() {
	memset(LevelOfNodes,-1,sizeof(LevelOfNodes));
	queue<int> Q;
	for(int i = 1 ; i <= useNum ; i ++) {
		LevelOfNodes[Dad[i]] = 0;
		Q.push(Dad[i]);
	}

	int levels = 0;
	while(!Q.empty()) {
		int len = Q.size();
		printf("%d\n",len);
		levels ++;
		for(int j = 1 ; j <= len ; j ++) {
			int x = Q.front();
			Q.pop();
			for(int i = head[x]; ~i ; i = edge[i].next) {
				int to = edge[i].to;
				if(edge[i].cost <= 0 || to == t || to == s || LevelOfNodes[to] >= 0) continue;
				LevelOfNodes[to] = levels;
				Q.push(to);
			}
		}
	}
	maxLevel = levels;
	puts("End Get Levels");
}

int CalcNodeLevel() {
	double TotalFlow = 0.0f;
	double TotalNetCost = 0.0f;
	for(int i = 1 ; i <= netNum ; i ++) {
		TotalNetCost += netCost[i];
	}

	TotalFlow = Maxflow;

	double AveCost = TotalNetCost / netNum;
	double AveFlow = TotalFlow    / useNum;

	printf("Ave : %.3f %.3f\n",AveFlow,AveCost);

	return 0;
}


int randnumber = 0;

void Transform(int x) {
	int cnt = 0; double all = 0;
	for(int i = head[x]; ~i; i = edge[i].next) {
		int to = edge[i].to;
		if(edge[i].cost <= 0 || to == t || to == s) continue;
		if(LevelOfNodes[to] == 0) {
			randch[++cnt].first = 40.0f;
		}
		else if(LevelOfNodes[to] == 1) {
			randch[++cnt].first = 30.0f;
		}
		else if(LevelOfNodes[to] == 2) {
			randch[++cnt].first = 80.0f;
		}
		else {
			randch[++cnt].first = 20.0f;
		}
		randch[cnt].second = to;
		all += randch[cnt].first;
	}
	for(int i = 1; i <= cnt; ++i) {
		prefix[i] = prefix[i-1] + randch[i].first / all;
	}
	double randnum = (rand() / (RAND_MAX + 1.0));


	int chpos = cnt;
	for(int i = 1; i <= cnt; ++i) {
		if(prefix[i] > randnum) {
			chpos = i; break;
		}
	}

  /*
  int l = 1; int r = cnt;
  while(l <= r) {
    int m = (l+r) >> 1;
    if(prefix[m] >= randnum) r = m-1;
    else l = m+1;
  }
  int chpos = l;
  */

	int pos = randch[chpos].second;
	cdn[x] = 0; cdn[pos] = 1;
}

void GetNext() {
	double choiceoflife = (rand() / (RAND_MAX + 1.0));
	double randnum ;

	double all = 0; int cnt = 0;

	for(int i = 1; i <= netNum; ++i) {
		if(cdn[i]) {
			int idx = netSt[i];
			double t2 = 2 * edge[idx].cap - edge[idx].flow;

			randch[++cnt].first = t2;
			randch[cnt].second = i;
			all += t2;
		}
	}
	prefix[0] = 0;
	for(int i = 1; i <= cnt; ++i) {
		prefix[i] = prefix[i-1] + randch[i].first / all;
	}

	double ratio = tem / T;

	int chpos[5];
  int flag = 0;
	const int NUM = 3;

	for(int j = 0 ; j < NUM ; j ++) {
		if(ratio > j * 1.0f / NUM) {
			randnum = (rand() / (RAND_MAX + 1.0));

			chpos[j] = cnt;
			for(int i = 1; i <= cnt; ++i) {
				if(prefix[i] > randnum) {
					bool si = false;
					for(int k = 0 ; k < j ; k ++) {
						if(i == chpos[k]) {
							si = true;
						}
					}
					if(si) continue;
					chpos[j] = i; break;
				}
			}
      /*
      int l = 1; int r = cnt;
  		while(l <= r) {
  			int m = (l+r) >> 1;
  			if(prefix[m] >= randnum) r = m-1;
  			else l = m+1;
  		}
      chpos[j] = l;
*/
			if(choiceoflife < j * 1.0f / NUM)
			{
        flag = 1;
				cdn[randch[chpos[j]].second] = 0;
			}
		}
	}

	if(!flag) {
		Transform(randch[chpos[0]].second);
	}
	MCMF(s, t);
}

int limit = INF;

void SA() {
	tem = T;
	int P_F = 0;

	int DeleteNodeNums = 0;
	int TransNums = 0;

	int cnt = 0;
	int tt = 0;
	while(1) {
		tt ++;
		if(tt > limit) break;
		edTime = clock();
		if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit ) return;

		for(int  j = 1; j <= netNum; ++j) {
			old[j] = cdn[j];
		}
		Aode oldans = tmpAns;
		int pre = tmpAns.cost;

		GetNext();
		int now = tmpAns.cost;
		double dE = now - pre;

		if(dE < 0)   //如果找到更优值，直接更新
		{
			P_F = 0;
		}
		else {
			cnt ++;
			if(cnt == 5) {
				cnt = 0;
				tem = tem * DELTA;
			}

			for(int j = 1; j <= netNum; ++j) {
				cdn[j] = old[j];
			}
			tmpAns = oldans;
			P_F ++;
		}

		if(P_F == 400)  {
			break;
		}
	}

	printf("%d SA END!!\n",P_F);
	printf("%.3f %d\n",tem,Ans);
}


void solve() {
	int useTimes;
	int useLimit;

	if(netNum < 800){
		useTimes = 6;
		useLimit = 300;
	}else {
		useTimes = 2;
		useLimit = 300;
	}

	for(int i = 1; i <= useNum; ++i) {
		Addedge(Dad[i], t, Need[i], 0); useEd[i] = tol-2;
	}
	for(int i = 1; i <= netNum; ++i) {
		Addedge(s, i, 0, 0); netSt[i] = tol-2;
	}
	for(int i = 1; i <= useNum; ++i) Maxflow += Need[i];

	GetLevelOfNodes();

	double tmpT = 0;
	for(int i = 1; i <= useTimes; ++i) {
		limit = useLimit;
		memset(cdn, 0, sizeof(cdn));
		for(int j = 1; j <= useNum; ++j) {
			cdn[Dad[j]] = 1;
		}
		MCMF(s, t);

		if(netNum < 800) {
			tmpT += 1.0f;
		} else {
			tmpT += tem;
		}
		SA();
		printf("%d\n", Ans);
	}

	T = tmpT / useTimes;
	limit = INF;

	while(1) {
		edTime = clock();
		if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit ) break;

		for(int i = 1; i <= netNum; ++i) cdn[i] = anscdn[i];
		MCMF(s, t);
		SA();
	}

	printf("%d\n", Ans);

	for(int i = 0; i < tol; ++i) {
		edge[i].to = E[i].to; edge[i].next = E[i].next; edge[i].cap = E[i].cap; edge[i].flow = E[i].flow; edge[i].cost = E[i].cost;
	}

	for(int i = head[s]; ~i; i = E[i].next) {
		if(E[i].flow == 0) continue;
		int to = E[i].to;
		tmp.push_back(to-1);
		findpath(to, s, E[i].flow, anscdn[E[i].to]-1); //找输出路径
		tmp.erase(--tmp.end());
	}
}



/***********Init and input and output ************/

void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
	Init();
  sscanf(topo[0], "%d %d %d", &netNum, &routeNum, &useNum);
  netLevel = 0;
  int cc = 2;
  while(topo[cc][0] != 13 && topo[cc][0] != 10) {
    int tmp;
    netLevel ++;
    sscanf(topo[cc], "%d %d %d", &tmp, &Levelflow[netLevel], &Levelcost[netLevel]);
		cc ++;
  }

  cc ++;
  for(int i = 1; i <= netNum; ++i) {
    int tmp;
    sscanf(topo[cc], "%d %d", &tmp, &netCost[i]);
    cc ++;
  }
  cc ++;
  for(int i = 1; i <= routeNum; ++i) {
    int a,b,c,d; sscanf(topo[cc], "%d %d %d %d", &a, &b, &c, &d); a++; b ++; cc ++;
    Addedge(a, b, c, d); Addedge(b, a, c, d);
  }
  cc ++;
  for(int i = 1; i <= useNum; ++i) {
    int a,b,c; sscanf(topo[cc], "%d %d %d",&a,&b,&c); ++a; ++b; cc ++;
    Son[b] = a; Need[a] = c; Dad[a] = b;
  }
  s = 0; t = netNum+1;
  solve();
	printf("%d\n",Ans);

  int off = 0;
  off += sprintf(output+off,"%d\n",PathAns.size());
  for(int i = 0; i < PathAns.size(); ++i) {
    off += sprintf(output+off, "\n");
    for(int j = 0; j < PathAns[i].size(); ++j) {
      if(j) off += sprintf(output+off, " ");
      off += sprintf(output+off, "%d",PathAns[i][j]);
    }
  }
  char * topo_file = output;
	write_result(topo_file, filename);

}

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
int ILOOP = 1500;
int OLOOP = 350;
double T = 3000;
double DELTA = 0.98;
double basarand = 1.0;
int LIMIT = 78;
int timelimit = 80;
double tem;
clock_t stTime, edTime;

int netNum, routeNum, useNum, netLevel;
int Levelcost[15], Levelflow[15];
int netCost[N];
int Son[N]; int Dad[N]; int Need[N];
int netSt[N], useEd[N];
int s,t;
int Maxflow;
int recall;
int cdn[N];
struct Edge{
	int to,next,cap,flow,cost;
}edge[M], E[M]; // edge 用于测试 E 用于 最后答案
int head[N],tol;
int anscdn[N];
struct Aode{
	int pos,flow,cost;
	Aode(int a=0, int b=0, int c=0):pos(a), flow(b), cost(c){}
}tmpAns;
int cur[N],dis[N];
int vis[N];

pair<double, int> randch[N];
double prefix[N];

vector<int> tmp; // 最后需要输出的答案
char output[500000];
int Ans;
vector<vector<int> > PathAns;

void Init() {
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
    if(vis[i]) {
      vis[i] = false;
      dis[i] += d;
    }
  }
  return true;
}

void MCMF(int s,int t){
	for(int i = 1; i <= netNum; ++i) {
		if(cdn[i]) {
			edge[netSt[i]].cap = Levelflow[netLevel];
		}else edge[netSt[i]].cap = 0;
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
			tmpAns.cost += recall;
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
int chooserand(int cnt, double all) {
	prefix[0] = 0;
	for(int i = 1; i <= cnt; ++i) {
		prefix[i] = prefix[i-1] + randch[i].first/ all;
	}
	double randnum = (rand() / (RAND_MAX + 1.0));
	int l = 1; int r = cnt;
	while(l <= r) {
		int m = (l+r) >> 1;
		if(prefix[m] >= randnum) r = m-1;
		else l = m+1;
	}
	return randch[l].second;
}
void Transform(int x) {
	int cnt = 0; double all = 0;
	for(int i = head[x]; ~i; i = edge[i].next) {
		int to = edge[i].to;
		if(edge[i].cost <= 0 || to == t || to == s) continue;
		double t2 = 1.0;
		randch[++cnt].first = t2; randch[cnt].second = to; all += t2;
	}
	int pos = chooserand(cnt, all);
	cdn[x] = 0; cdn[pos] = 1;
}
void GetNext() {
	if(tmpAns.flow != Maxflow) {
		cdn[tmpAns.pos] = 1;
	}else {
		double all = 0; int cnt = 0;
	  for(int i = 1; i <= netNum; ++i) {
	    if(cdn[i]) {
				double t2 = INF - edge[netSt[i]].flow;
	      randch[++cnt].first = t2; randch[cnt].second = i; all += t2;
	    }
	  }
		int pos = chooserand(cnt, all);
		Transform(pos);
	}
	MCMF(s, t);
}



void SA() {
	tem = T;
	int P_L = 0;
	int P_F = 0;
  int old[1005];

	while(1)       //外循环，主要更新参数t，模拟退火过程
	{
		edTime = clock();
		if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit ) return;

		for(int i = 0; i < ILOOP; i++)    //内循环，寻找在一定温度下的最优值
		{
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
				P_L = 0;
				P_F = 0;
			}else
			{
				if(exp(dE / tem) < basarand) {
				}  //如果找到比当前更差的解，以一定概率接受该解，并且这个概率会越来越小
				else {
					for(int j = 1; j <= netNum; ++j) {
            cdn[j] = old[j];
          }
					tmpAns = oldans;
        }
				P_L++;
			}
			if(P_L > LIMIT)
			{
				P_F++;
				break;
			}
		}
		if(P_F > OLOOP || tem < EPS) {
			break;
		}
		tem *= DELTA;
	}
	printf("%d %d %.3f %d\n",P_L, P_F, tem,Ans);
}

void solve() {
	if(netNum < 200) {
    EPS = 1e-8;
    ILOOP = 1500;
    OLOOP = 350;
    T = 1000;
    DELTA = 0.98;
    basarand = 1.0;
    LIMIT = 78;
  }else if(netNum <= 350) {
    EPS = 1e-8;
    ILOOP = 1500;
    OLOOP = 230;
    T = 3000;
    DELTA = 0.98;
    basarand = 1.0;
    LIMIT = 50;
  	}else {
    	EPS = 1e-8;
    	ILOOP = 1500;
    	OLOOP = 100;
    	T = 2000;
    	DELTA = 0.98;
    	basarand = 1.0;
    	LIMIT = 25;
  	}

  for(int i = 1; i <= useNum; ++i) {
		Addedge(Dad[i], t, Need[i], 0); useEd[i] = tol-2;
	}
	for(int i = 1; i <= netNum; ++i) {
		Addedge(s, i, 0, 0); netSt[i] = tol-2;
	}
	for(int i = 1; i <= useNum; ++i) Maxflow += Need[i];
	recall = 0; int tmp = 0;
	for(int i = 1; i <= netLevel; ++i) {
		recall += Levelcost[i];
	}
	for(int i = 1; i <= netNum; ++i) {
		tmp += netCost[i];
	}
	tmp /= netNum;
	recall /= netLevel;
	recall += tmp;

  timelimit = 85;

	while(1) {
		edTime = clock();
		if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit ) break;
		memset(cdn, 0, sizeof(cdn));
		for(int i = 1; i <= useNum; ++i) {
			cdn[Dad[i]] = 1;
		}
		MCMF(s, t);
		SA();
	}

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
int success = 1; int shouldAns = 0;
int checkflow[N];
void check(vector<int> path) {
	int tmp = 0;
	int flow = path[path.size()-2];
	int level = path[path.size()-1]+1;
	int tt = path[0]+1;
	checkflow[tt] += flow;
	if(checkflow[tt] > Levelflow[level]) {
		printf("wa at overflow\n");
		success &= 0;
	}
	for(int i = 0; i < path.size() - 3; ++i) {
		int fr = path[i]+1; int to = path[i+1]+1;
		if(i == path.size()-4) to = t;
		int ok = 0;
		for(int j = head[fr]; ~j; j = edge[j].next) {
			int tt = edge[j].to;
			if(tt == to && edge[j].cost >= 0) {
				if(edge[j].flow >= flow) {
					tmp += edge[j].cost;
					ok = 1; edge[j].flow -= flow;
				}
			}
			if(ok) break;
		}
		if(!ok) {
			printf("wa at %d to %d\n", fr, to);
			success &= 0; break;
		}
	}
	shouldAns += flow*tmp;
	return;
}
int main() {
	Init();
	freopen("/home/basasuya/case_example/0/case0.txt", "r", stdin);
	while(~scanf("%d %d %d",&netNum, &routeNum, &useNum)) {
		netLevel = 0;
		int cn = 0;
		char input[105];
    while(gets(input)) {
			if( input[0] == 13 || input[0] == 10 ) {
				cn ++;
				if(cn <= 2) continue;
				else break;
			}
			int tmp;
			netLevel ++;
			sscanf(input, "%d %d %d", &tmp, &Levelflow[netLevel], &Levelcost[netLevel]);
		}

		for(int i = 1; i <= netNum; ++i) {
			int tmp; scanf("%d %d", &tmp, &netCost[i]);
		}

    for(int i = 1; i <= routeNum; ++i) {
			int a,b,c,d; scanf("%d %d %d %d",&a,&b,&c,&d); a++; b ++;
			Addedge(a, b, c, d); Addedge(b, a, c, d);
		}
		for(int i = 1; i <= useNum; ++i) {
			int a,b,c; scanf("%d %d %d",&a,&b,&c); ++a; ++b;
      Son[b] = a; Need[a] = c; Dad[a] = b;
		}
		s = 0; t = netNum+1;
		solve();
		memset(cdn, 0, sizeof(cdn));

		for(int i = 1; i <= netNum; ++i) {
			if(anscdn[i]) {
				shouldAns += netCost[i];
				shouldAns += Levelcost[anscdn[i]];
			}
		}
		memset(checkflow, 0, sizeof(checkflow));
		for(int i = 0; i < PathAns.size(); ++i) check(PathAns[i]);
		success &= (Ans == shouldAns);
		printf("your Answer is %s\n",success?"Yes":"No");
		printf("%d\n", Ans);

		int off = 0;
		off += sprintf(output+off,"%d\n",Ans);
		for(int i = 0; i < PathAns.size(); ++i) {
			off += sprintf(output+off, "\n");
			for(int j = 0; j < PathAns[i].size(); ++j) {
				if(j) off += sprintf(output+off, " ");
				off += sprintf(output+off, "%d",PathAns[i][j]);
			}
		}
	//	printf("%s\n", output);
	}
	return 0;
}

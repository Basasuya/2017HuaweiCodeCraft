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
const double eps = 1e-8;
typedef long long ll;

/*************you can change this here *******/
double EPS = 1e-8;
int ILOOP = 1500;
int OLOOP = 50;
double T = 3000;
double DELTA = 0.98;
int LIMIT = 10;
double tem;
int timelimit = 160;
clock_t stTime, edTime;
int netNum, routeNum, useNum;
int Maxflow; // 满流大小
int netCost;
int s, t; // 开始点 结束点
char output[500000]; int outcnt;
int Son[1005]; int Dad[505]; int Need[505];
int cdn[1005]; //网络节点是否被选做 视频节点
int netSt[1005]; // 记录s所连节点的位置
int useEd[505]; // 记录t所连节点的位置
int Ans;
struct Hode{  // 用于Hash
	int pos,flow,cost;
	Hode(int x=0, int y=0, int z=0):pos(x), flow(y), cost(z){}
	bool operator < (const Hode &T) const {
		if(pos != T.pos) return pos < T.pos;
		else if(flow != T.flow) return flow < T.flow;
		else return cost < T.cost;
	}
	Hode& operator=(const Hode &T) {
		pos = T.pos;
		flow = T.flow;
		cost = T.cost;
		return *this;
	}
};
Hode tmpAns;
vector<vector<int> > PathAns;
struct Edge{
	int to,next,cap,flow,cost;
}edge[40005], E[40005]; // edge 用于测试 E 用于 最后答案
int head[1005],tol;
int pre[1005],dis[1005];
int vis[1005];
int Hashtime = 0;
map<Hode, Hode> mp;
Hode nowHash;
vector<int> tmp; // 最后需要输出的答案
int D[1005][1005];
int DistoCen[1005];
pair<double, int> randch[505];
double prefix[505];

void Init() {
  memset(D,INF,sizeof(D));
	netNum = -1; useNum = -1;  routeNum = -1;
	netCost = -1;
	Maxflow = 0;
	memset(cdn,0,sizeof(cdn));
	memset(head, -1, sizeof(head)); tol = 0;
	outcnt = 0;
	Ans = INF;
	PathAns.clear();
	mp.clear();
	tmp.clear();
	s = 0; t = netNum+1;
}
int Hash(){
	int P1=31, D1=1000000009, P2=151, D2=100000007, P3=29, D3 = 1000003;
	int has1 = 0, has2 = 0, has3 = 0;
	int num = 0;
	for(int i = 1; i <= netNum; ++i) {
		if(cdn[i]) {
			num ++;
			has1 = (1ll*has1*P1 + i) % D1;
			has2 = (1ll*has2*P2 + i) % D2;
			has3 = (1ll*has3*P3 + i) % D3;
		}
	}
	if( mp.find(Hode(has1, has2, has3)) == mp.end()) {
		nowHash = Hode(has1, has2, has3); return 1;
	}else {
		nowHash = mp[Hode(has1, has2, has3)]; return 0;
	}
}
void Addedge(int u,int v,int cap,int cost)
{
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
bool spfa(int s,int t)
{
	queue<pair<int, int> >q;
	for(int i = s;i <= t;i++)
	{
		dis[i] = INF;
		vis[i] = false;
		pre[i] = -1;
	}
	dis[s] = 0;
	vis[s] = true;
	q.push(make_pair(s, s));
	while(!q.empty())
	{
		int u = q.front().first; int Pre = q.front().second;
		q.pop();
		vis[u] = false;
		for(int i = head[u]; i != -1; i = edge[i].next)
		{
			int v = edge[i].to;
			//    printf("from %d to %d\n", u, v);
			if(edge[i].cap > edge[i].flow &&
					dis[v] > dis[u] + edge[i].cost )
			{
				dis[v] = dis[u] + edge[i].cost;
				pre[v] = i;
				if(!vis[v])
				{
					vis[v] = true;
					q.push(make_pair(v, u));
				}
			}
		}
	}
	if(pre[t] == -1 )return false;
	else return true;
}
void MCMF(int s,int t)
{

	int judge = Hash();
	if(!judge) {
		tmpAns = nowHash;
		return;
	}
	int num = 0;
	for(int i = 1; i <= netNum; ++i) {
		if(cdn[i]) {
			num ++;
			edge[netSt[i]].cap = INF;
		}else edge[netSt[i]].cap = 0;
	}
	for(int i = 0; i < tol; ++i) edge[i].flow = 0;
//	for(int i = 1; i <= netNum; ++i) if(cdn[i]) printf("%d ", i); printf("\n");
	int flow = 0; int cost = 0;
	while(spfa(s,t))
	{
		int Min = INF;
		for(int i = pre[t];i != -1;i = pre[edge[i^1].to])
		{
			if(Min > edge[i].cap - edge[i].flow)
				Min = edge[i].cap - edge[i].flow;
		}

		for(int i = pre[t];i != -1;i = pre[edge[i^1].to])
		{
			edge[i].flow += Min;
			edge[i^1].flow -= Min;
			cost += edge[i].cost * Min;
		}
		flow += Min;
	}

	for(int i = 1; i <= netNum; ++i) {
		if(edge[netSt[i]].flow > 0) {
			cdn[i] = 1; cost += netCost;
		}else cdn[i] = 0;
	}
	int chpos = 0;
	int allflow = 0;
	int chflow = -1;
	for(int i = 1; i <= useNum; ++i) {
		int tmp = edge[useEd[i]].cap - edge[useEd[i]].flow;
		if(tmp > chflow) {
			allflow += tmp;
			chflow = tmp; chpos = Dad[i];
		}
	}
	tmpAns = Hode(chpos, flow, cost);
	if(tmpAns.flow != Maxflow) {
			tmpAns.cost += 400;
	}
	else if(Ans > tmpAns.cost) {
		Ans = tmpAns.cost;
		for(int i = 0; i < tol; ++i) {
			E[i].to = edge[i].to; E[i].next = edge[i].next; E[i].cap = edge[i].cap; E[i].flow = edge[i].flow; E[i].cost = edge[i].cost;
		}
	}

	mp[nowHash] = tmpAns;
}
double bfs(int x) {
	memset(vis, 0, sizeof(vis));
	queue<pair<int, int> > Q;
	int t1 = 0; int t2 = 0;
	Q.push(make_pair(x, 0));
	vis[x] = 1;
	while(!Q.empty()) {
		int x = Q.front().first; int flor = Q.front().second; Q.pop();
		t2 ++;
		if(cdn[x]) t1 ++;
		if(flor > 5) break;
		for(int i = head[x]; ~i; i = edge[i].next) {
			int to = edge[i].to;
			if(to != s && to != t && !vis[to] && edge[i].cost > 0) {
				vis[to] = 1; Q.push(make_pair(to, flor+1));
			}
		}
	}
	return t1*1.0 / t2;
}

void findpath(int x, int pre, int flow) {
	if(x == t) {
		PathAns.push_back(tmp);
		return;
	}
	for(int i = head[x]; ~i; i = E[i].next) {
		if( E[i].cost > 0 &&  E[i].to != pre && E[i].flow > 0) {
			int tt = min(E[i].flow, flow);
			E[i].flow -= tt; flow -= tt;
			tmp.push_back(E[i].to-1);
			findpath(E[i].to, x, tt);
			tmp.erase(--tmp.end());
		}else if(E[i].to == t) {
			int tt = min(E[i].flow, flow);
			E[i].flow -= tt; flow -= tt;
			tmp.push_back(Son[x]-1);
			tmp.push_back(tt);
			findpath(E[i].to, x, tt);
			tmp.erase(--tmp.end());
			tmp.erase(--tmp.end());
		}
		if(!flow) break;
	}
}
void floyd() {
  for(int k = 1; k <= netNum; ++k) {
    for(int i = 1; i <= netNum; ++i) {
      for(int j = 1; j <= netNum; ++j) {
        D[i][j] = min(D[i][j], D[i][k] + D[k][j]);
      }
    }
  }
}
void Transform(int x) {
	int cnt = 0; double all = 0;
	for(int i = head[x]; ~i; i = edge[i].next) {
		int to = edge[i].to;
		if(edge[i].cost <= 0 || to == t || to == s) continue;
		randch[++cnt].first = 1; randch[cnt].second = to; all += 1;
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
	int pos = randch[chpos].second;
	cdn[x] = 0; cdn[pos] = 1;
}


void GetNext() {
	int fl = 0;

	if(tmpAns.flow != Maxflow) {
		double randnum = (rand() / (RAND_MAX + 1.0));
		int chnum = 1.0*(Maxflow - tmpAns.flow) / Maxflow;
		if(1 > randnum) {
			int tt = Hash();
			fl = 1;
			cdn[nowHash.pos] = 1;
		}
	}

	if(fl == 0){
	  double all = 0; int cnt = 0;
	  for(int i = 1; i <= netNum; ++i) {
	    if(cdn[i]) {
				double t2 = edge[netSt[i]].cap - edge[netSt[i]].flow;
				double tt = t2;
	      all += tt;
	      randch[++cnt].first = tt; randch[cnt].second = i;
	    }
	  }

	  prefix[0] = 0;
	  for(int i = 1; i <= cnt; ++i) {
	    prefix[i] = prefix[i-1] + randch[i].first/ all;
	  }

		double randnum = (rand() / (RAND_MAX + 1.0));
	  int chpos = cnt;
	  for(int i = 1; i <= cnt; ++i) {
	    if(prefix[i] > randnum) {
	      chpos = i; break;
	    }
	  }
	  Transform(randch[chpos].second);
	}
	MCMF(s, t);
}
int all = 0;

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
			Hode oldans = tmpAns;
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
				 double rd = rand() / (RAND_MAX + 1.0);
			//	double rd = 0.01;
				if(exp(dE / tem) < 1) {
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
			all ++;
			break;
		}
		tem *= DELTA;
	}
	printf("%d %d %.3f\n", P_L,P_F,tem);
}
void solve() {
	for(int i = 1; i <= netNum; ++i) {
		D[i][i] = 0;
	}
  for(int i = 1; i <= useNum; ++i) {
		Addedge(Dad[i], t, Need[i], 0); useEd[i] = tol-2;
	}
	for(int i = 1; i <= netNum; ++i) {
		Addedge(s, i, 0, 0); netSt[i] = tol-2;
	}
	for(int i = 1; i <= useNum; ++i) Maxflow += Need[i];
	/*
  floyd();
  for(int i = 1; i <= netNum; ++i) {
    DistoCen[i] = INF;
    for(int j = 1; j <= useNum; ++j) {
      DistoCen[i] = min(DistoCen[i], D[Dad[j]][i]);
    }
  }*/
	int tt = 0;
	while(1) {
		tt ++;
		edTime = clock();
		if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit ) break;
		memset(cdn, 0, sizeof(cdn));
		for(int i = 1; i <= useNum; ++i) {
			cdn[Dad[i]] = 1; edge[netSt[Dad[i]]].cap = INF;
		}
		MCMF(s, t);
		SA();
	}
	printf("%d %d\n", tt, all);
	printf("end\n");
	for(int i = 0; i < tol; ++i) {
		edge[i].to = E[i].to; edge[i].next = E[i].next; edge[i].cap = E[i].cap; edge[i].flow = E[i].flow; edge[i].cost = E[i].cost;
	}
	for(int i = head[s]; ~i; i = E[i].next) {
		if(E[i].flow == 0) continue;
		int to = E[i].to;
		tmp.push_back(to-1);
		findpath(to, s, E[i].flow); //找输出路径
		tmp.erase(--tmp.end());
	}
}
/***********Init and input and output ************/

int success = 1; int shouldAns = 0;
void check(vector<int> path) {
	int tmp = 0;
	int flow = path[path.size()-1];
	int tt = path[0]+1;
	if(!cdn[tt]) {
		cdn[tt] = 1;
		shouldAns += netCost;
	}
	for(int i = 0; i < path.size() - 2; ++i) {
		int fr = path[i]+1; int to = path[i+1]+1;
		if(i == path.size()-3) to = t;
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

int main(){
	srand(time(NULL));
	stTime = clock();
	Init();
	freopen("/home/basasuya/case_example/3/case7.txt","r",stdin);
	while(~scanf("%d %d %d",&netNum, &routeNum, &useNum)) {
		s = 0; t = netNum + 1;
		scanf("%d",&netCost);
		for(int i = 1; i <= routeNum; ++i) {
			int a,b,c,d; scanf("%d %d %d %d",&a,&b,&c,&d); a++; b ++;
			Addedge(a, b, c, d); Addedge(b, a, c, d);
      D[a][b] = d; D[b][a] = d;
		}
		for(int j = 1; j <= useNum; ++j) {
			int a,b,c; scanf("%d %d %d",&a,&b,&c); ++b; ++a;
			Son[b] = a; Need[a] = c; Dad[a] = b;
		}

		solve();

		memset(cdn, 0, sizeof(cdn));
		for(int i = 0; i < PathAns.size(); ++i) check(PathAns[i]);
		success &= (Ans == shouldAns);
		printf("shouans = %d your Answer is %d\n", shouldAns,success);

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

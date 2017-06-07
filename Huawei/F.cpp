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

/*************you can change this here *******/
int timelimit;
int netNum, routeNum, useNum;
int Maxflow; // 满流大小
int netCost;
int s, t; // 开始点 结束点
char output[500000]; int outcnt;

int cnt[1005];

clock_t stTime, edTime;
double WeiPoint[1005];
struct Hode{
	int a,b,c;
	Hode(int x=0, int y=0, int z=0):a(x), b(y), c(z){}
	bool operator < (const Hode & T) const {
		if(a != T.a) return a < T.a;
		else if(b != T.b) return b < T.b;
		else return c < T.c;
	}
};
int Son[1005]; int Dad[505]; int Need[505];
int cdn[1005];
int netSt[1005]; // 记录s所连节点的位置
int useEd[505]; // 记录t所连节点的位置
int Ans;
pair<int, int> tmpAns;
vector<vector<int> > PathAns;
struct Edge{
	int to,next,cap,flow,cost;
}edge[40005], E[40005]; // edge 用于测试 E 用于 最后答案
int head[1005],tol;
int pre[1005],dis[1005];
int vis[1005];
int Hashtime = 0;
set<Hode> st;
vector<int> tmp; // 最后需要输出的答案
vector<int> Prenum[505];
vector<int> Chnum[505];

bool cmp(int a,int b) {
  return WeiPoint[a] < WeiPoint[b];
}

void Init() {
  memset(WeiPoint, 0, sizeof(WeiPoint));
	netNum = -1; useNum = -1;  routeNum = -1;
	netCost = -1;
	Maxflow = 0;
	memset(head, -1, sizeof(head)); tol = 0;
	outcnt = 0;
	Ans = INF;
	s = 0; t = netNum+1;
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
void MCMF(int s,int t, int initcost)
{
	int flow = 0; int cost;
	cost = initcost;
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
		if(netCost < Min*dis[t] || cost > Ans) {
      tmpAns = make_pair(INF, INF); return;
    }
	}
	tmpAns =  make_pair(flow,cost);
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
int Hash(){
	int P1=31, D1=1000000009, P2=151, D2=100000007, P3=29, D3 = 1000003;
	int has1 = 0, has2 = 0, has3 = 0;
	for(int i = 1; i <= netNum; ++i) {
		if(cdn[i]) {
			has1 = (1ll*has1*P1 + i) % D1;
			has2 = (1ll*has2*P2 + i) % D2;
			has3 = (1ll*has3*P3 + i) % D3;
		}
	}
	if( st.find(Hode(has1, has2, has3)) == st.end()) {
		st.insert(Hode(has1, has2, has3));
		return 1;
	}else {
		Hashtime ++;
		return 0;
	}
}

void Evaluate(int id) {
  memset(vis, 0, sizeof(vis));
  queue<pair<int,int> > Q;
  WeiPoint[Dad[id]] += INF;
  Q.push(make_pair(1, Dad[id]));
  vis[Dad[id]] = 1;

  while(!Q.empty()) {
    int flor = Q.front().first; int x =  Q.front().second;
    WeiPoint[x] ++; Prenum[id].push_back(x);
    if(flor > 3) break;
    Q.pop();
    for(int i = head[x]; ~i; i = edge[i].next) {
      int to = edge[i].to;
      if(!vis[to] && to != s && to != t && edge[i].cost < 0) {
        vis[to] = 1;

        Q.push(make_pair(flor+1, to));
      }
    }
  }
}
void solve() {
  timelimit = 80;
  for(int i = 1; i <= useNum; ++i) {
		Addedge(Dad[i], t, Need[i], 0); useEd[i] = tol-2;
	}
  for(int i = 1; i <= useNum; ++i) Maxflow += Need[i];
	for(int i = 1; i <= netNum; ++i) {
		Addedge(s, i, 0, 0); netSt[i] = tol-2;
	}

  for(int i = 1; i <= useNum; ++i) {
    Evaluate(i);
  }

  for(int i = 1; i <= useNum; ++i) {
    sort(Prenum[i].begin(), Prenum[i].end(), cmp);
    for(int j =  min((int)Prenum[i].size()-1, (int)sqrt(netNum)); j >= 0; --j) {
      Chnum[i].push_back(Prenum[i][j]);
    }
  }
  for(int i = 1; i <= useNum; ++i) {
    for(int j = 0; j < Chnum[i].size(); ++j) {
      printf("%d ",Chnum[i][j]);
    }
    printf("\n");
  }
  while(1) {
    edTime = clock();
    if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) break;
    for(int i = 1; i <= netNum; ++i) {
      edge[netSt[i]].cap = 0; cdn[i] = 0;
    }
    int initcost = 0;
    for(int i = 1; i <= useNum; ++i) {
      int len = Chnum[i].size();
      int pos;
      int tt = rand() % (len*(len+1)/2);
      for(int j = 1; j <= len; ++j) {
        if(j*(j+1)/2 > tt) {
          pos = j-1; break;
        }
      }
      int x = Chnum[i][pos];
      if(!cdn) initcost += netCost;
      edge[netSt[x]].cap = INF; cdn[x] = 1;
    }
    if(!Hash())  continue;

    for(int i = 0; i < tol; ++i) edge[i].flow = 0;
    MCMF(s, t, initcost);
    if(tmpAns.first != Maxflow || tmpAns.second == INF) continue;
    for(int i = 1; i <= netNum; ++i) {
      if(edge[netSt[i]].flow) tmpAns.second += netCost;
    }
    if(Ans > tmpAns.second) {
      printf("hh\n");
      Ans = tmpAns.second;
      for(int i = 0; i < tol; ++i) {
  			E[i].to = edge[i].to; E[i].next = edge[i].next; E[i].cap = edge[i].cap; E[i].flow = edge[i].flow; E[i].cost = edge[i].cost;
  		}
    }
  }
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


/*************you can change this here *******/
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
					//  printf("%d %d %d %d\n",fr,to,edge[j].cap,edge[j].flow);
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


void Print(int x) {
	if(x == -1) {
		output[outcnt++] = 10; return;
	}else if(x == -2) {
		output[outcnt++] = 32; return;
	}else if(x == 0) {
		output[outcnt++] = '0'; return;
	}
	vector<int> vc; vc.clear();
	while(x) {
		vc.push_back(x%10);
		x /= 10;
	}
	for(int i = vc.size()-1; i >= 0; --i) output[outcnt++] = vc[i]+'0';
}
int main(){
	srand(time(NULL));
	stTime = clock();
	Init();
	freopen("/home/basasuya/case_example/2/case8.txt","r",stdin);
	while(~scanf("%d %d %d",&netNum, &routeNum, &useNum)) {
		s = 0; t = netNum + 1;
		scanf("%d",&netCost);
		for(int i = 1; i <= routeNum; ++i) {
			int a,b,c,d; scanf("%d %d %d %d",&a,&b,&c,&d); a++; b ++;
			Addedge(a, b, c, d); Addedge(b, a, c, d);
		}
		for(int j = 1; j <= useNum; ++j) {
			int a,b,c; scanf("%d %d %d",&a,&b,&c); ++b; ++a;
			Son[b] = a; Need[a] = c; Dad[a] = b;
		}

		solve();

		memset(cdn, 0, sizeof(cdn));
		for(int i = 0; i < PathAns.size(); ++i) check(PathAns[i]);
		success &= (Ans == shouldAns);
    printf("hash for %d times\n", Hashtime);
		printf("your Answer is %d\n", success);
		edTime = clock();
		printf("use time: %d\n", (int)(edTime - stTime)/CLOCKS_PER_SEC);
		Print(Ans); Print(-1);
		for(int i = 0; i < PathAns.size(); ++i) {
			Print(-1);
			int last;
			for(int j = 0; j < PathAns[i].size(); ++j) {
				if(j) Print(-2);
				Print(PathAns[i][j]);
			}
		}
		for(int i = 0; i < outcnt; ++i) printf("%c", output[i]); printf("\n");
	}
	return 0;
}

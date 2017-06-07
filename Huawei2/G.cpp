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
int OLOOP = 100;
double T = 3000;
double DELTA = 0.98;
double basarand = 1.0;
int LIMIT = 25;
int numofall = 300;
int changeornot = 1;
int acrosstime = 3;
int choftransform = 2500;

double tem;
int timelimit = 80;
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
int cur[1005],dis[1005];
int vis[1005];
int Hashtime = 0;
map<Hode, Hode> mp;
Hode nowHash;
vector<int> tmp; // 最后需要输出的答案

pair<double, int> randch[505];
double prefix[505];
struct Gode{
  int cdn[1005];
  int flag; int cost;
  bool operator <(const Gode &T) const{
    return cost < T.cost;
  }
  Gode& operator=(const Gode &T) {
    for(int i = 1; i <= netNum; ++i) cdn[i] = T.cdn[i];
    flag = T.flag;
    cost = T.cost;
    return *this;
  }
};
priority_queue<Gode> Eva3;
Gode gene[1005]; int tot;
int chosen[1005][1005];
int chtodo[505];
int saok[505][1005];

void Init() {
	memset(saok, 0, sizeof(saok));
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
  while(!Eva3.empty()) Eva3.pop();
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
void MCMF(int s,int t)
{
	memset(cdn, 0, sizeof(cdn));
	for(int i = 1; i <= useNum; ++i) {
		cdn[chtodo[i]] ++;
	}
	/*
	int judge = Hash();
	if(!judge) {
		tmpAns = nowHash;
    Gode tmp;
    for(int i = 1; i <= netNum; ++i) {
      tmp.cdn[i] = cdn[i];
    }
    tmp.flag = tmpAns.flow == Maxflow; tmp.cost = tmpAns.cost;
    if(Eva3.size() == numofall) {
      Eva3.push(tmp);
      Eva3.pop();
    }else Eva3.push(tmp);
		return;
	}*/

	int num = 0;
	for(int i = 1; i <= netNum; ++i) {
		if(cdn[i]) {
			num ++;
			edge[netSt[i]].cap = INF;
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
			cdn[i] = 1; cost += netCost;
		}else cdn[i] = 0;
	}
	int chpos = 0;
	int chflow = 0;
	for(int i = 1; i <= useNum; ++i) {
		int tmp = edge[useEd[i]].cap - edge[useEd[i]].flow;
		if(tmp > chflow) {
			chflow = tmp; chpos = i;
		}
	}
	tmpAns = Hode(chpos, flow, cost);
	if(tmpAns.flow != Maxflow) {
			tmpAns.cost += netCost;
	}
	else {
    if(Ans > tmpAns.cost) {
		  Ans = tmpAns.cost;
		  for(int i = 0; i < tol; ++i) {
			  E[i].to = edge[i].to; E[i].next = edge[i].next; E[i].cap = edge[i].cap; E[i].flow = edge[i].flow; E[i].cost = edge[i].cost;
		  }
	  }
  }
//	mp[nowHash] = tmpAns;

  Gode tmp;
  for(int i = 1; i <= netNum; ++i) {
    tmp.cdn[i] = cdn[i];
  }
  tmp.flag = tmpAns.flow == Maxflow; tmp.cost = tmpAns.cost;
  if(Eva3.size() == numofall) {
    Eva3.push(tmp);
    Eva3.pop();
  }else Eva3.push(tmp);
}

void Choose() {
  double all = 0;
  for(int i = 1; i <= tot; ++i) {
    all = all + 1.0/gene[i].cost;
  }
	for(int i = 1; i <= tot; ++i) {
		prefix[i] = 1.0/gene[i].cost/all;
	}
  prefix[0] = 0;
	for(int i = 1; i <= tot; ++i) {
		prefix[i] += prefix[i-1];
	}

  for(int i = 1; i <= tot; ++i) {
    double randnum = (rand() / (RAND_MAX + 1.0));
		int chpos = tot;
	  for(int j = 1; j <= tot; ++j) {
	    if(prefix[j] > randnum) {
	      chpos = j; break;
	    }
	  }
    randnum = (rand() / (RAND_MAX + 1.0));
    if(gene[chpos].flag == 0 && randnum < 0.5) {
      --i; continue;
    }
    for(int j = 1; j <= netNum; ++j) chosen[i][j] = gene[chpos].cdn[j];
  }
}
void Across() {
  for(int i = 1; i <= tot*acrosstime; ++i) {
		edTime = clock();
  	if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) return;

    int a = rand() % tot+1; int b = rand() % tot+1;
    if(a == b) {
      --i; continue;
    }
    int edge = rand()% netNum + 1;
    for(int j = 1; j <= netNum; ++j) {
      int tt;
      if(j <= edge) {
        tt = chosen[a][j];
      }else tt = chosen[b][j];
      if(tt) cdn[j] = 1;
      else cdn[j] = 0;
    }
    MCMF(s, t);
  }
}
void Change() {
  for(int i = 1; i <= tot; ++i) {
		edTime = clock();
  	if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) return;
    int chnum = rand()%3+1;
    for(int j = 1; j <= chnum; ++j) {
		  int pos = rand()% netNum + 1;
		  chosen[i][pos] ^= 1;
    }

		for(int j = 1; j <= netNum; ++j) {
      if(chosen[i][j]) {
        cdn[j] = 1;
      }else cdn[j] = 0;
    }
    MCMF(s, t);
	}
}
void Gene() {
  int tt = 0;
  while(1) {
    tt ++;
		tot = 0;
		edTime = clock();
  	if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) break;

    while(!Eva3.empty()) {
      ++tot;
      Gode tmp = Eva3.top(); Eva3.pop();
      gene[tot] = tmp;
    }

    Choose();
    Across();
    if(changeornot) Change();
  }
  printf("Gene for %d times\n", tt);
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
void bfs(int id) {
	memset(vis, 0, sizeof(vis));
	queue<pair<int, int> > Q;
	Q.push(make_pair(Dad[id], 0));
	vis[Dad[id]] = 1;
	while(!Q.empty()) {
		int no = Q.front().first; int flor = Q.front().second; Q.pop();
		if(flor > 1) break;
		saok[id][no] = 1;
		for(int i = head[no]; ~i; i = edge[i].next) {
			int to = edge[i].to;
			if(to != s && to != t && !vis[to] && edge[i].cost > 0) {
				vis[to] = 1; Q.push(make_pair(to, flor+1));
			}
		}
	}
}
void Transform(int id) {
	int x = chtodo[id];
	int cnt = 0; double all = 0;
	for(int i = head[x]; ~i; i = edge[i].next) {
		int to = edge[i].to;
		if(edge[i].cost <= 0 || to == t || to == s || !saok[id][edge[i].to]) continue;
		double t2;
		if(choftransform) t2 = edge[i].flow + choftransform;
  	else t2 = 1.0;
		randch[++cnt].first = t2; randch[cnt].second = to; all += t2;
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
	chtodo[id] = pos;
}

void GetNext() {
	if(tmpAns.flow != Maxflow) {
	//	Hash();
		chtodo[tmpAns.pos] = Dad[tmpAns.pos];
	}else {
		double all = 0; int cnt = 0;
		memset(cdn, 0, sizeof(cdn));
		for(int i = 1; i <= useNum; ++i) {
			cdn[chtodo[i]] ++;
		}
	  for(int i = 1; i <= netNum; ++i) {
	    if(cdn[i]) {
				double t2 = edge[netSt[i]].cap - edge[netSt[i]].flow;
	      all += t2;
	      randch[++cnt].first = t2; randch[cnt].second = i;
	    }
	  }
		double randnum = (rand() / (RAND_MAX + 1.0));

	  prefix[0] = 0;
	  for(int i = 1; i <= cnt; ++i) {
	    prefix[i] = prefix[i-1] + randch[i].first/ all;
	  }

		int chpos = cnt;
		for(int i = 1; i <= cnt; ++i) {
		 	if(prefix[i] > randnum) {
		  	chpos = i; break;
		  }
		}
		vector<int> vc;
		for(int i = 1; i <= useNum; ++i) {
			if(chtodo[i] == randch[chpos].second) {
				vc.push_back(i);
			}
		}
		int randch = rand() % vc.size();
		Transform(vc[randch]);
	}
	MCMF(s, t);
}

void SA() {
	tem = T;
	int P_L = 0;
	int P_F = 0;
  int old[505];

	while(1)       //外循环，主要更新参数t，模拟退火过程
	{
		edTime = clock();
		if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit ) return;

		for(int i = 0; i < ILOOP; i++)    //内循环，寻找在一定温度下的最优值
		{
			edTime = clock();
			if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit ) return;

			for(int  j = 1; j <= useNum; ++j) {
        old[j] = chtodo[j];
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
				if(exp(dE / tem) < basarand) {
				}  //如果找到比当前更差的解，以一定概率接受该解，并且这个概率会越来越小
				else {
					for(int j = 1; j <= useNum; ++j) {
            chtodo[j] = old[j];
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
}
void solve() {
  int time1; int time2;
  if(netNum < 200) {
    EPS = 1e-8;
    ILOOP = 1500;
    OLOOP = 350;
    T = 1000;
    DELTA = 0.98;
    basarand = 1.0;
    LIMIT = 78;
    time1 = 40; time2 = 86;
    acrosstime = 10;
		numofall = 110;
    changeornot = 1;
    choftransform = 0;
  }else if(netNum <= 350) {
    EPS = 1e-8;
    ILOOP = 1500;
    OLOOP = 230;
    T = 3000;
    DELTA = 0.98;
    basarand = 1.0;
    LIMIT = 50;
    time1 = 46; time2 = 86;
		numofall = 100;
    acrosstime = 10;
    changeornot = 1;
    choftransform = 0;
  }else {
    EPS = 1e-8;
    ILOOP = 1500;
    OLOOP = 100;
    T = 3000;
    DELTA = 0.98;
    basarand = 1.0;
    LIMIT = 25;
    time1 = 65; time2 = 86;
		numofall = 100;
    acrosstime = 7;
    changeornot = 1;
    choftransform = 0;
  }

  for(int i = 1; i <= useNum; ++i) bfs(i);

  for(int i = 1; i <= useNum; ++i) {
		Addedge(Dad[i], t, Need[i], 0); useEd[i] = tol-2;
	}
	for(int i = 1; i <= netNum; ++i) {
		Addedge(s, i, 0, 0); netSt[i] = tol-2;
	}
	for(int i = 1; i <= useNum; ++i) Maxflow += Need[i];
  timelimit = time1;
	while(1) {
		edTime = clock();
		if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit ) break;
		for(int i = 1; i <= useNum; ++i) chtodo[i] = Dad[i];
		MCMF(s, t);
		SA();
	}
  printf("Now ans is %d\n",Ans);
  timelimit = time2;
  Gene();
	for(int i = 0; i < tol; ++i) {
		edge[i].to = E[i].to; edge[i].next = E[i].next; edge[i].cap = E[i].cap; edge[i].flow = E[i].flow; edge[i].cost = E[i].cost;
	}
  for(int i = 1; i <= netNum; ++i) {
    if(E[netSt[i]].flow) {
      cdn[i] = 1;
    }
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
	freopen("/home/basasuya/case_example/3/case0.txt","r",stdin);
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
  //  netCost = 600;
		solve();

		memset(cdn, 0, sizeof(cdn));
		for(int i = 0; i < PathAns.size(); ++i) check(PathAns[i]);
		success &= (Ans == shouldAns);
		edTime = clock();
		printf("use time %d ", (int)( (edTime - stTime)/CLOCKS_PER_SEC) );
		printf("shouans = %d your Answer is %s\n", shouldAns, success?"Yes":"No");
    printf("%d\n", (int)mp.size());

		int off = 0;
		off += sprintf(output+off,"%d\n",Ans);
		for(int i = 0; i < PathAns.size(); ++i) {
			off += sprintf(output+off, "\n");
			for(int j = 0; j < PathAns[i].size(); ++j) {
				if(j) off += sprintf(output+off, " ");
				off += sprintf(output+off, "%d",PathAns[i][j]);
			}
		}
    for(int i = 1; i <= netNum; ++i) {
      if(cdn[i]) {
        if(Son[i]) printf("*");
        printf("%d ", i);
      }
    }
    printf("\n");
	//	printf("%s\n", output);
	}
	return 0;
}

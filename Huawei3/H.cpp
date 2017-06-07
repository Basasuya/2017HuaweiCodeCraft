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
int timelimit = 85;
int numofall = 200;
int acrosstime = 3;
int time1 = 40; int time2 = 85;
double tem;
int saedge = 6;
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
int satimes;

struct Gode{
  int cdn[N];
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
Gode gene[300]; int tot;
int chosen[300][N];


struct Hode{  // 用于Hash
	unsigned int a,b,c;
	Hode(unsigned int x=0, unsigned int y=0, unsigned int z=0):a(x), b(y), c(z){}
	bool operator < (const Hode &T) const {
		if(a != T.a) return a < T.a;
		else if(b != T.b) return b < T.b;
		else return c < T.c;
	}
	Hode& operator=(const Hode &T) {
		a = T.a;
		b = T.b;
		c = T.c;
		return *this;
	}
}tmpHash;
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
map<Hode, Aode> hashmp;

int cur[N],dis[N];
int vis[N];

int testcdn[N];
pair<double, int> randch[N];
double prefix[N];

vector<int> tmp; // 最后需要输出的答案
char output[500000];
int Ans;
vector<vector<int> > PathAns;

void Init() {
	hashmp.clear();
	srand(time(NULL));
	stTime = clock();
	Maxflow = 0;
	memset(cdn,0,sizeof(cdn));
	memset(head, -1, sizeof(head)); tol = 0;
	Ans = INF;
	while(!Eva3.empty()) Eva3.pop();
	PathAns.clear();
	tmp.clear();
}
int hashtimes = 0;
int Hash(){
	int P1=31, D1=1000000009, P2=131, D2=100000007, P3=13131, D3 = 1000007;
	unsigned int has1 = 0, has2 = 0, has3 = 0;
	int num = 0;
	for(int i = 1; i <= netNum; ++i) {
		if(cdn[i]) {
			num ++;
			has1 = (has1*P1 + i);
			has2 = (has2*P2 + i);
			has3 = (has3*P3 + i);
		}
	}
//	printf("%u %u %u\n", has1, has2, has3);
	if( hashmp.find(Hode(has1, has2, has3)) == hashmp.end()) {
		tmpHash = Hode(has1, has2, has3); return 0;
	}else {
		hashtimes ++;
		tmpAns = hashmp[Hode(has1, has2, has3)];
	}
	return 1;
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

int mcmftimes = 0;
void MCMF(int s,int t){

	mcmftimes ++;
	for(int i = 1; i <= netNum; ++i) {
		if(cdn[i]) {
			edge[netSt[i]].cap = Levelflow[5];
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
//	hashmp[tmpHash] = tmpAns;

	if(tmpAns.flow != Maxflow) return;

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
	double choiceoflife = (rand() / (RAND_MAX + 1.0));
	double randnum = (rand() / (RAND_MAX + 1.0));
	double all = 0; int cnt = 0;
	for(int i = 1; i <= netNum; ++i) {
		if(cdn[i]) {
			double t2 = 1.0;
			randch[++cnt].first = t2; randch[cnt].second = i; all += t2;
		}
	}
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

	if(choiceoflife < 0.2 + 0.4*(tem/3000) ) {
		cdn[randch[chpos].second] = 0;
	}else {
		Transform(randch[chpos].second);
	}

	MCMF(s, t);
}
void SA() {
	satimes = 0;
	tem = T;
	int P_L = 0;  int P_F = 0;
	while(1)       //外循环，主要更新参数t，模拟退火过程
	{
		satimes ++;
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
    Change();
  }
  printf("Gene for %d times\n", tt);
}

void solve() {
	if(netNum < 800){
    EPS = 1e-8;
    ILOOP = 1500;
    OLOOP = 205;
    T = 2000;
    DELTA = 0.98;
    basarand = 1.0;
    LIMIT = 60;
		time1 = 85;
		numofall = 150; acrosstime = 4; saedge = 2;
  }else {
		EPS = 1e-8;
		ILOOP = 1000;
		OLOOP = 150;
		T = 2000;
		DELTA = 0.98;
		basarand = 1.0;
		LIMIT = 45;
		time1 = 85;
		numofall = 100; acrosstime = 3; saedge = 3;
	}

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

		memset(cdn, 0, sizeof(cdn));
		for(int i = 1; i <= useNum; ++i) {
			cdn[Dad[i]] = 1;
		}
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
	freopen("/home/basasuya/case_example/1/case0.txt", "r", stdin);
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
    for(int i = 1; i <= netNum; ++i) {
      if(!anscdn[i]) continue;
      for(int j = 1; j <= useNum; ++j) {
          if(Dad[j] == i) printf("*");
      }
      printf("%d ", i);
    }
	//	printf("%s\n", output);
	}
	return 0;
}

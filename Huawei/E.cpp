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
int timelimit = 80;
int netNum, routeNum, useNum;
int Maxflow; // 满流大小
int netCost;
int s, t; // 开始点 结束点
char output[500000]; int outcnt;
struct Tode{  // 用于EVA1
	int id; int price; int mincap; // 位置， 整个路径的带宽价格和，路径带宽和
	Tode(int a=0, int b = 0,int c = 0):id(a), price(b), mincap(c){}
};

clock_t stTime, edTime;
struct Node{ // 用于WeiPoint
	int pos; double val;
	Node(int a=0, double b=0):pos(a), val(b){}
	bool operator < (const Node & T) const {
		if(val != T.val) return val > T.val;
		else return pos < T.pos;
	}
}WeiPoint[1005], Eva2val[1005];
struct Hode{  // 用于Hash
	int a,b,c;
	Hode(int x=0, int y=0, int z=0):a(x), b(y), c(z){}
	bool operator < (const Hode & T) const {
		if(a != T.a) return a < T.a;
		else if(b != T.b) return b < T.b;
		else return c < T.c;
	}
};
int Son[1005]; int Dad[505]; int Need[505];
int cdn[1005]; //网络节点是否被选做 视频节点
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
int cur[1005];
int Hashtime = 0;
set<Hode> st;
map<Hode, int> mp;
vector<int> tmp; // 最后需要输出的答案

struct Gode{
  int cdn[1005];
  int cost;
  bool operator <(const Gode & T) const{
    return cost > T.cost;
  }
};
priority_queue<Gode> Eva3;
Gode gene[1005]; int tot;
int chosen[1005][1005];
void Init() {
  tot = 0;
	prefix[0] = 0;
	while(!Eva3.empty()) {
		Eva3.pop();
	}
	netNum = -1; useNum = -1;  routeNum = -1;
	netCost = -1;
	Maxflow = 0;
	memset(cdn,0,sizeof(cdn));
	memset(head, -1, sizeof(head)); tol = 0;
	outcnt = 0;
	Ans = INF;
	PathAns.clear();
	st.clear();
	tmp.clear();
  mp.clear();
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
	}

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
			chflow = tmp; chpos = Dad[i];
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
	mp[nowHash] = tmpAns;

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
void Step2() {
	edTime = clock();
	if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) return;
	vector<int> choose;
	int cn = 0;
	for(int i = 1; i <= useNum; ++i) {
		if(edge[useEd[i]].cap - edge[useEd[i]].flow) {
			edge[netSt[Dad[i]]].cap = INF;
			choose.push_back(Dad[i]); cdn[Dad[i]] = 1; cn ++;
		}
	}
	if(cn) {
		for(int i = 0; i < tol; ++i) edge[i].flow = 0;
		MCMF(s, t, 0);
	}
	for(int i = 1; i <= netNum; ++i) {
		if(edge[netSt[i]].flow) tmpAns.second += netCost;
	}


  Gode tmp;
  for(int i = 1; i <= netNum; ++i) tmp.cdn[i] = cdn[i];
  tmp.cost = tmpAns.second;
  if(Eva3.size() == 20) {
    Eva3.push(tmp);
    Eva3.pop();
  }else Eva3.push(tmp);


	if(Ans > tmpAns.second) {
		Ans = tmpAns.second;
		for(int i = 0; i < tol; ++i) {
			E[i].to = edge[i].to; E[i].next = edge[i].next; E[i].cap = edge[i].cap; E[i].flow = edge[i].flow; E[i].cost = edge[i].cost;
		}
	}
	for(int i = 0; i < choose.size(); ++i) {
		int tt = choose[i];
		edge[netSt[tt]].cap = 0;
		cdn[tt] = 0;
	}
}

void Eva1(int x, int flow) { // 从已经做过一次网络流的图中跑这个点的bfs，对越远的点产生贡献越小
	queue<Tode> Q;
	while(!Q.empty()) Q.pop();
	Q.push(Tode(x, 0, flow));
	memset(vis, 0, sizeof(vis));
	int cn = 0;
	while(!Q.empty()) {
		cn ++;
		if(cn > netNum/2 ) break; //剪枝
		int id = Q.front().id;
		int price = Q.front().price;
		int mincap = Q.front().mincap;
		Q.pop();
		if(mincap == 0) continue;
		if( price*mincap  > netCost) continue; //剪枝

		WeiPoint[id].val += mincap*1.0 / sqrt( flow*(netCost + price*mincap)/netCost);

		for(int j = head[x]; ~j; j = edge[j].next) {
			int to = edge[j].to;
			int cost = edge[j].cost;
			if(to != s && to != t && !vis[to] && cost < 0) {
				vis[to] = 1;
				int a = to; int b = price - cost; int c = min(mincap, edge[j^1].cap - edge[j^1].flow);
				Q.push(Tode(a, b, c));
			}
		}
	}
}

void Step1(int flor) {  // 找到一个漫流状态下的较优结果
	edTime = clock();
	if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) return;

	int tt = Hash();
	if(!tt) return;

	for(int i = 1; i <= netNum; ++i) WeiPoint[i].pos = i, WeiPoint[i].val = 0;
	for(int i = 1; i <= useNum; ++i) {
		Eva1(Dad[i], edge[useEd[i]].cap - edge[useEd[i]].flow);
	}

	sort(WeiPoint+1, WeiPoint+netNum+1);
	vector<int> choose;

	double lar = -1;
  int valedge; int numedge;
  if(netNum < 300) numedge = 3;
  else numedge = 2;
  if(netNum < 300) valedge = 3;
  else if(netNum < 750) valedge = 5;
  else valedge = 7;

	for(int i = 1; i <= netNum; ++i) {
		if(choose.size() < numedge && !cdn[WeiPoint[i].pos]) {
			if(choose.empty()) {
				lar = WeiPoint[i].val;
			}
			if(lar != -1 && lar - WeiPoint[i].val > lar/valedge) break;
			choose.push_back(WeiPoint[i].pos);
		}
	}

	for(int i = 0; i < choose.size(); ++i) {
		edTime = clock();
		if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) return;
		int pos = choose[i];

		cdn[pos] = 1;

		MCMF(s, t, 1);
		if(tmpAns.first == Maxflow || flor > useNum*4/5) {
			Step2();
		}else Step1(flor+1);

		cdn[pos] = 0;
	}
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
		int edge = sqrt(useNum);
		edge = 3;
    int chnum = rand()%edge+1;
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

void solve() {
	for(int i = 1; i <= useNum; ++i) {
		Addedge(Dad[i], t, Need[i], 0); useEd[i] = tol-2;
	}
	for(int i = 1; i <= netNum; ++i) {
		Addedge(s, i, 0, 0); netSt[i] = tol-2;
	}
	for(int i = 1; i <= useNum; ++i) Maxflow += Need[i];

  timelimit = 25;
	XJBS(1);
	timelimit = 85;
  Gene();

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
	freopen("/home/basasuya/case_example/case0.txt","r",stdin);
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

		_A = netCost;
		_C = netCost * 1.0;
		_D = netCost * 0.5;

		solve();


		memset(cdn, 0, sizeof(cdn));
		for(int i = 0; i < PathAns.size(); ++i) check(PathAns[i]);
		success &= (Ans == shouldAns);
    printf("hash for %d times\n", Hashtime);
		printf("your Answer is %d\n", success);
		printf("MCMF for %d times\n",tt);
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

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
int timelimit = 80;
map<int,int> ::iterator it;
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
	int pos; double val; double edge;
	int t1[3]; double t2[3];
}WeiPoint[1005];
bool cmp(Node A, Node B) {
	if(A.val != B.val) return A.val > B.val;
	else return A.pos < B.pos;
}
bool cmp2(Node A, Node B) {
	return A.pos < B.pos;
}
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
int Hashtime = 0;
set<Hode> st;
vector<int> tmp; // 最后需要输出的答案

void Init() {
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
	s = 0; t = netNum+1;
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
bool spfa(int s,int t){
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
void MCMF(int s,int t, int initcost){
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
		if(cost > Ans) {
			tmpAns = make_pair(INF, INF); return;
		}
    edTime = clock();
  	if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) {
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
void Step2() {
	edTime = clock();
	if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) return;

	if(tmpAns.first != Maxflow) return;

	if(Ans > tmpAns.second) {
		Ans = tmpAns.second;
		for(int i = 0; i < tol; ++i) {
			E[i].to = edge[i].to; E[i].next = edge[i].next; E[i].cap = edge[i].cap; E[i].flow = edge[i].flow; E[i].cost = edge[i].cost;
		}
	}
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
	if( st.find(Hode(has1, has2, has3)) == st.end()) {
		st.insert(Hode(has1, has2, has3));
		return num;
	}else {
		Hashtime ++;
		return 0;
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
		if(cn > netNum/2 ) break;
		int id = Q.front().id;
		int price = Q.front().price;
		int mincap = Q.front().mincap;
		Q.pop();
		if(mincap == 0) continue;
		double value = 0;
		if(price*mincap < netCost) value = mincap*1.0 / sqrt( flow*(netCost + price*mincap)/netCost);

    WeiPoint[id].val += value;
		WeiPoint[id].edge = min(WeiPoint[id].edge, 1.0*flow);
		if(value > WeiPoint[id].t2[0]) {
       WeiPoint[id].t2[2] = WeiPoint[id].t2[1]; WeiPoint[id].t1[2] = WeiPoint[id].t1[1];
       WeiPoint[id].t2[1] = WeiPoint[id].t2[0]; WeiPoint[id].t1[1] = WeiPoint[id].t1[0];

			 WeiPoint[id].t2[0] = value;
			 WeiPoint[id].t1[0] = x;
		}else if(value > WeiPoint[id].t2[1]) {
      WeiPoint[id].t2[2] = WeiPoint[id].t2[1]; WeiPoint[id].t1[2] = WeiPoint[id].t1[1];

			WeiPoint[id].t2[1] = value;
			WeiPoint[id].t1[1] = x;
		}else if(value > WeiPoint[id].t2[2]) {
			WeiPoint[id].t2[2] = value;
			WeiPoint[id].t1[2] = x;
		}

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
//	printf("%d\n",flor);
	edTime = clock();
	if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) return;

	for(int i = 1; i <= netNum; ++i) {
		WeiPoint[i].pos = i, WeiPoint[i].val = 0; WeiPoint[i].edge = INF;
    WeiPoint[i].t1[0] = 0; WeiPoint[i].t1[1] = 0; WeiPoint[i].t1[2] = 0;
    WeiPoint[i].t2[0] = 0; WeiPoint[i].t2[1] = 0; WeiPoint[i].t2[2] = 0;
	}
	for(int i = 1; i <= useNum; ++i) {
		Eva1(Dad[i], edge[useEd[i]].cap - edge[useEd[i]].flow);
	}
	for(int i = 1; i <= netNum; ++i) {
		if(WeiPoint[i].edge < WeiPoint[i].val) WeiPoint[i].val = 0;
	}
	sort(WeiPoint+1, WeiPoint+netNum+1, cmp);
	vector<int> choose;


	double lar = -1;
  int valedge; int numedge;
  if(netNum < 300) numedge = 5;
  else if(netNum < 800) numedge = 6;
	else numedge = 7;

  if(netNum < 300) valedge = 3;
  else if(netNum < 800) valedge = 4;
  else valedge = 5;

	for(int i = 1; i <= netNum; ++i) {
		if(choose.size() >= numedge) break;
		if(WeiPoint[i].val == 0) break;
		if(!cdn[WeiPoint[i].pos]) {
			if(choose.empty()) {
				lar = WeiPoint[i].val;
			}
			if(lar != -1 && lar - WeiPoint[i].val > lar/valedge) break;
			choose.push_back(WeiPoint[i].pos);
		}
	}
  sort(WeiPoint+1, WeiPoint+netNum+1, cmp2);
	if(choose.size() <= 3) {
		int l = choose.size();
		for(int i = 0; i < l; ++i) {
			edTime = clock();
			if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) return;
			int pos1 = choose[i]; edge[netSt[pos1]].cap = INF; cdn[pos1] = 1;

			for(int j = 0; j < tol; ++j) edge[j].flow = 0;
			int num = Hash();
      if(!num) {
        edge[netSt[pos1]].cap = 0; cdn[pos1] = 0;
        continue;
      }

 			MCMF(s, t, num*netCost);
			if(tmpAns.second == INF);
			else if(tmpAns.first == Maxflow || num == useNum){
				Step2();
			}else Step1(flor+1);

			edge[netSt[pos1]].cap = 0; cdn[pos1] = 0;
		}
	}
	else if(choose.size() <= 5) {
		int l = choose.size();
		int chnum = 5; int hvnum = -1;
		for(int i = 0; i < l; ++i) {
			for(int j = i+1; j < l; ++j) {
				hvnum ++;
				if(hvnum == chnum) return;
				edTime = clock();
				if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) return;
				int pos1 = choose[i]; edge[netSt[pos1]].cap = INF; cdn[pos1] = 1;
				int pos2 = choose[j]; edge[netSt[pos2]].cap = INF; cdn[pos2] = 1;
        map<int, int> mp;
        for(int i = 0; i < 3; ++i) {
  				mp[WeiPoint[pos1].t1[i]] ++;  mp[WeiPoint[pos2].t1[i]] ++;
  			}
        int fl = 0;
  			for(it = mp.begin(); it != mp.end(); ++it) {
  				int tt = it->second;
  				if(tt > 1 && it->first) {
  					fl = 1; break;
  				}
  			}
        if(fl) {
          edge[netSt[pos2]].cap = 0; cdn[pos2] = 0;
        }
				for(int k = 0; k < tol; ++k) {
					edge[k].flow = 0;
				}
				int num = Hash();
        if(!num) {
          edge[netSt[pos1]].cap = 0; cdn[pos1] = 0;
  				edge[netSt[pos2]].cap = 0; cdn[pos2] = 0;
          continue;
        }
				MCMF(s, t,num*netCost);
				if(tmpAns.second == INF);
				else if(tmpAns.first == Maxflow || num == useNum) {
					Step2();
				}else Step1(flor+1);

				edge[netSt[pos1]].cap = 0; cdn[pos1] = 0;
				edge[netSt[pos2]].cap = 0; cdn[pos2] = 0;
			}
		}
	}else if(choose.size() <= 7){
		int l = choose.size();
		int chnum = 6; int hvnum = -1;
		for(int i = 0; i < l; ++i)
		for(int j = i+1; j < l; ++j)
		for(int k = j+1; k < l; ++k) {
			hvnum ++;
			if(chnum == hvnum) return;
			edTime = clock();
			if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) return;
			int pos1 = choose[i]; edge[netSt[pos1]].cap = INF; cdn[pos1] = 1;
			int pos2 = choose[j]; edge[netSt[pos2]].cap = INF; cdn[pos2] = 1;
			int pos3 = choose[k]; edge[netSt[pos3]].cap = INF; cdn[pos3] = 1;

			map<int, int> mp;
			for(int i = 0; i < 2; ++i) {
				mp[WeiPoint[pos1].t1[i]] ++;  mp[WeiPoint[pos2].t1[i]] ++; mp[WeiPoint[pos3].t1[i]] ++;
			}
			int fl = 0;
			for(it = mp.begin(); it != mp.end(); ++it) {
				int tt = it->second;
				if(tt > 1 && it->first) {
					fl = 1; break;
				}
			}
      if(fl) {
        edge[netSt[pos2]].cap = 0; cdn[pos2] = 0; edge[netSt[pos3]].cap = 0; cdn[pos3] = 0;
      }
			for(int p = 0; p < tol; ++p) {
				edge[p].flow = 0;
			}
			int num = Hash();
      if(!num) {
        edge[netSt[pos1]].cap = 0; cdn[pos1] = 0;
  			edge[netSt[pos2]].cap = 0; cdn[pos2] = 0;
  			edge[netSt[pos3]].cap = 0; cdn[pos3] = 0;
        continue;
      }
			MCMF(s, t, num*netCost);
			if(tmpAns.second == INF);
			else if(tmpAns.first == Maxflow || num == useNum) {
				Step2();
			}else Step1(flor+1);

			edge[netSt[pos1]].cap = 0; cdn[pos1] = 0;
			edge[netSt[pos2]].cap = 0; cdn[pos2] = 0;
			edge[netSt[pos3]].cap = 0; cdn[pos3] = 0;
		}
	}
}

void solve() {
	for(int i = 1; i <= useNum; ++i) {
		Addedge(Dad[i], t, Need[i], 0); useEd[i] = tol-2;
	}
	for(int i = 1; i <= netNum; ++i) {
		Addedge(s, i, 0, 0); netSt[i] = tol-2;
	}
	for(int i = 1; i <= useNum; ++i) Maxflow += Need[i];
	Step1(1);
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
	freopen("/home/basasuya/case_example/3/case7.txt","r",stdin);
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

#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <queue>
#include <cmath>
using namespace std;
const int INF = 0x3f3f3f3f;

/*************you can change this here *******/
int netNum, routeNum, useNum;
int Maxflow; // 满流大小
int netCost;
int s, t;
char output[500000]; int outcnt;

double WeiPoint[1005]; // 对于每个点的权重
struct Pode{
	int id; int price; int mincap; // 位置， 整个路径的带宽价格和，路径带宽和
	Pode(int a=0, int b = 0,int c = 0):id(a), price(b), mincap(c){}
};
int Son[2005]; //消费节点所对应的网络节点
int Need[505]; //消费节点的需求
int cdn[2005]; //网络节点是否被选做 视频节点
int Ans;
vector<vector<int> > PathAns;
struct Edge{
    int to,next,cap,flow,cost;
}edge[40005], E[40005]; // edge 用于测试 E 用于 最后答案
int tag;
int head[2005],tol;
int pre[2005],dis[2005];
bool vis[2005];
int Hash[2005];

vector<int> tmp; // 最后需要输出的答案

void Init() {
	netNum = -1; useNum = -1;  routeNum = -1;
	netCost = -1;
	tag = 1;
	Maxflow = 0;
	memset(cdn,0,sizeof(cdn));
	memset(head, -1, sizeof(head)); tol = 0;
	outcnt = 0;
	Ans = 0;
	PathAns.clear();
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
    queue<int>q;
    for(int i = s;i <= t;i++)
    {
        dis[i] = INF;
        vis[i] = false;
        pre[i] = -1;
    }
    dis[s] = 0;
    vis[s] = true;
    q.push(s);
    while(!q.empty())
    {
        int u = q.front();
        q.pop();
        vis[u] = false;
        for(int i = head[u]; i != -1;i = edge[i].next)
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
                    q.push(v);
                }
            }
        }
    }
    if(pre[t] == -1 )return false;
    else return true;
}
//返回的是最大流，cost存的是最小费用
pair<int,int> MCMF(int s,int t)
{
    int flow = 0; int cost;
    cost = 0;
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
				if(netCost < Min*dis[t] && tag) break;
    }
    return make_pair(flow,cost);
}

//int dfsClock = 0;
void findpath(int x, int pre, int flow) {
	if(x == t) {
		PathAns.push_back(tmp);
		return;
	}
	for(int i = head[x]; ~i; i = edge[i].next) {
		if(edge[i].cost > 0 &&  edge[i].to != pre && edge[i].flow > 0) {
			int tt = min(edge[i].flow, flow);
			edge[i].flow -= tt; flow -= tt;
			tmp.push_back(edge[i].to-1);
			findpath(edge[i].to, x, tt);
			tmp.erase(--tmp.end());
		}else if(edge[i].to == t) {
			int tt = min(edge[i].flow, flow);
			edge[i].flow -= tt; flow -= tt;
			tmp.push_back(Son[x]-1);
			tmp.push_back(tt);
			findpath(edge[i].to, x, tt);
			tmp.erase(--tmp.end());
			tmp.erase(--tmp.end());
		}
		if(!flow) break;
	}
}

void bfs(int x, int flow) { // 从已经做过一次网络流的图中跑这个点的bfs，对越远的点产生贡献越小
	queue<Pode> Q;
	while(!Q.empty()) Q.pop();
	Q.push(Pode(x, 0, flow));
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
		if( price*mincap > netCost ) continue; //剪枝

	//	WeiPoint[id] += mincap*1.0 / ( 1 + sqrt(price*mincap/flow/netCost) ); // 随着price增加，权重会越来越小
		WeiPoint[id] += mincap*1.0 / sqrt( flow*(netCost - price*mincap)/netCost);

		for(int j = head[x]; ~j; j = edge[j].next) {
			int to = edge[j].to;
			int cost = edge[j].cost;
			if(to != s && to != t && !vis[to] && cost < 0) {
				vis[to] = 1;
				int a = to; int b = price + cost; int c = min(mincap, edge[j^1].cap  -edge[j^1].flow);
				Q.push(Pode(a, b, c));
			}
		}
	}
}


void solve() {
	for(int i = 1; i <= useNum; ++i) Maxflow += Need[i];
	int cn = 0;
	while(1) {
		cn ++;
		memset(WeiPoint, 0, sizeof(WeiPoint));
		for(int i = head[t]; ~i; i = edge[i].next) {
			int to = edge[i].to; int cap = edge[i^1].cap - edge[i^1].flow;
			bfs(to, cap); // 注意这里每次bfs的环境是上次做完 网络流的残存网络 所以有些管道已经被占用
		}

		double maxx = -1; int pos;

	//	for(int i = 1; i <= netNum; ++i) printf("%.2f ",WeiPoint[i]); printf("\n");

		for(int i = 1; i <= netNum; ++i) {
			if(maxx < WeiPoint[i] && !cdn[i]) {
				maxx = WeiPoint[i]; pos = i;
			}
		} // 找到权重最高的点，并且设为视频终点
	//	printf("%d\n",pos);
		Addedge(s, pos, INF, 0);
		cdn[pos] = 1;
		for(int i = 0; i < tol; ++i) {
			edge[i].flow = 0;
		} // 将所有流量恢复

		pair<int, int> tmp = MCMF(s, t);
		Ans = tmp.second;
		if(tmp.first == Maxflow) break;
		if(cn > netNum/2) break;
	}
	tag = 0;
	cn = 0;
	for(int i = head[t]; ~i; i = edge[i].next) {
		int j = i^1; int to = edge[i].to;
		if(edge[j].cap - edge[j].flow != 0) {
			Addedge(s, to, INF, 0); cn ++; // 遍历每个消费节点 如果这个点 没有被满足，就暴力地(= =) 在该点加上视频终端
		}
	}
	if(cn) {
		for(int i = 0; i < tol; ++i) {
			edge[i].flow = 0;
		}
		Ans = MCMF(s, t).second;
	}
	for(int i = 0; i < tol; ++i) {
		E[i] = edge[i];
	}
 	tmp.clear();
	for(int i = head[s]; ~i; i = edge[i].next) {
		if(edge[i].flow == 0) continue;
		Ans += netCost;
		int to = edge[i].to;
		tmp.push_back(to-1);
		findpath(to, s, edge[i].flow); //找输出路径
		tmp.erase(--tmp.end());
	}
	return ;
}


/*************you can change this here *******/
int check(vector<int> path) {
	int flow = path[path.size()-1];
	for(int i = 0; i < path.size() - 2; ++i) {
		int fr = path[i]+1; int to = path[i+1]+1;
		if(i == path.size()-3) to = t;
		int ok = 0;
		for(int j = head[fr]; ~j; j = E[j].next) {
			int tt = E[j].to;
			if(tt == to && E[j].cost >= 0) {
			 	if(E[j].flow >= flow) {
					ok = 1; E[j].flow -= flow;
				}
			}
			if(ok) break;
		}
		if(!ok) {
			printf("wa at %d to %d\n", fr, to);
			return 0;
		}
	}
	return 1;
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
	Init();
	freopen("case3.txt","r",stdin);
	while(~scanf("%d %d %d",&netNum, &routeNum, &useNum)) {
		s = 0; t = netNum + 1;
		scanf("%d",&netCost);
		for(int i = 1; i <= routeNum; ++i) {
			int a,b,c,d; scanf("%d %d %d %d",&a,&b,&c,&d);
			a++; b ++;
			Addedge(a, b, c, d); Addedge(b, a, c, d);
		}
		for(int j = 1; j <= useNum; ++j) {
			int a,b,c; scanf("%d %d %d",&a,&b,&c); ++b; ++a;
			Addedge(b, t, c, 0);
			Son[b] = a; Need[a] = c;
		}
		solve();
//输出答案
		int success = 1;
		Print(Ans); Print(-1);
		for(int i = 0; i < PathAns.size(); ++i) {
			int tt = check(PathAns[i]); // 每行答案判断正确性
			success &= tt;
			Print(-1);
			int last;
			for(int j = 0; j < PathAns[i].size(); ++j) {
				if(j) Print(-2);
				Print(PathAns[i][j]);
			}
		}
		printf("Your Answer is %d\n", success);
		for(int i = 0; i < outcnt; ++i) printf("%c", output[i]); printf("\n");
	}
	return 0;
}

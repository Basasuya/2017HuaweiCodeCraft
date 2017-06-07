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
const int N = 1205;
const int M = 6e5+5;

/*************you can change this here *******/
double EPS = 1e-8;
double T = 3000;
double DELTA = 0.98;
int timelimit = 88;
double tem;
clock_t stTime, edTime;
double avergnetCost;
int netNum, routeNum, useNum, netLevel;
int Levelcost[15], Levelflow[15];
int netCost[N];
int Son[N]; int Dad[N]; int Need[N];
int netSt[N], useEd[N];
int s,t;
int Maxflow;

int cdn[N];
int anscdn[N];
int tmpcdn[N];
int old[N];

struct Edge{
	int to,next,cap,cost;
}edge[M],E[M]; // edge 用于测试 E 用于 最后答案
int gap[N], dep[N];
const double _NUM = 1.0/3;
int EdgeFlow[M];
int EEdgeFlow[M];

int head[N],tol;

struct Aode{
	int flow,cost;
	Aode(int a=0, int b=0):flow(a), cost(b){}
	Aode& operator = (const Aode &T) {
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
int Q[N], S[N];
int LevelOfNodes[N];
int maxLevel;
int LEVEL = 5;
void Init() {
	netLevel = 0;
	srand(time(NULL));
	stTime = clock();
	Maxflow = 0;
	memset(cdn, 0, sizeof(cdn));
	memset(head, -1, sizeof(head)); tol = 0;
	Ans = INF;
	PathAns.clear();
	tmp.clear();
}


void BFS(int start, int end){
	memset(dep, -1 ,sizeof(dep));
	memset(gap, 0 ,sizeof(gap));
	gap[0] = 1;
	int front = 0, rear = 0;
	dep[end] = 0;
	Q[rear++] = end;
	while(front != rear){
		int u = Q[front++];
		for(int i = head[u]; ~i; i = edge[i].next){
			int v = edge[i].to;
			if(dep[v] == -1) {
				Q[rear ++] = v;
				dep[v] = dep[u] + 1;
				gap[dep[v]]++;
			}
		}
	}
}
int sap(int start, int end){
	BFS(start, end);
	int all = t - s + 1;
	memcpy(cur, head, sizeof(head));
	int u = start;
	int top = 0;
	int ans = 0;
	while(dep[start] < all)
	{
		if(u == end){
			int Min = INF;
			int inser;
			for(int i = 0; i < top; ++i){
				if(Min > edge[S[i]].cap - EdgeFlow[S[i]]){
					Min = edge[S[i]].cap - EdgeFlow[S[i]];
					inser = i;
				}
			}
			for(int i = 0; i < top; ++i){
				EdgeFlow[S[i]] += Min;
				EdgeFlow[S[i]^1] -= Min;
			}
			ans += Min;
			top = inser;
			u = edge[S[top]^1].to;
		}else {
			bool flag = false;
			int v;
			for(int i = cur[u]; ~i; i = edge[i].next){
				v = edge[i].to;
				if(edge[i].cap > EdgeFlow[i] && dep[v] + 1 == dep[u]){
					flag = true;
					cur[u] = i;
					break;
				}
			}
			if(flag){
				S[top++] = cur[u];
				u = v;
			}else {
				int Min = N;
				for(int i = head[u]; ~i;i = edge[i].next){
					if(edge[i].cap > EdgeFlow[i] && dep[edge[i].to] < Min){
						Min = dep[edge[i].to];
						cur[u] = i;
					}
				}
				gap[dep[u]]--;
				if(!gap[dep[u]]) return ans;
				dep[u] = Min + 1;
				gap[dep[u]] ++;
				if(u != start)  u = edge[S[--top]^1].to;
			}
		}
	}
	return ans;
}
void Addedge(int u,int v,int cap,int cost){
	edge[tol].to = v;
	edge[tol].cap = cap;
	edge[tol].cost = cost;
	EdgeFlow[tol] = 0;
	edge[tol].next = head[u];
	head[u] = tol++;

	edge[tol].to = u;
	edge[tol].cap = 0;
	edge[tol].cost = -cost;
	EdgeFlow[tol] = 0;
	edge[tol].next = head[v];
	head[v] = tol++;
}
int aug(int u, int flow) {
	if(u == t) return flow;
	vis[u] = true;
	for(int i = cur[u]; ~i; i = edge[i].next) {
		int v = edge[i].to;
		if(edge[i].cap > EdgeFlow[i] && !vis[v] && dis[u] == dis[v]+edge[i].cost) {
			int tmp = aug(v, min(flow, edge[i].cap - EdgeFlow[i]));
			EdgeFlow[i] += tmp;
			EdgeFlow[i^1] -= tmp;
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
				if(edge[i].cap > EdgeFlow[i] && !vis[v])
					d = min(d, dis[v]+edge[i].cost-dis[u]);
			}
		}
	}
	if(d == INF) return false;
	for(int i = s; i <= t; ++i) {
		dis[i] = (vis[i] == true) ? dis[i]+d : dis[i];
		vis[i] = false;
	}
	return true;
}

int MCMF_TIMES = 0;
int WA_TIMES = 0;
void MCMF(int s,int t){
	MCMF_TIMES ++;
	for(int i = 1; i <= netNum; ++i) {
		if(cdn[i]) {
			int idx = netSt[i];
			edge[idx].cap = Levelflow[LEVEL];
		}
		else edge[netSt[i]].cap = 0;
	}

	memset(EdgeFlow,0,tol*4);
	int ans = sap(s, t);

	if(ans != Maxflow) {
		WA_TIMES ++;
		tmpAns = Aode(INF, INF);
		return;
	}

	memset(EdgeFlow,0,tol*4);

	int flow = 0; int cost = 0;
	memset(dis,0,sizeof(dis));
	while(1) {
		memcpy(cur,head,sizeof(cur));
		while(1) {
			memset(vis,0,sizeof(vis));
			int tmp = aug(s, INF);
			if(tmp == 0) break;
			flow += tmp;
			cost += tmp*dis[s];
		}
		if(!modify_label()) break;
	}

	for(int i = 1; i <= netNum; ++i) {
		if(EdgeFlow[netSt[i]] > 0) {
			for(int j = 1; j <= netLevel; ++j) {
				if(Levelflow[j] >= EdgeFlow[netSt[i]]) {
					cost += Levelcost[j]; cost += netCost[i];
					cdn[i] = j;
					break;
				}
			}
		}else cdn[i] = 0;
	}


	tmpAns = Aode(flow, cost);
	if(tmpAns.flow != Maxflow) {
		tmpAns.cost += INF;
	}else {
		if(Ans > tmpAns.cost) {
			Ans = tmpAns.cost;
			memcpy(anscdn,cdn,sizeof(anscdn));
		}
	}
}

void findpath(int x, int pre, int flow, int level) {
	if(x == t) {
		PathAns.push_back(tmp);
		return;
	}
	for(int i = head[x]; ~i; i = E[i].next) {
		if( E[i].cost > 0 &&  E[i].to != pre && EEdgeFlow[i] > 0) {
			int tt = min(EEdgeFlow[i], flow);
			EEdgeFlow[i] -= tt; flow -= tt;
			tmp.push_back(E[i].to-1);
			findpath(E[i].to, x, tt, level);
			tmp.erase(--tmp.end());
		}else if(E[i].to == t) {
			int tt = min(EEdgeFlow[i], flow);
			EEdgeFlow[i] -= tt; flow -= tt;
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
	//	printf("%d\n",len);
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
			randch[++cnt].first = 30.0f;//30.0f;
		}
		else if(LevelOfNodes[to] == 2) {
			randch[++cnt].first = 20.f;//80.0f;
		}
		else {
			randch[++cnt].first = 10.0f;
		}

    double diff = (netCost[to] - avergnetCost);

    if(diff < -800) {
      randch[cnt].first += 8.0f;
    }
    else if(diff > 800) {
      randch[cnt].first -= 8.0f;
    }
    else randch[cnt].first += -(diff) * 8.0 / 800.0f;

		randch[cnt].second = to;
		all += randch[cnt].first;
	}

  for(int i = 1; i <= cnt; ++i) {
    prefix[i] = prefix[i-1] + randch[i].first / all;
  }
  double randnum = (rand() / (RAND_MAX + 1.0));
  int l = 1; int r = cnt;

  while(l < r) {
    int m = (l + r) >> 1;
    if(prefix[m] > randnum) r = m;
    else l = m + 1;
  }
  int chpos = l;

	int pos = randch[chpos].second;
	cdn[x] = 0; cdn[pos] = 1;
}

void GetNext() {
	double choiceoflife = (rand() / (RAND_MAX + 1.0));
  double randnum;

	double all = 0; int cnt = 0;

	for(int i = 1; i <= netNum; ++i) {
		if(cdn[i]) {
			int idx = netSt[i];
			double t2 = Levelflow[LEVEL]*2 - EdgeFlow[idx];
			randch[++cnt].first = t2;
			randch[cnt].second = i;
			all += t2;
		}
	}

	double ratio = tem / T;

	int chpos[5];
	int flag = 0;
	const int NUM = 3;

  for(int j = 0 ; j < NUM ; j ++) {
		if(ratio > j * _NUM) {
			randnum = (rand() / (RAND_MAX + 1.0));
			chpos[j] = cnt;

			int l = 1; int r = cnt;

			while(l < r) {
				int m = (l + r) >> 1;
				if(prefix[m] > randnum) r = m;
				else l = m + 1;
			}

			for(int k = 0 ; k < j ; k ++) {
				if(l == chpos[k]) {
					++l;
				}
			}

			if(l <= cnt){
				chpos[j] = l;
			}
			else {
				continue;
			}
			chpos[j] = rand()%cnt + 1;
			if(choiceoflife < j * _NUM)
			{
				flag = 1;
				cdn[randch[chpos[j]].second] = 0;
			}
		}
  }
  if(!flag) {
		Transform(randch[chpos[0]].second);
	}
}


void SA() {
	tem = T;
	int P_F = 0;

	int cnt = 0;
	while(1) {

		edTime = clock();
		if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit ) return;
		memcpy(old,cdn,sizeof(old));

		Aode oldans = tmpAns;
		int pre = tmpAns.cost;

		GetNext();
    MCMF(s, t);

		int now = tmpAns.cost;
		double dE = now - pre;

		if(dE < 0)   //如果找到更优值，直接更新
		{
			P_F = 0;
      double TimeSpan = (edTime - stTime) / CLOCKS_PER_SEC;
      printf("%d %.3f %.3f\n", now, TimeSpan, tem);
		}
		else {
			cnt ++;
			if(cnt == 5) {
				cnt = 0;
				tem = tem * DELTA;
			}
			memcpy(cdn,old,sizeof(cdn));

			tmpAns = oldans;
			P_F ++;
		}

		if(P_F == 1000)  {
			printf("P_F : %d tem : %.3f\n", P_F, tem);;
			break;
		}
	}
}

void Step0(double timespan) {
	int wa_time_cnt = 0;

	tem = T;

	int pre_cost = INF;
	int now_cost = INF;

	double tmpTime = clock();

	while(1) {
		edTime = clock();
		double nowtimespan = (edTime - tmpTime) / CLOCKS_PER_SEC;

		if(nowtimespan > timespan) {
			break;
		}

		pre_cost = now_cost;

		memcpy(old,cdn,sizeof(old));

		GetNext();

		for(int i = 1; i <= netNum; ++i) {
			if(cdn[i]) {
				int idx = netSt[i];
				edge[idx].cap = Levelflow[LEVEL];
			}
			else edge[netSt[i]].cap = 0;
		}

		memset(EdgeFlow,0,tol*4);

		int tmp_flow = sap(s,t);
		if(tmp_flow != Maxflow) {
			now_cost = pre_cost;
			memcpy(cdn,old,sizeof(old));
			wa_time_cnt ++;
			continue;
		}

		now_cost = 0;
		for(int i = 1 ; i <= netNum ; i ++) {
			if(cdn[i]) {
				int idx = netSt[i];
				for(int j = 1 ; j <= netLevel ; j++) {
					if(EdgeFlow[idx] <= Levelflow[j]) {
						now_cost += Levelcost[j];
						break;
					}
				}
				now_cost += netCost[i];
			}
		}
		for(int i = 0; i < tol; i += 2) {
			now_cost += edge[i].cost * EdgeFlow[i];
		}

		if(now_cost < pre_cost) {
			wa_time_cnt = 0;
		}
		else {
			now_cost = pre_cost;
			memcpy(cdn,old,sizeof(cdn));
			wa_time_cnt ++;
		}
		if(wa_time_cnt == 10) {
			tem = tem * DELTA;
      wa_time_cnt = 0;
		}
	}
}

void solve() {
	int timeofstep0;

	if(netNum < 800){
		timeofstep0 = 3;
	}else {
		timeofstep0 = 2;
	}

  avergnetCost = 0;
  for(int i = 1; i <= netNum; ++i) {
    avergnetCost += netCost[i];
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
  int times = 0;
	for(int i = 1; i <= timeofstep0; ++i){
    tem = T;
		memset(cdn,0,sizeof(cdn));
		for(int j = 1; j <= useNum; ++j) {
			cdn[Dad[j]] = 1;
		}
		Step0(9);
		tmpT += tem;
    times ++;
		MCMF(s,t);
		printf("SA END AT STEP 0 RET : %d\n", Ans);
	}

	T = tmpT*1.0/ times;

	memcpy(tmpcdn,anscdn,sizeof(tmpcdn));
  timelimit = 88;
	while(1) {
		edTime = clock();
		if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit ) break;

		memcpy(cdn,tmpcdn,sizeof(cdn));
		MCMF(s, t);
		SA();
	}

	memcpy(cdn,anscdn,sizeof(cdn));
	MCMF(s,t);
	memcpy(E,edge,sizeof(Edge)*tol);
	memcpy(EEdgeFlow,EdgeFlow,sizeof(int)*tol);

	for(int i = head[s]; ~i; i = E[i].next) {
		if(EEdgeFlow[i] == 0) continue;
		int to = E[i].to;
		tmp.push_back(to-1);
		findpath(to, s, EEdgeFlow[i], anscdn[E[i].to]-1); //找输出路径
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
				if(EdgeFlow[j] >= flow) {
					tmp += edge[j].cost;
					ok = 1; EdgeFlow[j] -= flow;
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
	freopen("/home/basasuya/case_example/2/case0.txt", "r", stdin);
	while(~scanf("%d %d %d",&netNum, &routeNum, &useNum)) {
		netLevel = 0;
		int cn = 0;
		char input[105];
		while(gets(input)) {
			if( input[0] == 0 || input[0] == 10 || input[0] == 13) {
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

    int cdnNum = 0;
		int cdnNums[5];

		memset(cdnNums,0,sizeof(cdnNums));

		for(int i = 1; i <= netNum ; i++) {
			if(anscdn[i]) {
				cdnNum ++;
				cdnNums[LevelOfNodes[i]] ++;
			}
		}
		for(int i = 0 ; i < 5 ; i ++) {
			printf("%d %d\n",i,cdnNums[i]);
		}

		printf("Number Of True MCMF %d\n",MCMF_TIMES-WA_TIMES);
		printf("Number of WA %d\n",WA_TIMES);
		printf("Number Of Server : %d\n",cdnNum);
		double TimeSpan = (edTime - stTime) / CLOCKS_PER_SEC;
		printf("Time span : %.3f\n",TimeSpan);
		printf("Maxflow %.3f\n",Maxflow);

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
	}
	return 0;
}

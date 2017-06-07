void Delete(int x) {
  memset(vis, 0, sizeof(vis));
  queue<pair<int, int> > Q;
  Q.push(make_pair(x, 0));
  vis[x] = 1;
  int t2 = 0; int t1 = 0;
  while(!Q.empty()) {
    int x = Q.front().first; int flor = Q.front().second; Q.pop();
    t2 ++;
    if(flor > 4) break;
    if(cdn[x]) t1 ++;
    for(int i = head[x]; ~i; i = edge[i].next) {
      int to = edge[i].to;
      if(to != t && to != s && edge[i].cost > 0 && !vis[to]) {
        vis[to] = 1; Q.push(make_pair(to, flor+1));
      }
    }
  }
  double a1 = t1*1.0 / t2;
  double a2 = edge[netSt[x]].flow*1.0 / Maxflow;
  double a3 = sqrt(T-tem)*1.0 / T;
  double a4 = sqrt(a1+a2+a3) / 4;

  double randnum = (rand() / (RAND_MAX + 1.0));
  if(randnum <= a4) {
		cdn[x] = 0;
	}
  else Transform(x);
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
		if(flor > 4) break;
		for(int i = head[x]; ~i; i = edge[i].next) {
			int to = edge[i].to;
			if(to != s && to != t && !vis[to] && edge[i].cost > 0) {
				vis[to] = 1; Q.push(make_pair(to, flor+1));
			}
		}
	}
	return t1*1.0 / t2;
}


int chnum = rand() % 10+1;
for(int i = 1; i <= chnum; ++i) {
  int chpos = cnt;
  double randnum = (rand() / (RAND_MAX + 1.0));
  for(int i = 1; i <= cnt; ++i) {
    if(prefix[i] > randnum) {
      chpos = i; break;
    }
  }
  tt[chpos] ++;
  if(tt[chpos] == 1) {
    Transform(randch[chpos].second);
  }
}

void bfs(int x) {
  memset(vis, 0, sizeof(vis));
  queue<pair<int, int> > Q;
  vis[x] = 1;
  Q.push(make_pair(x, 0));
  int edflor = 1;
  while(!Q.empty()) {
      int x = Q.front().first; int flor = Q.front().second; Q.pop();
      if(flor > edflor) break;

      for(int i = head[x]; ~i; i = edge[i].next) {
        int to = edge[i].to;
        if(to == s || to == t || vis[to]) continue;
        vis[to] = 1;


        double t2; /****Evaluate****/


        randch[++cnt].first = t2; randch[cnt].second = to; all += t2;
        Q.push(make_pair(to, flor+1));
      }
  }

}

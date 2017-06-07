/*
  else {
		int l = choose.size();
		int chnum = 5; int hvnum = -1;
		for(int i = 0; i < l; ++i)
		for(int j = i+1; j < l; ++j)
		for(int k = j+1; k < l; ++k)
		for(int p = k+1; p < l; ++p) {
			hvnum ++;
			if(hvnum == chnum) return;
			edTime = clock();
			if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) return;
			int pos1 = choose[i]; edge[netSt[pos1]].cap = INF; cdn[pos1] = 1;
			int pos2 = choose[j]; edge[netSt[pos2]].cap = INF; cdn[pos2] = 1;
			int pos3 = choose[k]; edge[netSt[pos3]].cap = INF; cdn[pos3] = 1;
			int pos4 = choose[p]; edge[netSt[pos4]].cap = INF; cdn[pos4] = 1;

			map<int, int> mp;
			for(int i = 0; i < 1; ++i) {
				mp[WeiPoint[pos1].t1[i]] ++;  mp[WeiPoint[pos2].t1[i]] ++; mp[WeiPoint[pos3].t1[i]] ++; mp[WeiPoint[pos4].t1[i]] ++;
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
        edge[netSt[pos3]].cap = 0; cdn[pos3] = 0;
        edge[netSt[pos4]].cap = 0; cdn[pos4] = 0;
      }else printf("hhh\n");

      if(!Hash()) {
        edge[netSt[pos1]].cap = 0; cdn[pos1] = 0;
  			edge[netSt[pos2]].cap = 0; cdn[pos2] = 0;
  			edge[netSt[pos3]].cap = 0; cdn[pos3] = 0;
  			edge[netSt[pos4]].cap = 0; cdn[pos4] = 0;
        continue;
      }
			for(int o = 0; o < tol; ++o) {
				edge[o].flow = 0;
			}
			MCMF(s, t, 1);
			if(tmpAns.first == Maxflow || flor > useNum*4/5) {
				Step2();
			}else Step1(flor+1);

			edge[netSt[pos1]].cap = 0; cdn[pos1] = 0;
			edge[netSt[pos2]].cap = 0; cdn[pos2] = 0;
			edge[netSt[pos3]].cap = 0; cdn[pos3] = 0;
			edge[netSt[pos4]].cap = 0; cdn[pos4] = 0;
		}
	}*/

  if(choose.size() <= 3) {
		int l = choose.size();
		for(int i = 0; i < l; ++i) {
			edTime = clock();
			if( (edTime - stTime)/CLOCKS_PER_SEC > timelimit) return;
			int pos1 = choose[i]; edge[netSt[pos1]].cap = INF; cdn[pos1] = 1;

			for(int j = 0; j < tol; ++j) edge[j].flow = 0;
      if(!Hash()) {
        edge[netSt[pos1]].cap = 0; cdn[pos1] = 0;
        continue;
      }
 			MCMF(s, t, 1);
			if(tmpAns.first == Maxflow || flor > useNum*4/5) {
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
        if(!Hash()) {
          edge[netSt[pos1]].cap = 0; cdn[pos1] = 0;
  				edge[netSt[pos2]].cap = 0; cdn[pos2] = 0;
          continue;
        }
				MCMF(s, t, 1);
				if(tmpAns.first == Maxflow || flor > useNum*4/5) {
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
      if(!Hash()) {
        edge[netSt[pos1]].cap = 0; cdn[pos1] = 0;
  			edge[netSt[pos2]].cap = 0; cdn[pos2] = 0;
  			edge[netSt[pos3]].cap = 0; cdn[pos3] = 0;
        continue;
      }
			MCMF(s, t, 1);
			if(tmpAns.first == Maxflow || flor > useNum*4/5) {
				Step2();
			}else Step1(flor+1);

			edge[netSt[pos1]].cap = 0; cdn[pos1] = 0;
			edge[netSt[pos2]].cap = 0; cdn[pos2] = 0;
			edge[netSt[pos3]].cap = 0; cdn[pos3] = 0;
		}
	}
  map<int,int> mp;
  for(int i = 0; i < choose.size(); ++i) {
    int pos = choose[i];
    for(int j = 0; j < 3; ++j) {
      mp[WeiPoint[pos].t1[j]] ++;
    }
  }

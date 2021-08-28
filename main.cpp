#include <bits/stdc++.h>
#include "node.h"
#include "utils.h"
using namespace std;
#define pp pair<double, event>
#define fi first
#define se second
#define ll unsigned long long
double tx = 1000;
double mean_tk[1000];
int main()
{
	while (true)
	{
		int num = 100;
		init(num);
		for(int i = 0;i < num;i++)
		{
			mean_tk[i] = 5000 + 50*i;
		}
		while (true)
		{
			create_connected_graph();
			if (is_connected())
			{
				break;
			}
		}
		multiset<pp> simulator;
		for (int i = 0; i < num; i++)
		{
			event e;
			e.action = 1;
			e.type = 0;
			e.idx = i;
			simulator.insert({get_expo_dist(tx), e});
		}
		for (int i = 0; i < num; i++)
		{
			event e;
			e.action = 1;
			e.type = 1;
			e.idx = i;
			e.b = get_root_block(i);
			simulator.insert({get_expo_dist(mean_tk[i]), e});
		}
		ll start = get_time_in_ms();
		while (!simulator.empty())
		{
			pp top = (*simulator.begin());
			ll event_time = top.fi;
			event e = top.se;
			while(get_time_in_ms()-start < event_time){
			}
			if(e.type == 0)
			{
				if(e.action == 1)
				{
					int x = e.idx;
					int y = get_random_int(num-1);
					if(y >= x){
						y++;
					}
					int txn_id = create_txn(x, y, num);
					send_txn(txn_id, x, simulator, num);
					event e;
					e.action = 1;
					e.type = 0;
					e.idx = x;
					simulator.insert({get_expo_dist(tx) + event_time, e});
				}else{

				}
			}
		}
		break;
	}
}
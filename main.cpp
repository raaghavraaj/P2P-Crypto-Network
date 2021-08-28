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
		int num = 10;
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
			simulator.erase(simulator.find(top));
			double event_time = top.fi;
			event e = top.se;
			while(get_time_in_ms()-start < event_time){
			}
			cout<<event_time<<" "<<e.type<<" "<<e.action<<" "<<e.idx<<endl;
			if(e.type == 0)
			{
				if(e.action == 1)
				{
					int x = e.idx;
					int y = get_random_int(num-1);
					if(y >= x){
						y++;
					}
					event et;
					et.action = 1;
					et.type = 0;
					et.idx = x;
					simulator.insert({get_expo_dist(tx) + event_time, et});
					int txn_id = create_txn(x, y, num);
					if(txn_id == -1){
						continue;
					}
					send_txn(txn_id, x, simulator, num, event_time);
				}else{
					int x = e.idx;
					int txn_id = e.txn_id;
					receive_txn(x, txn_id);
					send_txn(txn_id, x, simulator, num, event_time);
				}
			}else{
				if(e.action == 1)
				{
					int x = e.idx;
					event et;
					et.action = 1;
					et.type = 1;
					et.idx = x;
					et.b = get_root_block(x);
					simulator.insert({get_expo_dist(mean_tk[x]) + event_time, et});
					if(e.b == get_root_block(x))
					{
						block * new_block = create_block(e.b, num);
						new_block->received[x] = 1;
						update_root(x, new_block);
						get_subset_txns(new_block, x, num);
						int txn_id = create_txn(-1, x, num);
						new_block->txn_ids.push_back(txn_id);
						send_block(new_block, x, simulator, num, event_time);
					}
				}else{
					int x = e.idx;
					e.b->received[x] = 1;
					block * r = get_root_block(x);
					if(r->chain_length < e.b->chain_length){
						update_root(x, e.b);
					}
					send_block(e.b, x, simulator, num, event_time);
				}
			}
		}
		break;
	}
}
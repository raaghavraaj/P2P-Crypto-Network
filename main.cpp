#include <bits/stdc++.h>
#include "node.h"
#include "utils.h"
using namespace std;
#define pp pair<double, event>
#define fi first
#define se second
#define ll unsigned long long
double tx = 2000;
double mean_tk[1000];
double z = 30;
double total_run_time = 60000;
int main()
{
	int num = 10;
	for(int i=0;i<num;i++){
		mean_tk[i] = 5000+1000*num;
	}
	init_event(num);
	init_node(num, z);
	while(true){
		create_connected_graph();
		if (is_connected()){
			break;
		}
	}
	multiset<pp> simulator;
	for (int i = 0; i < num; i++)
	{
		event e = create_send_txn_event(i);
		simulator.insert({get_expo_dist(tx), e});
		e = create_send_block_event(i);
		simulator.insert({get_expo_dist(mean_tk[i]), e});
	}
	ll start = get_time_in_ms();
	while (!simulator.empty()){
		ll end = get_time_in_ms();
		if(end-start>total_run_time){
			break;
		}
		pp top = (*simulator.begin());
		simulator.erase(simulator.find(top));
		double evt_time = top.fi;
		event evt = top.se;
		while(get_time_in_ms()-start < evt_time){
		}
		if(evt.type == 0){
			if(evt.parent_node_id == -1){
				int y = get_random_int(num-1);
				if(y >= evt.idx){
					y++;
				}
				int txn_id = create_txn(evt.idx, y);
				event e = create_send_txn_event(evt.idx);
				simulator.insert({get_expo_dist(tx) + evt_time, e});
				send_txn(txn_id,evt.idx,simulator,evt_time,evt.parent_node_id);
			}else{
				if(!is_txn_received(evt.idx,evt.txn_id)){
					send_txn(evt.txn_id,evt.idx,simulator,evt_time,evt.parent_node_id);	
				}
			}
		}else{
			if(evt.parent_node_id == -1){
				if(evt.block_id == get_root_block_id(evt.idx)){
					int block_id = create_block(evt.idx,evt.block_id);
					add_txns_to_block(evt.idx,block_id);
					send_block(block_id,evt.idx,simulator,evt_time,evt.parent_node_id,mean_tk[evt.idx]);
				}
			}else{
				if(!is_block_received(evt.idx,evt.block_id)){
					send_block(evt.block_id,evt.idx,simulator,evt_time,evt.parent_node_id,mean_tk[evt.idx]);	
				}	
			}
		}
	}
	print_tree();
	int root_id = get_root_block_id(0);
	for(int i=0;i<num;i++){
		int a = total_blocks_of_peer(i);
		int b = blocks_of_peer_in_chain(i,root_id);
		cout<<a<<" "<<b<<endl;
	}
}
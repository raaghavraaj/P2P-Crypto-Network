#include <bits/stdc++.h>
#include "node.h"
#include "utils.h"
using namespace std;
#define pp pair<double, event>
#define fi first
#define se second
#define ll unsigned long long
vector<int> invalid_block_ids;
void print_invalid_block_ids(){
	cout<<"Invalid block ids: ";
	for(auto b:invalid_block_ids){
		cout<<b<<" ";
	}
	cout<<endl;
}

/////////////////////  PARAMETERS ////////////////////////////
int num = 5; /// NUMBER OF PEERS
double tx = 250; // MEAN INTERARRIVAL TIME OF TXN (in msec)
double mean_tk[1000]; // MEAN BLOCK CREATION TIME OF PEER (in msec)
double z = 100; // %AGE OF SLOW NODES
int seed = 10;
bool invalid = true; // 0TH PEER CAN GENERATE INVALID BLOCKS IF SET TO TRUE
double total_run_time = 30000; // TOTAL RUN TIME OF PROGRAM (in msec)
//////////////////////////////////////////////////////////////


int main()
{
	for(int i=0;i<num;i++){
		mean_tk[i] = 2000+1000*i;
	}
	////////// INITIALISING THE SYSTEM STATE ///////////
	invalid_block_ids.clear();
	init_utils(seed);
	init_event(num);
	init_node(num, z);
	ll start = get_time_in_ms();
	////////// CREATE CONNECTED GRAPH /////////////////
	while(true){
		create_connected_graph();
		if (is_connected()){
			break;
		}
	}
	multiset<pp> simulator;
	/////// ADD START EVENTS TO EVENT-QUEUE ///////////
	for (int i = 0; i < num; i++)
	{
		event e = create_send_txn_event(i);
		simulator.insert({get_expo_dist(tx), e});
		e = create_send_block_event(i);
		simulator.insert({get_expo_dist(mean_tk[i]), e});
	}
	while (!simulator.empty()){
		ll end = get_time_in_ms();
		////////// CHECK FOR SIMULATION COMPLETION ///////
		if(end-start>total_run_time){
			break;
		}
		pp top = (*simulator.begin());
		simulator.erase(simulator.find(top));
		double evt_time = top.fi;
		event evt = top.se;
		////////// WAIT FOR EVENT TIME /////////////
		while(get_time_in_ms()-start < evt_time){
		}
		if(evt.type == 0){
			if(evt.parent_node_id == -1){
				//// EVENT OF SENDING TXN ////
				int y = get_random_int(num-1);
				if(y >= evt.idx){
					y++;
				}
				int txn_id = create_txn(evt.idx, y);
				event e = create_send_txn_event(evt.idx);
				simulator.insert({get_expo_dist(tx) + evt_time, e});
				send_txn(txn_id,evt.idx,simulator,evt_time,evt.parent_node_id);
			}else{
				//// EVENT OF RECEIVING TXN ////
				if(!is_txn_received(evt.idx,evt.txn_id)){
					send_txn(evt.txn_id,evt.idx,simulator,evt_time,evt.parent_node_id);	
				}
			}
		}else{
			if(evt.parent_node_id == -1){
				//// EVENT OF SENDING BLOCK ////
				if(evt.block_id == get_root_block_id(evt.idx)){
					int block_id = create_block(evt.idx,evt.block_id);
					if(invalid && evt.idx == 0){
						///// ADD SUBSET OF TXNS (CAN CREATE INVALID BLOCK) ////
						if(add_invalid_txns_to_block(evt.idx,block_id)){
							invalid_block_ids.push_back(block_id);
						}
					}else{
						///// ADD SUBSET OF TXNS TO CREATE VALID BLOCK////
						add_txns_to_block(evt.idx,block_id);
					}
					send_block(block_id,evt.idx,simulator,evt_time,evt.parent_node_id,mean_tk[evt.idx]);
				}
			}else{
				//// EVENT OF RECEIVING BLOCK ////
				if(!is_block_received(evt.idx,evt.block_id) && is_block_valid(evt.block_id)){
					send_block(evt.block_id,evt.idx,simulator,evt_time,evt.parent_node_id,mean_tk[evt.idx]);	
				}	
			}
		}
	}
	/////// GIVE OUTPUT ////////// 
	if(invalid){
		print_invalid_block_ids();
	}
	ofstream out_tree("tree.txt");
	cout.rdbuf(out_tree.rdbuf());
	print_tree();
	for(int i=0;i<num;i++){
		ofstream peer(to_string(i)+"_peer_info.txt");
		cout.rdbuf(peer.rdbuf());
		print_blocks_received(i);
	}
}
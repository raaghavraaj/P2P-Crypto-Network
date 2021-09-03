#include <bits/stdc++.h>
using namespace std;
#define pp pair<double, event>
#define fi first
#define se second
struct txn{
	int id;
	int idx; // -1 for coinbase txn
	int idy;
	int c;
};
struct block{
	int id;
	int parent_id; // parent block id
	int idx; // peer who created the block
	int chain_length;
	vector<int> txn_ids;
};
struct event
{
	int parent_node_id; // -1 for send event
	int type;	// 0 txn, 1 block
	int idx;
	int block_id;
	int txn_id;
	bool operator<(const event &e) const{
		return tie(idx) < tie(e.idx);
	}
};
void init_event(int n);
int create_txn(int x, int y);
int create_block(int x, int par);
event create_send_txn_event(int x);
event create_send_block_event(int x);
void send_txn(int txn_id,int x,multiset<pp> &simulator,double start_time,int par_node_id);
void add_txns_to_block(int x,int block_id);
void send_block(int block_id,int x,multiset<pp> &simulator,double start_time,int par_node_id);
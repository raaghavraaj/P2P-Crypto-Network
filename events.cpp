#include <bits/stdc++.h>
#include "node.h"
#include "utils.h"
#define pp pair<double, event>
#define fi first
#define se second
using namespace std;
int mining_fee = 50;
vector<txn> txns;
block *create_block(block *par, int num)
{
	block *new_block = new block;
	new_block->parent = par;
	if (par == NULL)
	{
		new_block->chain_length = 1;
	}
	else
	{
		new_block->chain_length = par->chain_length + 1;
	}
	new_block->txn_ids.clear();
	for (int i = 0; i < num; i++)
	{
		new_block->received[i] = 0;
	}
	return new_block;
}
int create_txn(int x, int y, int num)
{
	txn t;
	t.idx = x;
	t.idy = y;
	for (int i = 0; i < num; i++)
	{
		t.received[i] = 0;
	}
	if(x==-1){
		t.c = mining_fee;
	}else{
		t.received[x] = 1;
		int bal = get_balance(x);
		if (bal == 0)
		{
			return -1;
		}
		t.c = get_random_int(bal) + 1;
		add_txn(txns.size() - 1, x);
	}
	txns.push_back(t);
	return txns.size() - 1;
}
txn get_txn(int id){
	return txns[id];
}
void send_txn(int txn_id, int x, multiset<pp> &simulator, int num, double start)
{
	for (int i = 0; i < num; i++)
	{
		edge e = get_edge(x, i);
		if(e.present && txns[txn_id].received[i] == 0 && i!=x){
			double d_mean = 96/e.c;
			double d = get_expo_dist(d_mean);
			double time_receive = e.p + 8.0 / e.c + d;
			event et;
			et.type = 0;
			et.action = 0;
			et.idx = i;
			et.txn_id = txn_id;
			simulator.insert({time_receive + start, et});
		}
	}
}
void receive_txn(int x,int txn_id){
	txns[txn_id].received[x] = 1;
	add_txn(txn_id, x);
}
int balance[1000];
void get_subset_txns(block * b, int x, int num){
	for(int i=0;i<num;i++)
	{
		balance[i]=0;
	}
	vector<int> mark_txn(txns.size());
	iterate_longest_block_chain(b->parent, mark_txn, balance);
	vector<int> received_txns=get_received_txns(x);
	for(auto id:received_txns)
	{
		if(!mark_txn[id]){
			if(balance[txns[id].idx] >= txns[id].c)
			{
				balance[txns[id].idx] -= txns[id].c;
				balance[txns[id].idy] += txns[id].c;
				b->txn_ids.push_back(id);
			}
		}
	}
}
void send_block(block * b, int x, multiset<pp> &simulator, int num, double start){
	for (int i = 0; i < num; i++)
	{
		edge e = get_edge(x, i);
		if(e.present && b->received[i] == 0 && i!=x){
			double d_mean = 96/e.c;
			double d = get_expo_dist(d_mean);
			double time_receive = e.p + 8*(1+(int)b->txn_ids.size()) / e.c + d;
			event et;
			et.type = 1;
			et.action = 0;
			et.idx = i;
			et.b = b;
			simulator.insert({time_receive + start, et});
		}
	}
}
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
	t.received[x] = 1;
	int bal = get_balance(x);
	if (bal == 0)
	{
		return -1;
	}
	t.c = get_random_int(bal) + 1;
	txns.push_back(t);
	add_txn(txns.size() - 1, x);
	return txns.size() - 1;
}
txn get_txn(int id){
	return txns[id];
}
void send_txn(int txn_id, int x, multiset<pp> &simulator, int num)
{
	for (int i = 0; i < num; i++)
	{
		edge e = get_edge(x, i);
		if(e.present && txns[txn_id].received[i] == 0 && i!=x){
			double d_mean = 96/e.c;
			double d = 
		}
	}
}
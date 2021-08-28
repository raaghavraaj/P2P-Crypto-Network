#include <bits/stdc++.h>
using namespace std;
struct txn
{
	int idx; // -1 for coinbase txn
	int idy;
	int c;
	int received[1000];
};
struct block
{
	block *parent;
	int chain_length;
	vector<int> txn_ids;
	int received[1000];
};
struct event
{
	int action; // 0 receive, 1 send
	int type;	// 0 txn, 1 block
	int idx;
	block *b;
	bool operator<(const event &e2) const
	{
		return std::tie(idx) < std::tie(e2.idx);
	}
};
block *create_block(block *par, int num);
int create_txn(int x, int num);
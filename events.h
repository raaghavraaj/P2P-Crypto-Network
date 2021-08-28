#include <bits/stdc++.h>
#define pp pair<double, event>
#define fi first
#define se second
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
	int txn_id;
	bool operator<(const event &e) const
	{
		return std::tie(idx) < std::tie(e.idx);
	}
};
block *create_block(block *par, int num);
int create_txn(int x, int y, int num);
txn get_txn(int id);
void send_txn(int txn_id, int x, multiset<pp> &simulator, int num, double start);
void receive_txn(int x,int txn_id);
void get_subset_txns(block * b, int x, int num);
void send_block(block * b, int x, multiset<pp> &simulator, int num, double start);
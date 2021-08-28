#include "events.h"
struct edge
{
	int present; // 0 DNE, 1 exist
	double p;
	double c;
};
struct node
{
	int speed; // 0 slow, 1 fast
	block *root;
};
int get_balance(int x);
void init(int n);
void create_connected_graph();
bool is_connected();
block * get_root_block(int x);
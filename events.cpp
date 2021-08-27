#include<bits/stdc++.h>
#include "node.h"
#include "utils.h"
using namespace std;
int mining_fee=50;
block * create_block(block * par,int num){
	block * new_block=new block;
	new_block->parent=par;
	if(par==NULL){
		new_block->chain_length=1;
	}else{
		new_block->chain_length=par->chain_length+1;
	}
	new_block->txn_ids.clear();
	for(int i=0;i<num;i++){
		new_block->received[i]=0;
	}
	return new_block;
}
txn * create_txn(int x,int num){
	txn * new_txn=new txn;
	new_txn->idx=x;
	new_txn->idy=get_random_int(num-1);
	if(new_txn->idy>=x){
		new_txn->idy++;
	}
	for(int i=0;i<num;i++){
		new_txn->received[i]=0;
	}
	new_txn->received[x]=1;
	int bal=get_balance(x);
	if(bal==0){
		return NULL;
	}
	new_txn->c=get_random_int(min(mining_fee,bal))+1;
	return new_txn;
}
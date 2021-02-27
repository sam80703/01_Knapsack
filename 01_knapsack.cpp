#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <limits>  
#pragma warning(disable:4996) 
using namespace std;

typedef class Item {
public:
	int value;
	int weight;
};

typedef class Node {
public:
	int level;	//這個node位在tree的第幾階（也代表從第幾個item看起，來算ub）
	int ub;	//用剩下的weight去預估未來最高上限
	int confirm_value;	//到這個點已確定有多少value
	int remain_weight;	//到這個點還剩下多少weight
};

int total_weight, lb = 0;

void qswap(vector<Item>& item, int x, int y) {
	Item temp = item[x];
	item[x] = item[y];
	item[y] = temp;

}
void quicksort(vector<Item>& item, int l, int r) {
	int i = l, j = r, mid;
	float pivot;
	mid = l + (r - l) / 2;
	pivot = float(item[mid].value) / float(item[mid].weight);

	while (i<r || j>l) {
		while (float(item[i].value) / float(item[i].weight) > pivot)
			i++;
		while (float(item[j].value) / float(item[j].weight) < pivot)
			j--;

		if (i <= j) {
			qswap(item, i, j);
			i++;
			j--;
		}
		else {
			if (i < r)
				quicksort(item, i, r);
			if (j > l)
				quicksort(item, l, j);
			return;
		}
	}
}

//用confirm_value（已選好的item所組成的total value）再加上還沒做選擇的item，等於我們預估的ub
int find_ub(vector<Item> item, int confirm_value, int remain_weight, int level) {
	// 如果level等於item的數量，代表已經走到最底層（leaf，解都出現在leaf），然而也只有可行解才會走到這，此時它的confirm_value就是它的可行解
	// 額外點：最後leaf的ub也會是他們的可行解（因為都會return回給他們的ub）
	if (level == item.size()) {
		if (confirm_value > lb) {
			lb = confirm_value;
		}
		return confirm_value;
	}
	int ub = confirm_value;
	//從第level個item開始是還沒做選擇的item，從它開始在weight沒爆掉的前提下一個一個加進ub
	for (int i = level; i < item.size() - 1; i++) {
		if (remain_weight == 0) {
			return ub;
		}
		else if (item[i].weight <= remain_weight) {
			ub += item[i].value;
			remain_weight -= item[i].weight;
		}
		else {
			ub += item[i].value * remain_weight / item[i].weight;
			return ub;
		}
	}
	return ub;
}

// 從node這個點開始往左右做展開
void searching(Node node, vector<Item> item) {
	
	// lchild是"會"選這個item（標示為1）
	Node lchild;
	// 先設這個是為了，如果等等remain_weight < item[node.level].weight的話（代表第level個item不會被選中），那這條分支就不用展開了（因為不可行）
	// ub設成-1在等等判斷時會小於lb，以便直接砍掉
	lchild.ub = -1;
	// 如果這個item的重量超過你可用的重量，下面就不用展開了；反之做展開，更新remain_weight, confirm_value, level，
	// 然後再用confirm_value加後面的item預估ub
	if (node.remain_weight >= item[node.level].weight) {
		lchild.remain_weight = node.remain_weight - item[node.level].weight;
		lchild.confirm_value = node.confirm_value + item[node.level].value;
		lchild.level = node.level + 1;
		lchild.ub = find_ub(item, lchild.confirm_value, lchild.remain_weight, lchild.level);
	}


	// rchild是"不會"選這個item（標示為0）
	Node rchild;
	rchild.remain_weight = node.remain_weight;
	rchild.confirm_value = node.confirm_value;
	rchild.level = node.level + 1;
	rchild.ub = find_ub(item, rchild.confirm_value, rchild.remain_weight, rchild.level);
	// 判斷是不是已經到leaf了，不是leaf才需要再展開
	if (node.level + 1 < item.size() || 1) {
		// 如果左邊比較有前途，先展開左邊
		if (lchild.ub >= rchild.ub) {
			// 如果ub小於等於lb，那就沒必要展開了（因為不會得到更好的解） 
			if (lchild.ub > lb) {
				searching(lchild, item);
			}
			if (rchild.ub > lb) {
				searching(rchild, item);
			}
		}
		// 如果右邊比較有前途，先展開右邊
		else {
			if (rchild.ub > lb) {
				searching(rchild, item);
			}
			if (lchild.ub > lb) {
				searching(lchild, item);
			}
		}
	}
	
}

int main() {
	int input_number;
	scanf("%d %d", &total_weight, &input_number);
	vector<Item> item;
	for (int i = 0; i < input_number; i++) {
		Item temp;
		scanf("%d %d", &temp.value, &temp.weight);
		item.push_back(temp);
	}
	// 把所有物品照CP值由大排到小
	quicksort(item, 0, item.size() - 1);
	// 先設定好root的資訊
	Node root;
	root.confirm_value = 0;
	root.remain_weight = total_weight;
	root.level = 0;
	root.ub = find_ub(item, root.confirm_value, root.remain_weight, root.level);
	// 從root開始做展開
	searching(root, item);
	cout << lb;
}
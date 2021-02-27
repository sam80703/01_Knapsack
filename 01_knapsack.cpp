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
	int level;	//�o��node��btree���ĴX���]�]�N��q�ĴX��item�ݰ_�A�Ӻ�ub�^
	int ub;	//�γѤU��weight�h�w�����ӳ̰��W��
	int confirm_value;	//��o���I�w�T�w���h��value
	int remain_weight;	//��o���I�ٳѤU�h��weight
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

//��confirm_value�]�w��n��item�Ҳզ���total value�^�A�[�W�٨S����ܪ�item�A����ڭ̹w����ub
int find_ub(vector<Item> item, int confirm_value, int remain_weight, int level) {
	// �p�Glevel����item���ƶq�A�N��w�g����̩��h�]leaf�A�ѳ��X�{�bleaf�^�A�M�Ӥ]�u���i��Ѥ~�|����o�A���ɥ���confirm_value�N�O�����i���
	// �B�~�I�G�̫�leaf��ub�]�|�O�L�̪��i��ѡ]�]�����|return�^���L�̪�ub�^
	if (level == item.size()) {
		if (confirm_value > lb) {
			lb = confirm_value;
		}
		return confirm_value;
	}
	int ub = confirm_value;
	//�q��level��item�}�l�O�٨S����ܪ�item�A�q���}�l�bweight�S�z�����e���U�@�Ӥ@�ӥ[�iub
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

// �qnode�o���I�}�l�����k���i�}
void searching(Node node, vector<Item> item) {
	
	// lchild�O"�|"��o��item�]�Хܬ�1�^
	Node lchild;
	// ���]�o�ӬO���F�A�p�G����remain_weight < item[node.level].weight���ܡ]�N���level��item���|�Q�襤�^�A���o������N���ήi�}�F�]�]�����i��^
	// ub�]��-1�b�����P�_�ɷ|�p��lb�A�H�K�����屼
	lchild.ub = -1;
	// �p�G�o��item�����q�W�L�A�i�Ϊ����q�A�U���N���ήi�}�F�F�Ϥ����i�}�A��sremain_weight, confirm_value, level�A
	// �M��A��confirm_value�[�᭱��item�w��ub
	if (node.remain_weight >= item[node.level].weight) {
		lchild.remain_weight = node.remain_weight - item[node.level].weight;
		lchild.confirm_value = node.confirm_value + item[node.level].value;
		lchild.level = node.level + 1;
		lchild.ub = find_ub(item, lchild.confirm_value, lchild.remain_weight, lchild.level);
	}


	// rchild�O"���|"��o��item�]�Хܬ�0�^
	Node rchild;
	rchild.remain_weight = node.remain_weight;
	rchild.confirm_value = node.confirm_value;
	rchild.level = node.level + 1;
	rchild.ub = find_ub(item, rchild.confirm_value, rchild.remain_weight, rchild.level);
	// �P�_�O���O�w�g��leaf�F�A���Oleaf�~�ݭn�A�i�}
	if (node.level + 1 < item.size() || 1) {
		// �p�G���������e�~�A���i�}����
		if (lchild.ub >= rchild.ub) {
			// �p�Gub�p�󵥩�lb�A���N�S���n�i�}�F�]�]�����|�o���n���ѡ^ 
			if (lchild.ub > lb) {
				searching(lchild, item);
			}
			if (rchild.ub > lb) {
				searching(rchild, item);
			}
		}
		// �p�G�k�������e�~�A���i�}�k��
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
	// ��Ҧ����~��CP�ȥѤj�ƨ�p
	quicksort(item, 0, item.size() - 1);
	// ���]�w�nroot����T
	Node root;
	root.confirm_value = 0;
	root.remain_weight = total_weight;
	root.level = 0;
	root.ub = find_ub(item, root.confirm_value, root.remain_weight, root.level);
	// �qroot�}�l���i�}
	searching(root, item);
	cout << lb;
}
/*
 *	2023012169 软件33 陈禹默
 *
 *离散数学编程题2的功能实现（归结推理）
 *
 *输入规则（有查错，支持多组数据求解）：
 *	&    合取
 *  |    析取
 *  !    非
 *  ^    蕴含
 *  ~    双蕴含
 *命题变项只接受大写字母A-Z，且应当直接输入命题公式形式的待推理式（未交代永真蕴含符号如何表示）
 *
 *	基本实现流程：
 *		1.化合取范式（建树）
 *		2.建立子句集
 *		3.进行归结
 * ！！！注：main()函数在第545行
 */

#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>
#include <bitset>
#include <stack>
using namespace std;

const int N = 5000 + 10; // 字符串最大长度

char inp[N]; // 输入字符串
int m[130], tot = 0; // m[c]代表命题变项c对应与成序内的编号（序号）
char revm[40]; // reverse of m，即编号对应的命题变项名称

typedef bitset<80> Clause;
vector < bitset <80> > clause_set;
int conv(char c) { // convert，将命题变项及其非运算结果映射到正整数方便处理
	if(c < 0) return (m[int(-c)] + tot);
	return m[(int)c];
}

int pri_op(char c) { // 求c对应的运算符的优先级
	if(c == '~') return 1;
	if(c == '^') return 2;
	if(c == '|') return 3;
	if(c == '&') return 4;
	if(c == '!') return 5;
	if(c == '(' || c == ')') return 0;
	return -1;
}
void my_getline(char *s) { // 获取一行的内容
	char c;
	while(true) {
		c = getchar();
		if(c == '\n' || c == EOF) {
			*s = '\0';
			return;
		}
		if(c == ' ') continue;
		*s = c;
		s++;
	}
}
bool illegal(char c) { // 判断c是否是命题公式中的非法字符
	if(c >= 'A' && c <= 'Z') {
		if(m[(int)c] == 0){
			m[(int)c] = ++tot;
			revm[tot] = c;
		}
		return false;
	}
	if(pri_op(c) != -1) return false;
	return true;
}
bool check(char *s, int len) { // 检查命题公式字符串s的合法性
	memset(m, 0, sizeof(m));
	memset(revm, 0, sizeof(revm));
	tot = 0;
	for(int i = 1; i <= len; i++) {
		if(illegal(s[i])) return false;
	}
	return true;
}

vector <char> post; // 存储后缀表达式
bool analyze(char *s, int len) { // 转后缀表达式
	post.resize(0);
	stack <char> op; // 字符栈
	for(int i = 1; i <= len; i++) {
		int pri = pri_op(s[i]);
		if(pri == -1) {
			post.push_back(s[i]);
			while(!op.empty()) { // 特殊处理!运算
				if(op.top() != '!') break;
				post.push_back(op.top());
				op.pop();
			}
		}
		else {
			if(s[i] == '(') {
				op.push(s[i]);
			}
			else if(s[i] == ')') { // 括号匹配
				bool flag = false;
				while(!op.empty()) {
					char c = op.top();
					op.pop();
					if(c == '(') {
						flag = true;
						break;
					}
					post.push_back(c);
				}
				if(flag == false) return false;
			}
			else if(s[i] == '!') {
				op.push(s[i]);
			}
			else {
				while(!op.empty()) { // 优先级相关处理，确保后缀式优先度正常
					char c = op.top();
					if(pri_op(c) < pri) break;
					op.pop();
					post.push_back(c);
				}
				op.push(s[i]);
			}
		}
	}
	while(!op.empty()) {
		post.push_back(op.top());
		op.pop();
	}
	stack <char> num;
	//for(int i = 0; i < (int) post.size(); i++) printf("%c ", post[i]); printf("\n");
	//检测post后缀式能否正常计算，若不能则说明输入非法
	for(int i = 0; i < (int)post.size(); i++) {
		if(pri_op(post[i]) == -1) {
			num.push(post[i]);
		}
		else if(post[i] == '!') {
			if(num.empty()) return false;
			char tmp = num.top();
			num.pop();
			num.push(tmp);
		}
		else {
			if(num.empty()) return false;
			char tmp = num.top();
			num.pop();
			if(num.empty()) return false;
			tmp = num.top();
			num.pop();
			num.push(tmp);
		}
	}
	if(num.empty() || num.size() > 1) return false; // 不能正常计算
	return true; // 输入合法
}

struct Tree {
	char val; // 命题变项名或运算符
	Tree *fa;
	vector <Tree *> sons;
	bool is_leaf() {
		return ((int)sons.size() == 0);
	}
};

struct Node {
	Tree *ptr;
	int pos;
};
Node build(int vpos) { // 递归建树
	Tree *root = new Tree;
	root -> val = post[vpos];
	int final_pos = vpos;
	if(pri_op(root -> val) != -1) { // 是运算符
		if(root -> val == '!') {
			root -> sons.push_back(NULL);
			Node tmp = build(vpos - 1);
			root -> sons[0] = tmp.ptr;
			root -> sons[0] -> fa = root;
			final_pos = tmp.pos;
		}
		else {
			root -> sons.push_back(NULL);
			root -> sons.push_back(NULL);
			Node tmp = build(vpos - 1);
			root -> sons[1] = tmp.ptr;
			root -> sons[1] -> fa = root;
			tmp = build(tmp.pos - 1);
			root -> sons[0] = tmp.ptr;
			root -> sons[0] -> fa = root;
			final_pos = tmp.pos;
		}
	}
	return (Node){root, final_pos};
}

void print_tree(Tree *root) { // 调试用
	if(root -> val < 0) putchar('!');
	printf("%c", abs(root -> val));
	for(int i = 0; i < int(root -> sons.size()); i++)
		print_tree(root -> sons[i]);
}

Tree *copy(Tree *root) { // 复制以root为根节点的子树并返回新根节点指针
	Tree *ansroot = new Tree;
	*ansroot = *root;
	for(int i = 0; i < int(root -> sons.size()); i++) {
		ansroot -> sons[i] = copy(root -> sons[i]);
		ansroot -> sons[i] -> fa = ansroot;
	}
	return ansroot;
}
void erase(Tree *root) { // 删除以root为根节点的子树
	//char c = root -> val;
	//printf("Begin Erase: %c\n", c);
	//system("pause");
	for(int i = 0; i < int(root -> sons.size()); i++) {
		erase(root -> sons[i]);
		root -> sons[i] = NULL;
	}
	delete root;
	root = NULL;
	//printf("Erase Complete: %c\n", c);
	//system("pause");
}

void find_implications(Tree *ptr) { // 找到蕴含符号并将其转化
	for(int i = 0; i < int(ptr -> sons.size()); i++)
		find_implications(ptr -> sons[i]);
	if(ptr -> val == '^') { // P^Q=(!P|Q)
		ptr -> val = '|';
		Tree *add = new Tree;
		add -> sons.push_back(ptr -> sons[0]);
		ptr -> sons[0] -> fa = add;
		add -> val = '!';
		ptr -> sons[0] = add;
		add -> fa = ptr;
	}
	else if(ptr -> val == '~') { // P~Q=(!P|Q)&(!Q|P)
		Tree * A = copy(ptr -> sons[0]);
		Tree * B = copy(ptr -> sons[1]);

		ptr -> val = '&';
		Tree *lef1 = new Tree, *lef2 = new Tree;
		lef2 -> val = '!';
		lef2 -> sons.push_back(copy(A));
		ptr -> sons[0] -> fa = lef2;
		lef1 -> val = '|';
		lef1 -> sons.push_back(lef2);
		lef2 -> fa = lef1;
		lef1 -> sons.push_back(copy(B));
		ptr -> sons[1] -> fa = lef1;
		
		Tree *rig1 = new Tree, *rig2 = new Tree;
		rig2 -> val = '!';
		rig2 -> sons.push_back(copy(B));
		ptr -> sons[1] -> fa = rig2;
		rig1 -> val = '|';
		rig1 -> sons.push_back(rig2);
		rig2 -> fa = rig1;
		rig1 -> sons.push_back(copy(A));
		ptr -> sons[0] -> fa = rig1;

		ptr -> sons[0] = lef1;
		lef1 -> fa = ptr;
		ptr -> sons[1] = rig1;
		rig1 -> fa = ptr;

		erase(A);
		erase(B);
	}
}

void DeMorgan(Tree *ptr) { // 利用De Morgan律将!运算化简
	//cout << ptr -> val << endl;
	if((*ptr).is_leaf()) return;
	for(int i = 0; i < int(ptr -> sons.size()); i++) {
		while(ptr -> sons[i] -> val == '!' && ptr -> sons[i] -> sons[0] -> is_leaf() == false) {
			char c = ptr -> sons[i] -> sons[0] -> val;
			if(c == '!') { // 双重否定可消去
				Tree *del1 = ptr -> sons[i], *del2 = ptr -> sons[i] -> sons[0];
				ptr -> sons[i] -> sons[0] -> sons[0] -> fa = ptr;
				ptr -> sons[i] = ptr -> sons[i] -> sons[0] -> sons[0];
				//cout << ptr -> sons[i] -> val << endl;
				delete del1; delete del2;
			}
			else if(c == '&' || c == '|') { // !对&和|的运算律
				Tree *del1 = ptr -> sons[i];
				ptr -> sons[i] -> sons[0] -> fa = ptr;
				ptr -> sons[i] = ptr -> sons[i] -> sons[0];
				delete del1;

				ptr -> sons[i] -> val = (c == '&' ? '|' : '&');

				Tree *lef = new Tree, *rig = new Tree;
				lef -> val = rig -> val = '!';
				lef -> fa = rig -> fa = ptr -> sons[i];
				lef -> sons.push_back(ptr -> sons[i] -> sons[0]);
				rig -> sons.push_back(ptr -> sons[i] -> sons[1]);
				ptr -> sons[i] -> sons[0] -> fa = lef; ptr -> sons[i] -> sons[1] -> fa = rig;

				ptr -> sons[i] -> sons[0] = lef; ptr -> sons[i] -> sons[1] = rig;
			}
		}
	}
	for(int i = 0; i < int(ptr -> sons.size()); i++)
		DeMorgan(ptr -> sons[i]);
}

void distribute(Tree *ptr) { // 实现分配律
	if(ptr -> is_leaf()) return;
	for(int i = 0; i < int(ptr -> sons.size()); i++)
		distribute(ptr -> sons[i]);
	if(ptr -> val == '|') {
		if(ptr -> sons[0] -> val == '&' && ptr -> sons[1] -> val == '&') { // 特殊情况，需要一起分配，不然难以递归实现
			Tree *A = copy(ptr -> sons[0] -> sons[0]);
			Tree *B = copy(ptr -> sons[0] -> sons[1]);
			Tree *C = copy(ptr -> sons[1] -> sons[0]);
			Tree *D = copy(ptr -> sons[1] -> sons[1]);

			//system("pause");
			ptr -> val = '&';

			//printf("%c%c%c%c\n", A -> val, B -> val, C -> val, D -> val);

			Tree * L = ptr -> sons[0]; Tree * R = ptr -> sons[1];
			//delete L -> sons[0];
			//cout << L -> sons[0] -> val << endl;
			//system("pause");
			erase(L -> sons[0]);
			erase(L -> sons[1]);
			erase(R -> sons[0]);
			erase(R -> sons[1]);
			
			//system("pause");
			for(int i = 0; i < 2; i++) {
				L -> sons[i] = new Tree;
				R -> sons[i] = new Tree;
			}
			//print_tree(ptr);
			L -> sons[0] -> val = L -> sons[1] -> val = R -> sons[0] -> val = R -> sons[1] -> val = '|';

			L -> sons[0] -> sons.push_back(copy(A)); L -> sons[0] -> sons[0] -> fa = L -> sons[0];
			L -> sons[0] -> sons.push_back(copy(C)); L -> sons[0] -> sons[1] -> fa = L -> sons[0];

			L -> sons[1] -> sons.push_back(copy(B)); L -> sons[1] -> sons[0] -> fa = L -> sons[1];
			L -> sons[1] -> sons.push_back(copy(C)); L -> sons[1] -> sons[1] -> fa = L -> sons[1];

			R -> sons[0] -> sons.push_back(copy(A)); R -> sons[0] -> sons[0] -> fa = R -> sons[0];
			R -> sons[0] -> sons.push_back(copy(D)); R -> sons[0] -> sons[1] -> fa = R -> sons[0];

			R -> sons[1] -> sons.push_back(copy(B)); R -> sons[1] -> sons[0] -> fa = R -> sons[1];
			R -> sons[1] -> sons.push_back(copy(D)); R -> sons[1] -> sons[1] -> fa = R -> sons[1];

			erase(A);
			erase(B);
			erase(C);
			erase(D);
			for(int k = 0; k < 2; k++)
				distribute(ptr -> sons[k]);
			return; // 已分配完毕
		}
		for(int i = 0; i < 2; i++) { // 一般情况下的分配律
			int j = 1 - i; // 另一个节点
			if(ptr -> sons[i] -> val == '&') {
				ptr -> val = '&';
				ptr -> sons[i] -> val = '|';

				Tree *A = copy(ptr -> sons[j]);
				Tree *B = copy(ptr -> sons[i] -> sons[0]);
				Tree *C = copy(ptr -> sons[i] -> sons[1]);
				erase(ptr -> sons[i] -> sons[0]);
				erase(ptr -> sons[i] -> sons[1]);
				erase(ptr -> sons[j]);

				ptr -> sons[j] = new Tree;
				ptr -> sons[j] -> val = '|';
				ptr -> sons[j] -> fa = ptr;

				ptr -> sons[j] -> sons.push_back(copy(A)); ptr -> sons[j] -> sons[0] -> fa = ptr -> sons[j];
				ptr -> sons[j] -> sons.push_back(copy(B)); ptr -> sons[j] -> sons[1] -> fa = ptr -> sons[j];

				ptr -> sons[i] -> sons[0] = copy(A); ptr -> sons[i] -> sons[0] -> fa = ptr -> sons[i];
				ptr -> sons[i] -> sons[1] = copy(C); ptr -> sons[i] -> sons[1] -> fa = ptr -> sons[i];

				erase(A);
				erase(B);
				erase(C);
				for(int k = 0; k < 2; k++)
					distribute(ptr -> sons[k]);
				return;
			}
		}
	}
}

void clear_not(Tree *ptr) { // 将!与命题变项合二为一看作一个命题变项
	if(ptr -> is_leaf()) return;
	for(auto &i : ptr -> sons)
		clear_not(i);
	for(int i = 0; i < (int)(ptr -> sons.size()); i++) {
		if(ptr -> sons[i] -> val == '!') {
			Tree *del = ptr -> sons[i];
			ptr -> sons[i] -> sons[0] -> fa = ptr;
			ptr -> sons[i] -> sons[0] -> val = -(ptr -> sons[i] -> sons[0] -> val); // 取相反数保存
			ptr -> sons[i] = ptr -> sons[i] -> sons[0];
			delete del;
		}
	}
}

void add_all(Tree *root, int x) { // 将root下的所有命题变项合并为一个子句,添加到第x个位置
	if(root -> is_leaf()) {
		clause_set[x][conv(root -> val)] = 1;
		return;
	}
	for(auto &i : root -> sons)
		add_all(i, x);
}

bool tautology(Clause &x) { // 判断x是否是重言式
	for(int i = 1; i <= tot; i++) {
		if(x[conv(revm[i])] && x[conv(-revm[i])])
			return true;
	}
	return false;
}

void build_clause_set(Tree *ptr) { // 建立子句集
	for(auto &i : ptr -> sons) {
		if(i -> val == '&') {
			build_clause_set(i);
		}
		else {
			if(i -> val == '|') {
				clause_set.push_back((Clause){{0}});
				add_all(i, clause_set.size() - 1);
			}
			else if(i -> is_leaf()) {
				bitset <80> tmp;
				tmp = {0};
				tmp[conv(i -> val)] = true;
				clause_set.push_back(tmp);
			}
		}
	}
}

void print_clause_set() { // 输出子句集
	printf("子句集如下：\n{");
	for(auto &i : clause_set) {
		printf("[");
		bool first = true;
		for(int j = 1; j <= tot; j++) {
			if(i[conv(revm[j])]) {
				if(first) {first = false; printf("%c", revm[j]);}
				else printf(",%c", revm[j]);
			}
			if(i[conv(-revm[j])]) {
				if(first) {first = false; printf("!%c", revm[j]);}
				else printf(",!%c", revm[j]);
			}
		}
		printf("]");
	}
	printf("}\n");
}

void simplify_clause_set() { // 化简子句集，剔除多余项
	bool flag[(int)clause_set.size()];
	memset(flag, false, sizeof(flag));
	for(int i = 0; i < (int)clause_set.size(); i++) {
		bool same = false;
		for(int j = 0; j < i; j++) {
			if(clause_set[j] == clause_set[i]) {
				same = true;
				break;
			}
		}
		if(same || tautology(clause_set[i])) { // 如果该析取式是重言式或者其重复，则无意义
			flag[i] = true;
		}
	}
	vector <Clause> tmp;
	for(int i = 0; i < (int)clause_set.size(); i++) {
		if(flag[i] == false) {
			tmp.push_back(clause_set[i]);
		}
	}
	clause_set = tmp;
}

bool found() { // 判断是否存在空子句
	for(int i = 0; i < (int)clause_set.size(); i++) {
		if(clause_set[i].none()) return true;
	}
	return false;
}

void resolution() {
	int round = 0;
	while(true) {
		if(round > 100) {
			printf("在100轮归结后仍未找到可行解，归结推理失败\n");
			return;
		}
		int original_size = clause_set.size();
		bool changed = false;
		for(int i = 1; i <= tot; i++) {
			for(int j = 0; j < original_size; j++) {
				for(int k = 0; k < original_size; k++) {
					if(clause_set[j][conv(revm[i])] && clause_set[k][conv(-revm[i])]) { // 可归结
						auto val = clause_set[j] | clause_set[k];
						val[conv(revm[i])] = val[conv(-revm[i])] = 0;
						bool same = false;
						for(int idx = 0; idx < (int)clause_set.size(); idx++) {
							if(val == clause_set[idx]) {
								same = true;
								break;
							}
						}
						if(same || tautology(val)) continue; // 重复或者重言式则无意义
						clause_set.push_back(val);
						changed = true;
					}
				}
			}
		}
		round++;
		printf("第%d轮归结结果：\n", round);
		print_clause_set();
		printf("\n");
		if(found()) { // 得出矛盾，归结成功
			printf("归结得到空子句，推理成功！\n");
			return;
		}
		else if(!changed) { // 无法归结出新东西
			printf("归结推理无法继续！\n");
			return;
		}
	}
}

int main() {
	while(true) { // 不断读取输入，直至用户不需要
		system("cls");
		printf("请输入合法命题公式：(输入\33[1;31m-1\033[0m以退出)\n");
		my_getline(inp + 1); // input读入一行
		int n = strlen(inp + 1);
		if(inp[1] == '-' && inp[2] == '1') {
			break; // 读入-1，退出
		}
		if(check(inp, n) == false || n == 0) { // 判定输入是否包含非法字符
			printf("输入不合法！\n");
			system("pause");
			continue;
		}
		bool chk = analyze(inp, n); // 尝试将中缀式转化为后缀表达式，并返回是否转换成功
		if(chk == false) { // 表达式非法，不可接受
			printf("输入不合法！\n");
			system("pause");
			continue;
		}
		Tree *root0 = new Tree; root0 -> val = '$'; // 缓冲根节点
		Tree *root = build(post.size() - 1).ptr; root -> fa = root0; root0 -> sons.push_back(root);
		find_implications(root); // 将树内的单双蕴含符号转化为析取和非
		//print_tree(root); cout << endl;
		DeMorgan(root0); root = root0 -> sons[0]; // 利用De Morgan定律化简非运算
		//print_tree(root); cout << endl;
		distribute(root); // 利用分配律找到合取范式
		//print_tree(root); cout << endl;
		clear_not(root); // 将取反符号与命题变项合并看作一个命题变项而不是一个运算
		//print_tree(root); cout << endl;
		clause_set.resize(0);
		build_clause_set(root0); // 建立子句集
		simplify_clause_set();
		print_clause_set();
		printf("\n");
		resolution(); // 进行归结推理
		system("pause");
	}
	return 0;
}

// P|(Q&R&S)
// (A&B)|(C|(D&E))
// !(P|Q)~(P&Q)
// ((A^B)&(B^C))&!(A^C)
// ((P~Q)~((P&!Q)|(Q&!P)))&!(P&!P)
// (Q^R)&!((P|Q)^(P|R))
// ((P|Q)&(!P|!Q))~((!P|Q)&(!Q|P))
// (P~Q)~((P|Q)&(!P|!Q))((!P|Q)&(!Q|P))~((P|Q)&(!P|!Q))
// 
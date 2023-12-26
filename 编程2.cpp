/*
 *	2023012169 ���33 ����Ĭ
 *
 *��ɢ��ѧ�����2�Ĺ���ʵ�֣��������
 *
 *��������в��֧�ֶ���������⣩��
 *	&    ��ȡ
 *  |    ��ȡ
 *  !    ��
 *  ^    �̺�
 *  ~    ˫�̺�
 *�������ֻ���ܴ�д��ĸA-Z����Ӧ��ֱ���������⹫ʽ��ʽ�Ĵ�����ʽ��δ���������̺�������α�ʾ��
 *
 *	����ʵ�����̣�
 *		1.����ȡ��ʽ��������
 *		2.�����Ӿ伯
 *		3.���й��
 * ������ע��main()�����ڵ�545��
 */

#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>
#include <bitset>
#include <stack>
using namespace std;

const int N = 5000 + 10; // �ַ�����󳤶�

char inp[N]; // �����ַ���
int m[130], tot = 0; // m[c]�����������c��Ӧ������ڵı�ţ���ţ�
char revm[40]; // reverse of m������Ŷ�Ӧ�������������

typedef bitset<80> Clause;
vector < bitset <80> > clause_set;
int conv(char c) { // convert�������������������ӳ�䵽���������㴦��
	if(c < 0) return (m[int(-c)] + tot);
	return m[(int)c];
}

int pri_op(char c) { // ��c��Ӧ������������ȼ�
	if(c == '~') return 1;
	if(c == '^') return 2;
	if(c == '|') return 3;
	if(c == '&') return 4;
	if(c == '!') return 5;
	if(c == '(' || c == ')') return 0;
	return -1;
}
void my_getline(char *s) { // ��ȡһ�е�����
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
bool illegal(char c) { // �ж�c�Ƿ������⹫ʽ�еķǷ��ַ�
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
bool check(char *s, int len) { // ������⹫ʽ�ַ���s�ĺϷ���
	memset(m, 0, sizeof(m));
	memset(revm, 0, sizeof(revm));
	tot = 0;
	for(int i = 1; i <= len; i++) {
		if(illegal(s[i])) return false;
	}
	return true;
}

vector <char> post; // �洢��׺���ʽ
bool analyze(char *s, int len) { // ת��׺���ʽ
	post.resize(0);
	stack <char> op; // �ַ�ջ
	for(int i = 1; i <= len; i++) {
		int pri = pri_op(s[i]);
		if(pri == -1) {
			post.push_back(s[i]);
			while(!op.empty()) { // ���⴦��!����
				if(op.top() != '!') break;
				post.push_back(op.top());
				op.pop();
			}
		}
		else {
			if(s[i] == '(') {
				op.push(s[i]);
			}
			else if(s[i] == ')') { // ����ƥ��
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
				while(!op.empty()) { // ���ȼ���ش���ȷ����׺ʽ���ȶ�����
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
	//���post��׺ʽ�ܷ��������㣬��������˵������Ƿ�
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
	if(num.empty() || num.size() > 1) return false; // ������������
	return true; // ����Ϸ�
}

struct Tree {
	char val; // ����������������
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
Node build(int vpos) { // �ݹ齨��
	Tree *root = new Tree;
	root -> val = post[vpos];
	int final_pos = vpos;
	if(pri_op(root -> val) != -1) { // �������
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

void print_tree(Tree *root) { // ������
	if(root -> val < 0) putchar('!');
	printf("%c", abs(root -> val));
	for(int i = 0; i < int(root -> sons.size()); i++)
		print_tree(root -> sons[i]);
}

Tree *copy(Tree *root) { // ������rootΪ���ڵ�������������¸��ڵ�ָ��
	Tree *ansroot = new Tree;
	*ansroot = *root;
	for(int i = 0; i < int(root -> sons.size()); i++) {
		ansroot -> sons[i] = copy(root -> sons[i]);
		ansroot -> sons[i] -> fa = ansroot;
	}
	return ansroot;
}
void erase(Tree *root) { // ɾ����rootΪ���ڵ������
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

void find_implications(Tree *ptr) { // �ҵ��̺����Ų�����ת��
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

void DeMorgan(Tree *ptr) { // ����De Morgan�ɽ�!���㻯��
	//cout << ptr -> val << endl;
	if((*ptr).is_leaf()) return;
	for(int i = 0; i < int(ptr -> sons.size()); i++) {
		while(ptr -> sons[i] -> val == '!' && ptr -> sons[i] -> sons[0] -> is_leaf() == false) {
			char c = ptr -> sons[i] -> sons[0] -> val;
			if(c == '!') { // ˫�ط񶨿���ȥ
				Tree *del1 = ptr -> sons[i], *del2 = ptr -> sons[i] -> sons[0];
				ptr -> sons[i] -> sons[0] -> sons[0] -> fa = ptr;
				ptr -> sons[i] = ptr -> sons[i] -> sons[0] -> sons[0];
				//cout << ptr -> sons[i] -> val << endl;
				delete del1; delete del2;
			}
			else if(c == '&' || c == '|') { // !��&��|��������
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

void distribute(Tree *ptr) { // ʵ�ַ�����
	if(ptr -> is_leaf()) return;
	for(int i = 0; i < int(ptr -> sons.size()); i++)
		distribute(ptr -> sons[i]);
	if(ptr -> val == '|') {
		if(ptr -> sons[0] -> val == '&' && ptr -> sons[1] -> val == '&') { // �����������Ҫһ����䣬��Ȼ���Եݹ�ʵ��
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
			return; // �ѷ������
		}
		for(int i = 0; i < 2; i++) { // һ������µķ�����
			int j = 1 - i; // ��һ���ڵ�
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

void clear_not(Tree *ptr) { // ��!���������϶�Ϊһ����һ���������
	if(ptr -> is_leaf()) return;
	for(auto &i : ptr -> sons)
		clear_not(i);
	for(int i = 0; i < (int)(ptr -> sons.size()); i++) {
		if(ptr -> sons[i] -> val == '!') {
			Tree *del = ptr -> sons[i];
			ptr -> sons[i] -> sons[0] -> fa = ptr;
			ptr -> sons[i] -> sons[0] -> val = -(ptr -> sons[i] -> sons[0] -> val); // ȡ�෴������
			ptr -> sons[i] = ptr -> sons[i] -> sons[0];
			delete del;
		}
	}
}

void add_all(Tree *root, int x) { // ��root�µ������������ϲ�Ϊһ���Ӿ�,��ӵ���x��λ��
	if(root -> is_leaf()) {
		clause_set[x][conv(root -> val)] = 1;
		return;
	}
	for(auto &i : root -> sons)
		add_all(i, x);
}

bool tautology(Clause &x) { // �ж�x�Ƿ�������ʽ
	for(int i = 1; i <= tot; i++) {
		if(x[conv(revm[i])] && x[conv(-revm[i])])
			return true;
	}
	return false;
}

void build_clause_set(Tree *ptr) { // �����Ӿ伯
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

void print_clause_set() { // ����Ӿ伯
	printf("�Ӿ伯���£�\n{");
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

void simplify_clause_set() { // �����Ӿ伯���޳�������
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
		if(same || tautology(clause_set[i])) { // �������ȡʽ������ʽ�������ظ�����������
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

bool found() { // �ж��Ƿ���ڿ��Ӿ�
	for(int i = 0; i < (int)clause_set.size(); i++) {
		if(clause_set[i].none()) return true;
	}
	return false;
}

void resolution() {
	int round = 0;
	while(true) {
		if(round > 100) {
			printf("��100�ֹ�����δ�ҵ����н⣬�������ʧ��\n");
			return;
		}
		int original_size = clause_set.size();
		bool changed = false;
		for(int i = 1; i <= tot; i++) {
			for(int j = 0; j < original_size; j++) {
				for(int k = 0; k < original_size; k++) {
					if(clause_set[j][conv(revm[i])] && clause_set[k][conv(-revm[i])]) { // �ɹ��
						auto val = clause_set[j] | clause_set[k];
						val[conv(revm[i])] = val[conv(-revm[i])] = 0;
						bool same = false;
						for(int idx = 0; idx < (int)clause_set.size(); idx++) {
							if(val == clause_set[idx]) {
								same = true;
								break;
							}
						}
						if(same || tautology(val)) continue; // �ظ���������ʽ��������
						clause_set.push_back(val);
						changed = true;
					}
				}
			}
		}
		round++;
		printf("��%d�ֹ������\n", round);
		print_clause_set();
		printf("\n");
		if(found()) { // �ó�ì�ܣ����ɹ�
			printf("���õ����Ӿ䣬����ɹ���\n");
			return;
		}
		else if(!changed) { // �޷������¶���
			printf("��������޷�������\n");
			return;
		}
	}
}

int main() {
	while(true) { // ���϶�ȡ���룬ֱ���û�����Ҫ
		system("cls");
		printf("������Ϸ����⹫ʽ��(����\33[1;31m-1\033[0m���˳�)\n");
		my_getline(inp + 1); // input����һ��
		int n = strlen(inp + 1);
		if(inp[1] == '-' && inp[2] == '1') {
			break; // ����-1���˳�
		}
		if(check(inp, n) == false || n == 0) { // �ж������Ƿ�����Ƿ��ַ�
			printf("���벻�Ϸ���\n");
			system("pause");
			continue;
		}
		bool chk = analyze(inp, n); // ���Խ���׺ʽת��Ϊ��׺���ʽ���������Ƿ�ת���ɹ�
		if(chk == false) { // ���ʽ�Ƿ������ɽ���
			printf("���벻�Ϸ���\n");
			system("pause");
			continue;
		}
		Tree *root0 = new Tree; root0 -> val = '$'; // ������ڵ�
		Tree *root = build(post.size() - 1).ptr; root -> fa = root0; root0 -> sons.push_back(root);
		find_implications(root); // �����ڵĵ�˫�̺�����ת��Ϊ��ȡ�ͷ�
		//print_tree(root); cout << endl;
		DeMorgan(root0); root = root0 -> sons[0]; // ����De Morgan���ɻ��������
		//print_tree(root); cout << endl;
		distribute(root); // ���÷������ҵ���ȡ��ʽ
		//print_tree(root); cout << endl;
		clear_not(root); // ��ȡ���������������ϲ�����һ��������������һ������
		//print_tree(root); cout << endl;
		clause_set.resize(0);
		build_clause_set(root0); // �����Ӿ伯
		simplify_clause_set();
		print_clause_set();
		printf("\n");
		resolution(); // ���й������
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
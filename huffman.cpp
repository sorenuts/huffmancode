/*
 * 使用言語:C++
 * STLコンテナを使用した方が楽なので(主にソート関数の実装が不要、push_backが使用できる)C++で書いた。
 */
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <utility>

#define PB push_back
#define REP(i,n) for(int i=0;i<(n);i++)

using namespace std;

//ハフマン木のノード
typedef struct Node{
	string value;//文字列
	int count;//出現回数
	Node *childnode[2];
}node_t;

typedef map<string,string> Code;//符号化したもの

//ノードのソート用operator
bool nodecmp(node_t& a,node_t& b){
	return a.count > b.count;
}

//ハフマン木のクラス
class HuffmanTree{
	private:
		vector<node_t> heap;//ヒープ
		Code codetable;//符号表

		void show_entity(node_t *head);//show関数の実体
		void gencodetable_entity(node_t *head,string s);//encode関数の実体
	public:
		HuffmanTree();//コンストラクタ
		void makehuffmanheap();//ヒープを生成
		void makehuffmantree();//ヒープを元にハフマン木を生成

		node_t *gethead();//rootを返すゲッター
		void show();//ハフマン木を列挙
		void view_codetable();//符号表の表示
		void gencodetable();//符号表を生成
		bool IsOneElement();//ヒープがrootのみであることを示す
};

//コンストラクタ
HuffmanTree::HuffmanTree(){
}

//文字列をヒープに列挙していく
void HuffmanTree::makehuffmanheap(){
	string s;
	bool addflag;//新規文字列であるかのフラグ

	//標準入力から取得することを前提
	while(getline(cin,s)){
		while(!(s.empty())){//取得した文字列が空になるまで回す
			addflag = true;//新規文字列であることを示す
			for(int i=0;i<this->heap.size();i++){//ヒープを全探索
				if(this->heap[i].value.front() == s.back()){//ヒープに存在する文字の場合
					s.pop_back();//探索した文字は削除
					this->heap[i].count++;//出現回数をインクリメント

					addflag = false;//新規文字フラグオフ

					break;
				}
			}
			if(addflag){//新規文字出会った場合
				string str(1,s.back());//一文字抽出
				node_t tmp={str,1,{NULL,NULL}};//そこから新しくnodeを生成
				this->heap.PB(tmp);//nodeをpush
				s.pop_back();//探索した文字を削除
			}
		}
	}
	sort(this->heap.begin(),this->heap.end(),nodecmp);//操作完了後はソートしておく
	return;
}

//ハフマン木の生成
void HuffmanTree::makehuffmantree(){
	node_t *child,*leaf;

	if(this->heap.size() == 0)return;//入力が何も無い場合に無限ループになってバッファオーバーフローするので例外処理

	while(!(this->IsOneElement())){
		sort(this->heap.begin(),this->heap.end(),nodecmp);//内容のソート

		//追加する葉の初期化
		leaf = new node_t;

		//葉の子を初期化
		child = new node_t;
		child->childnode[0]=NULL;
		child->childnode[1]=NULL;

		//子をコピーして追加
		*child = this->heap.back();
		leaf->childnode[0] = child;
		this->heap.pop_back();//コピー元をpop_back
		
		//葉の子を初期化
		child = new node_t;
		child->childnode[0]=NULL;
		child->childnode[1]=NULL;

		//子をコピーして追加
		*child = this->heap.back();
		leaf->childnode[1] = child;
		this->heap.pop_back();//コピー元をpop_back

		//追加する葉の内容を初期化
		leaf->value = leaf->childnode[0]->value + leaf->childnode[1]->value;
		leaf->count = leaf->childnode[0]->count + leaf->childnode[1]->count;

		//葉をヒープにpush_back
		this->heap.PB(*leaf);
	}
	return;
}

//ハフマン木を表示
void HuffmanTree::show(){
	this->show_entity(this->gethead());
	cout << endl;
	return;
}

void HuffmanTree::show_entity(node_t *head){
	if(head == NULL){
		cout << "None" <<endl;
		return;
	}
	if(head->childnode[0] == NULL && head->childnode[1] == NULL)cout << head->value << " " << head->count << endl;

	if(head->childnode[0] != NULL)this->show_entity(head->childnode[0]);
	if(head->childnode[1] != NULL)this->show_entity(head->childnode[1]);

	return;
}

//符号表を生成する
void HuffmanTree::gencodetable(){
	this->gencodetable_entity(this->gethead(),"");
	cout << endl;
	return;
}

void HuffmanTree::gencodetable_entity(node_t *head,string s){
	if(head == NULL){
		cout << "None" <<endl;
		return;
	}
	if(head->childnode[0] == NULL && head->childnode[1] == NULL){
		cout << "gencode:" << head->value << "=" << s << endl;
		this->codetable[head->value] = s;
		return;
	}

	if(head->childnode[0] != NULL)this->gencodetable_entity(head->childnode[0],s+"0");
	if(head->childnode[1] != NULL)this->gencodetable_entity(head->childnode[1],s+"1");

	return;
}

//rootを取得するgetter
node_t *HuffmanTree::gethead(){
	return &(this->heap[0]);
}

//要素が一つであるかの判別
bool HuffmanTree::IsOneElement(){
	return (this->heap.size() == 1);
}

//符号表を表示
void HuffmanTree::view_codetable(){
	for(auto itr = this->codetable.begin();itr != this->codetable.end();++itr){
		cout << "'" << itr->first << "' = " << itr->second << endl;
	}
	return;
}

int main(){
	HuffmanTree tree;

	tree.makehuffmanheap();//ヒープに文字列列挙
	tree.makehuffmantree();//ヒープからハフマン木を生成(ヒープは破壊する)

	tree.show();//ハフマン木の表示

	tree.gencodetable();//符号生成

	tree.view_codetable();//符号表の表示

	return 0;
}

#include<iostream>
#include<string>
#include<vector>
#include<stack>
using namespace std;
struct Sentence{//(A|B)
    vector<string> subAxiom;//(A|B)中的A,B
};
vector<Sentence> pre;//在等号之前的子句集
vector<Sentence> post;//在等号之后的子句集
vector<string> Res_Stack;//储存临时的两个子句比较时候的subAxiom
string inpt;
bool succeed=false;
int main(){
    cout<<"欢迎来到归结法自动证明程序"<<endl;
    cout<<"由于一些技术原因当前只能接受已经化为合取范式的命题"<<endl;
    cout<<"一些说明:"<<endl;
    cout<<"请用&表示合取,｜表示析取"<<endl;
    cout<<"在合取的项之间,请使用()隔开"<<endl;
    cout<<"比如:(A)&(-A)"<<endl;
    cout<<"(A|B)&(C)|(D|E)"<<endl;
    cout<<"事实上可以省略合取符号,如(A)(B)也是合法的输入,代表(A)&(B)"<<endl;
    cout<<"----START----"<<endl;
    cout<<"请输入字符串公式:"<<endl;
    //下面首先读取合取范式，建立子句集
    cin>>inpt;
    int i=0;
    while(i<inpt.length()){
        while((inpt[i]!='(')){
            i++;
        }
        string tmp;
        Sentence tmpstc;
        i++;
        while(inpt[i]!=')'){
            if(inpt[i]!='|'){
            tmp+=inpt[i];}
            else{
                tmpstc.subAxiom.push_back(tmp);
                tmp.clear();
            }
            i++;
        }
        i++;
    pre.push_back(tmpstc);//一个while循环处理完一个子句
    }
    //对子句集进行归结
    for(int i=0;i<pre.size();i++){
        //先把第一个子句中的元素放入Res_Stack中
        for(int k=0;k<pre[i].subAxiom.size();k++){
            Res_Stack.push_back(pre[i].subAxiom[k]);
        }
        vector<string> Res_Copy=Res_Stack;//存一个只放入了一个子句的Res_Stack状态
        //然后开始放入第二个子句的元素
        for(int j=i+1;j<pre.size();j++){
            for(int m=0;m<pre[j].subAxiom.size();j++){
                string tmp1,tmp2;
                tmp1+='-';
                tmp1+=pre[j].subAxiom[m];
                bool hasTmp2=false;
                if(pre[j].subAxiom[m][0]=='-'){
                    hasTmp2=true;
                    for(int q=1;m<pre[j].subAxiom[m].size();q++){
                        tmp2+=pre[j].subAxiom[m][q];
                    }
                }//对于-P元素放入了--P与P
                bool needToEnter=true;
                for(int iii=0;iii<Res_Stack.size();iii++){
                    if((tmp1==Res_Stack[iii])||(tmp2==Res_Stack[iii])){
                        needToEnter=false;
                        Res_Stack.erase(remove(Res_Stack.begin(), Res_Stack.end(), Res_Stack[iii]), Res_Stack.end());
                        pre[j].subAxiom.erase(remove(pre[j].subAxiom.begin(), pre[j].subAxiom.end(), pre[j].subAxiom[m]), pre[j].subAxiom.end());
                        break;
                    }
                    if(pre[j].subAxiom[m]==Res_Stack[iii]){
                        needToEnter=false;
                    }
                }
                if(needToEnter){
                    Res_Stack.push_back(pre[j].subAxiom[m]);
                }
            }
            if((pre[j].subAxiom.size()==0)||(pre[i].subAxiom.size()==0)){
                //cout<<i<<" "<<j<<endl;调试用
                cout<<"该范式永假,对应的定理永真"<<endl;
                succeed=true;
                goto stop;
            }
            Res_Stack=Res_Copy;//判断完<i,j>了，还原Res_Stack
        }
    Res_Stack.clear();//遍历了所有<i,x>的x可能性，完全释放Res_Stack中的内容
    }
    stop:
    if(!succeed){
        cout<<"归结没有出现空子集,不代表这个命题不是永真的"<<endl;
    }
}

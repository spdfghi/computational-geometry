#include <bits/stdc++.h>
using namespace std;

typedef pair<int,int> PII;
typedef _Rb_tree<PII,PII,_Identity<PII>,less<PII>> RB_TREE;
typedef _Rb_tree_node_base TreeNode;

//一次生成的range tree
struct RangeTreeStatic
{
    static const int MAX_SIZE=1e5;
    map<PII,RB_TREE> ytree;
    RB_TREE xtree;
    int xmn,xmx,ymn,ymx;
    vector<PII> data;

    void addYSubTree(TreeNode *root, RB_TREE &src, RB_TREE &dst)
    {
        if(!root) return;
        auto [b,a]=src._S_value(root);
        dst._M_insert_unique(PII{a,b});
        addYSubTree(root->_M_left,src,dst);
        addYSubTree(root->_M_right,src,dst);
    }

    void buildYTree(TreeNode *root, RB_TREE &tree)
    {
        if(!root) return;
        PII p=tree._S_value(root);
        ytree[p]=RB_TREE();
        addYSubTree(root,tree,ytree[p]);
        buildYTree(root->_M_left,tree);
        buildYTree(root->_M_right,tree);
    }

    RangeTreeStatic(vector<PII> &data)
    {
        for(auto &x:data) xtree._M_insert_unique(x);
        buildYTree(xtree._M_root(),xtree);
    }

    void getYSubTree(TreeNode *root, RB_TREE &ytree)
    {
        if(!root) return;
        PII p=ytree._S_value(root);
        data.push_back({p.second,p.first});
        getYSubTree(root->_M_left,ytree);
        getYSubTree(root->_M_right,ytree);
    }

    void getYNode(TreeNode *root, RB_TREE &ytree, int st)
    {
        if(!root) return;
        PII p=ytree._S_value(root);

        if(p.first>=ymn&&p.first<=ymx) 
        {
            data.push_back({p.second,p.first});
            if(st==0)
            {
                getYNode(root->_M_left,ytree,1);
                getYNode(root->_M_right,ytree,2);
            }
            else if(st==1)
            {
                getYSubTree(root->_M_right,ytree);        
                getYNode(root->_M_left,ytree,1);
            }
            else 
            {
                getYSubTree(root->_M_left,ytree);
                getYNode(root->_M_right,ytree,2);
            }
        }
        else if(p.first<ymn)
        {
            getYNode(root->_M_right,ytree,0);
        }
        else 
        {
            getYNode(root->_M_left,ytree,0);
        }
    }

    void getXNode(TreeNode *root, int st)
    {
        if(!root) return;
        
        PII p=xtree._S_value(root);
        if(p.first>=xmn&&p.first<=xmx)
        {
            if(p.second>=ymn&&p.second<=ymx) data.push_back(p);
            if(st==0)
            {
                getXNode(root->_M_left,1);
                getXNode(root->_M_right,2);
            }
            else if(st==1)
            {
                if(root->_M_right)
                {
                    auto p=xtree._S_value(root->_M_right);
                    auto &tree=ytree[p];
                    getYNode(tree._M_root(),tree,0);
                    getXNode(root->_M_left,1);
                }
            }
            else 
            {
                if(root->_M_left)
                {
                    auto p=xtree._S_value(root->_M_left);
                    auto &tree=ytree[p];
                    getYNode(tree._M_root(),tree,0);
                    getXNode(root->_M_right,2);
                }
            }
        }
        else if(p.first<xmn)
        {
            getXNode(root->_M_right,0);
        }
        else 
        {
            getXNode(root->_M_left,0);
        }
    }

    vector<PII> find(int _xmn,int _xmx,int _ymn,int _ymx)
    {
        xmn=_xmn,xmx=_xmx,ymn=_ymn,ymx=_ymx;
        data.clear();
        getXNode(xtree._M_root(),0);
        return data;
    }
};

signed main(){
    set<PII> t,valid;
    vector<PII> data;
    int mn=3000,mx=4000;
    for(int i=0;i<5e5;++i)
    {
        int a=rand()%(int)1e5;
        int b=rand()%(int)1e5;
        t.insert({a,b});
        if(a>=mn&&a<=mx&&b>=mn&&b<=mx) valid.insert({a,b});
    }
    for(auto p:t) data.push_back(p);

    RangeTreeStatic rts(data);

    auto res=rts.find(mn,mx,mn,mx);
    bool flag=true;
    for(auto p:res)
    {
        if(!valid.count(p))
        {
            flag=false;
            break;
        }
        valid.erase(p);
    }
    if(valid.size()) flag=false;
    cout<<(flag?"pass":"ivalid");
}
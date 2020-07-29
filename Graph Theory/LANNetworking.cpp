//Functionalities as of now:
//1) Setting up Router Networks with Edges
//2) Calculation of Routing Tables via Bellmann Ford Algorithm in O(N^2);
//3) Finding the Maximal Strongly Connected Sub Network.
//4) Handling Link Failiure of between any two network routers with a direct edge.
#include<bits/stdc++.h>
using namespace std;
struct Edges{
    int a;    //Source
    int b;    //Destination
    int w;    //Cost(Favourability)

    Edges(int s,int d, int c){
        a=s;
        b=d;
        w=c;
    }
};
vector<Edges> ngraph;
vector<bool> vis(10000,false);
stack<int> st;
int tc=0;
string ans="";
void dfs(int src,vector<int> graph[],bool isecond=false){
    if(isecond){
        tc++;
    } 
       ans+=to_string(src)+" ";
       vis[src]=true;
    for(int i=0;i<graph[src].size();i++){
        if(!vis[graph[src][i]])
        dfs(graph[src][i],graph,isecond);
    }
    if(!isecond){
    st.push(src);
    }
}
void removeEdge(int u, int v)
{
    for(int j=0;j<ngraph.size();j++){
        if(ngraph[j].a==u && ngraph[j].b==v){
            ngraph.erase(ngraph.begin()+j);
        }
        if(ngraph[j].a==v && ngraph[j].b==v){
            ngraph.erase(ngraph.begin()+j);
        }
    }
}

void belford(int src,int n,vector<int> &dis){
    dis[src]=0;
    for(int i=0;i<n;i++){
        for(auto e: ngraph){
            int src,dest,cost;
            src=e.a;
            dest=e.b;
            cost=e.w;
            dis[dest]=min(dis[dest],dis[src]+cost);
        }
    }
}
int main(){
    int routers,connections;
    cin>>routers>>connections;
    vector<int> graph[routers+1];
    //For the first task of making Routing Tables for each node, we assume that the given graph is undirected.
    //For the second task of finding the Maximal Strongly Connected Sub-Network we have to assume that the graph is directed.
    for(int i=0;i<connections;i++){
        int origin;
        int destination;
        int cost;
        cin>>origin>>destination>>cost;
        ngraph.push_back({origin,destination,cost});
        graph[origin].push_back(destination);
        ngraph.push_back({destination,origin,cost});
    }
    //Cost or Link is assigned on the basis of desirability of Network Route for sending traffic.
    //We will now use Bellman Ford Algorithm in order to calculate the routing table for each node.
    int n=routers;
    vector<vector<int>> global(n+1,vector<int>(n+1,0));
    for(int i=1;i<=n;i++){
        vector<int> dis(n+1,10000);
        belford(i,connections*2,dis);
        cout<<" Routing Table for "<<i<<" router : "<<endl;
        for(int j=1;j<=n;j++){
            cout<<i<<" -> "<<j<<" With HOP Distance "<<dis[j]<<" "<<endl;
            global[i][j]=dis[j];
        }
        cout<<"____________________________________________________"<<endl;
    }
    //Compulsorily we had to run the loops O(N^2) times in order to accomodate shortest possible distance from one node
    //to every other node. A LAN System behaves in a similar manner, what we printed above is known as Routing Table.
    // There are some obvious shortcomings, firstly this method doesn't account for Network Link Failiures, hence, finding optimal route
    //for traffic isn't always guranteed.

    //Printing Global View of Each Node:
    cout<<" Final Distances Stored at Each Node (Global View) "<<endl;
    for(int i=1;i<=n;i++){
        cout<<i<<" -> ";
        for(int j=1;j<=n;j++){
            cout<<global[i][j]<<" ";
        }
        cout<<endl;
    }
            cout<<"____________________________________________________"<<endl;

    //Now we go on and find the maximal strongly connected sub-network.
    //A strongly connected component of a graph is defined as a component in which every node can visit every other node.
    //To do so we are going to implement KosaRaju Algorithm.
   for(int i=0;i<n;i++){
       if(!vis[i]){
             dfs(i,graph); //First make a normal dfs call to maintain the order of visiting elements in a stack.
       }
  }
  vector<int> revg[n+1];   //Now reverse the given graph and maintain it in another list.
  for(int i=0;i<n;i++){
      for(int j=0;j<graph[i].size();j++){
          revg[graph[i][j]].push_back(i);
     }
  }
  int max_ele=-1e9;
  string fans="";
  ans="";
  for(int i=0;i<n;i++)
  vis[i]=false;
  while(st.size()!=0){                          //Call DFS on each member of the elements still maintaing the given order of stack.
    int rvtx= st.top(); st.pop();
    if(!vis[rvtx]){
        ::tc=0;
    dfs(rvtx,revg,true);
    if(tc>max_ele){
        max_ele=tc;
        fans=ans;
    }
       ans="";
    }
  }
  cout<<"Maximal Strongly Connected Sub-Network has "<<max_ele<<" members and the members are : "<<fans<<endl;    
  //Typically a network failiures occurs when the link between two nodes is broken. This coerces the whole system to reset the rerouting tables.
  //Link Failiure is usually prevented by setting a node that continuously sends validation packets to it's neighbours and awaits acknowledgement.
  //If acknowledgement isn't received then, in terms of graph theory, the edge between the two "routers"/"nodes" is removed and the shortest distance is 
  //recalculated via Bellmann Ford Algorithm.
  cout<<"Enter link between two nodes to be killed"<<endl;
  int node1,node2;
  cin>>node1>>node2;
  removeEdge(node1,node2);
   for(int i=1;i<=n;i++){
        vector<int> dis(n+1,10000);
        belford(i,connections*2,dis);
        for(int j=1;j<=n;j++){
            global[i][j]=dis[j];
        }
    }
    cout<<"Global view after killing the link : "<<endl;
    for(int i=1;i<=n;i++){
        cout<<i<<" -> ";
        for(int j=1;j<=n;j++){
            cout<<global[i][j]<<" ";
        }
        cout<<endl;
    }
            cout<<"____________________________________________________"<<endl;
}

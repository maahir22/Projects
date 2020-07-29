#include<bits/stdc++.h>
using namespace std;
vector<tuple<int,int,int>> ngraph;
void belford(int src,int n,vector<int> &dis){
    dis[src]=0;
    for(int i=0;i<n;i++){
        for(auto e: ngraph){
            int src,dest,cost;
            tie(src,dest,cost)=e;
            dis[dest]=min(dis[dest],dis[src]+cost);
        }
    }
}
int main(){
    int routers,connections;
    cin>>routers>>connections;
    for(int i=0;i<connections;i++){
        int origin;
        int destination;
        int cost;
        cin>>origin>>destination>>cost;
        ngraph.push_back({origin,destination,cost});
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
    
}

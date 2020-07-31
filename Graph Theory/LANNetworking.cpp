//Contributions Handled : Graph Theory : Maahir Sharma
//Functionalities as of now:
//1) Setting up Router Networks with Edges
//2) Calculation of Routing Tables via Bellmann Ford Algorithm in O(N^2);
//3) Finding the Maximal Strongly Connected Sub Network via KosaRaju Algorithm.
//4) Handling Link Failiure of between any two network routers with a direct edge.
//5) Added Link State Packet and Flooding Feature so that when flood occurs each router has access to LSP of the flood initiator.
/*6) Added Optimal Cell Tower Placement Map is likened to a color,(channel–colors are limited to four), and found where to economically position broadcast towers 
     for maximum coverage via Four Color Map Problem || Vizing's theorem (Referenced) */

#include<bits/stdc++.h>

using namespace std;
struct Edges {
  int a; //Source
  int b; //Destination
  int w; //Cost(Favourability)

  Edges(int s, int d, int c) {
    a = s;
    b = d;
    w = c;
  }
    
};
struct LSP{
    int nodeid=-1;
    vector<int> neighbours;
    int sequence=1;
    LSP(int nodeid, vector<int> chk, int tt){
        this->nodeid=nodeid;
        neighbours=chk;
        sequence=tt;
    }
    LSP(){}
};
struct RFGraph{
    LSP packet;
    RFGraph(LSP obj){
        packet=obj;
    }
    RFGraph(){}
};
vector < Edges > ngraph;
vector < bool > vis(10000, false);
stack < int > st;
int tc = 0;
string ans = "";
void dfs(int src, vector < int > graph[], bool isecond = false) {
  if (isecond) {
    tc++;
  }
  ans += to_string(src) + " ";
  vis[src] = true;
  for (int i = 0; i < graph[src].size(); i++) {
    if (!vis[graph[src][i]])
      dfs(graph[src][i], graph, isecond);
  }
  if (!isecond) {
    st.push(src);
  }
}
void removeEdge(int u, int v) {
  for (int j = 0; j < ngraph.size(); j++) {
    if (ngraph[j].a == u && ngraph[j].b == v) {
      ngraph.erase(ngraph.begin() + j);
    }
    if (ngraph[j].a == v && ngraph[j].b == v) {
      ngraph.erase(ngraph.begin() + j);
    }
  }
}

void belford(int src, int n, vector < int > & dis) {
  dis[src] = 0;
  for (int i = 0; i < n; i++) {
    for (auto e: ngraph) {
      int src, dest, cost;
      src = e.a;
      dest = e.b;
      cost = e.w;
      dis[dest] = min(dis[dest], dis[src] + cost);
    }
  }
}
vector<bool> cvis(10000,false);
void lspdfs(int src, vector< int > lg[]){
    cvis[src]=true;
    for(int i=0;i<lg[src].size();i++){
        if(!cvis[lg[src][i]]){
            lspdfs(lg[src][i], lg);
        }
    }
}
void EdgeColor(int ed[][3], int e) {
   int i, c, j;
   for(i = 0; i < e; i++) {
      c = 1; //Assign color to current edge 1 initially.
      // If the same color is occupied by any of the adjacent edges,
      // then discard this color and go to flag again and try next color.
      flag:
      ed[i][2] = c;
      for(j = 0; j < e; j++) {
         if(j == i)
         continue;
         if(ed[j][0] == ed[i][0] || ed[j][0] == ed[i][1] || ed[j][1] == ed[i][0] || ed[j][1] == ed[i][1]) {
            if(ed[j][2] == ed[i][2]) {
               c++;
               goto flag;
            }
         }
      }
   }
}
int main() {
  int routers, connections;
  cout<<"Enter number of Routers"<<endl;
  cin >> routers;
  cout<<"Enter number of Connections"<<endl;
  cin >> connections;
  vector < int > graph[routers + 1];
  vector < int > lg [ routers + 1 ];
  vector< RFGraph > ls(routers + 1);
  //For the first task of making Routing Tables for each node, we assume that the given graph is undirected.
  //For the second task of finding the Maximal Strongly Connected Sub-Network we have to assume that the graph is directed.
  for (int i = 0; i < connections; i++) {
    int origin;
    int destination;
    int cost;
    cin >> origin >> destination >> cost;
    ngraph.push_back({ origin, destination,cost });
    graph[origin].push_back(destination);
    lg[origin].push_back(destination);
    lg[destination].push_back(origin);
    ngraph.push_back({destination, origin, cost });
  }
  //Cost or Link is assigned on the basis of desirability of Network Route for sending traffic.
  //We will now use Bellman Ford Algorithm in order to calculate the routing table for each node.
  int n = routers;
  vector < vector < int >> global(n + 1, vector < int > (n + 1, 0));
  for (int i = 1; i <= n; i++) {
    vector < int > dis(n + 1, 10000);
    belford(i, connections * 2, dis);
    cout << " Routing Table for " << i << " router : " << endl;
    for (int j = 1; j <= n; j++) {
      cout << i << " -> " << j << " With HOP Distance " << dis[j] << " " << endl;
      global[i][j] = dis[j];
    }
    cout << "____________________________________________________" << endl;
  }
  //Compulsorily we had to run the loops O(N^2) times in order to accomodate shortest possible distance from one node
  //to every other node. A LAN System behaves in a similar manner, what we printed above is known as Routing Table.
  // There are some obvious shortcomings, firstly this method doesn't account for Network Link Failiures, hence, finding optimal route
  //for traffic isn't always guranteed.

  //Printing Global View of Each Node:
  cout << " Final Distances Stored at Each Node (Global View) " << endl;
  for (int i = 1; i <= n; i++) {
    cout << i << " -> ";
    for (int j = 1; j <= n; j++) {
      cout << global[i][j] << " ";
    }
    cout << endl;
  }
  cout << "____________________________________________________" << endl;

  //Now we go on and find the maximal strongly connected sub-network.
  //A strongly connected component of a graph is defined as a component in which every node can visit every other node.
  //To do so we are going to implement KosaRaju Algorithm.
  for (int i = 0; i < n; i++) {
    if (!vis[i]) {
      dfs(i, graph); //First make a normal dfs call to maintain the order of visiting elements in a stack.
    }
  }
  vector < int > revg[n + 1]; //Now reverse the given graph and maintain it in another list.
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < graph[i].size(); j++) {
      revg[graph[i][j]].push_back(i);
    }
  }
  int max_ele = -1e9;
  string fans = "";
  ans = "";
  for (int i = 0; i < n; i++)
    vis[i] = false;
  while (st.size() != 0) { //Call DFS on each member of the elements still maintaing the given order of stack.
    int rvtx = st.top();
    st.pop();
    if (!vis[rvtx]) {
      ::tc = 0;
      dfs(rvtx, revg, true);
      if (tc > max_ele) {
        max_ele = tc;
        fans = ans;
      }
      ans = "";
    }
  }
  cout << "Maximal Strongly Connected Sub-Network has " << max_ele << " members and the members are : " << fans << endl;
  //Typically a network failiures occurs when the link between two nodes is broken. This coerces the whole system to reset the rerouting tables.
  //Link Failiure is usually prevented by setting a node that continuously sends validation packets to it's neighbours and awaits acknowledgement.
  //If acknowledgement isn't received then, in terms of graph theory, the edge between the two "routers"/"nodes" is removed and the shortest distance is 
  //recalculated via Bellmann Ford Algorithm.
  cout << "Enter link between two nodes to be killed" << endl;
  int node1, node2;
  cin >> node1 >> node2;
  removeEdge(node1, node2);
  for (int i = 1; i <= n; i++) {
    vector < int > dis(n + 1, 10000);
    belford(i, connections * 2, dis);
    for (int j = 1; j <= n; j++) {
      global[i][j] = dis[j];
    }
  }
  cout << "Global view after killing the link : " << endl;
  for (int i = 1; i <= n; i++) {
    cout << i << " -> ";
    for (int j = 1; j <= n; j++) {
      cout << global[i][j] << " ";
    }
    cout << endl;
  }
  cout << "____________________________________________________" << endl;
  //Creating LSP of a given node 
  //More precisely, each node creates an update packet, also called a link-state packet (LSP), which contains the following information:

/* The ID of the node that created the LSP
      A list of directly connected neighbors of that node, with the cost of the link to each one
      A sequence number
      A time to live for this packet */
  /* Consider a node X that receives a copy of an LSP that originated at some other node Y. 
  If not, it stores the LSP. If it already has a copy, it compares the sequence numbers; 
  if the new LSP has a larger sequence number, it is assumed to be the more recent, and that LSP is stored, replacing the old one.
  A smaller (or equal) sequence number would imply an LSP older (or not newer) than the one stored, so it would be discarded and no further action would be needed. 
  If the received LSP was the newer one, X then sends a copy of that LSP to all of its neighbors except the neighbor from which the LSP was just received.
  The fact that the LSP is not sent back to the node from which it was received helps to bring an end to the flooding of an LSP. 
  Since X passes the LSP on to all its neighbors, who then turn around and do the same thing, the most recent copy of the LSP eventually reaches all nodes.*/
  int lspnode;
  cout<<"Enter a node whose Link State packet is to be created"<<endl; //In real life this process is automated when update is done.
  cin>>lspnode;
  vector<int> neighbours(0);
  for(int j=0;j<lg[lspnode].size();j++)
    neighbours.push_back(lg[lspnode][j]);
   LSP obj={lspnode,neighbours,1};
   cout<<"Node Id : "<<obj.nodeid<<endl;
   cout<<"Sequence Number (Default) : "<<obj.sequence<<endl;
   cout<<"Neighbours : ";
   for(int i=0;i<obj.neighbours.size();i++){
       cout<<obj.neighbours[i]<<" ";
   }
   cout<<endl;                                                                  
   lspdfs(lspnode,lg);
   //Giving Node 2 a Node ID which is greater than 3, this will imply that Node 2 has received a more recent flood from node 3 with greater sequence.
   //Hence , it will not update it's LSP to the flood recieved from Node 1.
   ls[2].packet.nodeid=3;
   ls[2].packet.sequence=4;
   for(int i=1;i<=n;i++){
       if(cvis[i]){
              if(ls[i].packet.nodeid==-1 || ls[i].packet.sequence < obj.sequence ) //To replace the second condition of NodeId with Sequence Number.
               ls[i].packet = obj;
       }
   }
   cout<<"Flooding successful displaying details"<<endl;
   for(int i=1;i<=n;i++){
       if(ls[i].packet.nodeid!=obj.nodeid){
           continue;
       }
         cout<<" Flood received from "<<ls[i].packet.nodeid<<" @ Node : "<<i<<endl;   //Details of which Node has received the packet from initiator.
   }
   /*____________________________________________________________________________________________________________________________________________*/
    /*The next section deals with Cell Tower Placement Plan*/
    //We need to handle the following section carefully : 
    /*Allocation of a different channel in the spots where
    channel overlap occurs (marked in color). In analogy,
    colors must be different, so that cell phone signals are
    handed off to a different channel.*/
   int i, ni, e, j, max = -1;
   cout<<"Enter the number of towers to be placed on the map: ";
   cin>>ni;
   cout<<"Enter the number of links between these towers: ";
   cin>>e;
   int ed[e][3], deg[ni+1] = {0};
   for(i = 0; i < e; i++) {
      cout<<"\nEnter the vertex pair for tower "<<i+1;
      cout<<"\nN(1): ";
      cin>>ed[i][0];
      cout<<"N(2): ";
      cin>>ed[i][1];
      //calculate the degree of each vertex
      ed[i][2] = -1;
      deg[ed[i][0]]++;
      deg[ed[i][1]]++;
   }
   //find out the maximum degree.
   for(i = 1; i <= ni; i++) {
      if(max < deg[i])
      max = deg[i];
   }
   EdgeColor(ed , e);
   cout<<"\nAccording to Vizing's theorem this tower graph can use maximum of "<<max+1<<" colors to generate a valid edge coloring.\n";
   for(i = 0; i < e; i++)
   cout<<"\nThe color of the edge between Tower N(1):"<<ed[i][0]<<" and Tower N(2):"<<ed[i][1]<<" is: color"<<ed[i][2]<<".";
   /*Each cell region therefore uses one control tower with a
    specific channel and the region or control tower adjacent to it
    will use another tower and another channel. It is not hard to see
    how by using 4 channels, a node coloring algorithm can be
    used to efficiently plan towers and channels in a mobile
    network*/
}

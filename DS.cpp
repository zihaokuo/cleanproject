#include <cstdio>
#include <iostream>
#include <queue>
#include <stack> 
#include <cstring>
#include <string>
#include <fstream> 
using namespace std;
const int MAX_N = 1050;
const int MAX_M = 1050;
const int INF = 1000000;
int steps = 0;
int ans_x[MAX_N];
int ans_y[MAX_M];
int rows, cols , battery;
typedef pair<int, int> P;
char maze[MAX_N][MAX_M];
int Rx, Ry; ///location of R
bool flag[MAX_N][MAX_M]={0};

int k=0;
int s=0;
int pox[MAX_N];
int poy[MAX_M];
int sox[MAX_N];
int soy[MAX_M];
	
int d[MAX_N][MAX_M];///the distance of R to the point
int dx[4] = { 1,0,-1,0 }, dy[4] = { 0,1,0,-1 }; ///the displacement

bool flag2[4]={0};

int dir[4][2]={-1,0,1,0,0,-1,0,1};
struct node
{
	int x,y;
	node *last;
	node(int xx,int yy){x=xx;y=yy;};
	node(){};
};

void bfs3(int sx,int sy,int ex,int ey)
{
	node *n;
	queue<node*> Q;
	node *start = new node(sx,sy);
	start->last=NULL;
	Q.push(start);
	while (Q.size())
	{
		n = Q.front();
		Q.pop();

		if(n->x == ex&&n->y == ey)
			break;

		for (int i = 0; i < 4; i++)
		{
			int nx = n->x + dx[i];
			int ny = n->y + dy[i];
			if (0 <= nx&&nx < rows
				&& 0 <= ny&&ny < cols
				&&d[nx][ny] != INF
				&&d[nx][ny] != 0
				&&(d[nx][ny]-d[n->x][n->y]==-1||d[nx][ny]-d[n->x][n->y]==1))
			{
				if(i==0){node *right = new node(nx,ny); right->last=n; Q.push(right);}
				if(i==1){node *down = new node(nx,ny); down->last=n; Q.push(down);}
				if(i==2){node *left = new node(nx,ny); left->last=n; Q.push(left);}
				if(i==3){node *up = new node(nx,ny); up->last=n; Q.push(up);}
			}
		}
	}
	node *current = n;
	while(current!=NULL)
	{
		sox[s]=current->x;
		soy[s]=current->y;
		current = current->last;
		s++;
	}
	return;
}

void bfs2(int ex,int ey)
{
	if(flag[ex][ey]==1)return;
	node *n;
	queue<node*> Q;
	node *start = new node(Rx,Ry);
	start->last=NULL;
	Q.push(start);
	while (Q.size())
	{
		n = Q.front();
		Q.pop();

		if(n->x == ex&&n->y == ey)
			break;

		for (int i = 0; i < 4; i++)
		{
			int nx = n->x + dx[i];
			int ny = n->y + dy[i];
			if (0 <= nx&&nx < rows
				&& 0 <= ny&&ny < cols
				&&d[nx][ny] != INF
				&&d[nx][ny]>d[n->x][n->y])
			{
				if(i==0){node *right = new node(nx,ny); right->last=n; Q.push(right);}
				if(i==1){node *down = new node(nx,ny); down->last=n; Q.push(down);}
				if(i==2){node *left = new node(nx,ny); left->last=n; Q.push(left);}
				if(i==3){node *up = new node(nx,ny); up->last=n; Q.push(up);}
			}
		}
	}
	node *current = n;
	while(current!=NULL)
	{
		//cout<<current->x<<" "<<current->y<<endl;
		flag[current->x][current->y]=1;
		pox[k]=current->x;
		poy[k]=current->y;
		current = current->last;
		k++;
	}
	return;
}

void bfs()
{
	queue<P> Q;
	for (int i = 0; i < rows; i++)
    {
		for (int j = 0; j < cols; j++)
        {
			d[i][j] = INF;
        }
    }
	Q.push(P(Rx, Ry));
	d[Rx][Ry] = 0; ///start from R, distance = 0

	while (Q.size())
	{
		P p = Q.front();
		Q.pop();
		if(maze[p.first+1][p.second] != '0' &&
           maze[p.first-1][p.second] != '0' &&
           maze[p.first][p.second+1] != '0' &&
           maze[p.first][p.second-1] != '0') continue;

		for (int i = 0; i < 4; i++)
		{
			int nx = p.first + dx[i];
			int ny = p.second + dy[i];
			if (0 <= nx&&nx < rows
				&& 0 <= ny&&ny < cols
				&&maze[nx][ny] == '0')
			{
				Q.push(P(nx, ny));
				maze[nx][ny] = '2';
				steps++;
				ans_x[steps] = nx;
				ans_y[steps] = ny;
				d[nx][ny] = d[p.first][p.second] + 1;
			}
		}
	}
}

int main(int argc, char **argv)
{
    string filename = "./";
	string studentID = argv[1];
	string floor = "/floor.data";
	string final = "/final.path";
	string filenameinput =filename+studentID+floor;
	string filenameoutput =filename+studentID+final;
	ifstream fileinput;
	ofstream fileoutput;
	fileinput.open(filenameinput.c_str());
	fileinput>>rows>>cols>>battery;
	for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            fileinput >> maze[i][j];
        }
    }
    fileinput.close();

	for (int i = 0; i < rows; i++)
    {
		for (int j = 0; j < cols; j++)
		{
			if (maze[i][j] == 'R')
			{
				Rx = i;
				Ry = j;
			}
		}
    }
	
	bfs();

	for(int i = steps; i >= 1; --i)
	{
		bfs2(ans_x[i],ans_y[i]);
	}

    fileoutput.open(filenameoutput.c_str());

	stack<int> sx;
	stack<int> sy;
	int fx;
	int fy;
	int c = 0;
	for(int i=0;i<k;i++)
	{
		if(pox[k-i-1]==1&&poy[k-i-1]==1)
		{
			if(i==0){fx=pox[k-i-2];fy=poy[k-i-2];}
			else
			{
			sx.pop();sy.pop();
			while(!sx.empty())
			{
				c++;
				sx.pop();sy.pop();
			}
				c++;
				
			}	
			if(fx!=pox[k-i-2]||fy!=poy[k-i-2])
			{
				bfs3(fx,fy,pox[k-i-2],poy[k-i-2]);
				for(int j=0;j<s;j++)
				{
					c++; 
				}
				c++;
				fx=pox[k-i-2];fy=poy[k-i-2];
			}					  	
		}	
		else
		{
			c++;
			sx.push(pox[k-i-1]);
			sy.push(poy[k-i-1]);		
		}
		s=0;
	}
	sx.pop();sy.pop();
	while(!sx.empty())
	{
		c++;
		sx.pop();sy.pop();
	}
	c++;
	
	fileoutput<<c<<endl;
	for(int i=0;i<k;i++)
	{
		if(pox[k-i-1]==1&&poy[k-i-1]==1)
		{
			if(i==0){fx=pox[k-i-2];fy=poy[k-i-2];}
			else
			{
			sx.pop();sy.pop();
			while(!sx.empty())
			{
				fileoutput<<sx.top()<<" "<<sy.top()<<endl;
				sx.pop();sy.pop();
			}
				fileoutput<<Rx<<" "<<Ry<<endl;
				
			}	
			if(fx!=pox[k-i-2]||fy!=poy[k-i-2])
			{
				bfs3(fx,fy,pox[k-i-2],poy[k-i-2]);
				for(int j=0;j<s;j++)
				{
					fileoutput<<sox[s-j-1]<<" "<<soy[s-j-1]<<endl;
				}
				fileoutput<<Rx<<" "<<Ry<<endl;
				fx=pox[k-i-2];fy=poy[k-i-2];
			}					  	
		}	
		else
		{
			fileoutput<<pox[k-i-1]<<" "<<poy[k-i-1]<<endl;
			sx.push(pox[k-i-1]);
			sy.push(poy[k-i-1]);		
		}
		s=0;
	}
	sx.pop();sy.pop();
	while(!sx.empty())
	{
		fileoutput<<sx.top()<<" "<<sy.top()<<endl;
		sx.pop();sy.pop();
	}
	fileoutput<<Rx<<" "<<Ry;
	fileoutput.close();	
}

#include <fstream>
#include <queue>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>

using namespace std;

struct element{
	int r;	//termin dostepnosci
	int p;	//czas obslugi
	int q;	//czas dostarczenia
	
	element(){
		p = 0;
		q = 0;
		r = 0;
	}

	element(int _ri, int _pi, int _qi){
		p = _pi;
		q = _qi;
		r = _ri;
	}

};

struct wgQ{
    bool operator ()(const element& e1, const element& e2){
        if(e1.q < e2.q) return true;
        return false;
    }
};

struct wgR{
    bool operator ()(const element& e1, const element& e2){
        if(e1.r > e2.r) return true;
        return false;
    }
};

int main() {

	int n; 
	priority_queue<element, vector<element>, wgR> N;
	priority_queue<element, vector<element>, wgQ> G;

	ifstream infile;
	infile.open("SCHRAGE9.DAT", ios::in);
	infile >> n;
	if(infile.good() == false)
		cout << "Nie otworzyl sie\n";

	int p, r, q;

	for(int i=1;i<=n;i++){
		infile >> r;
		infile >> p;
		infile >> q;

		N.push(element(r,p,q));
	}

	int t = 0;
	element e;

	while(!G.empty() || !N.empty()){
		while(!N.empty() && N.top().r<=t){
			e = N.top();
			N.pop();
			G.push(e);
		}
		if (G.empty())
			t = N.top().r;
		else{
			e = G.top();
			G.pop();
			t = t + e.p;
			cmax = max(cmax, t+e.q);
		}

	}

	ofstream outfile("wynik9.txt", ios::out);
	outfile << cmax;


	infile.close();

	return 0;
}

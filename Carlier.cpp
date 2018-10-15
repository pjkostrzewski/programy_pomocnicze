#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <vector>

using namespace std;

struct element{
	int p,q,r;
	int k;

	element(){
		p = 0;
		q = 0;
		r = 0;
		k = 0;
	}

	element(int ri, int pi, int qi){
		p = pi;
		q = qi;
		r = ri;
		k = 0;
	}

};

struct wgR{
    bool operator ()(const element& e1, const element& e2){
        if(e1.r > e2.r) return true;
        return false;
    }
};

struct wgQ{
    bool operator ()(const element& e1, const element& e2){
        if(e1.q < e2.q) return true;
        return false;
    }
};

int schrageZ(vector<element> K){
	int cmax = -1;
	priority_queue<element, vector<element>, wgQ> G;
	priority_queue<element, vector<element>, wgR> N;
	int t = 0;
	element e;
	element l(0,0,0);

	for(int i=0;i<K.size();i++)
		N.push(element(K[i].r, K[i].p, K[i].q));

	while(!G.empty() || !N.empty()){
		while(!N.empty() && N.top().r<=t){
			e = N.top();
			N.pop();
			G.push(e);
			if(e.q > l.q) { //if do spd3
				l.p = t - e.r;
				t = e.r;
				if(l.p > 0)
					G.push(l);
			}
		}
		if (G.empty())
			t = N.top().r;
		else{

			e = G.top();
			G.pop();
			l = e; //spd3
			t = t + e.p;
			cmax = max(cmax, t+e.q);
		}

	}
	return cmax;
}

int schrageBez(vector<element>& K){
	int cmax = 0;
	priority_queue<element, vector<element>, wgQ> G;
	priority_queue<element, vector<element>, wgR> N;
	int t = 0;
	element e;

	for(int i=0;i<K.size();i++)
		N.push(element(K[i].r, K[i].p, K[i].q));

	K.clear();

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
			e.k = e.p + t; //id zadania
			K.push_back(e);
			G.pop();
			t = t + e.p;
			cmax = max(cmax, t+e.q);
		}

	}
	return cmax;
}

int maxK(int cmax, int n, vector<element> K){
	int maxK = 0;
	bool found = false;
	for(int i = 0; !found && i<n;i++){
		if(cmax == (K[i].k + K[i].q)){
			found = true;
			maxK = i;
		}
	}
	return maxK;
}

int minK(int cmax, int n, vector<element> K, int maxK){
	int tmp = 0, minK;
	for(minK = 0; minK<maxK; minK++){
		tmp = 0;
		for (int i=minK; i<=maxK; i++)
			tmp += K[i].p;
		if(cmax == (tmp + K[minK].r + K[maxK].q))
			return minK;
	}
	return minK;
}

int findC(int minK, int maxK, vector<element> K){
	int tmp = -1;
	bool found = false;
	for(int i=maxK; !found && i>=minK; i--)
		if(K[i].q < K[maxK].q){
			found = true;
			tmp = i;
		}
	return tmp;
}

int carlier(int ub, vector<element>& K, int n){
	int schrage = schrageBez(K);
	int a, b, c;
	int rMin = INT_MAX, qMin = INT_MAX, pSum = 0;

	if(schrage < ub)
		ub = schrage;


	b = maxK(schrage, n, K);
	a = minK(schrage, n, K, b);
	c = findC(a, b, K);

	if (c == -1)
		return ub;

	for(int i=c+1;i<=b;i++){
		rMin = min(rMin, K[i].r);
		qMin = min(qMin, K[i].q);
		pSum += K[i].p;
	}

	//dla r

	int rTmp = K[c].r;
	vector<element> Tmp = K;

	K[c].r = max(K[c].r, rMin + pSum);


	int lb = schrageZ(K);

	if(lb < ub){
		int ubTmp = carlier(ub, K, n);
		if(ubTmp != -1 && ubTmp < ub)
			ub = ubTmp;
	}

	K = Tmp;
	K[c].r = rTmp;

	//dla q

	int qTmp = K[c].q;

	K[c].q = max(K[c].q, qMin + pSum);

	lb = schrageZ(K);

	if(lb < ub){
		int ubTmp = carlier(ub, K, n);
		if(ubTmp != -1 && ubTmp < ub)
			ub = ubTmp;
	}


	K[c].q = qTmp;

	return ub;
}

int main() {

	int n;

	ifstream infile;
	infile.open("WT.DAT", ios::in);
	infile >> n;
	if(infile.good() == false)
		cout << "Nie otworzyl sie\n";

	int p, r, q;
	vector<element> K;

	for(int i=1;i<=n;i++){
		infile >> r;
		infile >> p;
		infile >> q;

		K.push_back(element(r,p,q));
	}



	ofstream outfile("wynikWT.txt", ios::out);
	outfile << carlier(INT_MAX, K, n);


	infile.close();



	return 0;
}

#include <iostream>
#include <cmath>
#include <queue>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

void czytaj (vector<int> &T,vector<int> &A,vector<int> &P,vector<int> &Permutacja)
{
    int n=0,m=0;
    fstream plik;
    plik.open("plik8.txt", ios_base::in);

    T.push_back(0);
    A.push_back(0);
    P.push_back(0);

    plik>>n;
    // T[i] - technologiczni zadania i, A[i] - maszynowi zadania i, P[i] - czas wykonania zadania i
    int a,b,c,d;
    a=b=c=d=0;

    for (int j=0; j<n; ++j){
        plik>>a;
        T.push_back(a);
    }
    for (int j=0; j<n; ++j){
        plik>>b;
        A.push_back(b);
    }
    for (int j=0; j<n; ++j){
        plik>>c;
        P.push_back(c);
    }
    plik>>m;
    for (int j=0; j<m+n; ++j){
        plik>>c;
        Permutacja.push_back(c);
    }
}

void dzialaj_LP(vector<int> &T,vector<int> &A,vector<int> &LP)
{
    int n=T.size();
    for(int i=1;i<=n;i++){
        if(T[i]!=0) LP[T[i]]++;
        if(A[i]!=0)LP[A[i]]++;
    }
}

void ALG(vector<int> &T,vector<int> &A,vector<int> &P,vector<int> &Permutacja)
{
    int n=T.size();
    vector<int> LP(n),PT(n),PA(n);
    queue<int> zadania;
    int C[n+1][2];

    for(int i=1;i<=n;i++){
        PT[T[i]]=i;
        PA[A[i]]=i;
    }

    dzialaj_LP(T,A,LP);

    for(int i=1;i<=n;++i)
    if(LP[i]==0){
        zadania.push(i);

    }

    for(int i=0;i<n+1;++i){
        C[i][0]=0;
        C[i][1]=0;
    }

    while(!zadania.empty()){
        int i=zadania.front();
        zadania.pop();

        LP[T[i]]--;

        if(LP[T[i]]==0 && T[i]!=0)
        {
            zadania.push(T[i]);
        }

        LP[A[i]]--;

        if(LP[A[i]]==0 && A[i]!=0) zadania.push(A[i]);

        C[i][0]=max(C[PT[i]][1],C[PA[i]][1]);
        C[i][1]=C[i][0]+P[i];
    }


    for(int i=0;i<n;++i){
        if(C[i][1]>C[n][0])
            C[n][0]=C[i][1];
    }
    cout<<"Cmax: "<<C[n][0];
}

void wyswietl(vector<int> &N)
{
    int n=N.size(); 
    cout<<"wyswietl: "<<" ";

    for(int i=1;i<n;i++){
        cout<<N[i]<<" ";
    }
    cout<<endl;
}

int main()
{
    vector<int> T,A,P,Permutacja;
    czytaj(T,A,P,Permutacja);
    ALG(T,A,P,Permutacja);
    return 0;
}


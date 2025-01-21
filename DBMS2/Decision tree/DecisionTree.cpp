#include "decision_tree.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <string>
#include <iostream>
#include <random>
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

random_device rd;
mt19937 gen(rd());

vector<ll> k1, k2;

// void RandomGenerator(int N)
// {
//     k1.push_back(N);
//     for (int i = 0; i < 4; i++)
//     {
//         k1.push_back(rand() % N);
//     }
//     // cout << "K1" << '\n';
//     // for( auto val : k1 ) cout << val << ' ';
// }

// void RandomGenerator2(int N)
// {
//     k2.push_back(0);
//     N /= 2;
//     for (int i = 0; i < 4; i++)
//     {
//         k2.push_back(rand() % N);
//     }
//     // cout << "K2" << '\n';
//     // for( auto val : k2 ) cout << val << ' ';
// }
void k_folds_cross_validation(vvs DATA, int k) {
    k = 15;
    int folds = k;
    int accuracy = 0;
    while(k--) {
    node *root = new node();
    vvs DATA2;

    func(root, DATA);
    map<string, string> mq;
    string expected = DATA[k][4];
    cout << expected << ' ';
    for (int j = 0; j < 4; j++)
    {
        // string s;
        // cin >> s;
        // cout << s << "  ";
        mq[DATA[0][j]] = DATA[k][j];
    }
    string actual;
    if (decision_function(root, mq)){
        actual = "Yes";
    }
    else {
        actual = "No";
        folds--;
    }
    cout << actual << '\n';
    if(actual == expected) accuracy++;
    cout << accuracy << '\n';
    }
    cout << "Average Accuracy is : " << (accuracy*1.0/folds)*100 << "%" << '\n';
}
int main()
{
    // cout << "Number of files: ";
    // int k, i, index = 0;
    // cin >> k;

    ll n, m;
    cin >> n >> m;
    vvs DATA(n);

    // RandomGenerator(n);
    // RandomGenerator2(n/2);

    // int yes = 0;
    // int no = 0;
    int k = 10;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            string s;
            cin >> s;
            DATA[i].push_back(s);
        }
    }
    map<string, string> mq, mq2;
    for (int j = 0; j < m - 1; j++)
    {
        string s;
        cin >> s;
        mq[DATA[0][j]] = s;
    }
    // for (int k = 0; k < 5; ++k)
    // {
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << DATA[i][j] << "       ";
        }
        cout << '\n';
    }
    // initial attr_vals;

    for (int j = 0; j < m - 1; j++)
    {
        map<string, bool> mp;
        for (int i = 1 ; i < n; i++)
        {
            if (!mp[DATA[i][j]])
            {
                mp[DATA[i][j]] = 1;
                attr_vals[DATA[0][j]].push_back(DATA[i][j]);
            }
        }
    }
    k_folds_cross_validation(DATA, k);
    // node *root = new node();
    // func(root, DATA);
    // int question;
    // cin >> question;

    // for (int i = 0; i < question; i++)
    // {
    // cout << "\n\nFor the following data : ";

    // for (int j = 0; j < m - 1; j++)
    // {
    //     string s;
    //     s = mq[DATA[0][j]];
    //     // cout << s << "  ";
    //     mq2[DATA[0][j]] = s;
    // }

    // // cout << "\n\nMy decision is : ";

    // if (decision_function(root, mq2))
    // {
    //     yes++;
    // }
    // else
    // {
    //     no++;
    // }
    // //}
    // //cout << yes <<  ' ' << no << '\n';
    // if (yes > no)
    // {
    //     cout << "Yes,I will play tennis\n";
    // }
    // else
    // {
    //     cout << "No, I won't play tennis\n";
    // }
}
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef vector<vector<string>> vvs;
vvs DATA(15);
#define F first
#define S second

ll yes = 0, no = 0;
ll n, m;
int question;
map<string, vector<string>> attr_vals;
map<string, string> mq;

double entropy(double pos, double neg);
double gain(vector<pair<int, int>> v, int sum_pos, int sum_neg);
pair<int, int> pure(vvs table, string attr, string value);

struct node
{
    string label;
    map<string, node *> child;
    map<string, bool> value;
    map<string, bool> isleaf;
};

double entropy(double pos, double neg)
{
    if (pos == 0)
    {
        return 0;
    }
    else if (neg == 0)
    {
        return 0;
    }
    double store1 = -pos / (pos + neg) * log2(pos / (pos + neg));
    double store2 = neg / (pos + neg) * log2(neg / (pos + neg));
    double final = store1 - store2;
    return final;
}

double gain(vector<pair<int, int>> v, int sum_pos, int sum_neg)
{
    // cout<<sum_pos<<" pn "<<sum_neg<<'\n';
    double sum_entropy = 0.0;
    for (int i = 0; i < v.size(); i++)
    {
        int pos = v[i].first;
        int neg = v[i].second;
        sum_entropy += entropy(pos, neg) * ((pos + neg) / ((double)(sum_pos + sum_neg)));
    }
    // cout<<"sume="<<sum_entropy<<'\n';
    return entropy(sum_pos, sum_neg) - sum_entropy;
}

string best_entropy_gain(vvs data)
{
    string best_attr;
    double bestgain = 0.0;

    for (int j = 0; j < data[0].size() - 1; j++)
    {
        vector<pair<int, int>> acc;
        int sum_pos = 0, sum_neg = 0;
        for (int z = 0; z < attr_vals[data[0][j]].size(); z++)
        {
            int pos = 0, neg = 0;
            for (int i = 1; i < data.size(); i++)
            {
                if (data[i][j] == attr_vals[data[0][j]][z])
                {
                    if (data[i][data[0].size() - 1] == "Yes")
                        pos++;
                    else
                        neg++;
                }
            }
            sum_pos += pos;
            sum_neg += neg;
            acc.push_back({pos, neg});
        }
        double ans = gain(acc, sum_pos, sum_neg);
        // cout<<ans<<" "<<data[0][j]<<'\n';
        if (ans > bestgain)
        {
            bestgain = ans;
            best_attr = data[0][j];
        }
    }

    return best_attr;
}

vvs filterData(vvs data, string attr_name, string val)
{
    int attr_col = 0;
    vvs filtered;
    vector<string> header;
    for (int j = 0; j < data[0].size(); j++)
    {
        if (data[0][j] == attr_name)
        {
            attr_col = j;
            continue;
        }
        header.push_back(data[0][j]);
    }
    filtered.push_back(header);

    for (int i = 1; i < data.size(); i++)
    {
        if (data[i][attr_col] == val)
        {
            vector<string> row;
            for (int j = 0; j < data[i].size(); j++)
            {
                if (j == attr_col)
                    continue;
                row.push_back(data[i][j]);
            }
            filtered.push_back(row);
        }
    }

    return filtered;
}

void printTable(vvs f)
{
    for (int i = 0; i < f.size(); i++)
    {
        for (int j = 0; j < f[i].size(); j++)
        {
            cout << f[i][j] << " ";
        }
        cout << '\n';
    }
    return;
}

pair<int, int> pure(vvs table, string attr, string value)
{
    int pos = 0, neg = 0, attr_col;

    for (int j = 0; j < table[0].size(); j++)
        if (table[0][j] == attr)
        {
            attr_col = j;
            break;
        }

    for (int i = 1; i < table.size(); i++)
    {
        if (table[i][attr_col] == value)
        {
            if (table[i][table[0].size() - 1] == "Yes")
                pos++;
            else
                neg++;
        }
    }
    pair<int, int> store;
    store = {pos, neg};
    return store;
}

bool vote(vvs table)
{
    int pos = 0, neg = 0;
    for (int i = 1; i < table.size(); i++)
    {
        if (table[i][table[0].size() - 1] == "Yes")
            pos++;
        else
            neg++;
    }
    if (pos >= neg)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void func(node *n, vvs table)
{

    string label = best_entropy_gain(table);
    // cout<<label<<'\n';
    n->label = label;
    for (int i = 0; i < attr_vals[label].size(); i++)
    {
        string val = attr_vals[label][i];
        pair<int, int> pn = pure(table, label, val);

        vvs newTable = filterData(table, label, val);

        if (pn.first == 0 && pn.second == 0)
        {
            bool res = vote(newTable);
            n->value[val] = res;
            n->isleaf[val] = true;
        }
        else if (pn.first == 0)
        {
            n->value[val] = false;
            n->isleaf[val] = true;
        }
        else if (pn.second == 0)
        {
            n->value[val] = true;
            n->isleaf[val] = true;
        }
        else
        {
            node *ch = new node();
            n->child[val] = ch;
            func(ch, newTable);
        }
    }
    return;
}

int decision_function(node *n, map<string, string> mq)
{
    string val = mq[n->label];
    if (n->isleaf[val])
    {
        if (n->value[val])
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (n->child.find(val) == n->child.end())
    {
        return -1;
    }
    decision_function(n->child[val], mq);
}

int DecisionTree()
{

    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    // cout << "attributes are : " << '\n';

    // cout << "Outlook"
    //      << "  Temperature"
    //      << "  Humidity"
    //      << "  Wind"
    //      << " Play Tennis\n";
    // vvs DATA(15);
    // n, m;
    // cin >> n;
    // cin >> m;

    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; j < m; j++)
    //     {
    //         string s;
    //         cin >> s;
    //         DATA[i].push_back(s);
    //     }
    // }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << DATA[i][j] << "       ";
        }
        cout << '\n';
    }
    /// initial attr_vals;
    // for (int j = 0; j < m - 1; j++)
    // {
    //     map<string, bool> mp;
    //     for (int i = 1; i < n; i++)
    //     {
    //         if (!mp[DATA[i][j]])
    //         {
    //             mp[DATA[i][j]] = 1;
    //             attr_vals[DATA[0][j]].push_back(DATA[i][j]);
    //         }
    //     }
    // }

    // node *root = new node();
    // func(root, DATA);
    // int question;
    // cin >> question;

    // for (int i = 0; i < question; i++)
    // {
    //     cout << "\n\nFor the following data : ";
    //     map<string, string> mq;
    //     for (int j = 0; j < m - 1; j++)
    //     {
    //         string s;
    //         cin >> s;
    //         // cout << s << "  ";
    //         mq[DATA[0][j]] = s;
    //     }
    //     // cout << "\n\nMy decision is : ";
    //     if (decision_function(root, mq))
    //         return 1;
    //     return 0;
    // }

    // vvs DATA;
    // attr_vals.clear();
    // for (int j = 0; j < m - 1; j++)
    // {
    //     map<string, bool> mp;
    //     for (int i = 1; i < n; i++)
    //     {
    //         if (!mp[DATA[i][j]])
    //         {
    //             mp[DATA[i][j]] = 1;
    //             attr_vals[DATA[0][j]].push_back(DATA[i][j]);
    //         }
    //     }
    // }
    // node *root = new node();
    // func(root, DATA);

    // if (decision_function(root, mq))
    // {
    //     yes++;
    // }
    // else
    // {
    //     no++;
    // }
    // if (yes >= no)
    // {
    //     return 1;
    // }
    // else
    // {
    //     return 0;
    // }
}
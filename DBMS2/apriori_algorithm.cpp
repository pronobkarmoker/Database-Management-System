#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
using namespace std;

vector<vector<int>> transactions;
int min_support = 2;

void loadTransactions()
{
    ifstream file("Apriori.txt");

    if (!file.is_open())
    {
        cerr << "Unable to open file Apriori.txt" << endl;
        return;
    }

    int n;
    file >> n;

    string line;
    getline(file, line);

    for (int i = 0; i < n; ++i)
    {
        getline(file, line);
        istringstream iss(line);
        int item;
        vector<int> transaction;

        while (iss >> item)
        {
            transaction.push_back(item);
        }

        transactions.push_back(transaction);
    }

    file.close();
}

vector<set<int>> generateCandidates(const vector<set<int>> &frequent_itemsets, int k)
{
    vector<set<int>> candidates;

    int n = frequent_itemsets.size();
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            set<int> candidate;
            set_union(frequent_itemsets[i].begin(), frequent_itemsets[i].end(),
                      frequent_itemsets[j].begin(), frequent_itemsets[j].end(),
                      inserter(candidate, candidate.begin()));

            if (candidate.size() == k)
            {
                candidates.push_back(candidate);
            }
        }
    }
    return candidates;
}

map<set<int>, int> supportCount(const vector<set<int>> &candidates)
{
    map<set<int>, int> support_count;

    for (const auto &transaction : transactions)
    {
        set<int> transaction_set(transaction.begin(), transaction.end());
        for (const auto &candidate : candidates)
        {
            if (includes(transaction_set.begin(), transaction_set.end(), candidate.begin(), candidate.end()))
            {
                support_count[candidate]++;
            }
        }
    }
    return support_count;
}

vector<set<int>> filterCandidates(const map<set<int>, int> &support_count)
{
    vector<set<int>> frequent_itemsets;

    for (const auto &item : support_count)
    {
        if (item.second >= min_support)
        {
            frequent_itemsets.push_back(item.first);
        }
    }
    return frequent_itemsets;
}

void printFrequentItemsets(const vector<set<int>> &frequent_itemsets, int level)
{
    cout << "Frequent Itemsets of size " << level << ":\n";
    for (const auto &itemset : frequent_itemsets)
    {
        cout << "{ ";
        for (const auto &item : itemset)
        {
            cout << item << " ";
        }
        cout << "}\n";
    }
    cout << endl;
}

void apriori()
{
    // Generate itemsets-1
    map<int, int> item_support;
    for (const auto &transaction : transactions)
    {
        for (int item : transaction)
        {
            item_support[{item}]++;
        }
    }

    // Filter itemsets-1
    vector<set<int>> frequent_itemsets;
    for (const auto &item : item_support)
    {
        if (item.second >= min_support)
        {
            frequent_itemsets.push_back({item.first});
        }
    }

    int k = 2;
    while (!frequent_itemsets.empty())
    {
        printFrequentItemsets(frequent_itemsets, k - 1);

        vector<set<int>> candidates = generateCandidates(frequent_itemsets, k);

        map<set<int>, int> support_count = supportCount(candidates);

        frequent_itemsets = filterCandidates(support_count);

        ++k;
    }
}

int main()
{
    loadTransactions();
    apriori();
    return 0;
}
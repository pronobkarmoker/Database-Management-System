#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
using namespace std;

void apriori_algo();
vector<set<int>> findCandidates(const vector<set<int>>& _itemsets, int k);
map<set<int>, int> supportCount(const vector<set<int>>& candidates);
vector<set<int>> filterCandidates(const map<set<int>, int>& support_count);
void printFrequentItemsets(const vector<set<int>>& _itemsets, int level);
void loadTransactions();

vector<vector<int>> transactions;
int min_support = 2;


int main() {
    loadTransactions();
    apriori_algo();
    return 0;
}

void apriori_algo() {
    // Generate itemsets-1
    map<int, int> item_support;
    for (const auto& transaction : transactions) {
        for (int item : transaction) {
            item_support[{item}]++;
        }
    }

    // Filter itemsets-1
    vector<set<int>> _itemsets;
    for (const auto& item : item_support) {
        if (item.second >= min_support) {
            _itemsets.push_back({item.first});
        }
    }

    int k = 2;
    while (!_itemsets.empty()) {
        printFrequentItemsets(_itemsets, k - 1);

        vector<set<int>> candidates = findCandidates(_itemsets, k);

        map<set<int>, int> support_count = supportCount(candidates);

        _itemsets = filterCandidates(support_count);

        ++k;
    }
}

vector<set<int>> findCandidates(const vector<set<int>>& _itemsets, int k) {
    vector<set<int>> candidates;
    
    int n = _itemsets.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            set<int> candidate;
            set_union(_itemsets[i].begin(), _itemsets[i].end(),
                      _itemsets[j].begin(), _itemsets[j].end(),
                      inserter(candidate, candidate.begin()));

            if (candidate.size() == k) {
                candidates.push_back(candidate);
            }
        }
    }
    return candidates;
}

map<set<int>, int> supportCount(const vector<set<int>>& candidates) {
    map<set<int>, int> support_count;
    
    for (const auto& transaction : transactions) {
        set<int> transaction_set(transaction.begin(), transaction.end());
        for (const auto& candidate : candidates) {
            if (includes(transaction_set.begin(), transaction_set.end(), candidate.begin(), candidate.end())) {
                support_count[candidate]++;
            }
        }
    }
    return support_count;
}

vector<set<int>> filterCandidates(const map<set<int>, int>& support_count) {
    vector<set<int>> _itemsets;
    
    for (const auto& item : support_count) {
        if (item.second >= min_support) {
            _itemsets.push_back(item.first);
        }
    }
    return _itemsets;
}

void printFrequentItemsets(const vector<set<int>>& _itemsets, int level) {
    cout << "Itemsets of size " << level << ":\n";
    for (const auto& itemset : _itemsets) {
        int i = 0;
        cout << "|| ";
        for (const auto& item : itemset) {
            if(i == itemset.size()-1){
                cout << item << " ";
                break;
            }
            cout << item << ", ";
            i++;
        }
        cout << "||\n";
    }
    cout << endl;
}

void loadTransactions(){
    ifstream file("Apriori.txt");
    
    if (!file.is_open()) {
        cerr << "Unable to open file Apriori.txt" << endl;
        return;
    }
    
    int n;
    file >> n;
    
    string line;
    getline(file, line);
    
    for (int i = 0; i < n; ++i) {
        getline(file, line);
        istringstream iss(line);
        int item;
        vector<int> transaction;
        
        while (iss >> item) {
            transaction.push_back(item);
        }
        
        transactions.push_back(transaction);
    }
    
    file.close();
}
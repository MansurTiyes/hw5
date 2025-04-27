#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// Helper function prototype
void wordleHelper(
    const string& in,
    string& current,
    string remaining_floating,
    const set<string>& dict,
    set<string>& results,
    size_t position);

// Main function implementation
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> results;
    string current = in;
    wordleHelper(in, current, floating, dict, results, 0);
    
    return results;
}

// basically find
int findChar(const string& str, char c) {
    // size t to silence warning
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == c) {
            return i;
        }
    }
    return -1;  
}

void wordleHelper(
    const string& in,           
    string& current,            
    string remaining_floating,  
    const set<string>& dict,    
    set<string>& results,       
    size_t position)            
{
    // Base case
    if (position == in.length()) {
        if (dict.find(current) != dict.end()) {
            if (remaining_floating.empty()) {
                results.insert(current);
            }
        }
        return;
    }
    
    if (in[position] != '-') {
        wordleHelper(in, current, remaining_floating, dict, results, position + 1);
        return;
    }
    
    for (char c = 'a'; c <= 'z'; c++) {
        current[position] = c;
        
        int float_idx = findChar(remaining_floating, c);
        if (float_idx != -1) {
            string new_remaining = remaining_floating;
            new_remaining.erase(float_idx, 1);
            
            wordleHelper(in, current, new_remaining, dict, results, position + 1);
        } 
        else {
            if (in.length() - position - 1 >= remaining_floating.length()) {
                wordleHelper(in, current, remaining_floating, dict, results, position + 1);
            }
        }
    }
    
    // BACKTRACK! DO NOT FORGET
    current[position] = '-';
}
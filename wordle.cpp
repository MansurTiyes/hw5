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
    size_t position,
    size_t remaining_slots);

// Main function implementation
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> results;
    string current = in;
    
    // Count empty slots for better pruning
    size_t empty_slots = 0;
    for(size_t i = 0; i < in.length(); i++) {
        if(in[i] == '-') empty_slots++;
    }
    
    wordleHelper(in, current, floating, dict, results, 0, empty_slots);
    
    return results;
}

// Custom find function
int findChar(const string& str, char c) {
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == c) {
            return i;
        }
    }
    return -1;  
}

// Helper function to check if a partial word could potentially be in the dictionary
bool isValidPrefix(const string& partial, const set<string>& dict) {
    // Skip prefix checking for very short partials (performance optimization)
    if(partial.length() < 3) return true;
    
    // Find the first word that would come after this prefix
    auto it = dict.lower_bound(partial);
    
    // If we found a word that starts with our prefix
    if(it != dict.end() && it->substr(0, partial.length()) == partial) {
        return true;
    }
    
    return false;
}

void wordleHelper(
    const string& in,           
    string& current,            
    string remaining_floating,  
    const set<string>& dict,    
    set<string>& results,       
    size_t position,
    size_t remaining_slots)            
{
    // Early pruning - if our partial word can't possibly be in the dictionary
    if(!isValidPrefix(current.substr(0, position), dict)) {
        return;
    }
    
    // Base case
    if (position == in.length()) {
        if (dict.find(current) != dict.end() && remaining_floating.empty()) {
            results.insert(current);
        }
        return;
    }
    
    // If this position already has a fixed letter
    if (in[position] != '-') {
        wordleHelper(in, current, remaining_floating, dict, results, position + 1, remaining_slots);
        return;
    }
    
    // Early pruning - if not enough slots left for remaining floating letters
    if (remaining_slots < remaining_floating.length()) {
        return;
    }
    
    // Try all possible letters at this position
    // First try floating letters (optimization)
    string used_floaters = "";
    for (size_t i = 0; i < remaining_floating.length(); i++) {
        char c = remaining_floating[i];
        
        // Skip duplicates to avoid redundant work
        if (findChar(used_floaters, c) != -1) continue;
        used_floaters += c;
        
        current[position] = c;
        
        // Remove this floating letter
        string new_remaining = remaining_floating;
        new_remaining.erase(i, 1);
        
        wordleHelper(in, current, new_remaining, dict, results, position + 1, remaining_slots - 1);
    }
    
    // Then try other letters if there are enough slots left for all floating letters
    if (remaining_slots > remaining_floating.length()) {
        for (char c = 'a'; c <= 'z'; c++) {
            // Skip if this is a floating letter (we already tried those)
            if (findChar(remaining_floating, c) != -1) continue;
            
            current[position] = c;
            wordleHelper(in, current, remaining_floating, dict, results, position + 1, remaining_slots - 1);
        }
    }
    
    // Backtrack
    current[position] = '-';
}
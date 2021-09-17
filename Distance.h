//
// Created by andrzej on 9/17/21.
//

#ifndef JUDDPERFT_DISTANCE_H
#define JUDDPERFT_DISTANCE_H

#include <iostream>
#include <vector>
#include <algorithm>

struct WordScores {
    std::string word;
    int priority;
    int scores = 0;
    bool returnIt = false;
    WordScores(std::string word, int priority) : word(word), priority(priority) {};
};

class Distance {
    static int lcslen(std::string a, std::string b);

    static int similarity(int lcs, std::string a, std::string b) {
        return lcs - std::max(a.size(), b.size());
    }

    static int added(int lcs, std::string a, std::string b) {
        return b.length()-lcs;
    }

    static int deleted(int lcs, std::string a, std::string b) {
        return a.length()-lcs;
    }

    static bool compareByScorePriority(const WordScores &a, const WordScores &b);

    static int distance(std::string a, std::string b);

    static void mostSimilar(std::string myword, std::vector<WordScores> &list);

public:
    //must not be reference, but copy
    static std::string process(std::string myword, std::vector<WordScores> list, int &code);
};


#endif //JUDDPERFT_DISTANCE_H

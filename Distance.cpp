//
// Created by andrzej on 9/17/21.
//

#include "Distance.h"

int Distance::lcslen(std::string a, std::string b) {
    int h = a.length() + 1;
    int w = b.length() + 1;
    int *C = new int[w * h]; // (a, b).length() + 1
    for (int i = 0; i <= a.length(); i++)
        C[i * w + 0] = 0;
    for (int j = 0; j <= b.length(); j++)
        C[0 * w + j] = 0;
    for (int i = 1; i <= a.length(); i++)
        for (int j = 1; j <= b.length(); j++) {
            if (a[i - 1] == b[j - 1])//i-1,j-1
                C[i * w + j] = C[(i - 1) * w + j - 1] + 1;
            else
                C[i * w + j] = std::max(C[i * w + j - 1], C[(i - 1) * w + j]);
        }
    int len = C[w * h - 1];
    delete[] C;
    return len;
}

bool Distance::compareByScorePriority(const WordScores &a, const WordScores &b) {
    if (a.scores != b.scores)
        return a.scores > b.scores;
    else if (a.priority != b.priority)
        return a.priority > b.priority;
    else
        return a.word > b.word;
}

int Distance::distance(std::string a, std::string b) {
    int lcs = lcslen(a, b);
    int _added = added(lcs, a, b);
    int _deleted = deleted(lcs, a, b);
    if (_added>_deleted)
        return  _added+2*_deleted;
    else
        return 2*_added+2*_deleted;
}

/*
 * If returns with similarity 0, returns only one
 * if max priority < -len myword / 2 -> return nothing
 * if returns all with max similarity if have the same priority
 * else returns all with maximal priority
 * */
void Distance::mostSimilar(std::string myword, std::vector<WordScores> &list) {
    for (auto &ws: list) {
        ws.scores = -distance(myword, ws.word);
    }
    std::sort(list.begin(), list.end(), compareByScorePriority);
    int maxscores = list[0].scores;
    int maxpriority = list[0].priority;
    if (maxscores == 0) {
        list[0].returnIt = true;
        return;
    }
    for (auto &ws: list) {
        if (ws.scores < maxscores || ws.priority < maxpriority) break;
        ws.returnIt = true;
    }
}

/*
 * 0: found exact implemented method
 * 1: found exact not implemented method
 * 2: found one implemented method
 * 3: found many implemented methods
 * 4: found one not implemented method
 * 5: found many not implemented methods
 * 6: not found too short
 * 7: not found tool long
 * 8: comparable length
 * 9: count = 0
 * */
std::string Distance::process(std::string myword, std::vector<WordScores> list, int &code) {
    mostSimilar(myword,list);
    const WordScores &first = list[0];
    if (first.word==myword) {
        if (first.priority > 0) {
            code = 0;
            return "";
        }
        else {
            code = 1;
            return "command '"+first.word+"' is unimplemented";
        }
    }
    int count = 0;
    for (auto &ws : list)
        if (ws.returnIt) count++;
    if (count==0) {
        code = 8;
        return "matching command '"+myword+"' not found";
    }
    if (-first.scores>myword.size()*1.5) {
        double len_ratio =  (double) myword.size() / first.word.size();
        if (len_ratio<0.7) {
            code = 6;
            return "command '"+myword+"' not found, probably too short";
        } else if (len_ratio>1.4) {
            code = 7;
            return "command '"+myword+"' not found, probably too long";
        } else {
            code = 8;
            return "command '"+myword+"' not found";
        }
    }
    int maxpriority = first.priority;
    std::string result;
    if (count==1) {
        result = "do you mean the ";
        if (maxpriority>0) {
            result+="command ";
            code = 2;
        }
        else {
            result+="unimplemented command ";
            code = 3;
        }
        result += "'"+first.word+"'";
        return result+"?";
    } else {
        result = "do you mean any of the ";
        if (maxpriority>0) {
            result+="commands";
            code = 4;
        }
        else {
            result+="unimplemented commands";
            code = 5;
        }
        bool add_or = false;
        for (auto &ws: list) {
            if (!ws.returnIt) break;
            if (add_or)
                result += " or ";
            else
                result += " ";
            result += "'"+ws.word+"'";
            add_or = true;
        }
        return result+"?";
    }
}


#include <stdio.h>
#include <string>
#include <algorithm>
#include <iostream>

#define NoError 0
using namespace std;


int GetConentIdFromURL(const string&url, string&conentId)
{
    string lower = url;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    string::size_type pos1 = lower.find("rtsp://");
    if (pos1 == string::npos) {
        return -1;
    }
    pos1 += 7;
    string::size_type pos2 = lower.find("/", pos1);
    if (pos2 == string::npos) {
        return -2;
    }
    string serverAddr = lower.substr(pos1, pos2 - pos1);
    pos2++;
    string::size_type pos3 = lower.find("/", pos2);
    if (pos3 == string::npos) {
        return -3;
    }
    string randnum =  lower.substr(pos2, pos3 - pos2);
    pos3++;
    string::size_type pos4 = lower.find("?", pos3);
    if (pos4 == string::npos) {
        conentId = lower.substr(pos3);
    } else   {
        conentId = lower.substr(pos3, pos4 - pos3);
    }
    return NoError;
}
int GetSTBidFromURL(const string& url, string& stbid)
{
    string lower = url;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    string::size_type pos1 = lower.find("?");
    if (pos1 == string::npos) {
        return -1; // Not found
    }
    string::size_type pos2 = lower.find("stbid=", pos1);
    if (pos2 == string::npos) {
        return -1;
    }
    pos2 += 6; // len("stbid=")
    string::size_type pos3 = lower.find("&", pos2);
    if (pos3 == string::npos) {
        stbid = lower.substr(pos2);
    } else {
        stbid = lower.substr(pos2, pos3 - pos2);
    }
    return NoError;
}
int main()
{
    string s("rTSp://127.0.0.1/1/123.mP4?sTBId=123456&asdf");
    cout << s << endl;
    string contentid;
    GetConentIdFromURL(s, contentid);
    cout << contentid << endl;
    string stbid;
    GetSTBidFromURL(s, stbid);
    cout << stbid << endl;
    return 0;
}

#pragma once
#include <urlmon.h>
#pragma comment(lib,"urlmon.lib")
#include <string>
#ifndef HTTPUTIL_H
#define HTTPUTIL_H
#include <windows.h>
#include <string>
#include <stdio.h>
#include<iostream>
#include <MQCore/MQHeader.h>
#include <GlobalVar.h>
#include <cstdlib> //random
#include <ctime>
using namespace std;
using namespace MQ;

#pragma comment(lib,"ws2_32.lib")
using namespace std;

bool DownloadUrlmon(string strURL, string strPath)
{
    size_t len0 = strURL.length();
    int nmlen0 = MultiByteToWideChar(CP_ACP, 0, strURL.c_str(), len0 + 1, NULL, 0);
    wchar_t* bufferURL = new wchar_t[nmlen0];
    MultiByteToWideChar(CP_ACP, 0, strURL.c_str(), len0 + 1, bufferURL, nmlen0);
    size_t len1 = strURL.length();
    int nmlen1 = MultiByteToWideChar(CP_ACP, 0, strURL.c_str(), len1 + 1, NULL, 0);
    wchar_t* bufferPath = new wchar_t[nmlen1];
    MultiByteToWideChar(CP_ACP, 0, strPath.c_str(), len1 + 1, bufferPath, nmlen1);
    HRESULT hr = URLDownloadToFile(NULL, bufferURL, bufferPath, 0, NULL);
    if (hr != S_OK)
        return false;
    return true;
}

//string strPathImageR1 = "https:\/\/tvax4.sinaimg.cn\/large\/ec43126fgy1gt3xaqo85aj20xc1eox6p.jpg";
//"https:\\/\\/tvax2.sinaimg.cn\\/large\\/ec43126fgy1h0wy9q5ftfj21zk2m0qv5.jpg"
// "https:/\\/tvax4.sinaimg.cn\\/large\\/004kfMibgy1gvoixrj7e5j61111jkqnj02.jpg"
//"https:\"\\/tva2.sinaimg.cn\\/large\\/ec43126fgy1h0d9py0r03j228e35pqv7.jpg"
//string strPathImage1 = R"(aaa.png)";
//int main()
//{
//	DownloadUrlmon(strPathImageR1, strPathImage1);
//}

void mParseUrl(char* mUrl, string& serverName, string& filepath, string& filename);
SOCKET connectToServer(char* szServerName, WORD portNum);
int getHeaderLength(char* content);
char* readUrl2(char* szUrl, long& bytesReturnedOut, char** headerOut);


char* sendRequest(char szUrl[]) {
    WSADATA wsaData;
    long fileSize;
    char* memBuffer, * headerBuffer;

    memBuffer = headerBuffer = nullptr;

    if (WSAStartup(0x101, &wsaData) != 0)
        return nullptr;

    memBuffer = readUrl2(szUrl, fileSize, &headerBuffer);
    printf("returned from readUrl\n");
    printf("data returned:\n%s", memBuffer);
    if (fileSize != 0) {
        //delete (memBuffer);
        delete (headerBuffer);
    }
    WSACleanup();
    return memBuffer;
}

void mParseUrl(char* mUrl, string& serverName, string& filepath, string& filename) {
    string::size_type n;
    string url = mUrl;

    if (url.substr(0, 7) == "http://")
        url.erase(0, 7);

    if (url.substr(0, 8) == "https://")
        url.erase(0, 8);

    n = url.find('/');
    if (n != string::npos) {
        serverName = url.substr(0, n);
        filepath = url.substr(n);
        n = filepath.rfind('/');
        filename = filepath.substr(n + 1);
    }

    else {
        serverName = url;
        filepath = "/";
        filename = "";
    }
}

SOCKET connectToServer(char* szServerName, WORD portNum) {
    struct hostent* hp;
    unsigned int addr;
    struct sockaddr_in server;
    SOCKET conn;

    conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (conn == INVALID_SOCKET)
        return NULL;

    if (inet_addr(szServerName) == INADDR_NONE) {
        hp = gethostbyname(szServerName);
    }
    else {
        addr = inet_addr(szServerName);
        hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
    }

    if (hp == nullptr) {
        closesocket(conn);
        return NULL;
    }

    server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(portNum);
    if (connect(conn, (struct sockaddr*)&server, sizeof(server))) {
        closesocket(conn);
        return NULL;
    }
    return conn;
}

int getHeaderLength(char* content) {
    const char* srchStr1 = "\r\n\r\n", * srchStr2 = "\n\r\n\r";
    char* findPos;
    int ofset = -1;

    findPos = strstr(content, srchStr1);
    if (findPos != nullptr) {
        ofset = findPos - content;
        ofset += strlen(srchStr1);
    }

    else {
        findPos = strstr(content, srchStr2);
        if (findPos != nullptr) {
            ofset = findPos - content;
            ofset += strlen(srchStr2);
        }
    }
    return ofset;
}

char* readUrl2(char* szUrl, long& bytesReturnedOut, char** headerOut) {
    const int bufSize = 512;
    char readBuffer[bufSize], sendBuffer[bufSize], tmpBuffer[bufSize];
    char* tmpResult = nullptr, * result;
    SOCKET conn;
    string server, filepath, filename;
    long totalBytesRead, thisReadSize, headerLen;

    mParseUrl(szUrl, server, filepath, filename);

    // step 1, connect //
    conn = connectToServer((char*)server.c_str(), 80);

    // step 2, send GET request /
    sprintf(tmpBuffer, "GET %s HTTP/1.0", filepath.c_str());
    strcpy(sendBuffer, tmpBuffer);
    strcat(sendBuffer, "\r\n");
    sprintf(tmpBuffer, "Host: %s", server.c_str());
    strcat(sendBuffer, tmpBuffer);
    strcat(sendBuffer, "\r\n");
    strcat(sendBuffer, "\r\n");
    send(conn, sendBuffer, strlen(sendBuffer), 0);

    //    SetWindowText(edit3Hwnd, sendBuffer);
    printf("Buffer being sent:\n%s", sendBuffer);

    // step 3 - get received bytes
        // Receive until the peer closes the connection
    totalBytesRead = 0;
    while (1) {
        memset(readBuffer, 0, bufSize);
        thisReadSize = recv(conn, readBuffer, bufSize, 0);

        if (thisReadSize <= 0)
            break;

        tmpResult = (char*)realloc(tmpResult, thisReadSize + totalBytesRead);

        memcpy(tmpResult + totalBytesRead, readBuffer, thisReadSize);
        totalBytesRead += thisReadSize;
    }

    headerLen = getHeaderLength(tmpResult);
    long contenLen = totalBytesRead - headerLen;
    result = new char[contenLen + 1];
    memcpy(result, tmpResult + headerLen, contenLen);
    result[contenLen] = 0x0;
    char* myTmp;

    myTmp = new char[headerLen + 1];
    strncpy(myTmp, tmpResult, headerLen);
    myTmp[headerLen] = NULL;
    delete (tmpResult);
    *headerOut = myTmp;

    bytesReturnedOut = contenLen;
    closesocket(conn);
    return (result);
}
#endif // HTTPUTIL_H



string getimage(string &url)
{
    char* resData = sendRequest("http://aqua.iw233.cn/api.php?sort=yin&type=json");
    string str = resData;
    str.erase(0, str.find("[\"") + 2);
    str.erase(str.find("\"]"), str.length() - str.find("\"]"));
    while (str.find("\\") != string::npos)
    {
        str.replace(str.find("\\"), 2, "\/");
    }
    string name = "C:\\Users\\Administrator\\Desktop\\MyQQ\\aaa.jpg";
    string strPathImage1 = name;
    DownloadUrlmon(str, strPathImage1);
    delete resData;
    return str;
}

void imagepost(const MQ::Event::NormalEvent& e)
{
    string imageurl = "";
    if (e.msg=="白毛")
    {
        Api::MessageAPI::UpLoadPic(e.botQQ, 2, e.sourceId, getimage(imageurl));
        Api::MessageAPI::UpLoadPic(e.botQQ, 2, e.sourceId, "C:\\Users\\Administrator\\Desktop\\MyQQ\\aaa.jpg");
    }
}
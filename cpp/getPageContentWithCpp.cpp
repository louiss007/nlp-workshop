#include <iostream>
#include <string>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

void parseHostAndPagePath(const string url, string &hostUrl, string &pagePath) {
    hostUrl = url;
    pagePath = "/";
    int pos = hostUrl.find("http://");
    if (-1 != pos)
        hostUrl = hostUrl.replace(pos, 7, "");
    pos = hostUrl.find("https://");
    if(-1 != pos)
        hostUrl = hostUrl.replace(pos, 8, "");
    pos = hostUrl.find("/");
    if(-1 != pos) {
        pagePath = hostUrl.substr(pos);
        hostUrl = hostUrl.substr(0, pos);
    }
}

string getPageContent(const string url){
    struct hostent *host;
    string hostUrl, pagePath;
    parseHostAndPagePath(url, hostUrl, pagePath);
    if (0 == (host = gethostbyname(hostUrl.c_str()))){
        cout << "gethostbyname error" << endl;
        exit(1);
    }

    struct sockaddr_in pin;
    int port = 80;
    bzero(&pin, sizeof(pin));
    pin.sin_family = AF_INET;
    pin.sin_port = htons(port);
    pin.sin_addr.s_addr = ((struct in_addr*)(host->h_addr))->s_addr;
    int isock;
    if((isock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        cout << "open socket error" << endl;
        exit(1);
    }

    string requestHeader;
    requestHeader = "GET " + pagePath + " HTTP/1.1\r\n";
    requestHeader += "Host: " + hostUrl + "\r\n";
    requestHeader += "Accept: */*\r\n";
    requestHeader += "User-Agent: Mozilla/4.0(compatible)\r\n";
    requestHeader += "connection:Keep-Alive\r\n";
    requestHeader += "\r\n";

    if(connect(isock, (const sockaddr*)&pin, sizeof(pin)) == -1){
        cout << "connect error" << endl;
        exit(1);
    }
    if(send(isock, requestHeader.c_str(), requestHeader.size(), 0) == -1){
        cout<<"send error\n"<<endl;
        exit(1);
    }

    struct timeval timeout = {1,0};
    setsockopt(isock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(struct timeval));
    char c;
    bool flag = true;
    while (recv(isock, &c, 1, 0) > 0) {
        if ('\r' == c) {
            continue;
        } else if('\n' == c) {
            if(false == flag)
                break;
            flag = false;
        }else{
            flag = true;
        }
    }

    int len, BUFFER_SIZE=512;
    char buffer[BUFFER_SIZE];
    string pageContent="";
    while((len = recv(isock, buffer, BUFFER_SIZE-1, 0)) > 0){
        buffer[len] = '\0';
        pageContent += buffer;
    }
    return pageContent;
}

int main()
{
    //cout << getPageContent("http://www.hao123.com") << endl;
    cout << getPageContent("https://hot.browser.miui.com/api/comment/review/admin/list?appId=browser&docId=0JizNFIn&userSign=7b25efccdb40a190217d15713e9e50f29f1fed5a") << endl;
    return 0;
}

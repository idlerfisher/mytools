#pragma once
#include <stdhdr.h>
#include <curl/curl.h>

class CCurl
{
public:
    CCurl();
    virtual ~CCurl();

    bool    Init();
    void    Destroy();
    bool    Get(const string &visitUrl, string &htmlStr, string *errStr = NULL);
    bool    Get(const string &visitUrl, FILE *fp, string *errStr = NULL);
    bool    Post(const string &actionUrl, const string &postData, string &htmlStr, string *errStr = NULL);
    void    Reset();
    void    SetCookiesFileName(const string &filename);
    string  GetCookiesFileName();

    static bool     GlobalInit(long flags = CURL_GLOBAL_ALL);
    static void     GlobalCleanup();
    static size_t   WriteToMem(char *ptr, size_t size, size_t nmemb, void *userdata);
    static size_t   WriteToFile(char *ptr, size_t size, size_t nmemb, void *userdata);
    static int      CurlDebug(CURL *pcurl, curl_infotype itype, char * pData, size_t size, void *);

    //url编码
    static int      HexToI(const char *s);
    static string   UrlEncode(const string &src);
    static string   UrlDecode(const string &src);

private:
    CURL   *m_pCurl;
    string  m_CookiesFileName;
    struct  curl_slist *m_pConnectionList;
    static  const char *m_UserAgent;
};

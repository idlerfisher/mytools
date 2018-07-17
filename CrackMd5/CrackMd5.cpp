#include <iostream>
#include <ctime>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "CMd5.h"

using namespace std;

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

static int g_srcLen = 0;
static int64_t g_count = 0;

// 取 [min, max) 之间的随机数，半开半闭区间
int GetRandom(int min, int max)
{
    return (int)((double)rand() / (RAND_MAX + 1) * (max - min) + min);
}

void CrackPasswd(std::string& pwd, int len)
{
    if (0 == len)
    {
        g_count++;
        //cout << g_count++ << "\t" << pwd << endl;
    }
    else
    {
        for (int i = 0; i < g_srcLen; ++i)
        {
            //pwd[len - 1] = g_szText[i];
            CrackPasswd(pwd, len - 1);
        }
    }
}

void Crack(int len)
{
    for (int i = 1; i <= len; ++i)
    {
        std::string pwd;
        pwd.resize(i);
        CrackPasswd(pwd, i);
    }
}

int64_t CalcCrackNum(int nSrcLen, int nPwdLen)
{
    int64_t nResultNum = 0;
    for (int i = nPwdLen; i > 0; --i)
    {
        nResultNum += (int64_t)pow(nSrcLen, i);
    }
    return nResultNum;
}

std::string SizeToString(int64_t size)
{
    static const int64_t s_kb = (int64_t)pow(1024, 1);
    static const int64_t s_mb = (int64_t)pow(1024, 2);
    static const int64_t s_gb = (int64_t)pow(1024, 3);
    static const int64_t s_tb = (int64_t)pow(1024, 4);

    char buf[256] = { 0 };

    if (size < s_kb)
    {
        sprintf_s(buf, sizeof(buf), "%lldB", size);
    }
    else if (size < s_mb)
    {
        sprintf_s(buf, sizeof(buf), "%.1fKB", 1.0 * size / s_kb);
    }
    else if (size < s_gb)
    {
        sprintf_s(buf, sizeof(buf), "%.1fMB", 1.0 * size / s_mb);
    }
    else if (size < s_tb)
    {
        sprintf_s(buf, sizeof(buf), "%.1fGB", 1.0 * size / s_gb);
    }
    else
    {
        sprintf_s(buf, sizeof(buf), "%.1fTB", 1.0 * size / s_tb);
    }

    return std::string(buf);
}

std::string TotalSpace(int nPwdLen)
{
    int64_t nResultNum = CalcCrackNum(g_srcLen, nPwdLen);
    int64_t nSize = nResultNum * (32 + nPwdLen);
    return SizeToString(nSize);
}

//!"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
int InitSrcText(char* szText, int nLen)
{
    for (int i = 32, j = 0; i <= 126 && j < nLen; ++i, ++j)
    {
        szText[j] = (char)i;
    }
    return min(nLen, 126-32+1);
}

int main()
{
    time_t tmLast = time(NULL);
    char szSrc[100] = { 0 };
    const int c_pwdLen = 10;

    g_srcLen = InitSrcText(szSrc, sizeof(szSrc));
    //cout << "原始字符个数:" << g_srcLen << endl;

    cout << "密码长度:" << c_pwdLen << endl;
    cout << "结果个数:" << CalcCrackNum(g_srcLen, c_pwdLen) << endl;
    cout << "占用空间:" << TotalSpace(c_pwdLen) << endl;

//     Crack(c_crackNum);
//     cout << g_count << endl;

//    cout << "cost time:" << time(NULL) - tmLast << "秒" << endl;

    system("pause");

    return 0;
}
// FileInfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <afxwin.h>
#include <iostream>
#include <map>
#include "CMd5.h"
#include <Shlwapi.h>

using namespace std;

//路径 时间戳
typedef std::map<CString, CString> MapFileInfo;

//程序名称
CString g_AppName;

#define OUT_FILE "project.manifest"

CString CalcMd5(const CString& strFileName)
{
    std::string strMd5;
    CFile file;
    if (file.Open(strFileName, CFile::modeRead))
    {
        DWORD dwLen = (DWORD)file.GetLength();
        char *szBuf = new char[dwLen];
        if (szBuf)
        {
            file.Read(szBuf, dwLen);

            strMd5 = CMd5::GetMd5(std::string(szBuf, dwLen));
        }

        file.Close();
    }
    return strMd5.c_str();
}

void SearchFile(const CString& strBasePath, const CString& strPath, MapFileInfo& mapInfo)
{
    CFileFind ff;
    BOOL bRet = ff.FindFile(strPath + "\\*.*");
    if (FALSE == bRet)
    {
        return;
    }

    CTime tmFile;
    CString strPathTmp, strFileName;
    while (bRet)
    {
        bRet = ff.FindNextFile();

        if (ff.IsDots())
        {
            continue;
        }
        else if (ff.IsDirectory())
        {
            SearchFile(strBasePath, ff.GetFilePath(), mapInfo);
        }
        else //文件
        {
            strFileName = ff.GetFileName();
            if (strFileName == g_AppName)
            {
                continue;
            }

            ff.GetLastWriteTime(tmFile);
            CString strTmp = ff.GetFilePath();
            strTmp.Replace(strBasePath, "");
            strTmp.Replace("\\", "/");
            mapInfo[strTmp] = CalcMd5(strTmp);
        }
    }
}

void Print(const MapFileInfo& mapInfo)
{
    //test
//     for (MapFileInfo::iterator it = mapInfo.begin(); it != mapInfo.end(); ++it)
//     {
//         cout << it->first << " " << it->second << endl;
//     }
    CFile file;
    if (!file.Open(OUT_FILE, CFile::modeWrite | CFile::modeCreate))
    {
        return;
    }

    CString strLine;
    const int nSize = mapInfo.size();
    int nCount = 0;

    strLine = "{\n"
        "\t\"packageUrl\": \"http://192.168.3.49:3003/update\",\n"
        "\t\"remoteManifestUrl\": \"http://192.168.3.49:3003/update/project.manifest\",\n"
        "\t\"remoteVersionUrl\": \"http://192.168.3.49:3003/update/version.manifest\",\n"
        "\t\"version\": \"1.0.11273\",\n"
        "\t\"engineVersion\": \"3.2\",\n";
    file.Write(strLine, strLine.GetLength());

    strLine = "\t\"assets\" : {\n";
    file.Write(strLine, strLine.GetLength());
    for (MapFileInfo::const_iterator it = mapInfo.begin(); it != mapInfo.end(); ++it)
    {
        ++nCount;
        if (nCount == nSize)
        {
            strLine.Format("\t\t\"%s\" : {\n"
                "\t\t\t\"md5\" : \"%s\"\n"
                "\t\t}\n",
                it->first, it->second);
            //strLine.Format("\"%s\" : { \"md5\" : \"%d\" }\n", it->first, it->second);
        }
        else
        {
            strLine.Format("\t\t\"%s\" : {\n"
                "\t\t\t\"md5\" : \"%s\"\n"
                "\t\t},\n",
                it->first, it->second);
            //strLine.Format("\"%s\" : { \"md5\" : \"%d\" },\n", it->first, it->second);
        }
        file.Write(strLine, strLine.GetLength());
    }

    strLine = "\t},\n"
        "\t\"searchPaths\": [\n"
        "\t\t\"script\",\n"
        "\t\t\"src\",\n"
        "\t\t\"res\",\n"
        "\t\t\"res/map\",\n"
        "\t\t\"res/scene\",\n"
        "\t\t\"res/scene/gameMain\",\n"
        "\t\t\"res/scene/battleResult\",\n"
        "\t\t\"res/scene/battleBg/train\",\n"
        "\t\t\"res/scene/gameStart\",\n"
        "\t\t\"res/scene/publish\"\n"
        "\t]\n";
    file.Write(strLine, strLine.GetLength());

    strLine = "}\n";
    file.Write(strLine, strLine.GetLength());

    file.Close();
}

// void CreateIni()
// {
// 
//     if (PathFileExists("FileInfo.ini") == TRUE) //已经存在
//     {
// 
//         char szIp[32];
//         GetPrivateProfileString(NULL, "ip", "http://192.168.3.49:3003", szIp, sizeof(szIp), "FileInfo.ini");
//     }
//     else
//     {
//         WritePrivateProfileString(NULL, "ip", "http://192.168.3.49:3003", "FileInfo.ini");
// //         CFile file;
// //         if (file.Open("FileInfo.ini", CFile::modeCreate | CFile::modeWrite))
// //         {
// //             file.Write()
// //             file.Close();
// //         }
//     }
// }

int _tmain(int argc, _TCHAR* argv[])
{
    char szAppName[MAX_PATH];
    GetModuleFileName(NULL, szAppName, sizeof(szAppName));

    CString strPath(szAppName);
    int nPos = strPath.ReverseFind('\\');
    if (-1 == nPos)
    {
        return 0;
    }
    g_AppName = strPath.Right(strPath.GetLength() - nPos - 1);
    //cout << g_AppName << endl;

    CString strBasePath = strPath.Left(nPos + 1);
    //cout << strBasePath << endl;

    DeleteFile(strBasePath + "\\" + CString(OUT_FILE));

    //CreateIni();

    MapFileInfo mapInfo;
    SearchFile(strBasePath, strBasePath, mapInfo);
    Print(mapInfo);

    return 0;
}


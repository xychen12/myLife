#include <Windows.h>
#include <string>
#include <fstream>

#include "LogUtil.h"

using namespace std;

//wstring=>string
std::string WString2String(const std::wstring& ws)
{
    std::string strLocale = setlocale(LC_ALL, "");
    const wchar_t* wchSrc = ws.c_str();
    size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
    char *chDest = new char[nDestSize];
    memset(chDest, 0, nDestSize);
    wcstombs(chDest, wchSrc, nDestSize);
    std::string strResult = chDest;
    delete[]chDest;
    setlocale(LC_ALL, strLocale.c_str());
    return strResult;
}

// string => wstring
std::wstring String2WString(const std::string& s)
{
    std::string strLocale = setlocale(LC_ALL, "");
    const char* chSrc = s.c_str();
    size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
    wchar_t* wchDest = new wchar_t[nDestSize];
    wmemset(wchDest, 0, nDestSize);
    mbstowcs(wchDest, chSrc, nDestSize);
    std::wstring wstrResult = wchDest;
    delete[]wchDest;
    setlocale(LC_ALL, strLocale.c_str());
    return wstrResult;
}

wstring GetCurDir()
{
    wstring wstrCurDir;
    wchar_t *path = new wchar_t[MAX_PATH+1];
    if (GetModuleFileName(nullptr, path, MAX_PATH) != 0)
    {
        *(wcsrchr(path, L'\\') + 1) = 0;  // 去掉xxx.exe文件名
        wstrCurDir = path;
    }
    delete[] path;

    //string strCurDir = WString2String(wstrCurDir);
    return wstrCurDir;
}

bool CheckDirExist(const wchar_t* dir)
{
    WIN32_FIND_DATA wfd;
    HANDLE hFind = FindFirstFile(dir, &wfd);
    if (INVALID_HANDLE_VALUE == hFind && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        return false;
    CloseHandle(hFind);

    return true;
}

void LogUtil::FatalMessageDump(const char* data, int size)
{
    wstring wstrCurDir = GetCurDir();
    wstring wstrLogDir = wstrCurDir + L"\\log\\";

    fstream file;
    wstring crashPath = wstrLogDir + L"crash.log";
    file.open(crashPath, ios::out | ios::app);

    string errInfo = string(data, size);
    file << errInfo;
    file.close();
}

LogUtil::LogUtil(const char* appName)
{
    wstring wstrCurDir = GetCurDir();    
    wstring wstrLogDir = wstrCurDir + L"\\log\\";
    if (!CheckDirExist(wstrLogDir.c_str()))
    {
        CreateDirectory(wstrLogDir.c_str(), nullptr);
    }
    wstring basePath = wstrLogDir + L"LOG_";

    google::InitGoogleLogging(appName);
    google::SetLogDestination(google::GLOG_INFO, WString2String(basePath).c_str());

    // 设置dumphandler
    google::InstallFailureSignalHandler();
    google::InstallFailureWriter(&FatalMessageDump);
    LOG(INFO) << "---Application Start---";
}

LogUtil::~LogUtil()
{
    LOG(INFO) << "---Application End---";
    google::ShutdownGoogleLogging();
}

/*****************************************************
**Copyright(C), 2019-2025
**File:  LogUtil.h
**Version: v1.0
**Author: Bruce Lee          
**Date:  2019/09/19
**Description:  glog�ļ򵥰�װ����Ҫ����Ϊ��ʼ���͹ر���־��Ԫ
*****************************************************/
#pragma once
#include "glog/logging.h"

#define LogInfo    LOG(INFO)
#define LogError   LOG(ERROR)
#define LogWarn    LOG(WARNING)
#define LogFatal   LOG(FATAL)

class LogUtil
{
public:
    LogUtil(const char* appName);
    ~LogUtil();

private:
    static void FatalMessageDump(const char* data, int size);

};


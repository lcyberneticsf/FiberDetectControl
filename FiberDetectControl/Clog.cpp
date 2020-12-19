//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Clog.h"
#include <time.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLog::CLog()
{
    m_nLevel = LOG_LEVEL_ERROR;
    ::InitializeCriticalSection(&m_crit);   //��ʼ���ٽ���
}

CLog::~CLog()
{
    ::DeleteCriticalSection(&m_crit);    //�ͷ����ٽ���
}

int CLog::GetCurrentPath(char *chPath)
{
    int ch = '\\';
    GetModuleFileNameA(NULL, chPath, MAX_PATH);

    char *start = (char *)chPath;

    while (*chPath++)                       /* find end of string */
        ;
    /* search towards front */
    while (--chPath != start && *chPath != (char)ch)
        ;

    if (*chPath == (char)ch)                /* char found ? */
        *chPath = '\0';

    chPath = (char *)start;

    return 0;
}

void CLog::Add(const char *fmt, ...)
{
    char chPath[512], chFile[512];
    GetCurrentPath(chPath);

    strcat_s(chPath, "\\Log");
    _mkdir(chPath);

    struct tm *now;
    time_t ltime;

    time(&ltime);
    now = localtime(&ltime);

    char szDate[20], szTime[20];
    _strdate_s(szDate, 20);
    _strtime_s(szTime, 20);

    sprintf_s(chFile, 512, "%s\\Log_%d_%d_%d.txt", chPath
              , now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);

    /*-----------------------�����ٽ���(д�ļ�)------------------------------*/
    ::EnterCriticalSection(&m_crit);
    //try
    //{
    //    va_list argptr;          //�����ַ����ĸ�ʽ
    //    va_start(argptr, fmt);
    //    _vsnprintf_s(m_tBuf, BUFSIZE, fmt, argptr);
    //    va_end(argptr);
    //}
    //catch (...)
    //{
    //    m_tBuf[0] = 0;
    //}

    FILE *fp;
    errno_t err = fopen_s(&fp, chFile, "a"); //����ӵķ�ʽ������ļ�
    if (fp)
    {
        fprintf(fp, "[%s %s]\t", szDate, szTime);
		fprintf(fp, "%s\n", fmt);
        fclose(fp);
    }
    ::LeaveCriticalSection(&m_crit);
    /*----------------------------�˳��ٽ���---------------------------------*/

}


void CLog::Add_L(const char *fmt, int nBufferLength)
{
	char chPath[512], chFile[512];
	GetCurrentPath(chPath);

	strcat_s(chPath, "\\Log");
	_mkdir(chPath);

	struct tm *now;
	time_t ltime;

	time(&ltime);
	now = localtime(&ltime);

	char szDate[20], szTime[20];
	_strdate_s(szDate, 20);
	_strtime_s(szTime, 20);

	sprintf_s(chFile, 512, "%s\\Log_%d_%d_%d.txt", chPath
		, now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);

	/*-----------------------�����ٽ���(д�ļ�)------------------------------*/
	::EnterCriticalSection(&m_crit);
	//try
	//{
	//    va_list argptr;          //�����ַ����ĸ�ʽ
	//    va_start(argptr, fmt);
	//    _vsnprintf_s(m_tBuf, BUFSIZE, fmt, argptr);
	//    va_end(argptr);
	//}
	//catch (...)
	//{
	//    m_tBuf[0] = 0;
	//}

	FILE *fp;
	errno_t err = fopen_s(&fp, chFile, "a"); //����ӵķ�ʽ������ļ�
	if (fp)
	{
		char* buffer = new char[nBufferLength];
		memset(buffer, 0, nBufferLength);
		memcpy(buffer, fmt, nBufferLength);
		fprintf(fp, "[%s %s]\t", szDate, szTime);
		fprintf(fp, "%s\n", buffer);
		fclose(fp);
		delete[] buffer;
	}
	::LeaveCriticalSection(&m_crit);
	/*----------------------------�˳��ٽ���---------------------------------*/

}

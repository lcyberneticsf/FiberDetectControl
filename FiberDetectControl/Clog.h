//
//////////////////////////////////////////////////////////////////////

#if !defined(__TEST_LOG__INCLUDED_)
#define __TEST_LOG__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <direct.h>
#include <time.h>
#include <stdio.h>

/************************************************************************/
/* �����¼������־����־��*/
/************************************************************************/

#define LOG_LEVEL_ALL			0
#define LOG_LEVEL_IMPORT		1
#define LOG_LEVEL_ERROR			2
#define LOG_LEVEL_NONE			3

class CLog
{
public:
    CLog();
    virtual ~CLog();

    //��Ϊ0��1��2��3�ĸ�����Ĭ��Ϊ����2
    //0--������ͣ���¼������־
    //1--����һ�㣬��¼������־����Ҫ��Ϣ
    //2--����ϸߣ�ֻ��¼�����쳣��־
    //3--������ߣ�����¼��־
    void SetLogLevel(int nLevel)
    {
        m_nLevel = nLevel;
    };
    BOOL GetWriteLog()
    {
        return m_nLevel;
    };

    int GetCurrentPath(char *chPath);
    void Add(const char *fmt, ...);
	void Add_L(const char *fmt, int nBufferLength);

private:
    enum {BUFSIZE = 20000};  //����������
    char	m_tBuf[BUFSIZE];

    int 	m_nLevel;			//�Ƿ��¼��־
    CRITICAL_SECTION  m_crit;  	//����һ���ٽ���
};

#endif // !defined(__TEST_LOG__INCLUDED_)

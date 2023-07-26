#include <afxwin.h>
#include <iostream>
#include <vector>
#include <string>
#pragma comment(lib, "version.lib")

/*
�� ���ֽ� �ַ����л᷵�������汾��
�� Unicode �ַ����з���V
*/

void GetApplicationVersion(char* cVersion)
{
#if UNICODE

#else
	TCHAR szFullPath[MAX_PATH];
	DWORD dwVerInfoSize = 0;
	DWORD dwVerHnd;
	VS_FIXEDFILEINFO* pFileInfo;

	GetModuleFileName(AfxGetInstanceHandle(), szFullPath, sizeof(szFullPath));
	dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
	if (dwVerInfoSize)
	{
		HANDLE  hMem;
		LPVOID  lpvMem;
		unsigned int uInfoSize = 0;

		hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpvMem = GlobalLock(hMem);
		GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);
		::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (void**)&pFileInfo, &uInfoSize);

		WORD nProdVersion[4];

		nProdVersion[0] = HIWORD(pFileInfo->dwProductVersionMS);
		nProdVersion[1] = LOWORD(pFileInfo->dwProductVersionMS);
		nProdVersion[2] = HIWORD(pFileInfo->dwProductVersionLS);
		nProdVersion[3] = LOWORD(pFileInfo->dwProductVersionLS);
		CString strAppVersion;
		strAppVersion.Format(_T("Ver : %d.%d.%d.%d"), nProdVersion[0],
			nProdVersion[1], nProdVersion[2], nProdVersion[3]);
		memcpy(cVersion, strAppVersion.GetBuffer(0), strAppVersion.GetLength() + 1);
		GlobalUnlock(hMem);
		GlobalFree(hMem);
	}
#endif // UNICODE
}

//��ȡ��������
std::string CheckProcessName()
{
	char szFileFullPath[MAX_PATH], szProcessName[MAX_PATH];
	memset(szFileFullPath, 0, MAX_PATH);
	memset(szProcessName, 0, MAX_PATH);
	::GetModuleFileNameA(NULL, szFileFullPath, MAX_PATH);//��ȡ�ļ�·��
	int length = ::strlen(szFileFullPath);
	for (int i = length - 1; i >= 0; i--)
	{
		if (szFileFullPath[i] == '\\')
		{
			i++;
			for (int j = 0; i <= length; j++)
			{
				szProcessName[j] = szFileFullPath[i++];
			}
			break;
		}
	}
	return szProcessName;
}

//��ȡ����·��
std::string GetProgramPath() {
	char buffer[MAX_PATH] = { 0 };
	// GetModuleFileName �ĵ�һ������Ϊ NULL����ʾ��ȡ��ǰ���̵�ģ���ļ���
	if (GetModuleFileNameA(NULL, buffer, MAX_PATH) == 0) {
		std::cout << "�޷���ȡ����·��" << std::endl;
		return "";
	}
	return buffer;
}
/*
std::wstring GetProgramPath() {
	wchar_t buffer[MAX_PATH] = {0};
	if (GetModuleFileNameW(NULL, buffer, MAX_PATH) == 0) {
		std::wcout << L"�޷���ȡ����·��" << std::endl;
		return L"";
	}
	return buffer;
}
*/

//�������Ƿ�Ψһ
bool CheckOnly(const char* processName)
{
	HANDLE m_hMutex = CreateMutex(NULL, TRUE, processName);
	DWORD dwRet = GetLastError();
	if (m_hMutex)
	{
		if (ERROR_ALREADY_EXISTS == dwRet)
		{
			CloseHandle(m_hMutex);
			return true;
		}
	}
	else
	{
		CloseHandle(m_hMutex);
		return true;
	}
	return false;
}

void main()
{
	char Version[MAX_PATH] = "";
	memset(Version, 0, MAX_PATH);
	GetApplicationVersion(Version);
	std::cout << Version << std::endl;

	std::string Name = CheckProcessName();
	std::cout << Name << std::endl;

	std::string Path = GetProgramPath();
	Path = Path.substr(0, Path.length() - Name.length());
	std::cout << Path << std::endl;
	
	if (CheckOnly(Name.c_str()))
		std::cout << "�Ѵ��ڽ���" << std::endl;
}
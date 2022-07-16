#pragma comment (lib, "Wininet")

#include "ftp.h"

#include <Winerror.h>

#include <iostream>
#include <vector>

#include "utilities.h"

FTP::FTP(const std::string& host, int port, const std::string& username, const std::string& password)
  : m_host(host)
  , m_port(port)
  , m_username(username)
  , m_password(password)
  , m_hint_root(nullptr)
  , m_hint_ftp(nullptr)
{}

bool FTP::establish_connection()
{
  std::cout << "\n- Establishing an FTP Connection...\n\n";

  m_hint_root = InternetOpen(TEXT("ftpput/1.0"),
                             INTERNET_OPEN_TYPE_DIRECT,
                             nullptr,
                             nullptr, 
                             0);
  if (!m_hint_root) {
    std::cerr << "ERRORO: Failed InternetOpen: errcode " << GetLastError() << std::endl;
    return false;
  }
  std::cout << "=== Initialize use of the WinINet functions: PASSED\n\n";

  m_hint_ftp = InternetConnectA(m_hint_root,
                                m_host.c_str(),
                                m_port,
                                m_username.c_str(),
                                m_password.c_str(),
                                INTERNET_SERVICE_FTP,
                                INTERNET_FLAG_PASSIVE,
                                0);
  if (!m_hint_ftp) {
    std::cout << "ERROR: Failed to connect to FTP server for host " << m_host << ": errcode " << GetLastError() << std::endl;
    InternetCloseHandle(m_hint_root);
    return false;
  }
  std::cout << "=== Open an FTP session for site " << m_host << ": PASSED\n\n";

  return true;
}

bool FTP::process_file(const std::string& local_file, const std::string& remote_path)
{
   if (!FtpSetCurrentDirectoryA(m_hint_ftp, "/")) {
     std::cerr << "ERROR: Failed setting \"/\" folder as a current directory on server:  errcode " << GetLastError() << std::endl;
     return false;
   }

  if (!FtpSetCurrentDirectoryA(m_hint_ftp, remote_path.c_str())) {
    std::cout << "- user defined folder does not exist, creating ... \n" << std::endl;
    if (!FtpCreateDirectoryA(m_hint_ftp, remote_path.c_str())) {
      std::cerr << "ERROR: Failed to create directory on server: errcode "
                << GetLastError() << std::endl;
      return false;
    }
    if (!FtpSetCurrentDirectoryA(m_hint_ftp, remote_path.c_str())) {
      std::cerr << "ERROR: Failed to set created " << remote_path << " dir as current directory on server: errcode "
                << GetLastError() << std::endl;
      return false;
    }
  }

  std::cout << "- set user defined " << remote_path << " folder as current directory\n\n";
  std::cout << "- uploading " << local_file << " local file to " << remote_path << " remote folder on server\n";
  std::cout << "> ftpput " << local_file << " " << remote_path + "/" + local_file << " \n\n";

  if (!FtpPutFileA(m_hint_ftp,
                   local_file.c_str(), 
                   local_file.c_str(),
                   FTP_TRANSFER_TYPE_BINARY,
                   0)) {
    std::cerr << "ERROR: Failed to fransfer file to the server: errcode " << GetLastError() << std::endl;
    return false;
  }
  std::cout << "=== FtpPutFile: PASSED\n\n";

  return true;
}

void FTP::close_connection()
{
  if (!InternetCloseHandle(m_hint_ftp)) {
    std::cerr << "ERROR: Failed to close FTP session: errcode " << GetLastError() << std::endl;
    return;
  }
  std::cout << "=== Close FTP session: PASSED\n\n";

  if (!InternetCloseHandle(m_hint_root)) {
    std::cerr << "ERROR: Failed to close use of the WinINet functions: errcode " << GetLastError() << std::endl;
    return;
  }
  std::cout << "=== Close the WinINet functions use: PASSED\n\n";
}

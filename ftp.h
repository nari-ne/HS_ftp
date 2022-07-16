#pragma once
#include <windows.h>
#include <string>

//FTP server class
class FTP
{
public:
  FTP(const std::string& h, int p, const std::string& u, const std::string& pw);

  // Send file to a FTP server: transfer the dump file to the ftp server’s given folder
  bool process_file(const std::string& local_file, const std::string& remote_destination);
  void FileSubmit(const std::string& local_file, const std::string& remote_destination);
  bool directory_exists(const std::string& directory_path) const;

private:
  bool check() const;

  /*
  //bool check_connectivity() const;
  bool check_connectivity(SOCKET& FSoc);

  void sendLogIn(SOCKET FSoc);
  // Send typea command to transfer text data.
  // It works better for windows to unix text files transfer, bacause it fixes the line endnings CR, LF on most ftp servers
  // Use typei command for binary data.
  void sendTypeACmd(SOCKET FSoc) const;
  //void sendTypeICmd() const;
  int sendPasvCmd(SOCKET FSoc) const;
  // Generate a unique filename that does not exists on the FTP remote directory
  std::string generate_unique_filename(SOCKET _FSoc, int port, const std::string& local_file, bool useLocalFileExtension, const std::string& remout_path);
  void sendFileRequest(SOCKET FSoc, const std::string&, const std::string&);
  bool ftpSocket(const std::string& local_file, int port);
  int readSocketData(int, LPSTR, int);
  void sendQuitCmd(SOCKET FSoc) const;
  */
private:
  const std::string m_host;
  const int m_port;
  const std::string m_user_name;
  const std::string m_password;
  //SOCKET m_FSoc;
  const std::string m_path;
};

// conversion from std::string to LPCWSRT
// typedef _Null_terminated_ CONST WCHAR *LPCWSTR, *PCWSTR;
LPCWSTR to_LPCWSTR(const std::string& str);

//void echoResponse(std::string&);
//void echoResponse(LPSTR);
//void recvResponseAndEcho(SOCKET, LPSTR, int);
//void recvResponseAndEcho(SOCKET);

#pragma once

#include <windows.h> // do not change the order
#include <wininet.h>

#include <string>

class FTP
{
public:
  FTP(const std::string& host, int port, const std::string& username, const std::string& password);

  bool establish_connection();
  bool process_file(const std::string& local_file, const std::string& remote_destination);
  void close_connection();

private:
  const std::string m_host;
  const int m_port;
  const std::string m_username;
  const std::string m_password;
  const std::string m_server_path;

  HINTERNET m_hint_root;
  HINTERNET m_hint_ftp;
};

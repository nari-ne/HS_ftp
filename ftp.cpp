// File ftp.cpp
#pragma comment (lib, "Wininet")

#define _WINSOCKAPI_ 
#include <winsock2.h>
#include <wininet.h>
#include <Winerror.h>
#include <shellapi.h>
#include <windows.h>
#include <Ws2tcpip.h>

#include <sstream>
#include <iostream>

#include <vector>

#include "utilities.h"
#include "ftp.h"

FTP::FTP(const std::string& h, int p, const std::string& u, const std::string& pw)
  : m_host(h)
  , m_port(p)
  , m_user_name(u)
  , m_password(pw)
  //, m_FSoc(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
{}

// Send file to a FTP server
/*
 * Program uploads a file to an FTP site in binary mode with the assistance of the Wininet library.
 * The program accepts five command line arguments:
 * 
 * -site (no “ftp://” in front)
 * -userid
 * -password
 * -path for the file to upload
 * -location to place the uploaded file
 * For example, 
 * ftpput ftp.contoso.com admin seinfeld newversion.zip subdir/newversion.zip
 */
bool FTP::process_file(const std::string& local_file, const std::string& remote_path)
{
  if (!check()) {
    std::cout << "Error: !!! !check() " << std::endl;
    return false;
  }
  //
  //Establishing an FTP Connection
  std::cout << "- Establishing an FTP Connection...\n\n";

  HINTERNET hintRoot = InternetOpen(TEXT("ftpput/1.0"),
                                    INTERNET_OPEN_TYPE_DIRECT,
                                    nullptr, nullptr, 0);
 
  if (!hintRoot) {
    std::cerr << "ERRORO: Failed InternetOpen: errcode " << GetLastError() << std::endl;
    return false;
  }
  std::cout << "=== Initialize using of the WinINet functions: PASSED\n";


  HINTERNET hintFtp = InternetConnectA(hintRoot,
                                      m_host.c_str(),
                                      m_port,          //INTERNET_DEFAULT_FTP_PORT is 21
                                      m_user_name.c_str(),
                                      m_password.c_str(),
                                      INTERNET_SERVICE_FTP,
                                      INTERNET_FLAG_PASSIVE,
                                      NULL);
  if (hintFtp == nullptr) {
    std::cout << "ERROR: Failed connecting FTP for " << m_host <<" host: errcode " << GetLastError() << std::endl;
    InternetCloseHandle(hintRoot);
    return false;
  }
  std::cout << "=== Open an FTP session for " << m_host <<  " site: PASSED\n\n";

  
  //FtpWebRequest  request = (FtpWebRequest)WebRequest.Create("ftp://ftp.microsoft.com/12345");
  // set root as current
  //if (!FtpSetCurrentDirectoryA(hintFtp, "/nm_FTP")) { //TODO: !!!!!!!!
   if (!FtpSetCurrentDirectoryA(hintFtp, "/")) {
     std::cerr << "ERROR: Failed setting \"/\" directory as a current directory on server:  errcode " << GetLastError() << std::endl;
     return false;
   }

  if (!FtpSetCurrentDirectoryA(hintFtp, remote_path.c_str())) {
    std::cout << "- user defined folder does not exist, creating now.... \n" << std::endl;
    if (!FtpCreateDirectoryA(hintFtp, remote_path.c_str())) {
      std::cerr << "ERROR: Failed creating directory on server: errcode " << GetLastError() << std::endl;
      return false;


    }
    if (!FtpSetCurrentDirectoryA(hintFtp, remote_path.c_str())) {
      std::cerr << "ERROR: Failed setting new created " << remote_path << " dir as current directory on FTP server: errcode " << GetLastError() << std::endl;
      return false;
    }
  }

  std::cout << "- set user defined " << remote_path << " folder as current directory\n\n";
  std::cout << "- Uploading " << local_file << " local file to " << remote_path << " remote folder on FTP server\n";

  //if (!FtpPutFile(hintFtp, to_LPCWSTR(local_file), to_LPCWSTR(remote_path),
  std::cout << "> ftpput " << local_file << " " << remote_path + "/" + local_file << " \n\n";

  if (!FtpPutFileA(hintFtp,
      //"D:\\NM\\Interview\\HelpSystems\\Dev\\HS_db_backup\\HS_db_backup\\d2.db",
      local_file.c_str(), 
      local_file.c_str(), //"d2.db", 
      FTP_TRANSFER_TYPE_BINARY, NULL)) {
    std::cerr << "ERROR: Failed FtpPutFile: errcode " << GetLastError() << std::endl;
    return false;
  }

  /*
  if (!FtpPutFile(hintFtp, 
                  LPCWSTR(L"D:\\NM\\Interview\\HelpSystems\\Dev\\HS_db_backup\\HS_db_backup\\dmp1410.db"),
                  LPCWSTR(L"dmp14100.db"), FTP_TRANSFER_TYPE_BINARY, NULL)) {
    std::cerr << "ERROR: FtpPutFile():  errcode " << GetLastError() << std::endl;
    return false;
  }
  */
  std::cout << "=== FtpPutFile: PASSED\n\n";

  if (!InternetCloseHandle(hintFtp)) {
    std::cerr << "ERROR: Failed close FTP session: errcode " << GetLastError() << std::endl;    return 0;
  }
  std::cout << "=== Close FTP session: PASSED\n\n";

  if (!InternetCloseHandle(hintRoot)) {
    std::cerr << "ERROR: Failed closing using of the WinINet functions: errcode " << GetLastError() << std::endl;
    return 0;
  }
  std::cout << "=== Close session using of the WinINet functions: PASSED\n\n";
  return true;
}

/*
  Pseudo code

  - Initialize ftp server command connection
  - Send ftp login information (username, password)
  - Generate unique filename
  - Read the remote directory filenames
  - Generate random names until we don’t have the name to the files list
  - Send the file with the unique filename to the server
*/
/*
bool FTP::process_file(const std::string& local_file, const std::string& remote_path)
{
  FreeConsole();
 
  SOCKET FSoc;
  if (!check_connectivity(FSoc)) {
    return false;
  }
  recvResponseAndEcho(FSoc);

  sendLogIn(FSoc);
  Sleep(1000); // give the server and the client sometime to deal with the influx of new messages
               //so that data for the ip doesnt get mixed up.

  sendTypeACmd(FSoc); // Send typea command to transfer text data.
  
  int port = sendPasvCmd(FSoc); // Open passing mode for the files list data

    // Generate a unique filename that does not exists on the FTP remote directory
  std::string filename = generate_unique_filename(FSoc, port, remote_path, true, local_file);
  port = sendPasvCmd(FSoc); // Open passing mode for the files transfer data

  sendFileRequest(FSoc, remote_path, filename);

  ftpSocket(local_file, port);
  recvResponseAndEcho(FSoc);

  sendQuitCmd(FSoc);
  shutdown(FSoc, SD_BOTH);
  closesocket(FSoc);
  WSACleanup();
  
  return true;
}
*/
bool FTP::check() const
{
  //TODO:
  return true;
}

/*
bool FTP::check_connectivity(SOCKET& FSoc)
{
  WSAData WData;
  WSAStartup(MAKEWORD(2, 2), &WData);

  //SOCKET FSoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  FSoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (FSoc == INVALID_SOCKET) {
    WSACleanup();
    return false;
  }

  SOCKADDR_IN server;
  server.sin_family = AF_INET;
  server.sin_port = htons(m_port);
  //server.sin_addr.s_addr = inet_addr(m_host.c_str()); //this is the drivehq ftp server address. ***deprecated                                              
  if (inet_pton(AF_INET, m_host.c_str(), &server.sin_addr.s_addr) != 1) {  //converts an IPv4 or IPv6 Internet network address in its standard text presentation form into its numeric binary form
    std::cout << "Error: !!!  pAddrBuf parameter points to a string that is not a valid IPv4 dotted-decimal string or a valid IPv6 address string." << std::endl;
    // a specific error code can be retrieved by calling the WSAGetLastError for extended error information.
    return false;
  }

  int trycount = 2;
  int connectionerror = connect(FSoc, (LPSOCKADDR)&server, sizeof(struct sockaddr));
  while (connectionerror == SOCKET_ERROR) {
    connectionerror = connect(FSoc, (LPSOCKADDR)&server, sizeof(struct sockaddr));
    trycount++;
    if (trycount = 10) {
      closesocket(FSoc);
      WSACleanup();
      return false;
    }
  }
  return true;
}

void FTP::sendLogIn(SOCKET FSoc)
{
  std::string userCmd("USER ");
  std::string passCmd("PASS ");

  userCmd += m_user_name + "\r\n";
  passCmd += m_password + "\r\n";

  send(FSoc, userCmd.c_str(), userCmd.size(), 0);
  recvResponseAndEcho(FSoc);
  send(FSoc, passCmd.c_str(), passCmd.size(), 0); 
  recvResponseAndEcho(FSoc);
}

void FTP::sendTypeACmd(SOCKET FSoc) const
{
  std::string typeCmd("TYPE A\r\n");
  send(FSoc, typeCmd.c_str(), typeCmd.size(), 0);
  Sleep(1000);
  recvResponseAndEcho(FSoc);
}

int FTP::sendPasvCmd(SOCKET FSoc) const
{
  int result = -1;
  
  std::string pasvCmd("PASV\r\n");
  send(FSoc, pasvCmd.c_str(), pasvCmd.size(), 0);
  Sleep(1000);
  char srvResponse[4096];
  recvResponseAndEcho(FSoc, (LPSTR)&srvResponse, sizeof(srvResponse));

  // Read the pasv response, something like:
  // 227 Entering Passive Mode (x,x,x,x,y,y)
  // where x are the ip bytes and y are the port bytes
  std::string pasvMessage = srvResponse;
  int modeStart = pasvMessage.find("Mode");

  if (modeStart > 0) {
    int parenthesisStart = pasvMessage.find("(", modeStart);
    if (parenthesisStart > 0) {
      std::string currentPasvByte;
      int currentCharPos = parenthesisStart,
        currentPasvByteIndex = 0,
        highByte = -1,
        lowByte = -1;
      
      char currentChar;
      do {
        currentChar = pasvMessage[++currentCharPos];
        if (currentChar == ',' || currentChar == ')') {
          if (currentPasvByteIndex == 4) {
            highByte = stoi(currentPasvByte);
          } else if (currentPasvByteIndex == 5) {
            lowByte = stoi(currentPasvByte);
          }
          currentPasvByteIndex++;
          currentPasvByte = "";
        } else {
          currentPasvByte += currentChar;
        }
      } while (currentChar != ')');

      // Assemble the port number by joing hi and lo port number bytes
      if (highByte != -1 && lowByte != -1) {
        result = (highByte << 8) | lowByte;
      }
    }
  }
  return result;
}

std::string FTP::generate_unique_filename(SOCKET FSoc, int port, const std::string& remote_path,
                                          bool useLocalFileExtension, const std::string& local_file) 
{
  std::string newFilename;
  
  //char cmd[7] = "NLST\r\n";
  std::string cmd("NLST\r\n");
  char listFilesBuffer[4096];
  char servermessage[2048];
  
  std::string cwd("CWD ");
  cwd += remote_path + "\r\n";
  send(FSoc, cwd.c_str(), cwd.size(), 0);
  Sleep(1000);

  recvResponseAndEcho(FSoc, (LPSTR)&servermessage, sizeof(servermessage));
  Sleep(1000);

  send(FSoc, cmd.c_str(), cmd.size(), 0);
  int listFilesBufferReadLength = readSocketData(port, listFilesBuffer, 4096);

  if (listFilesBufferReadLength > 0) {
    std::istringstream fileLines(listFilesBuffer);
    std::string fileLine, localFileExtension;
    std::vector<std::string> filesList;

    while (std::getline(fileLines, fileLine)) {
      // Trim whitespaces from each line
      fileLine = fileLine.erase(fileLine.find_last_not_of(" \t\n\r") + 1);
      if (fileLine != "." && fileLine != "..")
        // Save to a vector or strings if it's not current and level up directory
        filesList.push_back(fileLine);
    }

    if (useLocalFileExtension) {
      int dotPos = local_file.find_last_of(".");
      localFileExtension = dotPos > 0 ? local_file.substr(dotPos) : "";
    }

    std::vector<std::string>::iterator fileFind;
    do {
      // Generate random filenames of 14 characters length untill the filename does not exist on the remote FTP server path
      newFilename = generate_random_filename(14);
      std::cout << "\tnew file name generated: " << newFilename << std::endl;
      if (useLocalFileExtension)
        newFilename += localFileExtension;
    } while ((fileFind = std::find(filesList.begin(), filesList.end(), newFilename)) != filesList.end());
  }
  return newFilename;
}

void FTP::sendFileRequest(SOCKET FSoc, const std::string& remote_dir, const std::string& stor_filename)
{
  std::string cwdCmd("CWD ");
  cwdCmd += remote_dir + "\r\n";
  // Change current working directory
  send(FSoc, cwdCmd.c_str(), cwdCmd.size(), 0);
  Sleep(1000);
  recvResponseAndEcho(FSoc);

  std::string storCmd("STOR ");
  storCmd += stor_filename + "\r\n";
  // Send stor ftp command
  send(FSoc, storCmd.c_str(), storCmd.size(), 0);

  //std::string stouCmd("STOR\r\n");
  // Send stou ftp command insted of stor
  //send(FSoc, stouCmd, stouCmd.size(), 0);
}

bool FTP::ftpSocket(const std::string& local_file, int port) 
{
  bool result = false;
  SOCKET sock;
  int connectionerror2;
  int trycount2 = 2;

  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == INVALID_SOCKET) {
    WSACleanup();
    return 0;
  }

  SOCKADDR_IN pasvserver;
  pasvserver.sin_family = AF_INET;
  pasvserver.sin_port = htons(port);
  //pasvserver.sin_addr.s_addr = inet_addr(m_host.c_str()); //Once again the drivehq ftp server
  if (inet_pton(AF_INET, m_host.c_str(), &pasvserver.sin_addr.s_addr) != 1) {  //Once again the drivehq ftp server
    std::cerr << "ERROR!!!...FTP::ftpSocket(const std::string& local_file, int port) " << std::endl;
    return false;
  }

  connectionerror2 = connect(sock, (LPSOCKADDR)&pasvserver, sizeof(struct sockaddr));
  while (connectionerror2 == SOCKET_ERROR) {
    connectionerror2 = connect(sock, (LPSOCKADDR)&pasvserver, sizeof(struct sockaddr));
    trycount2++;
    if (trycount2 = 10) {
      closesocket(sock);
      WSACleanup();
      return false;
    }
  }

  HANDLE hFile = CreateFile((const TCHAR*)local_file.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (hFile != INVALID_HANDLE_VALUE) {
    DWORD read;
    char buffer[4096];
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

    while (ReadFile(hFile, buffer, 4096, &read, NULL) && read > 0) {
      send(sock, buffer, read, 0);
    }

    CloseHandle(hFile);
    result = true;
  }

  shutdown(sock, SD_BOTH);
  closesocket(sock);
  return result;
}

int FTP::readSocketData(int port, LPSTR buffer, int buffer_size) 
{
  SOCKET sock;
  SOCKADDR_IN pasvserver;
  int trycount2 = 2;

  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == INVALID_SOCKET) {
    WSACleanup();
    return 0;
  }

  pasvserver.sin_family = AF_INET;
  pasvserver.sin_port = htons(port);
  if (inet_pton(AF_INET, m_host.c_str(), &pasvserver.sin_addr.s_addr) != 1) {
    std::cerr << "ERROR!!!..." << std::endl;
    return 0;
  }

  int connectionerror2 = connect(sock, (LPSOCKADDR)&pasvserver, sizeof(struct sockaddr));
  while (connectionerror2 == SOCKET_ERROR) {
    connectionerror2 = connect(sock, (LPSOCKADDR)&pasvserver, sizeof(struct sockaddr));
    trycount2++;
    if (trycount2 = 10) {
      closesocket(sock);
      WSACleanup();
      return 0;
    }
  }

  int result = recv(sock, buffer, buffer_size, 0);
  shutdown(sock, SD_BOTH);
  closesocket(sock);
  return result;
}


void FTP::sendQuitCmd(SOCKET FSoc) const
{
  std::string quitCmd("QUIT\r\n");
  send(FSoc, quitCmd.c_str(), quitCmd.size(), 0);
  recvResponseAndEcho(FSoc);
}

//********************************************
#define ECHO_FTP_RESPONSE 1 // Set to 0 to disable ftp responses output
// utility functions, may be needs to be move to utilities.h/cpp
void echoResponse(std::string& response) {
  if (ECHO_FTP_RESPONSE) {
    std::string respclean = response.erase(response.find_last_not_of(" \t\n\r") + 1);
    std::cout << respclean << std::endl;
  }
}

void echoResponse(LPSTR response) {
  if (ECHO_FTP_RESPONSE) {
    std::string respstr = response;
    echoResponse(respstr);
  }
}

void recvResponseAndEcho(SOCKET socket, LPSTR buffer, int bufsize) {
  int nread = recv(socket, buffer, bufsize, 0);
  *(buffer + nread) = 0;
  echoResponse(buffer);
}

void recvResponseAndEcho(SOCKET socket) {
  char srvResponse[4096];
  recvResponseAndEcho(socket, (LPSTR)&srvResponse, sizeof(srvResponse));
}
*/

LPCWSTR to_LPCWSTR(const std::string& str)
{
  std::wstring ws;
  ws.assign(str.begin(), str.end());
  return ws.c_str();
}

LPCWSTR to_wstr(const std::string& str)
{
  std::wstring wide_str = std::wstring(str.begin(), str.end());
  return wide_str.c_str();
}

//********************

// private void btnCheck_Click(object sender, EventArgs e)
// {
//   bool result = FtpDirectoryExists("ftp://micro123/First", "anonymous", "anonymous");
//   MessageBox.Show("Folder" + result);
// }
//bool FtpDirectoryExists(string directoryPath, string ftpUser, string ftpPassword)
bool FTP::directory_exists(const std::string& directory_path) const
{
  /*
  bool exists = true;
  try {
    FtpWebRequest request = (FtpWebRequest)WebRequest.Create(directory_path);
    request.Credentials = new NetworkCredential(m_user_name, m_password);
    request.Method = WebRequestMethods.Ftp.PrintWorkingDirectory;
    //request.Method = WebRequestMethods.Ftp.UploadFile; // save file to dir
    //request.Method = WebRequestMethods.Ftp.DeleteFile; //delete file

    FtpWebResponse response = (FtpWebResponse)request.GetResponse();
  }  catch (WebException& ex)  {
    exists = false;
  }
  return exists;
  */
  return true;
}

void FTP::FileSubmit(const std::string& local_file, const std::string& remote_destination)
{
  HINTERNET hInternet;
  HINTERNET hFtpSession;
  hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
  if (!hInternet) {
    std::cerr << "ERROR: Failed InternetOpen: errcode " << GetLastError() << std::endl;;
  } else {
    hFtpSession = InternetConnect(hInternet, LPCWSTR("server"), INTERNET_DEFAULT_FTP_PORT, (LPCWSTR)"user", (LPCWSTR)"pass", INTERNET_SERVICE_FTP, 0, 0);
    if (!hFtpSession) {
      std::cout << "ERROR: Failed InternetConnect: errcode " << GetLastError() << std::endl;;
    }  else if (!FtpPutFile(hFtpSession, to_LPCWSTR("D://file.txt"), (LPCWSTR)"/file.txt", FTP_TRANSFER_TYPE_BINARY, 0)) {
        std::cout << "ERROR: Failed FtpPutFile: errcode: " << GetLastError() << std::endl;;
    }
  }
}
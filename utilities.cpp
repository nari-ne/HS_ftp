#include "utilities.h"

#include <windows.h>

#include <ctime>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <set>
#include <string>

// Enables/disables input echo mode
void set_echo_mode(bool enable)
{
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD mode;
  GetConsoleMode(hStdin, &mode);

  if (enable)
    mode |= ENABLE_ECHO_INPUT;
  else
    mode &= ~ENABLE_ECHO_INPUT;

  SetConsoleMode(hStdin, mode);
}

void input_parameters_for_ftp_server(std::string& host, int port, std::string& username, std::string& password)
{
  std::cout << "Host: ";  // "x.x.x.x"
  std::cin >> host;

  std::cout << "Port: ";
  std::cin >> port;

  std::cout << "Username: ";
  std::cin >> username;

  std::cout << "Password: ";
  set_echo_mode(false);
  std::cin >> password;
  set_echo_mode(true);

  std::cout << std::endl;
}

std::string generate_random_string(int length, bool include_digits)
{
  srand((unsigned int)time(nullptr));

  static const std::string valid_char_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                            "abcdefghijklmnopqrstuvwxyz";
                                            "0123456789";
  size_t len = valid_char_set.size();
  if (!include_digits) {
    len -= 10;  // letters only
  }

  std::string str;
  for (int i = 0; i < length; i++) {
    str += valid_char_set[rand() % len];
  }
  
  return str;
}

// Generate random username of 8 characters length 
// until the username does not exist in already existing emails list
std::string generate_unique_email()
{
  srand((unsigned int)time(nullptr));

  const std::string extention("@");
  const int count = 5;
  static const std::string p[count] = { "gmail.com", "yahoo.com", "hotmail.com", "ysu.am", "hsbc.am" };
  
  static std::set<std::string> emails;
  
  std::string new_email;
  do {
    new_email = generate_random_string(8) + extention + p[rand() % count];
  } while (!emails.insert(new_email).second);
  
  return new_email;
}

bool file_exists(const std::string& file_name)
{
  std::ifstream fin(file_name);
  return fin.is_open();
}

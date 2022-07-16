// file: utilities.cpp

#include <iostream>
#include <windows.h>
#include <string>
#include <set>
#include <ctime>

#include "utilities.h"

// Input parameters for the ftp server
void input_parameters_for_ftp_server(std::string& host, int port, std::string& username, std::string& password)
{
  std::cout << "Host: ";  //"x.x.x.x"
  std::cin >> host;


  std::cout << "Port: ";   //21
  std::cin >> port;

  std::cout << "Username: ";
  std::cin >> username;

  std::cout << "Password: ";
  //set_echo(false);
  show_input(false);
  std::cin >> password;
  show_input();
  std::cout << std::endl;
}

void input_ftp_directory_path(std::string& path)
{
  std::cout << "\nDirectory path on the ftp server: ";
  std::cin >> path;
  std::cout << std::endl;
}

//void set_echo(bool enable)
void show_input(bool enable)
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


// Generate a random name using a string of charactres [A-Za-z0-9]
std::string generate_random_string(int length, bool digits_included)
{
  srand((unsigned int)time(nullptr));

  std::string rnd_name;
  //static const char valid_char_set[] =
  static const std::string valid_char_set =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
    "0123456789";

  size_t len = valid_char_set.size();
  if (!digits_included) {
    len -= 10;  // letters only
  }

  for (int i = 0; i < length; i++) {
    rnd_name += valid_char_set[rand() % len];
  }

  return rnd_name;
}

// Generate random username of 8 characters length 
// until the username does not exist in already existing emails list
std::string generate_unique_email()
{
  srand((unsigned int)time(nullptr));

  static std::set<std::string> emails;

  std::string new_user_name;
  const std::string extention("@");
  const int count = 5;
  static const std::string p[count] = { "gmail.com", "yahoo.com", "hotmail.com", "ysu.am", "hsbc.am" };

  std::pair<std::set<std::string>::iterator, bool> r;
  do {
    new_user_name = generate_random_string(8, true) + extention + p[rand() % count];
  } while (!emails.insert(new_user_name).second);
  
  return new_user_name;
}

// HS_db_backup.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "sqlite3.h"
#include "sqlite_db.h"
#include "ftp.h"
#include "utilities.h"

int main()
{
  std::cout << "Creating SQLite Database and Transferring Data to FTP server...\n\n" << std::endl;

  std::cout << "Database (file) name: ";
  std::string fname;
  std::cin >> fname;

  // Create a new SQLite database connection and generate data for database
  SQLite_DB db(fname);
  if (!db.create_sql_database()) {
    db.disconnect();
    return 0;
  }
  std::cout << "=== Create Database: PASSED\n\n";

  const int RECORD_COUNT = 21;
  if (!db.generate_data(RECORD_COUNT)) {
    db.disconnect();
    return 0;
  }
  std::cout << "=== Generate data and fill up table: PASSED\n\n";

  std::string local_file;
  std::cout << "File name into which to dump database: ";
  std::cin >> local_file;
  db.dump_to_file(local_file); // //TODO: result of function didn't used (boolean)
  db.disconnect();

  //Sign in to FTP 
  std::cout << "- Sign in to server...\n";
  std::string host;
  int port {};
  std::string user;
  std::string psswd;
  input_parameters_for_ftp_server(host, port, user, psswd);
  FTP ftp_server(host, port, user, psswd);
  
  std::string remote_path;
  input_ftp_directory_path(remote_path);
  
  if (!ftp_server.process_file(local_file, remote_path)) { //example: process_file("D:\\NM\dump2101.txt", "/remote/ftp/path/");
                                                          //ftpput ftp.contoso.com narinem pswd dmp15.db backup/dmp15.db
    std::cerr << GetLastError() << " Failed processing file..." << std::endl; //error handling
    return 0; //remove if no commands will be added after if()
  }

  return 0;
}



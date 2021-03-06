#include "sqlite3.h"
#include "sqlite_db.h"

#include <iostream>

#include "ftp.h"
#include "utilities.h"

int main()
{
	std::cout << "Creating SQLite Database and Transferring Data to FTP server...\n\n" << std::endl;

	std::cout << "Database (file) name: ";
	std::string fname;
	std::cin >> fname;
	if (file_exists(fname)) {
		std::cerr << "ERROR: Could not create database. Specified file already exists." << std::endl;
		return 0;
	}

	// Create a new SQLite database connection and generate data for database
	SQLite_DB db(fname);
	if (!db.create_database()) {
		return 0;
	}
	std::cout << "=== Create Database: PASSED\n\n";

	const int RECORD_COUNT = 100;
	if (!db.generate_data(RECORD_COUNT)) {
		return 0;
	}
	std::cout << "=== Generate data and fill up table: PASSED\n\n";

	std::string local_file;
	std::cout << "File name to dump database to: ";
	std::cin >> local_file;
	
	if (!db.dump_to_file(local_file)) {
		return 0;
	}
	
	std::cout << "- Sign in to FTP server...\n";
	std::string host, user, password;
	int port = 0;
	input_parameters_for_ftp_server(host, port, user, password);
	FTP ftp_server(host, port, user, password);

	if (!ftp_server.establish_connection()) {
		std::cerr << "ERROR: Failed to connect to server: please check input data." << std::endl;
		return 0;
	}

	std::cout << "\nDirectory path on the ftp server: ";
	std::string remote_path;
	std::cin >> remote_path;

	if (!ftp_server.process_file(local_file, remote_path)) {
		std::cerr << "ERROR: Failed processing file: errcode " << GetLastError() << std::endl;
	}
	std::cout << "\nFinished transferring Data to the FTP server\n" << std::endl;

	ftp_server.close_connection();
	return 0;
}

#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <set>

// Enables/ disables echo input
void show_input(bool enable = true);

// Gets parameters for the ftp server
void input_parameters_for_ftp_server(std::string& host, int port, std::string& user_name, std::string& password);

// Gets directory path on the ftp server, where the export file should be copied
void input_ftp_directory_path(std::string& path);

// Generate a random string using an array of characters [A-Za-z0-9]
std::string generate_random_string(int length, bool digits_included = false);

// Generate a random string for email address, which is unique in existing emails list
std::string generate_unique_email();

#endif //UTILITIES_H


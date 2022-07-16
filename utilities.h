#pragma once

#include <set>
#include <string>

// Reads parameters for the ftp server from the input stream
void input_parameters_for_ftp_server(std::string& host, int port, std::string& user_name, std::string& password);

// Generates a random string using an array of characters [A-Za-z0-9]
std::string generate_random_string(int length, bool include_digits = true);

// Generates a random string for email address, which is unique in existing email list
std::string generate_unique_email();

bool file_exists(const std::string& file_name);

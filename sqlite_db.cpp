#include "sqlite_db.h"

#include "sqlite3.h"

#include <cassert>
#include <experimental/filesystem>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "utilities.h"

int SQLite_DB::s_record_id = 0;

SQLite_DB::SQLite_DB(const std::string& n)
  : m_db_ptr(nullptr)
  , m_name(n)
  , m_table_name("Contacts")
{}

SQLite_DB::~SQLite_DB()
{
	disconnect();
}

bool SQLite_DB::create_database()
{
  if (sqlite3_open(m_name.c_str(), &m_db_ptr) != SQLITE_OK) {
    report_sqlite_error(__FILE__, __LINE__);
	disconnect();
	return false;
  }
  return true;
}

bool SQLite_DB::generate_data(int row_count)
{
  std::cout << "- Creating table and inserting __ " << row_count << " __ rows with random data to it...\n"
            << "\t\tPlease wait for a while...\n";

  if (!create_table()) {
	disconnect();
    return false;
  } 

  if (!fillup_table(row_count)) {
	  disconnect();
	  return false;
  }
  std::cerr << "\n- Finished adding data...\n\n";

  return true;
}

bool SQLite_DB::create_table() const
{
  const std::string sqlstatement =
    "CREATE TABLE IF NOT EXISTS " + m_table_name + " ("
            "ID      INT   PRIMARY KEY,"
            "NAME    TEXT  NOT NULL,"
            "COUNTRY TEXT  NOT NULL,"
            "EMAIL   TEXT  NOT NULL UNIQUE);";

  assert(m_db_ptr != nullptr);    // must be valid and open database connection.
  int rc = sqlite3_exec(m_db_ptr, sqlstatement.c_str(), 0, 0, 0);
  if (rc != SQLITE_OK) {
	report_sqlite_error(__FILE__, __LINE__);
    return false;
  }
  return true;
}

bool SQLite_DB::fillup_table(int row_count)
{
  std::cout << "- Filling up...";

  for (int i = 0; i < row_count; ++i) {
    Record rec = generate_record();
    if (!add_record(rec)) {
      return false;
    }
    std::cout << ".";
  }

  return true;
}

bool SQLite_DB::dump_to_file(const std::string& file_path)
{
  std::ofstream fout(file_path);
  if (!fout.is_open()) {
    std::cerr << "Error: Failed open the file..."  << std::endl;
	disconnect();
    return false;
  }

  std::experimental::filesystem::path p1(file_path);
  if (p1.compare(m_name) == 0) {                      // TODO: Enhance to compare absolute paths
	  std::cerr << "Error: Dump file name should differ from database file name." << std::endl;
	  disconnect();
	  return false;
  }
  
  assert(m_db_ptr != nullptr);

  sqlite3_stmt *select_stmt = nullptr;
  const std::string sql_statement = "SELECT * FROM Contacts;";
  int rc = sqlite3_prepare_v2(m_db_ptr, sql_statement.c_str(), -1, &select_stmt, 0);
  if (rc != SQLITE_OK) {
    report_sqlite_error(__FILE__, __LINE__);
	disconnect();
    return false;
  }
  /* dump columns names into the file */
  const int N = sqlite3_column_count(select_stmt);
  for (int i = 0; i < N; i++) {
    const int w = (i == 0) ? 8 : 24;
    fout << std::setw(w) << sqlite3_column_name(select_stmt, i);
  }
  fout << std::endl;

  /* Dump columns data into the file */
  while (sqlite3_step(select_stmt) == SQLITE_ROW) {
    for (int i = 0; i < N; i++) {
      const int w = (i == 0) ? 8 : 24;
      fout << std::setw(w) << sqlite3_column_text(select_stmt, i);
    }
    fout << std::endl;
  }

  sqlite3_finalize(select_stmt);

  fout.close();

  std::cout << "\n=== Dump to file: PASSED\n\n";
  return true;
}

SQLite_DB::Record SQLite_DB::generate_record() const
{
  srand((unsigned int)time(nullptr));

  Record rec;
  rec.name = generate_random_string(6 + rand() % 5, false);     // lenght from 6 - 10 chars ;
  rec.country = generate_random_string(5 + rand() % 16, false); // [5 - 20] ;
  rec.email = generate_unique_email();  
  
  return rec;
}

bool SQLite_DB::add_record(const Record& rec) const
{
  sqlite3_stmt* insert_stmt = nullptr;
  std::string id = std::to_string(++s_record_id);
  const std::string sql_statement = "INSERT INTO " + m_table_name +
    " VALUES (" + id + ", \"" + rec.name + "\", \"" + rec.country + "\", \"" + rec.email + "\");";
  
  char* err = nullptr;
  int rc = sqlite3_exec(m_db_ptr, sql_statement.c_str(), 0, 0, &err);
  if (rc != SQLITE_OK) {
    report_sqlite_error(__FILE__, __LINE__);
    sqlite3_free(err);
    return false;
  }
  
  return true;
}

void SQLite_DB::disconnect() const
{
  sqlite3_close(m_db_ptr);
}

void SQLite_DB::report_sqlite_error(const std::string& file, int line) const
{
  std::cerr << "Error " << sqlite3_errcode(m_db_ptr) << ": " << sqlite3_errmsg(m_db_ptr)
            << " at " << file << ":" << line << std::endl;
}

void SQLite_DB::drop_table() const
{
  const std::string sql_stmt = "DROP TABLE IF EXISTS " + m_table_name + ";";
  
  int rc = sqlite3_exec(m_db_ptr, sql_stmt.c_str(), 0, 0, 0);
  if (rc != SQLITE_OK) {
    report_sqlite_error(__FILE__, __LINE__);
  }
}


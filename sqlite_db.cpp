// file sqlite_db.cpp
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include "sqlite3.h"
#include "sqlite_db.h"
#include "utilities.h"

int SQLite_DB::m_id = 0;

SQLite_DB::SQLite_DB(const std::string& n)
  : m_db_ptr(nullptr)
  , m_name(n)
  , m_file_name(n)
{}

bool SQLite_DB::create_sql_database()
{
  if (sqlite3_open(m_file_name.c_str(), &m_db_ptr) != SQLITE_OK) {
    report_sqlite_error(__FILE__, __LINE__);
    return false;
  }
  //std::cout << "\t === Created database... name- " << m_name << std::endl;
  return true;
}


bool SQLite_DB::generate_data(int r)
{
  std::cout << "- Creating table and inserting __ " << r << " __ rows with random data to it...\n"
            << "\t\tPlease wait for a while...\n";

  std::string table_name = "Contacts";
  if (!create_table(table_name)) {
    std::cerr << "!!!!!!! Failed to create table...\n";
    sqlite3_close(m_db_ptr);
    return false;
  } 

  if (!fillup_table(table_name, r)) {
    return false;
  }
  std::cerr << "\n- Finished adding data...\n\n";

  sqlite3_close(m_db_ptr);
  return true;
}

bool SQLite_DB::create_table(const std::string& table_name)
{
  /* Create SQL statement */
  const std::string sqlstatement =
    "CREATE TABLE IF NOT EXISTS " /* + m_name + "."*/ + table_name + " ("
            "ID      INT   PRIMARY KEY,"
            "NAME    TEXT  NOT NULL,"
            "COUNTRY TEXT  NOT NULL,"
            "EMAIL   TEXT  NOT NULL UNIQUE);";
  //std::cout << "\t\tsqlstatement:\n" << sqlstatement << std::endl;

  int rc = sqlite3_exec(m_db_ptr, sqlstatement.c_str(), 0, 0, 0);
  //std::cout << "\trc = " << rc << std::endl;
  if (rc != SQLITE_OK) {
    report_sqlite_error(__FILE__, __LINE__);
    return false;
  }
  return true;
}

bool SQLite_DB::fillup_table(const std::string& table_name, int r)
{
  int rc = sqlite3_open(m_file_name.c_str(), &m_db_ptr);
  if (rc != SQLITE_OK) {
    std::cerr << "!!!!! Failed to open db\n";
    report_sqlite_error(__FILE__, __LINE__);
    return false;
  }
  std::cout << "- Filling up...";

  for (int i = 0; i < r; ++i) {
    db_record rec = generate_record();
    if (!add_record(rec, table_name)) {
      return false;
    }
    std::cout << ".";
  }

  rc = sqlite3_close(m_db_ptr);
  if (rc != SQLITE_OK) {
    report_sqlite_error(__FILE__, __LINE__);
    return false;
  }
  return true;
}

bool SQLite_DB::dump_to_file(const std::string& file_path)
{
  //std::cout << "- Dumping database to a file ... \n\n";

  //FILE* fout = fopen(file_path.c_str(), "w");
  std::ofstream fout(file_path);
  if (!fout.is_open()) {
    /* Error handling with errno and exit */
    std::cerr << "Error: Failed open file..."  << std::endl;
    return false;
  }

  if (sqlite3_open(m_file_name.c_str(), &m_db_ptr) != SQLITE_OK) {
    report_sqlite_error(__FILE__, __LINE__);
    return false;
  }

  sqlite3_stmt *select_stmt = nullptr;
  const std::string sql_statement = "SELECT * FROM Contacts;";
  int rc = sqlite3_prepare_v2(m_db_ptr, sql_statement.c_str(), -1, &select_stmt, 0);
  if (rc != SQLITE_OK) {
    report_sqlite_error(__FILE__, __LINE__);
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
  while (SQLITE_ROW == sqlite3_step(select_stmt)) {
    for (int i = 0; i < N; i++) {
      const int w = (i == 0) ? 8 : 24;
      fout << std::setw(w) << sqlite3_column_text(select_stmt, i);
    }
    fout << std::endl;
    //sqlite3_reset(select_stmt);
  }

  sqlite3_finalize(select_stmt);
  //disconnect();

  fout.close();

  std::cout << "\n=== Dump to file: PASSED\n\n";
  return true;
}

db_record SQLite_DB::generate_record()
{
  srand((unsigned int)time(nullptr));

  db_record rec;
  rec.name = generate_random_string(6 + rand() % 5);     // lenght from 6 - 10 chars ;
  rec.country = generate_random_string(5 + rand() % 16); // [5 - 20] ;
  rec.email = generate_unique_email();  
  
  return rec;
}

bool SQLite_DB::add_record(const db_record& rec, const std::string& table_name)
{
  sqlite3_stmt* insert_stmt = nullptr;
  std::string id = std::to_string(++m_id);
  const std::string sql_statement = "INSERT INTO " + table_name +
    " VALUES (" + id + ", \"" + rec.name + "\", \"" + rec.country + "\", \"" + rec.email + "\");";

  int rc = sqlite3_prepare_v2(m_db_ptr, sql_statement.c_str(), sql_statement.size(), &insert_stmt, 0);//preparing the statement
  if (rc != SQLITE_OK) {
    report_sqlite_error(__FILE__, __LINE__);
    sqlite3_finalize(insert_stmt);
    return false;
  }

  rc = sqlite3_step(insert_stmt); // executing the statement
  if (!(rc == SQLITE_DONE || rc == SQLITE_OK)) {
    report_sqlite_error(__FILE__, __LINE__);
    sqlite3_finalize(insert_stmt);
    return false;
  }

  sqlite3_reset(insert_stmt);
  sqlite3_finalize(insert_stmt);
  return true;
}

void SQLite_DB::disconnect()
{
  std::cout << "- Disconnecting SQLite db...\n\n";
  sqlite3_close(m_db_ptr);
  //return sqlite3_close(m_db_ptr) == SQLITE_OK;
}

void SQLite_DB::report_sqlite_error(const std::string& file, int line) const
{
  std::cerr << "Error " << sqlite3_errcode(m_db_ptr) << ": " << sqlite3_errmsg(m_db_ptr)
            << " at " << file << ":" << line << std::endl;
}
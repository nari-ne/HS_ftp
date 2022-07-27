#include "CppUnitTest.h"

#include "ftp.h"
#include "sqlite_db.h"
#include "utilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestv03
{
	const std::string name("A0011");
	SQLite_DB db(name);

	TEST_CLASS(UnitTestv03)
	{
	public:
		
		TEST_METHOD(CreateDatabase)
		{
			Assert::IsTrue(db.create_database());
			Logger::WriteMessage("\t\t*** DB Create: PASSED.\n");
		}
		
		TEST_METHOD(CreateTable)
		{
			sqlite3* ptr = nullptr;
			int rc = sqlite3_open(name.c_str(), &ptr);

			Assert::AreEqual(SQLITE_OK, rc);
			Assert::IsNotNull(ptr);
			
			db.drop_table();

			const int row_count = 5;
			bool filledup = db.generate_data(row_count);
			Assert::IsTrue(filledup);

			sqlite3_stmt* stmt = nullptr;
			std::string select("SELECT * FROM Contacts LIMIT 1;");
			rc = sqlite3_prepare_v2(ptr, select.c_str(), -1, &stmt, 0);
			if (rc != SQLITE_OK) {
				sqlite3_finalize(stmt);
				Logger::WriteMessage("*** ERROR: Failed Prepare Table...");
				return;
			}
			
			Assert::AreEqual(4, sqlite3_column_count(stmt));
			Assert::AreEqual("ID", sqlite3_column_name(stmt, 0));
			Assert::AreEqual("NAME", sqlite3_column_name(stmt, 1));
			Assert::AreEqual("COUNTRY", sqlite3_column_name(stmt, 2));
			Assert::AreEqual("EMAIL", sqlite3_column_name(stmt, 3));

			sqlite3_finalize(stmt);

			/////////////// row count test ////////////////
			select = "SELECT ID FROM Contacts;";
			int r, c;
			char** result;
			char* err_msg = nullptr;
			rc = sqlite3_get_table(ptr, select.c_str(), &result, &r, &c, &err_msg);
			if (rc != SQLITE_OK) {
				Logger::WriteMessage(err_msg);
				sqlite3_free(err_msg);
				return;
			}

			Assert::AreEqual(row_count, r);
			
			Logger::WriteMessage("\t\t*** Table Create: PASSED.\n");
		}
		
		TEST_METHOD(DumpToFile)
		{
			std::string fname("UTest_dump.db");

			int status = 0;
			if (file_exists(fname.c_str())) {
				Logger::WriteMessage((fname + " exists: should be removed.").c_str());
				status = remove(fname.c_str());
			}
			const std::string msg = status == 0 ? "File Deleted Successfully." : "Error: Failed to delete file.";
			Logger::WriteMessage(msg.c_str());

			Assert::IsTrue(status == 0);
			Assert::IsFalse(file_exists(fname));

			bool transfered = db.dump_to_file(fname);
			
			Assert::IsTrue(transfered);
			Assert::IsTrue(file_exists(fname));
			
			Logger::WriteMessage("\t\t*** Dump db to file: PASSED.\n");
		}

	};
}

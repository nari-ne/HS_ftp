# HS_ftp
SQL Backup to FTP server

Created a Windows console application, that is designed for generating data for a SQLLite database, exporting database data into a file and send the file to a FTP server.

The input parameters for the application should be
- SQLite database file name
- host, port, username and password for the ftp server
- directory path on the ftp server, where the export file should be copied

The console application
- creates a SQLLite database and a table
- inserts 100 rows with random data to it
- dumps the database to a file provided by user
- transfers the dump file to the ftp server’s given folder

Follow the runtime instructions of the executable.

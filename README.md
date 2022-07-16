# HS_ftpi
SQL Database Backup to FTP server

This is a simple project in C++ intended for generating data for a SQLite database, exporting database data into a file and send the file to a FTP server.

Compilation has been tested with Microsoft Visual Studio 2017 (Windows 10)

Application
- creates a SQLLite database and a table
- inserts 100 rows with random data to it
- dumps the database to a file provided by user
- transfers the dump file to the ftp server’s given folder

Input parameters for the application:
- SQLite database file name
- host, port, username and password for the ftp server
- directory path on the ftp server, where the export file should be copied.

Database table name:
- Contacts

Database table structure:
colomn name    | constraint 
----------------------------
- ID           | int
- NAME	       | TEXT
- COUNTRY      | TEXT
- EMAIL        | TEXT UNIQE


Note.
- SQLite database file to be created should not be existed
- If one of server specific parameters is incorrect, project is stoped.
- If a directory by given path does not exist on FTP server, a new directory is created.
- After each error a special error message with error code is appeared to help to get the exact place and location of an error 
- At the end of each module final status is appeared on console.

After launch the application please follow the runtime instructions of the executable.

---------------------------------------------------------------------------------------------------------------------------------
Application log example:


Creating SQLite Database and Transferring Data to FTP server...


Database (file) name: demo.db
=== Create Database: PASSED

- Creating table and inserting __ 100 __ rows with random data to it...
                Please wait for a while...
- Filling up.......................................................................................................
- Finished adding data...

=== Generate data and fill up table: PASSED

File name to dump database to: demo.dump

=== Dump to file: PASSED

- Sign in to FTP server...
Host: 127.0.0.1
Port: 21
Username: narinem
Password:

- Establishing an FTP Connection...

=== Initialize use of the WinINet functions: PASSED

=== Open an FTP session for site 127.0.0.1: PASSED


Directory path on the ftp server: backup
- user defined folder does not exist, creating ...

- set user defined backup folder as current directory

- uploading demo.dump local file to backup remote folder on server
> ftpput demo.dump backup/demo.dump

=== FtpPutFile: PASSED


Finished transferring Data to the FTP server

=== Close FTP session: PASSED

=== Close the WinINet functions use: PASSED

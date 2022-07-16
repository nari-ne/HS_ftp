# HS_ftp
SQL Database Backup to FTP server

This is a simple project in C++ intended for generating data for SQLite database, exporting database data into a file and send the file to a FTP server.

Implementation has been done in Microsoft Visual Studio 2017 (Windows 10) environment.

Application:
- creates a SQLLite database and a table
- inserts 100 rows with random data to it
- dumps the database to a file provided by the user
- transfers the dump file to the given folder on FTP server

Input parameters:
- SQLite database file name
- host, port, username and password for the FTP server
- directory path on the FTP server, where the export file should be copied to

Database table name:
- Contacts

Database table structure:
colomn name    | constraint 
----------------------------
- ID           | int
- NAME	       | TEXT
- COUNTRY      | TEXT
- EMAIL        | TEXT UNIQE


Notes:
- If one of server-specific parameters is incorrect, the execution is stopped.
- If a remote directory path does not exist on FTP server, a new directory is created.
- After each error a special error message with error code is being reported, including the file and the error line 
- At the end of each module final status is begin reported on the console

After launching the application, please follow the runtime instructions of the executable.

---------------------------------------------------------------------------------------------------------------------------------
Application log example:

>Creating SQLite Database and Transferring Data to FTP server...
>
>
>Database (file) name: demo.db
>
>=== Create Database: PASSED
>
>\- Creating table and inserting __ 100 __ rows with random data to it...
>
>   Please wait for a while...
>
>\- Filling up.......................................................................................................
>
>\- Finished adding data...
>
>=== Generate data and fill up table: PASSED
>
>File name to dump database to: demo.dump
>
>=== Dump to file: PASSED
>
>\- Sign in to FTP server...
>
>Host: 127.0.0.1
>Port: 21
>Username: narinem
>Password:
>
>\- Establishing an FTP Connection...
>
>=== Initialize use of the WinINet functions: PASSED
>
>=== Open an FTP session for site 127.0.0.1: PASSED
>
>
>Directory path on the ftp server: backup
>
>\- user defined folder does not exist, creating ...
>
>\- set user defined backup folder as current directory
>
>\- uploading demo.dump local file to backup remote folder on server
>
>  ftpput demo.dump backup/demo.dump
>
>=== FtpPutFile: PASSED
>
>
>Finished transferring Data to the FTP server
>
>=== Close FTP session: PASSED
>
>=== Close the WinINet functions use: PASSED
>

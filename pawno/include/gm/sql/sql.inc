stock LoadMySQLSettings()
{
	new mysqlSettings = ini_openFile("mysql_settings.ini"), 
		errCode;
		
	if(mysqlSettings < 0) {
		printf("[scriptfiles/mysql_settings.ini]: Error while opening MySQL settings file. Error code: %d",mysqlSettings);
		return 0;
	}

	errCode = ini_getString(mysqlSettings,"host", sqlData[SQL_HOST]);
	if(errCode < 0) printf("[scriptfiles/mysql_settings.ini]: Error while reading MySQL settings file (host). Error code: %d",errCode);

	errCode = ini_getString(mysqlSettings,"user", sqlData[SQL_USER]);
	if(errCode < 0) printf("[scriptfiles/mysql_settings.ini]: Error while reading MySQL settings file (username). Error code: %d",errCode);

	errCode = ini_getString(mysqlSettings,"password", sqlData[SQL_PASS]);
	if(errCode < 0) printf("[scriptfiles/mysql_settings.ini]: Error while reading MySQL settings file (password). Error code: %d",errCode);

	errCode = ini_getString(mysqlSettings,"database", sqlData[SQL_DB]);
	if(errCode < 0) printf("[scriptfiles/mysql_settings.ini]: Error while reading MySQL settings file (database). Error code: %d",errCode);

	ini_closeFile(mysqlSettings);
	return 1;
}

stock SQL_OnQueryError(errorid, error[], callback[], query[])
{
    if (errorid == CR_SERVER_LOST)
	{
		if(!LoadMySQLSettings()) return 0;
		
	    dbHandle = mysql_connect(sqlData[SQL_HOST], sqlData[SQL_USER], sqlData[SQL_DB], sqlData[SQL_PASS]);
		mysql_log();
		if (mysql_errno() != 0) printf("[MySQL]: Failed to reconnect to %s", sqlData[SQL_HOST]);
		else
		{
			printf("[MySQL]: Reconnected to %s with username %s", sqlData[SQL_HOST], sqlData[SQL_USER]);
			
			mysql_set_charset("cp1251");
		    mysql_query(dbHandle, "SET NAMES cp1251;", false);
			mysql_query(dbHandle, "SET SESSION character_set_server='utf8';", false);

		    new name[32];
			mysql_get_charset(name, dbHandle, 32);
			printf("[MySQL]: Encoding: %s", name);
		}
	}
	printf("[MySQL]: <|| error: %s | callback: %s | query: %s ||>", error, callback, query);
    return 1;
}

stock SQL_OnGameModeInit()
{
	if(!LoadMySQLSettings()) return 0;

	dbHandle = mysql_connect(sqlData[SQL_HOST], sqlData[SQL_USER], sqlData[SQL_DB], sqlData[SQL_PASS]);
	mysql_log();
	
	if (mysql_errno() != 0) print("[MySQL]: Failed to connect to database");
	else
	{
        print("[MySQL]: The connection to the database was successful.");

        mysql_set_charset("cp1251");
	    mysql_tquery(dbHandle, "SET NAMES cp1251;");
		mysql_tquery(dbHandle, "SET SESSION character_set_server='utf8';");

		/*mysql_tquery(dbHandle, "UPDATE `users` SET `online`=0 WHERE `online` = 1");*/
	}
	return 1;
}//дб

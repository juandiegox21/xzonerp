#include <YSI_Coding\y_hooks>

new MySQL: dbHandle;

forward MySQL_CreateConnection();
public MySQL_CreateConnection()
{
    new hostname[70];
    if(Env_Has("MYSQL_HOST")) {
        Env_Get("MYSQL_HOST", hostname, sizeof hostname);
    }
 
    new username[70];
    if(Env_Has("MYSQL_USER")) {
        Env_Get("MYSQL_USER", username, sizeof username);
    }

    new password[70];
    if(Env_Has("MYSQL_PASSWORD")) {
        Env_Get("MYSQL_PASSWORD", password, sizeof password);
    }
 
    new database[70];
    if(Env_Has("MYSQL_DATABASE")) {
        Env_Get("MYSQL_DATABASE", database, sizeof database);
    }

    new MySQLOpt: options = mysql_init_options();
    mysql_set_option(options, AUTO_RECONNECT, true);
    mysql_set_option(options, MULTI_STATEMENTS, true);
    mysql_set_option(options, POOL_SIZE, 2);

    dbHandle = mysql_connect(hostname, username, password, database, options);

    if (dbHandle == MYSQL_INVALID_HANDLE || mysql_errno(dbHandle) != 0)
	{
		Logger_Err("[MySQL] La conexion al servidor de MySQL ha fallado.");
		SendRconCommand("exit"); // close the server if there is no connection
		return 1;
	}

    Logger_Log("[MySQL]: La conexion al servidor de MySQL ha sido exitosa.");

    return 1;
}

stock MySQL:MySQL_GetHandle()
{
    return dbHandle;
}
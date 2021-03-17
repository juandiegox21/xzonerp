#include <YSI_Coding\y_hooks>
#include <YSI\y_iterate>

#define MAX_FACTIONS            5
#define MAX_FACTION_RANKS       8    
#define MAX_FACTION_MEMBERS     30    

// Factions
enum E_FACTIONS
{
	fId,
	fName[42],
};
new Factions[MAX_FACTIONS][E_FACTIONS];

// Faction Members
enum E_FACTION_MEMBERS
{
    fmId,
    fmName[42],
    fmRankId[32],
    fmFactionId,
    fmOnline
}
new FactionMembers[MAX_FACTIONS][MAX_FACTION_MEMBERS][E_FACTION_MEMBERS];

// Faction Ranks
enum E_FACTION_RANKS
{
    frId,
    frName[32],
    frElevation,
    frFactionId
};
new FactionRanks[MAX_FACTIONS][MAX_FACTION_RANKS][E_FACTION_RANKS];

hook OnGameModeInit()
{
    LoadFactions();
}

static LoadFactions()
{
    new query[128];
    mysql_format(MySQL_GetHandle(), query, sizeof(query), "SELECT * FROM `factions`");
	mysql_tquery(MySQL_GetHandle(), query, "OnFactionsLoaded");
    return 1;
}

forward OnFactionsLoaded();
public OnFactionsLoaded()
{
    new rows;
    cache_get_row_count(rows);

    if(rows < 0) return 1;

    for (new row = 0; row < rows; row++) {
        cache_get_value_name_int(row, "id", Factions[row][fId]);
        cache_get_value_name(row, "name", Factions[row][fName]);

        LoadFactionRanks(Factions[row][fId]);
        LoadFactionMembers(Factions[row][fId]);
    }
    
    return 1;
}

static LoadFactionRanks(FactionId)
{
    new query[128];
    mysql_format(MySQL_GetHandle(), query, sizeof(query), \
        "SELECT * FROM `faction_ranks `" \
        "where faction_id = %d", FactionId);

	mysql_tquery(MySQL_GetHandle(), query, "OnFactionRanksLoaded", "d", FactionId);

    return 1;
}

forward OnFactionRanksLoaded(FactionId);
public OnFactionRanksLoaded(FactionId)
{
    new 
        rows,
        FactionIndex = GetFactionIndexBySqlId(FactionId);
        
    cache_get_row_count(rows);

    if(rows < 1) return 1;

    for (new row = 0; row < rows; row++) {
        cache_get_value_name_int(row, "id", FactionRanks[FactionIndex][row][frId]);
        cache_get_value_name(row, "name", FactionRanks[FactionIndex][row][frName]);
        cache_get_value_name_int(row, "elevation", FactionRanks[FactionIndex][row][frElevation]);
        cache_get_value_name_int(row, "faction_id", FactionRanks[FactionIndex][row][frFactionId]);
    }

    return 1;
}

static LoadFactionMembers(FactionId)
{
    new query[128];
    mysql_format(MySQL_GetHandle(), query, sizeof(query), \
        "SELECT * FROM `faction_members `" \
        "where faction_id = %d", FactionId);

	mysql_tquery(MySQL_GetHandle(), query, "OnFactionMembersLoaded", "d", FactionId);

    return 1;
}

forward OnFactionMembersLoaded(FactionId);
public OnFactionMembersLoaded(FactionId)
{
    new 
        rows,
        FactionIndex = GetFactionIndexBySqlId(FactionId);

    cache_get_row_count(rows);

    if(rows < 1) return 1;

    for (new row = 0; row < rows; row++) {
        cache_get_value_name_int(row, "id", FactionMembers[FactionIndex][row][fmId]);
        cache_get_value_name(row, "name", FactionMembers[FactionIndex][row][fmName]);
        cache_get_value_name(row, "rank_id", FactionMembers[FactionIndex][row][fmRankId]);
        cache_get_value_name_int(row, "faction_id", FactionMembers[FactionIndex][row][fmFactionId]);
        cache_get_value_name_int(row, "online", FactionMembers[FactionIndex][row][fmOnline]);
    }

    return 1;
}

//@TODO
forward GetFactionRanks(FactionId);
stock GetFactionRanks(FactionId)
{
    
}

forward GetFactionMembers(FactionId);
stock GetFactionMembers(FactionId)
{
    new
        FactionIndex = GetFactionIndexBySqlId(FactionId),
        stringFactionMembers[540], 
        string[120];
        
    for (new FactionMember = 0; FactionMember < MAX_FACTION_MEMBERS; FactionMember++) {
        if (FactionMembers[FactionIndex][FactionMember][fmFactionId] != FactionId) continue;
    
        new FactionMemberIsOnline[24];
        format(FactionMemberIsOnline, sizeof FactionMemberIsOnline, "%s", (FactionMembers[FactionIndex][FactionMember][fmOnline] == 1) ? "Activo" : "Desconectado");
        format(string, sizeof string, "%s - %s\n", FactionMembers[FactionIndex][FactionMember][fmName], FactionMemberIsOnline);
        strcat(stringFactionMembers, string);
    }

    return stringFactionMembers;
}

forward GetFactionsCount();
stock GetFactionsCount()
{
    new count = 0;
    for (new i = 0; i < MAX_FACTIONS; i++) {
        if (Factions[i][fId] == 0) continue;
        count++;
    }

    return count;
}

forward GetFactionSqlId(index);
stock GetFactionSqlId(index)
{
    return Factions[index][fId];
}

forward GetFactionIndexBySqlId(const FactionSqlId);
stock GetFactionIndexBySqlId(const FactionSqlId)
{
    new FactionIndex = -1;

    for (new i = 0; i < GetFactionsCount(); i++) {
        if (Factions[i][fId] == FactionSqlId) {
            FactionIndex = i;
            break;
        }
    }

    return FactionIndex;
}

forward GetFactionName(const FactionId);
stock GetFactionName(const FactionId)
{
    new 
        FactionNameString[42],
        FactionIndex = GetFactionIndexBySqlId(FactionId);
        
    format(FactionNameString, sizeof(FactionNameString), "%s", Factions[FactionIndex][fName]);
    return FactionNameString;
}

forward AddFactionMember(const PlayerName[], FactionId, Elevation);
public AddFactionMember(const PlayerName[], FactionId, Elevation)
{
    new 
        query[128];

    mysql_format(MySQL_GetHandle(), query, sizeof(query), \
        "INSERT INTO `faction_members`" \
        "(`name`, `faction_id`, `online`)" \
        "VALUES ('%s', '%d', '%d')", PlayerName, FactionId, 1);

	mysql_tquery(MySQL_GetHandle(), query, "OnFactionRanksLoaded", "d", FactionId);

    return 1;
}

forward SendMessageToFactionMembers(SenderId, FactionId, const message[]);
public SendMessageToFactionMembers(SenderId, FactionId, const message[])
{
    new string[220], SenderName[120];
    SenderName = GetPlayerNameRP(SenderId);

    for (new i = 0; i < MAX_PLAYERS; i++) {
        if(!IsPlayerConnected(i)) continue;
        else if (Player[i][pFactionId] != FactionId) continue;

        format(string, sizeof(string), "[%s] %s: %s", GetFactionName(FactionId), SenderName, message);
        SendClientMessage(i, COLOR_DODGER_BLUE, string);
    }

    return 1;
}
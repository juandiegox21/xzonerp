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

enum E_FACTION_RANKS
{
    frId,
    frName[32],
    frElevation,
    frFactionId
};
new FactionRanks[MAX_FACTIONS][MAX_FACTION_RANKS][E_FACTION_RANKS];

enum E_FACTION_MEMBERS
{
    fmId,
    fmName[42],
    fmRankId[32],
    fmFactionId,
    fmOnline
}
new FactionMembers[MAX_FACTIONS][MAX_FACTION_MEMBERS][E_FACTION_MEMBERS];

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

        LoadFactionRanks(row, Factions[row][fId]);
        LoadFactionMembers(row, Factions[row][fId]);
    }
    
    return 1;
}

static LoadFactionMembers(FactionIndex, FactionId)
{
    new query[128];
    mysql_format(MySQL_GetHandle(), query, sizeof(query), \
        "SELECT * FROM `faction_members `" \
        "where faction_id = %d", FactionId);

	mysql_tquery(MySQL_GetHandle(), query, "OnFactionMembersLoaded", "d", FactionIndex);

    return 1;
}

forward OnFactionMembersLoaded(FactionIndex);
public OnFactionMembersLoaded(FactionIndex)
{
    new rows;
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

static LoadFactionRanks(FactionIndex, FactionId)
{
    new query[228];
    mysql_format(MySQL_GetHandle(), query, sizeof(query), \
        "SELECT * FROM `faction_ranks` " \
        "where faction_id = %d", FactionId);

	mysql_tquery(MySQL_GetHandle(), query, "OnFactionRanksLoaded", "d", FactionIndex);

    return 1;
}

forward OnFactionRanksLoaded(FactionIndex);
public OnFactionRanksLoaded(FactionIndex)
{
    new rows;
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

forward GetFactionRanks(FactionId);
stock GetFactionRanks(FactionId)
{
    new 
        FactionSqlId = GetFactionSqlId(FactionId),
        string[120],
        stringFactionMembers[520];

    for (new FactionRank = 0; FactionRank < MAX_FACTION_RANKS; FactionRank++) {
        if (FactionRanks[FactionId][FactionRank][frFactionId] != FactionSqlId) continue;

        format(string, sizeof string, "Rank Elevation: %d | Rank Name: %s\n", FactionRanks[FactionId][FactionRank][frElevation], FactionRanks[FactionId][FactionRank][frName]);
        strcat(stringFactionMembers, string);
    }

    return stringFactionMembers;
}

forward GetFactionMembers(FactionId);
stock GetFactionMembers(FactionId)
{
    new 
        FactionSqlId = GetFactionSqlId(FactionId),
        stringFactionMembers[540], 
        string[120];
        
    for (new FactionMember = 0; FactionMember < MAX_FACTION_MEMBERS; FactionMember++) {
        if (FactionMembers[FactionId][FactionMember][fmFactionId] != FactionSqlId) continue;
    
        new FactionMemberIsOnline[24];
        format(FactionMemberIsOnline, sizeof FactionMemberIsOnline, "%s", (FactionMembers[FactionId][FactionMember][fmOnline] == 1) ? "Activo" : "Desconectado");
        format(string, sizeof string, "%s - %s\n", FactionMembers[FactionId][FactionMember][fmName], FactionMemberIsOnline);
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

forward GetFactionRanksCount(FactionId);
stock GetFactionRanksCount(FactionId)
{
    new count = 0;
    for (new i = 0; i < MAX_FACTION_RANKS; i++) {
        if (FactionRanks[FactionId][i][frId] == 0) continue;
        count++;
    }

    return count;
}

forward GetFactionSqlId(index);
stock GetFactionSqlId(index)
{
    return Factions[index][fId];
}

forward GetFactionName(const FactionId);
stock GetFactionName(const FactionId)
{
    new FactionNameString[42];
    format(FactionNameString, sizeof(FactionNameString), "%s", Factions[FactionId][fName]);

    return FactionNameString;
}

forward GetFactionRankElevationName(FactionId, Elevation); 
stock GetFactionRankElevationName(FactionId, Elevation)
{
    new FactionRankElevationName[64];
    for (new i = 0; i < MAX_FACTION_RANKS; i++) {
        if (FactionRanks[FactionId][i][frElevation] == Elevation) {
            format(FactionRankElevationName, sizeof FactionRankElevationName, "%s", FactionRanks[FactionId][i][frName]);
            break;
        }
    }
    
    return FactionRankElevationName;
}

//@TODO
forward SetPlayerFactionRankElevation(playerid, FactionId, Elevation);
public SetPlayerFactionRankElevation(playerid, FactionId, Elevation)
{
    // new string[120];

    // if (Elevation < 1 && Elevation >= MAX_FACTION_RANKS) {
    //     format(string, sizeof string, "[ATENCION] Por favor especifica un rango entre 0 y %d", MAX_FACTION_RANKS);
    //     return SendClientMessage(playerid, COLOR_RED, string);
    // }

    // format(string, sizeof string, "[%s] Has sido promovido a rango: %s", GetFactionName(FactionId), GetFactionRankElevationName(FactionId, Elevation));
    // SendClientMessage(playerid, COLOR_DODGER_BLUE, string);

    // for (new i = 0; i < MAX_FACTION_RANKS; i++) {
    //     if (FactionRanks[FactionId][i][frElevation] == Elevation) {
    //         FactionMembers[FactionId][]
    //         break;
    //     }
    // }
    return 1;
}

forward GetFactionRankIdByElevation(FactionId, Elevation);
stock GetFactionRankIdByElevation(FactionId, Elevation)
{
    new RankId = 0;

    for (new i = 0; i < MAX_FACTION_RANKS; i++) {
        if (FactionRanks[FactionId][i][frElevation] == Elevation) {
            RankId = FactionRanks[FactionId][i][frId];
            break;
        }
    }

    return RankId;
}

forward AddFactionMember(const PlayerName[], FactionId, Elevation);
public AddFactionMember(const PlayerName[], FactionId, Elevation)
{
    // new 
    //     query[128],
    //     RankId = GetFactionRankIdByElevation(FactionId, Elevation);

    // mysql_format(MySQL_GetHandle(), query, sizeof(query), \
    //     "INSERT INTO `faction_members`" \
    //     "(`name`, `rank_id`, `faction_id`, `online`)" \
    //     "VALUES ('%s', '%d', '%d', '%d')", PlayerName, RankId, FactionId, 1);

	// mysql_tquery(MySQL_GetHandle(), query, "OnFactionRanksLoaded", "d", FactionId);

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
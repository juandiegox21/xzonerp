#include <YSI_Coding\y_hooks>
#include <YSI\y_iterate>

#define MAX_FACTIONS            5
#define MAX_FACTION_MEMBERS     30    

// Factions
enum E_FACTIONS
{
	fId,
	fName[42],
};
new Factions[MAX_FACTIONS][E_FACTIONS];

enum E_FACTION_MEMBERS
{
    fmId,
    fmName[42],
    fmFactionId
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

        // LoadFactionMembers(Factions[row][fId]);
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
    new rows;
    cache_get_row_count(rows);

    if(rows < 1) return 1;

    for (new row = 0; row < rows; row++) {
        cache_get_value_name_int(row, "id", FactionMembers[FactionId][row][fmId]);
        cache_get_value_name(row, "name", FactionMembers[FactionId][row][fmName]);
        cache_get_value_name(row, "faction_id", FactionMembers[FactionId][row][fmFactionId]);
    }

    return 1;
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

forward GetFactionName(const FactionId);
stock GetFactionName(const FactionId)
{
    new FactionNameString[42];
    format(FactionNameString, sizeof(FactionNameString), "%s", Factions[FactionId][fName]);

    return FactionNameString;
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
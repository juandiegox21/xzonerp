#include <YSI_Coding\y_hooks>
#include <YSI\y_iterate>

#define MAX_FACTIONS    5

// Factions
enum E_FACTIONS
{
	ID,
	Name[42],
};
new Factions[MAX_FACTIONS][E_FACTIONS];

hook OnGameModeInit()
{
    new query[128];
    mysql_format(MySQL_GetHandle(), query, sizeof(query), "SELECT * FROM `factions`");
	mysql_tquery(MySQL_GetHandle(), query, "OnFactionsLoaded");
}

forward OnFactionsLoaded();
public OnFactionsLoaded()
{
    new rows;
    cache_get_row_count(rows);

    if(rows < 1) return 1;

    for (new row = 0; row < rows; row++) {
        cache_get_value_name_int(row, "id", Factions[row][ID]);
        cache_get_value_name(row, "name", Factions[row][Name]);
    }
    
    return 1;
}
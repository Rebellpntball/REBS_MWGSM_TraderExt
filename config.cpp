class CfgPatches
{
	class REBS_MWGSM_TraderExt
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {
			"DZ_Data",
			"DZ_Structures",
			"DZ_Characters",
			"DZ_Gear_Consumables",
			"MWGSM_RoamingTrader"          // hard dependency on the original mod
		};
	};
};

class CfgMods
{
	class REBS_MWGSM_TraderExt
	{
		dir = "REBS_MWGSM_TraderExt";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 1;
		name = "REBS MWGSM Trader Ext";
		credits = "REBS / Muddywaters Gaming & Server Management LLC";
		author = "REBS + MWGSM";
		authorID = "0";
		version = "1.0.0";
		extra = 0;
		type = "mod";
		dependencies[] = {"Game", "World", "Mission"};

		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"REBS_MWGSM_TraderExt/scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"REBS_MWGSM_TraderExt/scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"REBS_MWGSM_TraderExt/scripts/5_Mission"};
			};
		};
	};
};

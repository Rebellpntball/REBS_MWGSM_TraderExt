// REBS_MWGSM_TraderExt - Per-dealer vehicle catalog files
// 3_Game — player-authored JSON only (no CfgVehicles scan)
// Path: $profile:REBS_MWGSM_TraderExt/<VehicleFile name>
// Non-vehicle traders never touch this. They stay on MWGSM GenerateStock.

class REBS_VehicleEntry
{
	string ClassName;
	int BuyPrice;
	int SellPrice;
	int MaxStock;
	int Enabled;
	ref TStringArray Attachments;

	void REBS_VehicleEntry()
	{
		ClassName = "";
		BuyPrice = 1000;
		SellPrice = 200;
		MaxStock = 2;
		Enabled = 1;
		Attachments = new TStringArray();
	}
}

class REBS_VehicleCatalogFile
{
	ref array<ref REBS_VehicleEntry> Vehicles;

	void REBS_VehicleCatalogFile()
	{
		Vehicles = new array<ref REBS_VehicleEntry>();
	}
}

class REBS_VehicleCatalog
{
	static const string FOLDER = "$profile:REBS_MWGSM_TraderExt";
	private static ref map<string, ref REBS_VehicleCatalogFile> s_Cache;

	static void InitCache()
	{
		if (!s_Cache)
			s_Cache = new map<string, ref REBS_VehicleCatalogFile>();
	}

	static REBS_VehicleCatalogFile Load(string fileName)
	{
		InitCache();
		if (fileName == "")
			return null;
		if (s_Cache.Contains(fileName))
			return s_Cache.Get(fileName);

		string path = FOLDER + "/" + fileName;
		if (!FileExist(FOLDER))
			MakeDirectory(FOLDER);

		ref REBS_VehicleCatalogFile cat = new REBS_VehicleCatalogFile();
		if (!FileExist(path))
		{
			Print("[REBS_MWGSM_TraderExt] VehicleFile not found: " + path);
			s_Cache.Set(fileName, cat);
			return cat;
		}

		JsonFileLoader<REBS_VehicleCatalogFile>.JsonLoadFile(path, cat);
		if (!cat.Vehicles)
			cat.Vehicles = new array<ref REBS_VehicleEntry>();
		Print("[REBS_MWGSM_TraderExt] Loaded vehicle catalog \"" + fileName + "\" — " + cat.Vehicles.Count().ToString() + " entries");
		s_Cache.Set(fileName, cat);
		return cat;
	}

	static REBS_VehicleEntry FindEntry(string fileName, string className)
	{
		REBS_VehicleCatalogFile cat = Load(fileName);
		if (!cat || !cat.Vehicles)
			return null;
		int count = cat.Vehicles.Count();
		for (int i = 0; i < count; i++)
		{
			REBS_VehicleEntry e = cat.Vehicles.Get(i);
			if (e && e.ClassName == className)
				return e;
		}
		return null;
	}

	static void EnsureExampleFile()
	{
		if (!FileExist(FOLDER))
			MakeDirectory(FOLDER);
		string path = FOLDER + "/Dealer1.json";
		if (FileExist(path))
			return;

		ref REBS_VehicleCatalogFile cat = new REBS_VehicleCatalogFile();
		ref REBS_VehicleEntry oh = new REBS_VehicleEntry();
		ooh.ClassName = "OffroadHatchback";
		ooh.BuyPrice = 2500;
		ooh.SellPrice = 400;
		ooh.MaxStock = 2;
		ooh.Attachments.Insert("HatchbackWheel");
		ooh.Attachments.Insert("HatchbackWheel");
		ooh.Attachments.Insert("HatchbackWheel");
		ooh.Attachments.Insert("HatchbackWheel");
		ooh.Attachments.Insert("HatchbackWheel");
		ooh.Attachments.Insert("CarBattery");
		ooh.Attachments.Insert("CarRadiator");
		ooh.Attachments.Insert("SparkPlug");
		cat.Vehicles.Insert(oh);
		JsonFileLoader<REBS_VehicleCatalogFile>.JsonSaveFile(path, cat);
		Print("[REBS_MWGSM_TraderExt] Wrote example vehicle catalog: " + path);
	}
}

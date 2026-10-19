// REBS_MWGSM_TraderExt - Configuration Manager
// 3_Game Layer - Separate config for static / specialized traders
// Depends on MWGSM_RoamingTrader

class REBS_MWGSM_TraderExtConfig
{
	private static ref REBS_MWGSM_TraderExtConfig m_Instance;

	static const string CONFIG_PATH   = "$profile:REBS_MWGSM_TraderExt/REBS_MWGSM_TraderExtConfig.json";
	static const string CONFIG_FOLDER = "$profile:REBS_MWGSM_TraderExt";
	static const string README_PATH   = "$profile:REBS_MWGSM_TraderExt/CONFIG_EXPLANATION.txt";

	int Enabled;
	float MaxVehicleDistanceToTrader;
	ref array<ref REBS_VehicleSpawnPoint> GlobalVehicleSpawnPoints;
	ref map<string, ref TStringArray> VehicleDefaultAttachments;
	ref array<ref REBS_StaticTraderDefinition> StaticTraders;
	int EnableDebugMode;
	int LogTransactions;
	int LogSpawnLocations;

	void REBS_MWGSM_TraderExtConfig()
	{
		GlobalVehicleSpawnPoints   = new array<ref REBS_VehicleSpawnPoint>();
		VehicleDefaultAttachments  = new map<string, ref TStringArray>();
		StaticTraders              = new array<ref REBS_StaticTraderDefinition>();
		SetDefaults();
	}

	static REBS_MWGSM_TraderExtConfig GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new REBS_MWGSM_TraderExtConfig();
			m_Instance.Load();
		}
		return m_Instance;
	}

	void SetDefaults()
	{
		Enabled = 1;
		MaxVehicleDistanceToTrader = 120.0;
		EnableDebugMode   = 0;
		LogTransactions    = 1;
		LogSpawnLocations  = 1;

		ref REBS_StaticTraderDefinition vehTrader = new REBS_StaticTraderDefinition();
		vehTrader.Name              = "Vehicle Trader";
		vehTrader.Enabled           = 0;
		vehTrader.Position          = "0 0 0";
		vehTrader.Orientation       = "0 0 0";
		vehTrader.CategoryFilter    = "Vehicles";
		vehTrader.MaxItemsInStock   = 20;
		vehTrader.BuyPriceMultiplier  = 2.0;
		vehTrader.SellPriceMultiplier = 0.35;
		vehTrader.TraderNPCGear.Insert("GorkaHelmet");
		vehTrader.TraderNPCGear.Insert("USMCJacket_Desert");
		vehTrader.TraderNPCGear.Insert("USMCPants_Desert");
		vehTrader.TraderNPCGear.Insert("MilitaryBoots_Black");
		vehTrader.GuaranteedStock.Insert("OffroadHatchback");
		vehTrader.GuaranteedStock.Insert("Hatchback_02");
		vehTrader.GuaranteedStock.Insert("CivilianSedan");
		StaticTraders.Insert(vehTrader);

		ref REBS_StaticTraderDefinition medTrader = new REBS_StaticTraderDefinition();
		medTrader.Name              = "Field Medic";
		medTrader.Enabled           = 0;
		medTrader.Position          = "0 0 0";
		medTrader.Orientation       = "0 0 0";
		medTrader.CategoryFilter    = "Medical";
		medTrader.MaxItemsInStock   = 40;
		medTrader.BuyPriceMultiplier  = 1.4;
		medTrader.SellPriceMultiplier = 0.5;
		medTrader.TraderNPCGear.Insert("SurgicalGloves_Blue");
		medTrader.TraderNPCGear.Insert("MedicalScrubsShirt_Blue");
		medTrader.TraderNPCGear.Insert("MedicalScrubsPants_Blue");
		medTrader.GuaranteedStock.Insert("Morphine");
		medTrader.GuaranteedStock.Insert("Epinephrine");
		medTrader.GuaranteedStock.Insert("BandageDressing");
		medTrader.GuaranteedStock.Insert("SalineBagIV");
		StaticTraders.Insert(medTrader);

		SetDefaultVehicleAttachments();
	}

	private void SetDefaultVehicleAttachments()
	{
		ref TStringArray oh = new TStringArray();
		oh.Insert("HatchbackWheel"); oh.Insert("HatchbackWheel"); oh.Insert("HatchbackWheel");
		oh.Insert("HatchbackWheel"); oh.Insert("HatchbackWheel");
		oh.Insert("CarBattery"); oh.Insert("CarRadiator"); oh.Insert("SparkPlug");
		oh.Insert("HatchbackHood"); oh.Insert("HatchbackTrunk");
		oh.Insert("HatchbackDoors_Driver"); oh.Insert("HatchbackDoors_CoDriver");
		oh.Insert("HeadlightH7"); oh.Insert("HeadlightH7");
		VehicleDefaultAttachments.Set("OffroadHatchback", oh);

		ref TStringArray h2 = new TStringArray();
		h2.Insert("Hatchback_02_Wheel"); h2.Insert("Hatchback_02_Wheel"); h2.Insert("Hatchback_02_Wheel");
		h2.Insert("Hatchback_02_Wheel"); h2.Insert("Hatchback_02_Wheel");
		h2.Insert("Hatchback_02_Door_1_1"); h2.Insert("Hatchback_02_Door_2_1");
		h2.Insert("Hatchback_02_Door_1_2"); h2.Insert("Hatchback_02_Door_2_2");
		h2.Insert("Hatchback_02_Hood"); h2.Insert("Hatchback_02_Trunk");
		h2.Insert("CarBattery"); h2.Insert("CarRadiator"); h2.Insert("SparkPlug");
		h2.Insert("HeadlightH7"); h2.Insert("HeadlightH7");
		VehicleDefaultAttachments.Set("Hatchback_02", h2);

		ref TStringArray cs = new TStringArray();
		cs.Insert("CivSedanWheel"); cs.Insert("CivSedanWheel"); cs.Insert("CivSedanWheel");
		cs.Insert("CivSedanWheel"); cs.Insert("CivSedanWheel");
		cs.Insert("CarBattery"); cs.Insert("CarRadiator"); cs.Insert("SparkPlug");
		cs.Insert("CivSedanHood"); cs.Insert("CivSedanTrunk");
		cs.Insert("CivSedanDoors_Driver"); cs.Insert("CivSedanDoors_CoDriver");
		cs.Insert("CivSedanDoors_BackLeft"); cs.Insert("CivSedanDoors_BackRight");
		cs.Insert("HeadlightH7"); cs.Insert("HeadlightH7");
		VehicleDefaultAttachments.Set("CivilianSedan", cs);

		ref TStringArray s2 = new TStringArray();
		s2.Insert("Sedan_02_Wheel"); s2.Insert("Sedan_02_Wheel"); s2.Insert("Sedan_02_Wheel");
		s2.Insert("Sedan_02_Wheel"); s2.Insert("Sedan_02_Wheel");
		s2.Insert("CarBattery"); s2.Insert("CarRadiator"); s2.Insert("SparkPlug");
		s2.Insert("Sedan_02_Hood"); s2.Insert("Sedan_02_Trunk");
		s2.Insert("Sedan_02_Door_1_1"); s2.Insert("Sedan_02_Door_2_1");
		s2.Insert("Sedan_02_Door_1_2"); s2.Insert("Sedan_02_Door_2_2");
		s2.Insert("HeadlightH7"); s2.Insert("HeadlightH7");
		VehicleDefaultAttachments.Set("Sedan_02", s2);

		ref TStringArray tr = new TStringArray();
		tr.Insert("Truck_01_Wheel"); tr.Insert("Truck_01_Wheel"); tr.Insert("Truck_01_Wheel");
		tr.Insert("Truck_01_Wheel");
		tr.Insert("Truck_01_WheelDouble"); tr.Insert("Truck_01_WheelDouble");
		tr.Insert("Truck_01_WheelDouble"); tr.Insert("Truck_01_WheelDouble");
		tr.Insert("TruckBattery"); tr.Insert("Truck_01_Hood");
		tr.Insert("Truck_01_Door_1_1"); tr.Insert("Truck_01_Door_2_1");
		tr.Insert("HeadlightH7"); tr.Insert("HeadlightH7");
		VehicleDefaultAttachments.Set("Truck_01_Covered", tr);

		ref TStringArray o2 = new TStringArray();
		o2.Insert("Offroad_02_Wheel"); o2.Insert("Offroad_02_Wheel"); o2.Insert("Offroad_02_Wheel");
		o2.Insert("Offroad_02_Wheel"); o2.Insert("Offroad_02_Wheel");
		o2.Insert("Offroad_02_Door_1_1"); o2.Insert("Offroad_02_Door_2_1");
		o2.Insert("Offroad_02_Door_1_2"); o2.Insert("Offroad_02_Door_2_2");
		o2.Insert("Offroad_02_Hood"); o2.Insert("Offroad_02_Trunk");
		o2.Insert("CarBattery"); o2.Insert("GlowPlug");
		o2.Insert("HeadlightH7"); o2.Insert("HeadlightH7");
		VehicleDefaultAttachments.Set("Offroad_02", o2);
	}

	bool Load()
	{
		if (!FileExist(CONFIG_FOLDER))
		{
			Print("[REBS_MWGSM_TraderExt] Creating config folder: " + CONFIG_FOLDER);
			MakeDirectory(CONFIG_FOLDER);
		}

		if (!FileExist(CONFIG_PATH))
		{
			Print("[REBS_MWGSM_TraderExt] Config not found, writing defaults: " + CONFIG_PATH);
			Save();
			return true;
		}

		StaticTraders.Clear();
		GlobalVehicleSpawnPoints.Clear();

		Print("[REBS_MWGSM_TraderExt] Loading config: " + CONFIG_PATH);
		JsonFileLoader<REBS_MWGSM_TraderExtConfig>.JsonLoadFile(CONFIG_PATH, this);
		return true;
	}

	bool Save()
	{
		if (!FileExist(CONFIG_FOLDER))
			MakeDirectory(CONFIG_FOLDER);

		Print("[REBS_MWGSM_TraderExt] Saving config: " + CONFIG_PATH);
		JsonFileLoader<REBS_MWGSM_TraderExtConfig>.JsonSaveFile(CONFIG_PATH, this);
		GenerateReadme();
		return true;
	}

	void DebugLog(string msg)
	{
		if (EnableDebugMode)
			Print("[REBS_MWGSM_TraderExt] " + msg);
	}

	void Log(string msg)
	{
		Print("[REBS_MWGSM_TraderExt] " + msg);
	}

	void GenerateReadme()
	{
		FileHandle file = OpenFile(README_PATH, FileMode.WRITE);
		if (!file)
			return;

		FPrintln(file, "================================================================================");
		FPrintln(file, "  REBS_MWGSM_TraderExt - Configuration Guide");
		FPrintln(file, "  Separate static / specialized traders for MWGSM Roaming Trader");
		FPrintln(file, "================================================================================");
		FPrintln(file, "");
		FPrintln(file, "This mod REQUIRES MWGSM_RoamingTrader to be loaded.");
		FPrintln(file, "Edit the .json file, not this .txt file.");
		FPrintln(file, "");
		FPrintln(file, "enabled (0/1) - Master switch");
		FPrintln(file, "MaxVehicleDistanceToTrader - Max distance (m) for vehicle sell / spawn search");
		FPrintln(file, "StaticTraders[] - Name, Enabled, Position, Orientation, CategoryFilter,");
		FPrintln(file, "  MaxItemsInStock, Buy/SellPriceMultiplier, GuaranteedStock, TraderNPCGear, VehicleSpawnPoints");
		FPrintln(file, "VehicleDefaultAttachments - classname to attachment list map");
		CloseFile(file);
	}
}

class REBS_StaticTraderDefinition
{
	string Name;
	int Enabled;
	vector Position;
	vector Orientation;
	string CategoryFilter;
	int MaxItemsInStock;
	float BuyPriceMultiplier;
	float SellPriceMultiplier;
	ref TStringArray GuaranteedStock;
	ref TStringArray TraderNPCGear;
	ref array<ref REBS_VehicleSpawnPoint> VehicleSpawnPoints;

	void REBS_StaticTraderDefinition()
	{
		Name = "Static Trader";
		Enabled = 0;
		Position = "0 0 0";
		Orientation = "0 0 0";
		CategoryFilter = "All";
		MaxItemsInStock = 30;
		BuyPriceMultiplier = 1.5;
		SellPriceMultiplier = 0.5;
		GuaranteedStock = new TStringArray();
		TraderNPCGear = new TStringArray();
		VehicleSpawnPoints = new array<ref REBS_VehicleSpawnPoint>();
	}
}

class REBS_VehicleSpawnPoint
{
	vector Position;
	vector Orientation;

	void REBS_VehicleSpawnPoint()
	{
		Position = "0 0 0";
		Orientation = "0 0 0";
	}
}

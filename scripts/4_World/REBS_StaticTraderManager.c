// REBS_MWGSM_TraderExt - Static Trader Manager
// 4_World Layer - Spawns and tracks fixed-position specialized traders

class REBS_StaticTraderManager
{
	private static ref REBS_StaticTraderManager m_Instance;

	private ref array<MWGSM_TraderNPC> m_StaticTraders;
	private ref REBS_MWGSM_TraderExtConfig m_Config;
	private bool m_IsInitialized;

	void REBS_StaticTraderManager()
	{
		m_StaticTraders = new array<MWGSM_TraderNPC>();
		m_Config = null;
		m_IsInitialized = false;
	}

	static REBS_StaticTraderManager GetInstance()
	{
		if (!m_Instance)
			m_Instance = new REBS_StaticTraderManager();
		return m_Instance;
	}

	void Initialize()
	{
		if (m_IsInitialized)
			return;

		if (!GetGame().IsServer())
			return;

		m_Config = REBS_MWGSM_TraderExtConfig.GetInstance();
		if (!m_Config || !m_Config.Enabled)
		{
			Print("[REBS_MWGSM_TraderExt] Extension disabled or config missing – static traders will not spawn.");
			m_IsInitialized = true;
			return;
		}

		SpawnAllStaticTraders();
		m_IsInitialized = true;

		m_Config.Log(string.Format("Static trader manager ready – %1 traders active", m_StaticTraders.Count()));
	}

	private void SpawnAllStaticTraders()
	{
		int count = m_Config.StaticTraders.Count();
		for (int i = 0; i < count; i++)
		{
			REBS_StaticTraderDefinition def = m_Config.StaticTraders.Get(i);
			if (!def || !def.Enabled)
				continue;

			if (def.Position == vector.Zero)
			{
				m_Config.Log("Skipping trader \"" + def.Name + "\" – position is zero");
				continue;
			}

			SpawnOneTrader(def);
		}
	}

	private void SpawnOneTrader(REBS_StaticTraderDefinition def)
	{
		vector pos = def.Position;
		pos[1] = GetGame().SurfaceY(pos[0], pos[2]);

		// Re-use the exact same NPC class from the parent mod
		MWGSM_TraderNPC trader = MWGSM_TraderNPC.Cast(
			GetGame().CreateObjectEx("MWGSM_TraderNPC", pos,
				ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOPERSISTENCY_WORLD)
		);

		if (!trader)
		{
			m_Config.Log("Failed to spawn static trader: " + def.Name);
			return;
		}

		trader.SetOrientation(def.Orientation);

		// Apply gear from the definition
		int gearCount = def.TraderNPCGear.Count();
		for (int g = 0; g < gearCount; g++)
		{
			string gear = def.TraderNPCGear.Get(g);
			if (gear != "")
				trader.GetInventory().CreateInInventory(gear);
		}

		// Close the attachment window (same as parent mod)
		trader.FinalizeSpawn();

		// Generate stock filtered by the category this trader is responsible for
		GenerateFilteredStock(trader, def);

		m_StaticTraders.Insert(trader);

		if (m_Config.LogSpawnLocations)
			m_Config.Log(string.Format("Static trader \"%1\" spawned at %2 (category: %3)",
				def.Name, pos.ToString(), def.CategoryFilter));
	}

	private void GenerateFilteredStock(MWGSM_TraderNPC trader, REBS_StaticTraderDefinition def)
	{
		if (!trader)
			return;

		trader.GenerateStock();

		MWGSM_TraderStock stock = trader.GetStock();
		if (!stock)
			return;

		if (def.CategoryFilter != "" && def.CategoryFilter != "All")
		{
			ref array<int> toRemove = new array<int>();
			int itemCount = stock.GetItemCount();
			for (int i = 0; i < itemCount; i++)
			{
				MWGSM_TraderItem item = stock.GetItem(i);
				if (!item)
					continue;

				if (item.Category != def.CategoryFilter)
					toRemove.Insert(i);
			}

			for (int r = toRemove.Count() - 1; r >= 0; r--)
				stock.RemoveItem(toRemove.Get(r));
		}

		int gCount = def.GuaranteedStock.Count();
		for (int g = 0; g < gCount; g++)
		{
			string cls = def.GuaranteedStock.Get(g);
			if (cls == "")
				continue;

			if (stock.FindItemByClassName(cls) != null)
				continue;

			if (stock.GetItemCount() >= def.MaxItemsInStock)
				break;

			ref MWGSM_TraderItem newItem = new MWGSM_TraderItem();
			newItem.SetFromConfig(cls);
			newItem.Stock = Math.RandomInt(1, newItem.MaxStock + 1);

			newItem.BuyPrice  = Math.Round(newItem.BuyPrice  * def.BuyPriceMultiplier);
			newItem.SellPrice = Math.Round(newItem.SellPrice * def.SellPriceMultiplier);

			stock.AddItem(newItem);
		}

		while (stock.GetItemCount() > def.MaxItemsInStock)
			stock.RemoveItem(stock.GetItemCount() - 1);
	}

	int GetStaticTraderCount()
	{
		return m_StaticTraders.Count();
	}

	MWGSM_TraderNPC GetNearestStaticTrader(vector pos)
	{
		MWGSM_TraderNPC nearest = null;
		float nearestDist = 999999.0;

		int count = m_StaticTraders.Count();
		for (int i = 0; i < count; i++)
		{
			MWGSM_TraderNPC t = m_StaticTraders.Get(i);
			if (!t)
				continue;

			float d = vector.Distance(pos, t.GetPosition());
			if (d < nearestDist)
			{
				nearestDist = d;
				nearest = t;
			}
		}
		return nearest;
	}

	REBS_StaticTraderDefinition GetDefinitionForTrader(MWGSM_TraderNPC trader)
	{
		if (!trader || !m_Config)
			return null;

		vector tPos = trader.GetPosition();
		int count = m_Config.StaticTraders.Count();
		for (int i = 0; i < count; i++)
		{
			REBS_StaticTraderDefinition def = m_Config.StaticTraders.Get(i);
			if (!def || !def.Enabled)
				continue;

			if (vector.Distance(tPos, def.Position) < 5.0)
				return def;
		}
		return null;
	}
}

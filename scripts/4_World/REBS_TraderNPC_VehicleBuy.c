// REBS_MWGSM_TraderExt - Vehicle purchase override
// 4_World Layer - Intercepts buy for vehicle classnames and spawns them at parking spots
// 1.30 safe – uses only CreateObjectEx + CreateAttachment
//
// NOTE: Parent MWGSM_TraderNPC::RemoveCurrencyFromPlayer / GiveCurrencyToPlayer are private.
//       We therefore re-use the public GetPlayerCurrency and perform the currency
//       transfer with the same logic the parent uses (EnumerateInventory).

modded class MWGSM_TraderNPC
{
	override bool BuyItem(PlayerBase player, string className, int quantity = 1)
	{
		if (!REBS_VehicleHelpers.IsVehicleClass(className))
			return super.BuyItem(player, className, quantity);

		if (!GetGame().IsServer())
			return false;

		if (!player || !CanInteract(player))
			return false;

		MWGSM_TraderItem item = GetStock().FindItemByClassName(className);
		if (!item || item.Stock < quantity)
			return false;

		int totalCost = item.BuyPrice * quantity;
		if (GetPlayerCurrency(player) < totalCost)
			return false;

		vector spawnPos, spawnOri;
		array<ref REBS_VehicleSpawnPoint> preferred = null;

		REBS_StaticTraderManager mgr = REBS_StaticTraderManager.GetInstance();
		if (mgr)
		{
			REBS_StaticTraderDefinition def = mgr.GetDefinitionForTrader(this);
			if (def)
				preferred = def.VehicleSpawnPoints;
		}

		if (!REBS_VehicleHelpers.FindFreeSpawnPoint(GetPosition(), spawnPos, spawnOri, preferred))
		{
			REBS_MWGSM_TraderExtConfig.GetInstance().Log("Vehicle buy refused – no free spawn point near trader");
			return false;
		}

		if (!REBS_RemoveCurrency(player, totalCost))
			return false;

		for (int i = 0; i < quantity; i++)
		{
			EntityAI veh = REBS_VehicleHelpers.SpawnVehicleWithParts(className, spawnPos, spawnOri);
			if (!veh)
			{
				if (i == 0)
					REBS_GiveCurrency(player, totalCost);
				return false;
			}

			if (i + 1 < quantity)
			{
				if (!REBS_VehicleHelpers.FindFreeSpawnPoint(GetPosition(), spawnPos, spawnOri, preferred))
					break;
			}
		}

		item.Stock -= quantity;

		PlayerIdentity identity = player.GetIdentity();
		if (identity)
		{
			ref MWGSM_Transaction transaction = new MWGSM_Transaction();
			transaction.PlayerID = identity.GetId();
			transaction.PlayerName = identity.GetName();
			transaction.ItemClassName = className;
			transaction.TransactionType = "BUY_VEHICLE";
			transaction.Price = totalCost;
			transaction.Quantity = quantity;
			transaction.Timestamp = GetGame().GetTime();
			transaction.TraderLocation = GetPosition();
			transaction.LogTransaction();
		}

		return true;
	}

	private bool REBS_RemoveCurrency(PlayerBase player, int amount)
	{
		if (!player || amount <= 0)
			return amount <= 0;

		MWGSM_RoamingTraderConfig config = MWGSM_RoamingTraderConfig.GetInstance();
		int remaining = amount;

		ref array<EntityAI> items = new array<EntityAI>();
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		ref array<string> currencyTypes = new array<string>();
		currencyTypes.Insert(config.CurrencyItem);
		currencyTypes.InsertAll(config.AlternateCurrencies);

		int itemCount = items.Count();
		for (int i = 0; i < itemCount; i++)
		{
			if (remaining <= 0)
				break;

			EntityAI item = items.Get(i);
			if (!item)
				continue;

			string itemType = item.GetType();
			int currencyCount = currencyTypes.Count();
			for (int c = 0; c < currencyCount; c++)
			{
				string currency = currencyTypes.Get(c);
				if (itemType != currency)
					continue;

				float rate = 1.0;
				if (config.CurrencyExchangeRates.Contains(currency))
					rate = config.CurrencyExchangeRates.Get(currency);

				ItemBase itemBase = ItemBase.Cast(item);
				if (!itemBase)
					continue;

				Magazine mag = Magazine.Cast(item);
				if (mag)
				{
					int ammoCount = mag.GetAmmoCount();
					int ammoValue = Math.Round(ammoCount * rate);
					if (ammoValue <= remaining)
					{
						remaining -= ammoValue;
						GetGame().ObjectDelete(item);
					}
					else
					{
						int ammoToRemove = Math.Ceil(remaining / rate);
						mag.ServerSetAmmoCount(ammoCount - ammoToRemove);
						remaining = 0;
					}
				}
				else
				{
					int stackQuantity = itemBase.GetQuantity();
					if (stackQuantity > 0)
					{
						int stackValue = Math.Round(stackQuantity * rate);
						if (stackValue <= remaining)
						{
							remaining -= stackValue;
							GetGame().ObjectDelete(item);
						}
						else
						{
							int toRemove = Math.Ceil(remaining / rate);
							itemBase.SetQuantity(stackQuantity - toRemove);
							remaining = 0;
						}
					}
					else
					{
						int itemValue = Math.Round(rate);
						if (itemValue <= remaining)
						{
							remaining -= itemValue;
							GetGame().ObjectDelete(item);
						}
					}
				}
				break;
			}
		}

		return remaining <= 0;
	}

	private bool REBS_GiveCurrency(PlayerBase player, int amount)
	{
		if (!player || amount <= 0)
			return true;

		MWGSM_RoamingTraderConfig config = MWGSM_RoamingTraderConfig.GetInstance();
		string currencyClass = config.CurrencyItem;
		int remaining = amount;

		while (remaining > 0)
		{
			EntityAI currency = player.GetInventory().CreateInInventory(currencyClass);
			if (!currency)
			{
				vector dropPos = player.GetPosition() + Vector(Math.RandomFloat(-0.5, 0.5), 0, Math.RandomFloat(-0.5, 0.5));
				dropPos[1] = GetGame().SurfaceY(dropPos[0], dropPos[2]);
				currency = EntityAI.Cast(GetGame().CreateObject(currencyClass, dropPos, false, false, false));
			}
			if (!currency)
				break;

			Magazine mag = Magazine.Cast(currency);
			if (mag)
			{
				int maxAmmo = mag.GetAmmoMax();
				int stackAmt = Math.Min(remaining, maxAmmo);
				mag.ServerSetAmmoCount(stackAmt);
				remaining -= stackAmt;
			}
			else
			{
				ItemBase itemBase = ItemBase.Cast(currency);
				if (itemBase)
				{
					int maxQty = itemBase.GetQuantityMax();
					if (maxQty > 1)
					{
						int stackAmt2 = Math.Min(remaining, maxQty);
						itemBase.SetQuantity(stackAmt2);
						remaining -= stackAmt2;
					}
					else
					{
						remaining -= 1;
					}
				}
				else
				{
					remaining -= 1;
				}
			}
		}
		return true;
	}
}

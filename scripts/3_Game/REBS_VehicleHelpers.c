// REBS_MWGSM_TraderExt - Vehicle helpers
// 3_Game Layer - Shared utility functions (1.30 safe)

class REBS_VehicleHelpers
{
	// Returns true if the classname is a known vehicle (vanilla + common patterns)
	static bool IsVehicleClass(string className)
	{
		if (className == "")
			return false;

		// Fast path – common vanilla vehicles
		if (className == "OffroadHatchback" || className == "Hatchback_02" ||
			className == "CivilianSedan" || className == "Sedan_02" ||
			className == "Truck_01_Covered" || className == "Offroad_02")
			return true;

		// Config inheritance check (works for modded cars that inherit CarScript / Car)
		if (GetGame().IsKindOf(className, "CarScript"))
			return true;
		if (GetGame().IsKindOf(className, "Car"))
			return true;

		// Expansion / other common bases
		if (GetGame().IsKindOf(className, "ExpansionVehicleBase"))
			return true;
		if (GetGame().IsKindOf(className, "ExpansionHelicopterScript"))
			return true;
		if (GetGame().IsKindOf(className, "ExpansionBoatScript"))
			return true;

		return false;
	}

	// Attach the default parts defined in config (or fall back to empty)
	static void AttachDefaultParts(EntityAI vehicle, string className)
	{
		if (!vehicle || className == "")
			return;

		REBS_MWGSM_TraderExtConfig cfg = REBS_MWGSM_TraderExtConfig.GetInstance();
		if (!cfg)
			return;

		if (!cfg.VehicleDefaultAttachments.Contains(className))
		{
			cfg.DebugLog("No default attachments defined for " + className);
			return;
		}

		TStringArray parts = cfg.VehicleDefaultAttachments.Get(className);
		if (!parts)
			return;

		int count = parts.Count();
		for (int i = 0; i < count; i++)
		{
			string partClass = parts.Get(i);
			if (partClass == "")
				continue;

			EntityAI attached = vehicle.GetInventory().CreateAttachment(partClass);
			if (!attached)
			{
				// Some parts may fail (slot already filled / wrong type) – just log
				cfg.DebugLog("Failed to attach " + partClass + " to " + className);
			}
		}
	}

	// Find a free spawn point near the given trader position
	// Returns true and fills outPos / outOri when a free spot is found
	static bool FindFreeSpawnPoint(vector traderPos, out vector outPos, out vector outOri, array<ref REBS_VehicleSpawnPoint> preferredPoints = null)
	{
		REBS_MWGSM_TraderExtConfig cfg = REBS_MWGSM_TraderExtConfig.GetInstance();
		if (!cfg)
			return false;

		float maxDist = cfg.MaxVehicleDistanceToTrader;

		// 1. Prefer points that belong to this trader
		if (preferredPoints && preferredPoints.Count() > 0)
		{
			int pCount = preferredPoints.Count();
			for (int i = 0; i < pCount; i++)
			{
				REBS_VehicleSpawnPoint sp = preferredPoints.Get(i);
				if (!sp)
					continue;

				if (vector.Distance(traderPos, sp.Position) > maxDist)
					continue;

				if (IsSpawnPointClear(sp.Position))
				{
					outPos = sp.Position;
					outOri = sp.Orientation;
					return true;
				}
			}
		}

		// 2. Fall back to global list
		int gCount = cfg.GlobalVehicleSpawnPoints.Count();
		for (int g = 0; g < gCount; g++)
		{
			REBS_VehicleSpawnPoint gsp = cfg.GlobalVehicleSpawnPoints.Get(g);
			if (!gsp)
				continue;

			if (vector.Distance(traderPos, gsp.Position) > maxDist)
				continue;

			if (IsSpawnPointClear(gsp.Position))
			{
				outPos = gsp.Position;
				outOri = gsp.Orientation;
				return true;
			}
		}

		return false;
	}

	// Simple clearance check – no objects of type Car / large objects in a small radius
	static bool IsSpawnPointClear(vector pos)
	{
		ref array<Object> objects = new array<Object>();
		GetGame().GetObjectsAtPosition(pos, 4.0, objects, null);

		int count = objects.Count();
		for (int i = 0; i < count; i++)
		{
			Object obj = objects.Get(i);
			if (!obj)
				continue;

			if (obj.IsInherited(CarScript) || obj.IsInherited(Car))
				return false;

			// Block if a building is right on the spot
			if (obj.IsBuilding())
				return false;
		}
		return true;
	}

	// Spawn a vehicle at the given position with default parts
	// Returns the created EntityAI or null on failure
	static EntityAI SpawnVehicleWithParts(string className, vector pos, vector ori)
	{
		if (className == "" || pos == vector.Zero)
			return null;

		// Surface Y correction
		pos[1] = GetGame().SurfaceY(pos[0], pos[2]);

		Object obj = GetGame().CreateObjectEx(className, pos,
			ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOPERSISTENCY_WORLD);

		if (!obj)
			return null;

		obj.SetOrientation(ori);

		EntityAI veh = EntityAI.Cast(obj);
		if (veh)
			AttachDefaultParts(veh, className);

		return veh;
	}
}

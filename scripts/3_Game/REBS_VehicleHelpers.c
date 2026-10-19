// REBS_MWGSM_TraderExt - Vehicle helpers
// 3_Game Layer - Shared utility functions (1.30 safe)

class REBS_VehicleHelpers
{
	// Returns true if the classname is a known vehicle (vanilla + common patterns)
	static bool IsVehicleClass(string className)
	{
		if (className == "")
			return false;

		// Fast path – common vanilla cars (1.30)
		if (className == "OffroadHatchback" || className == "Hatchback_02" ||
			className == "CivilianSedan" || className == "Sedan_02" ||
			className == "Truck_01_Covered" || className == "Offroad_02")
			return true;

		// 1.30 motorbikes (Jana 50 / Bitrak 682) – MotorbikeScript is NOT CarScript
		if (className.IndexOf("Motorbike_01") == 0 || className.IndexOf("Motorbike_02") == 0)
			return true;

		// Config inheritance – cars, trucks
		if (GetGame().IsKindOf(className, "CarScript"))
			return true;
		if (GetGame().IsKindOf(className, "Car"))
			return true;

		// 1.30 two-wheel hierarchy (sibling of Car, not child)
		if (GetGame().IsKindOf(className, "MotorbikeScript"))
			return true;
		if (GetGame().IsKindOf(className, "Motorbike"))
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
				cfg.DebugLog("Failed to attach " + partClass + " to " + className);
		}
	}

	static bool FindFreeSpawnPoint(vector traderPos, out vector outPos, out vector outOri, array<ref REBS_VehicleSpawnPoint> preferredPoints = null)
	{
		REBS_MWGSM_TraderExtConfig cfg = REBS_MWGSM_TraderExtConfig.GetInstance();
		if (!cfg)
			return false;

		float maxDist = cfg.MaxVehicleDistanceToTrader;

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
			if (obj.IsInherited(MotorbikeScript) || obj.IsInherited(Motorbike))
				return false;

			if (obj.IsBuilding())
				return false;
		}
		return true;
	}

	static EntityAI SpawnVehicleWithParts(string className, vector pos, vector ori)
	{
		if (className == "" || pos == vector.Zero)
			return null;

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

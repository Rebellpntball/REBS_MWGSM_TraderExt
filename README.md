# REBS_MWGSM_TraderExt

**Static / specialized traders extension for [MWGSM Roaming Trader](https://mwgsm.com)**

Separate Workshop mod that depends on the original MWGSM Roaming Trader.  
Adds fixed-position traders (vehicles, medical/drugs, building, etc.) while leaving the roaming military trader completely untouched.

---

## Requirements

| Mod | Required |
|-----|----------|
| **MWGSM Roaming Trader** | Yes (hard dependency) |
| DayZ 1.30+ | Recommended |

Load order in `serverDZ.cfg` / launch parameters:

```
-mod=@MWGSM_RoamingTrader;@REBS_MWGSM_TraderExt
```

---

## Features

- Fully separate config (`$profile:REBS_MWGSM_TraderExt/`)
- Static traders defined in JSON (position, orientation, category filter, gear, prices)
- Vehicle trading support following the proven Expansion-style pattern:
  - Vehicles never go into inventory
  - Spawned at configured parking spots with default attachments
  - Clearance check so cars don’t stack
- Category filtering (`Vehicles`, `Medical`, `Weapons`, `All`, …)
- Per-trader buy/sell price multipliers
- Guaranteed stock list per trader
- 1.30-safe scripting (no obsolete inventory helpers, low parameter counts, no sealed-class issues)

---

## Quick Start

1. Install both mods and start the server once.
2. Open  
   `$profile:REBS_MWGSM_TraderExt/REBS_MWGSM_TraderExtConfig.json`
3. Set a real position + `"Enabled": 1` for the traders you want.
4. (Optional) Add `VehicleSpawnPoints` near vehicle traders.
5. Restart.

Example vehicle trader entry:

```json
{
  "Name": "Vehicle Trader",
  "Enabled": 1,
  "Position": [4500.0, 0, 10200.0],
  "Orientation": [90, 0, 0],
  "CategoryFilter": "Vehicles",
  "MaxItemsInStock": 15,
  "BuyPriceMultiplier": 2.0,
  "SellPriceMultiplier": 0.35,
  "GuaranteedStock": [
    "OffroadHatchback",
    "Hatchback_02",
    "CivilianSedan"
  ],
  "TraderNPCGear": [
    "GorkaHelmet",
    "USMCJacket_Desert",
    "USMCPants_Desert",
    "MilitaryBoots_Black"
  ],
  "VehicleSpawnPoints": [
    {
      "Position": [4510.0, 0, 10210.0],
      "Orientation": [90, 0, 0]
    },
    {
      "Position": [4515.0, 0, 10215.0],
      "Orientation": [90, 0, 0]
    }
  ]
}
```

---

## Config Location

```
$profile:REBS_MWGSM_TraderExt/
├── REBS_MWGSM_TraderExtConfig.json
└── CONFIG_EXPLANATION.txt          (auto-generated)
```

---

## Vehicle Attachments

Default attachment lists for vanilla vehicles are already included.  
Add or override any classname in the `VehicleDefaultAttachments` map inside the JSON.

---

## Credits

- Original roaming trader system: **MWGSM** (Muddywaters Gaming & Server Management LLC)
- Extension & vehicle support: **REBS**

---

## License / Workshop

This is an extension mod. Respect the license of the original MWGSM Roaming Trader.  
Publish as a separate Workshop item and list MWGSM Roaming Trader as a required dependency.

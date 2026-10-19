# REBS_MWGSM_TraderExt — Player / Admin Config Guide (DayZ 1.30)

## Install

1. Load **MWGSM_RoamingTrader** then **REBS_MWGSM_TraderExt**
2. Start the server once (creates the folder)
3. Copy the example JSON to:

```
<profiles>/REBS_MWGSM_TraderExt/REBS_MWGSM_TraderExtConfig.json
```

4. Edit positions for **your** map (Y is height — the server snaps to surface Y on spawn)
5. Set `"Enabled": 1` on traders you want
6. Restart

## 1.30 notes

- **Motorbikes** supported (`Motorbike_01_*` Jana 50, `Motorbike_02_*` Bitrak 682)
- Config uses `FileExist` + `JsonFileLoader` with `$profile:` — works on 1.30 (FindFile bug does **not** affect us)
- Vehicles never go into inventory; they spawn at free parking spots with default parts

## Positions in this example (Chernarus)

| Trader | Approx area | Default |
|--------|-------------|---------| 
| Vehicle Trader | Green Mountain | **Enabled** |
| Field Medic | Elektrozavodsk | **Enabled** |
| Building Supplies | Severograd | **Enabled** |
| Weapons Dealer | NWAF South | Disabled (enable + verify coords) |

**Always verify coordinates in-game** (DayZ Editor / admin tools). Wrong Y or blocked spots = no spawn.

## CategoryFilter values

Must match categories used by the parent MWGSM stock system, e.g.:

- `Vehicles`
- `Medical`
- `Weapons`
- `Tools`
- `All` (no filter)

## VehicleSpawnPoints

Only needed for vehicle traders. Place 3–6 free parking spots within `MaxVehicleDistanceToTrader` (default 120 m).

## Prices

- `BuyPriceMultiplier` / `SellPriceMultiplier` stack on top of MWGSM base prices
- Vehicles are expensive by design (2.0 buy / 0.35 sell in the example)

## Disabling a trader

```json
"Enabled": 0
```

or remove the entry from `StaticTraders`.

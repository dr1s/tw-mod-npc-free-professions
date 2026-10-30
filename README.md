# tw-mod-npc-free-professions

Tortoise-WoW port of [AzerothCore mod-npc-free-professions](https://github.com/azerothcore/mod-npc-free-professions).

## Features

- Adds an NPC (`Kaylub`, entry `601016`) that can teach players a profession with all of its recipes.
- Supports the Vanilla/Tortoise professions: Alchemy, Blacksmithing, Enchanting, Engineering, Leatherworking, Tailoring, Herbalism, Mining, Skinning, Cooking, First Aid, and Fishing.
- Inscription and Jewelcrafting are not available in Vanilla and have been removed.
- Secondary professions (Cooking, First Aid, Fishing) do not count toward the primary profession limit.
- Skill level granted is configurable; default is `300` (Vanilla maximum).
- Recipe learning is filtered to `req_skill_value <= 300`, so only Vanilla and Tortoise custom recipes are taught.

## Requirements

- Tortoise-WoW core
- MariaDB/MySQL world database access for the SQL migration

## Installation

1. Place the module under `modules/tw-mod-npc-free-professions`.
2. Apply `data/sql/world/tw-mod-npc-free-professions.sql` to your world database.
3. Copy `conf/tw-mod-npc-free-professions.conf.dist` to `conf/tw-mod-npc-free-professions.conf` and adjust settings.
4. Re-run CMake and rebuild the server.

## Configuration

Config section: `[NpcFreeProfessions]`

| Key | Default | Description |
| --- | --- | --- |
| `NpcFreeProfessions.Enable` | `1` | Enable the module |
| `NpcFreeProfessions.GivenCraftLevel` | `300` | Skill level granted when learning a profession |
| `NpcFreeProfessions.Enable.<Profession>` | `1` | Toggle each profession in the gossip menu |

## Spawning the NPC

After applying the SQL and rebuilding, spawn `Kaylub` in-game with a GM character:

```
.npc add 601016
```

Or insert a row into the `creature` table manually:

```sql
INSERT INTO `creature` (`id`, `map`, `position_x`, `position_y`, `position_z`, `orientation`)
VALUES (601016, 0, -8809.37, 628.35, 94.25, 0.0); -- Stormwind example coordinates
```


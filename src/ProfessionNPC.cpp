#include "ProfessionNPC.h"

#include "ScriptObjects.h"
#include "Config/Config.h"
#include "Chat.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "SpellMgr.h"
#include "DBCStores.h"
#include "DBCStructure.h"
#include "SharedDefines.h"
#include "World.h"
#include "ObjectMgr.h"

namespace
{
    bool ModConfigEnable = true;
    uint16 ModConfigGivenCraftLevel = 300;
    bool ModConfigEnableAlchemy = true;
    bool ModConfigEnableBlacksmithing = true;
    bool ModConfigEnableLeatherworking = true;
    bool ModConfigEnableTailoring = true;
    bool ModConfigEnableEngineering = true;
    bool ModConfigEnableEnchanting = true;
    bool ModConfigEnableHerbalism = true;
    bool ModConfigEnableSkinning = true;
    bool ModConfigEnableMining = true;
    bool ModConfigEnableCooking = true;
    bool ModConfigEnableFirstAid = true;
    bool ModConfigEnableFishing = true;

    class NpcFreeProfessionsConfig : public WorldScript
    {
    public:
        NpcFreeProfessionsConfig()
            : WorldScript("NpcFreeProfessionsConfig", { WORLDHOOK_ON_BEFORE_WORLD_INITIALIZED })
        {
        }

        void OnBeforeWorldInitialized() override
        {
            ModConfigEnable                = sConfig.GetBoolDefault("NpcFreeProfessions.Enable", true);
            ModConfigGivenCraftLevel       = static_cast<uint16>(sConfig.GetIntDefault("NpcFreeProfessions.GivenCraftLevel", 300));
            ModConfigEnableAlchemy         = sConfig.GetBoolDefault("NpcFreeProfessions.Enable.Alchemy", true);
            ModConfigEnableBlacksmithing   = sConfig.GetBoolDefault("NpcFreeProfessions.Enable.Blacksmithing", true);
            ModConfigEnableLeatherworking  = sConfig.GetBoolDefault("NpcFreeProfessions.Enable.Leatherworking", true);
            ModConfigEnableTailoring       = sConfig.GetBoolDefault("NpcFreeProfessions.Enable.Tailoring", true);
            ModConfigEnableEngineering     = sConfig.GetBoolDefault("NpcFreeProfessions.Enable.Engineering", true);
            ModConfigEnableEnchanting      = sConfig.GetBoolDefault("NpcFreeProfessions.Enable.Enchanting", true);
            ModConfigEnableHerbalism       = sConfig.GetBoolDefault("NpcFreeProfessions.Enable.Herbalism", true);
            ModConfigEnableSkinning        = sConfig.GetBoolDefault("NpcFreeProfessions.Enable.Skinning", true);
            ModConfigEnableMining          = sConfig.GetBoolDefault("NpcFreeProfessions.Enable.Mining", true);
            ModConfigEnableCooking         = sConfig.GetBoolDefault("NpcFreeProfessions.Enable.Cooking", true);
            ModConfigEnableFirstAid        = sConfig.GetBoolDefault("NpcFreeProfessions.Enable.FirstAid", true);
            ModConfigEnableFishing         = sConfig.GetBoolDefault("NpcFreeProfessions.Enable.Fishing", true);
        }
    };

    class NpcFreeProfessionsCreatureScript : public CreatureScript
    {
    public:
        NpcFreeProfessionsCreatureScript() : CreatureScript("npc_free_professions") {}

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            if (!ModConfigEnable)
                return false;

            AddGossipItemForProfession(ModConfigEnableAlchemy, player, "Alchemy", SKILL_ALCHEMY);
            AddGossipItemForProfession(ModConfigEnableBlacksmithing, player, "Blacksmithing", SKILL_BLACKSMITHING);
            AddGossipItemForProfession(ModConfigEnableLeatherworking, player, "Leatherworking", SKILL_LEATHERWORKING);
            AddGossipItemForProfession(ModConfigEnableTailoring, player, "Tailoring", SKILL_TAILORING);
            AddGossipItemForProfession(ModConfigEnableEngineering, player, "Engineering", SKILL_ENGINEERING);
            AddGossipItemForProfession(ModConfigEnableEnchanting, player, "Enchanting", SKILL_ENCHANTING);
            AddGossipItemForProfession(ModConfigEnableHerbalism, player, "Herbalism", SKILL_HERBALISM);
            AddGossipItemForProfession(ModConfigEnableSkinning, player, "Skinning", SKILL_SKINNING);
            AddGossipItemForProfession(ModConfigEnableMining, player, "Mining", SKILL_MINING);
            AddGossipItemForProfession(ModConfigEnableCooking, player, "Cooking", SKILL_COOKING);
            AddGossipItemForProfession(ModConfigEnableFirstAid, player, "First Aid", SKILL_FIRST_AID);
            AddGossipItemForProfession(ModConfigEnableFishing, player, "Fishing", SKILL_FISHING);

            player->PlayerTalkClass->SendGossipMenu(DEFAULT_GOSSIP_MESSAGE, creature->GetObjectGuid());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 sender, uint32 skill) override
        {
            if (!ModConfigEnable)
                return false;

            player->PlayerTalkClass->ClearMenus();

            if (sender == GOSSIP_SENDER_MAIN)
            {
                if (player->HasSkill(static_cast<uint16>(skill)))
                {
                    player->GetSession()->SendNotification("You already have this work!");
                    player->PlayerTalkClass->CloseGossip();
                }
                else
                {
                    LearnAllRecipesInProfession(player, static_cast<SkillType>(skill));
                    player->PlayerTalkClass->CloseGossip();
                }
            }

            return true;
        }

        static void LearnAllRecipesInProfession(Player* player, SkillType skill)
        {
            if (!HasFreeProfession(player, skill))
                return;

            switch (skill)
            {
                case SKILL_ALCHEMY:
                    LearnSpells(player, {
                        SPELL_APPRENTICE_ALCHEMY,
                        SPELL_JOURNEYMAN_ALCHEMY,
                        SPELL_EXPERT_ALCHEMY,
                        SPELL_ARTISAN_ALCHEMY});
                    break;
                case SKILL_BLACKSMITHING:
                    LearnSpells(player, {
                        SPELL_APPRENTICE_BLACKSMITHING,
                        SPELL_JOURNEYMAN_BLACKSMITHING,
                        SPELL_EXPERT_BLACKSMITHING,
                        SPELL_ARTISAN_BLACKSMITHING});
                    break;
                case SKILL_ENCHANTING:
                    LearnSpells(player, {
                        SPELL_APPRENTICE_ENCHANTING,
                        SPELL_JOURNEYMAN_ENCHANTING,
                        SPELL_EXPERT_ENCHANTING,
                        SPELL_ARTISAN_ENCHANTING});
                    break;
                case SKILL_ENGINEERING:
                    LearnSpells(player, {
                        SPELL_APPRENTICE_ENGINEERING,
                        SPELL_JOURNEYMAN_ENGINEERING,
                        SPELL_EXPERT_ENGINEERING,
                        SPELL_ARTISAN_ENGINEERING});
                    break;
                case SKILL_LEATHERWORKING:
                    LearnSpells(player, {
                        SPELL_APPRENTICE_LEATHERWORKING,
                        SPELL_JOURNEYMAN_LEATHERWORKING,
                        SPELL_EXPERT_LEATHERWORKING,
                        SPELL_ARTISAN_LEATHERWORKING});
                    break;
                case SKILL_TAILORING:
                    LearnSpells(player, {
                        SPELL_APPRENTICE_TAILORING,
                        SPELL_JOURNEYMAN_TAILORING,
                        SPELL_EXPERT_TAILORING,
                        SPELL_ARTISAN_TAILORING});
                    break;
                case SKILL_COOKING:
                    LearnSpells(player, {
                        SPELL_APPRENTICE_COOKING,
                        SPELL_JOURNEYMAN_COOKING,
                        SPELL_EXPERT_COOKING,
                        SPELL_ARTISAN_COOKING});
                    break;
                case SKILL_FIRST_AID:
                    LearnSpells(player, {
                        SPELL_APPRENTICE_FIRST_AID,
                        SPELL_JOURNEYMAN_FIRST_AID,
                        SPELL_EXPERT_FIRST_AID,
                        SPELL_ARTISAN_FIRST_AID});
                    break;
                case SKILL_FISHING:
                    LearnSpells(player, {
                        SPELL_APPRENTICE_FISHING,
                        SPELL_JOURNEYMAN_FISHING,
                        SPELL_EXPERT_FISHING,
                        SPELL_ARTISAN_FISHING});
                    break;
                case SKILL_MINING:
                    LearnSpells(player, {
                        SPELL_APPRENTICE_MINING,
                        SPELL_JOURNEYMAN_MINING,
                        SPELL_EXPERT_MINING,
                        SPELL_ARTISAN_MINING});
                    break;
                case SKILL_SKINNING:
                    LearnSpells(player, {
                        SPELL_APPRENTICE_SKINNING,
                        SPELL_JOURNEYMAN_SKINNING,
                        SPELL_EXPERT_SKINNING,
                        SPELL_ARTISAN_SKINNING});
                    break;
                case SKILL_HERBALISM:
                    LearnSpells(player, {
                        SPELL_APPRENTICE_HERBALISM,
                        SPELL_JOURNEYMAN_HERBALISM,
                        SPELL_EXPERT_HERBALISM,
                        SPELL_ARTISAN_HERBALISM});
                    break;
                default:
                    break;
            }

            if (SkillLineEntry const* skillInfo = sSkillLineStore.LookupEntry(skill))
            {
                player->SetSkill(skillInfo->id, ModConfigGivenCraftLevel, ModConfigGivenCraftLevel);

                uint32 classMask = player->GetClassMask();

                SkillLineAbilityMapBounds bounds = sSpellMgr.GetSkillLineAbilityMapBoundsBySkillId(skillInfo->id);
                for (auto itr = bounds.first; itr != bounds.second; ++itr)
                {
                    SkillLineAbilityEntry const* ability = itr->second;
                    if (!ability)
                        continue;

                    if (ability->skillId != skillInfo->id)
                        continue;

                    if (ability->forward_spellid)
                        continue;

                    if (ability->racemask != 0)
                        continue;

                    if (ability->classmask && (ability->classmask & classMask) == 0)
                        continue;

                    if (ability->req_skill_value > 300)
                        continue;

                    SpellEntry const* spellInfo = sSpellMgr.GetSpellEntry(ability->spellId);
                    if (!spellInfo || !sSpellMgr.IsSpellValid(spellInfo, player, false) || spellInfo->Id == SPELL_APPRENTICE_FISHING_DUPLICATE)
                        continue;

                    player->LearnSpell(ability->spellId, false);
                }
            }
        }

        static bool HasFreeProfession(Player* player, SkillType skill)
        {
            if (skill == SKILL_FISHING || skill == SKILL_COOKING || skill == SKILL_FIRST_AID)
                return true;

            uint8 skillCount = 0;

            if (player->HasSkill(SKILL_MINING))
                skillCount++;
            if (player->HasSkill(SKILL_SKINNING))
                skillCount++;
            if (player->HasSkill(SKILL_HERBALISM))
                skillCount++;

            for (uint32 i = 1; i < sSkillLineStore.GetNumRows(); ++i)
            {
                if (SkillLineEntry const* skillInfo = sSkillLineStore.LookupEntry(i))
                {
                    if (skillInfo->categoryId == SKILL_CATEGORY_SECONDARY)
                        continue;

                    if (skillInfo->categoryId != SKILL_CATEGORY_PROFESSION)
                        continue;

                    if (player->HasSkill(skillInfo->id))
                        skillCount++;
                }
            }

            if (skillCount > 0 && player->HasSkill(skill))
                skillCount--;

            return skillCount < sWorld.getConfig(CONFIG_UINT32_MAX_PRIMARY_TRADE_SKILL);
        }

        static void AddGossipItemForProfession(bool enabled, Player* player, const std::string& name, SkillType skill)
        {
            if (enabled)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, name.c_str(), GOSSIP_SENDER_MAIN, static_cast<uint32>(skill));
        }

        static void LearnSpells(Player* player, std::initializer_list<uint32> spellIds)
        {
            for (uint32 spellId : spellIds)
                player->LearnSpell(spellId, false);
        }
    };
}

void AddSC_Professions()
{
    new NpcFreeProfessionsConfig();
    new NpcFreeProfessionsCreatureScript();
}

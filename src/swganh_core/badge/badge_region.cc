// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <swganh_core/badge/badge_region.h>
#include <swganh/connection/connection_client_interface.h>

#include <swganh_core/messages/chat_system_message.h>
#include <swganh_core/messages/play_music_message.h>
#include <swganh_core/messages/out_of_band.h>
#include <swganh_core/messages/system_message.h>

#include "badge_service.h"

using namespace swganh::badge;
using namespace swganh::messages;

BadgeRegion::BadgeRegion(std::string badge_name, BadgeService* service)
	: badge_service_(service)
	, badge_name_(badge_name)
{
	SetCollidable(true);
}

BadgeRegion::~BadgeRegion(void)
{
}

void BadgeRegion::OnCollisionEnter(std::shared_ptr<swganh::object::Object> collider)
{
	//badge_service_->GiveBadge(collider, badge_name_);
	badge_service_->GiveBadge(collider, "poi_rabidbeast");
	badge_service_->GiveBadge(collider, "poi_prisonbreak");
	badge_service_->GiveBadge(collider, "poi_twoliars");
	badge_service_->GiveBadge(collider, "poi_factoryliberation");
	badge_service_->GiveBadge(collider, "poi_heromark");
	badge_service_->GiveBadge(collider, "exp_tat_bens_hut");
	badge_service_->GiveBadge(collider, "exp_tat_tusken_pool");
	badge_service_->GiveBadge(collider, "exp_tat_krayt_skeleton");
	badge_service_->GiveBadge(collider, "exp_tat_escape_pod");
	badge_service_->GiveBadge(collider, "exp_tat_sarlacc_pit");
	badge_service_->GiveBadge(collider, "exp_tat_lars_homestead");
	badge_service_->GiveBadge(collider, "exp_tat_krayt_graveyard");
	badge_service_->GiveBadge(collider, "exp_nab_gungan_sacred_place");
	badge_service_->GiveBadge(collider, "exp_cor_agrilat_swamp");
	badge_service_->GiveBadge(collider, "exp_yav_temple_woolamander");
	badge_service_->GiveBadge(collider, "exp_yav_temple_blueleaf");
	badge_service_->GiveBadge(collider, "exp_yav_temple_exar_kun");
	badge_service_->GiveBadge(collider, "exp_lok_volcano");
	badge_service_->GiveBadge(collider, "exp_dat_tarpit");
	badge_service_->GiveBadge(collider, "exp_dat_sarlacc");
	badge_service_->GiveBadge(collider, "exp_dat_escape_pod");
	badge_service_->GiveBadge(collider, "exp_dat_misty_falls_1");
	badge_service_->GiveBadge(collider, "exp_dat_misty_falls_2");
	badge_service_->GiveBadge(collider, "exp_dan_jedi_temple");
	badge_service_->GiveBadge(collider, "exp_dan_rebel_base");
	badge_service_->GiveBadge(collider, "event_project_dead_eye_1");
	badge_service_->GiveBadge(collider, "acc_good_samaritan");
	badge_service_->GiveBadge(collider, "acc_fascinating_background");
	badge_service_->GiveBadge(collider, "acc_brave_soldier");
	badge_service_->GiveBadge(collider, "acc_interesting_personage");
	badge_service_->GiveBadge(collider, "acc_professional_demeanor");
	badge_service_->GiveBadge(collider, "warren_compassion");
	badge_service_->GiveBadge(collider, "warren_hero");
	badge_service_->GiveBadge(collider, "event_coa2_rebel");
	badge_service_->GiveBadge(collider, "event_coa2_imperial");
	badge_service_->GiveBadge(collider, "combat_1hsword_master");
	badge_service_->GiveBadge(collider, "combat_2hsword_master");
	badge_service_->GiveBadge(collider, "combat_bountyhunter_master");
	badge_service_->GiveBadge(collider, "combat_brawler_master");
	badge_service_->GiveBadge(collider, "combat_carbine_master");
	badge_service_->GiveBadge(collider, "combat_commando_master");
	badge_service_->GiveBadge(collider, "combat_marksman_master");
	badge_service_->GiveBadge(collider, "combat_pistol_master");
	badge_service_->GiveBadge(collider, "combat_polearm_master");
	badge_service_->GiveBadge(collider, "combat_rifleman_master");
	badge_service_->GiveBadge(collider, "combat_smuggler_master");
	badge_service_->GiveBadge(collider, "combat_unarmed_master");
	badge_service_->GiveBadge(collider, "crafting_architect_master");
	badge_service_->GiveBadge(collider, "crafting_armorsmith_master");
	badge_service_->GiveBadge(collider, "crafting_artisan_master");
	badge_service_->GiveBadge(collider, "crafting_chef_master");
	badge_service_->GiveBadge(collider, "crafting_droidengineer_master");
	badge_service_->GiveBadge(collider, "crafting_merchant_master");
	badge_service_->GiveBadge(collider, "crafting_tailor_master");
	badge_service_->GiveBadge(collider, "crafting_weaponsmith_master");
	badge_service_->GiveBadge(collider, "outdoors_bio_engineer_master");
	badge_service_->GiveBadge(collider, "outdoors_creaturehandler_master");
	badge_service_->GiveBadge(collider, "outdoors_ranger_master");
	badge_service_->GiveBadge(collider, "outdoors_scout_master");
	badge_service_->GiveBadge(collider, "outdoors_squadleader_master");
	badge_service_->GiveBadge(collider, "science_combatmedic_master");
	badge_service_->GiveBadge(collider, "science_doctor_master");
	badge_service_->GiveBadge(collider, "science_medic_master");
	badge_service_->GiveBadge(collider, "social_dancer_master");
	badge_service_->GiveBadge(collider, "social_entertainer_master");
	badge_service_->GiveBadge(collider, "social_imagedesigner_master");
	badge_service_->GiveBadge(collider, "social_musician_master");
	badge_service_->GiveBadge(collider, "social_politician_master");
	badge_service_->GiveBadge(collider, "bdg_exp_nab_theed_falls_bottom");
	badge_service_->GiveBadge(collider, "bdg_exp_nab_deeja_falls_top");
	badge_service_->GiveBadge(collider, "bdg_exp_nab_amidalas_sandy_beach");
	badge_service_->GiveBadge(collider, "bdg_exp_cor_rebel_hideout");
	badge_service_->GiveBadge(collider, "bdg_exp_cor_rogue_corsec_base");
	badge_service_->GiveBadge(collider, "bdg_exp_cor_tyrena_theater");
	badge_service_->GiveBadge(collider, "bdg_exp_cor_bela_vistal_fountain");
	badge_service_->GiveBadge(collider, "bdg_exp_dat_crashed_ship");
	badge_service_->GiveBadge(collider, "bdg_exp_dat_imp_prison");
	badge_service_->GiveBadge(collider, "bdg_exp_dan_dantari_village1");
	badge_service_->GiveBadge(collider, "bdg_exp_dan_dantari_village2");
	badge_service_->GiveBadge(collider, "bdg_exp_end_ewok_tree_village");
	badge_service_->GiveBadge(collider, "bdg_exp_end_ewok_lake_village");
	badge_service_->GiveBadge(collider, "bdg_exp_end_dulok_village");
	badge_service_->GiveBadge(collider, "bdg_exp_end_imp_outpost");
	badge_service_->GiveBadge(collider, "bdg_exp_tal_creature_village");
	badge_service_->GiveBadge(collider, "bdg_exp_tal_imp_base");
	badge_service_->GiveBadge(collider, "bdg_exp_tal_imp_vs_reb_battle");
	badge_service_->GiveBadge(collider, "bdg_exp_tal_aqualish_cave");
	badge_service_->GiveBadge(collider, "bdg_exp_ror_kobala_spice_mine");
	badge_service_->GiveBadge(collider, "bdg_exp_ror_rebel_outpost");
	badge_service_->GiveBadge(collider, "bdg_exp_ror_imp_camp");
	badge_service_->GiveBadge(collider, "bdg_exp_ror_imp_hyperdrive_fac");
	badge_service_->GiveBadge(collider, "bdg_exp_lok_imp_outpost");
	badge_service_->GiveBadge(collider, "bdg_exp_lok_kimogila_skeleton");
	badge_service_->GiveBadge(collider, "bdg_thm_park_jabba_badge");
	badge_service_->GiveBadge(collider, "bdg_thm_park_imperial_badge");
	badge_service_->GiveBadge(collider, "bdg_thm_park_rebel_badge");
	badge_service_->GiveBadge(collider, "bdg_thm_park_nym_badge");
	badge_service_->GiveBadge(collider, "event_coa3_rebel");
	badge_service_->GiveBadge(collider, "event_coa3_imperial");
	badge_service_->GiveBadge(collider, "bdg_library_trivia");
	badge_service_->GiveBadge(collider, "bdg_corvette_imp_destroy");
	badge_service_->GiveBadge(collider, "bdg_corvette_imp_rescue");
	badge_service_->GiveBadge(collider, "bdg_corvette_imp_assassin");
	badge_service_->GiveBadge(collider, "bdg_corvette_neutral_destroy");
	badge_service_->GiveBadge(collider, "bdg_corvette_neutral_rescue");
	badge_service_->GiveBadge(collider, "bdg_corvette_neutral_assassin");
	badge_service_->GiveBadge(collider, "bdg_corvette_reb_destroy");
	badge_service_->GiveBadge(collider, "bdg_corvette_reb_rescue");
	badge_service_->GiveBadge(collider, "bdg_corvette_reb_assassin");
	badge_service_->GiveBadge(collider, "bdg_racing_agrilat_swamp");
	badge_service_->GiveBadge(collider, "bdg_racing_keren_city");
	badge_service_->GiveBadge(collider, "bdg_racing_mos_espa");
	badge_service_->GiveBadge(collider, "bdg_accolade_live_event");
	badge_service_->GiveBadge(collider, "bdg_racing_lok_marathon");
	badge_service_->GiveBadge(collider, "bdg_racing_narmle_memorial");
	badge_service_->GiveBadge(collider, "bdg_racing_nashal_river");
	badge_service_->GiveBadge(collider, "destroy_deathstar");
	badge_service_->GiveBadge(collider, "crafting_shipwright");
	badge_service_->GiveBadge(collider, "pilot_rebel_navy_naboo");
	badge_service_->GiveBadge(collider, "pilot_rebel_navy_corellia");
	badge_service_->GiveBadge(collider, "pilot_rebel_navy_tatooine");
	badge_service_->GiveBadge(collider, "pilot_imperial_navy_naboo");
	badge_service_->GiveBadge(collider, "pilot_imperial_navy_corellia");
	badge_service_->GiveBadge(collider, "pilot_imperial_navy_tatooine");
	badge_service_->GiveBadge(collider, "pilot_neutral_naboo");
	badge_service_->GiveBadge(collider, "pilot_neutral_corellia");
	badge_service_->GiveBadge(collider, "pilot_neutral_tatooine");
	badge_service_->GiveBadge(collider, "bdg_accolade_home_show");
	
}
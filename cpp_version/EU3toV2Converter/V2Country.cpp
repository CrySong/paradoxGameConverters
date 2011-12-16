#include "V2Country.h"
#include "tempFuncs.h"
#include "Log.h"
#include <algorithm>
#include <math.h>
#include <float.h>


void V2Country::init(string newTag, string newCountryFile, vector<int> newParties)
{
	tag			= newTag;
	countryFile	= newCountryFile;
	parties		= newParties;

	for (unsigned int i = 0; i < naval_exercises; i++)
	{
		inventions[i] = illegal;
	}

	plurality = 0.0;
}


void V2Country::setSourceCountryIndex(int oldCountryIndex)
{
	sourceCountryIndex	= oldCountryIndex;
}


string V2Country::getTag() const
{
	return tag;
}


void V2Country::addState(V2State newState)
{
	states.push_back(newState);
}


void V2Country::setCapital(int newCapital)
{
	capital = newCapital;
}


void V2Country::setCivilized(bool isIt)
{
	civilized = isIt;
}


bool V2Country::isCivilized()
{
	return civilized;
}


void V2Country::setPrimaryCulture(string culture)
{
	primaryCulture = culture;
}


string V2Country::getPrimaryCulture() const
{
	return primaryCulture;
}


void V2Country::addAcceptedCulture(string culture)
{
	acceptedCultures.push_back(culture);
}


void V2Country::setReligion(string newReligion)
{
	religion = newReligion;
}


string V2Country::getReligion()
{
	return religion;
}


void V2Country::sortRelations(const vector<string>& order)
{
	vector<V2Relations> sortedRelations;
	for (vector<string>::const_iterator oitr = order.begin(); oitr != order.end(); ++oitr)
	{
		for (vector<V2Relations>::iterator itr = relations.begin(); itr != relations.end(); ++itr)
		{
			if (itr->getTag() == (*oitr))
			{
				sortedRelations.push_back(*itr);
				break;
			}
		}
	}
	relations.swap(sortedRelations);
}


void V2Country::output(FILE* output)
{
	fprintf(output, "%s=\n", tag.c_str());
	fprintf(output, "	{\n");
	if (capital > 0)
	{
		fprintf(output, "	capital=%d\n", capital);
	}
	outputTech(output);
	reforms.output(output);
	outputCountryHeader(output);
	fprintf(output, "	ruling_party=%d\n", parties[0]);
	for (unsigned int i = 0; i < parties.size(); i++)
	{
		fprintf(output, "	active_party=%d\n", parties[i]);
	}
	fprintf(output, "	diplomatic_points=%f\n", diploPoints);
	fprintf(output, "	religion=\"%s\"\n", religion.c_str());
	fprintf(output, "	government=%s\n", government.c_str());
	fprintf(output, "	plurality=%f\n", plurality);
	outputCountryHeader2(output);
	for (vector<V2Leader>::iterator itr = leaders.begin(); itr != leaders.end(); ++itr)
	{
		itr->output(output);
	}
	for (vector<V2Army>::iterator itr = armies.begin(); itr != armies.end(); ++itr)
	{
		itr->output(output);
	}
	for (vector<V2Relations>::iterator itr = relations.begin(); itr != relations.end(); ++itr)
	{
		itr->output(output);
	}
	outputInventions(output);
	if (primaryCulture.size() > 0)
	{
		fprintf(output, "	primary_culture=\"%s\"\n", primaryCulture.c_str());
	}
	if (acceptedCultures.size() > 0)
	{
		fprintf(output, "	culture=\n");
		fprintf(output, "	{\n");
		for(unsigned int i = 0; i < acceptedCultures.size(); i++)
		{
			fprintf(output, "		\"%s\"\n", acceptedCultures[i].c_str());
		}
		fprintf(output, "	}\n");
	}
	fprintf(output, "	prestige=%f\n", prestige);
	fprintf(output, "	bank=\n");
	fprintf(output, "	{\n");
	fprintf(output, "		money=%f\n", bankReserves);
	fprintf(output, "		money_lent=0.00000\n");
	fprintf(output, "	}\n");
	fprintf(output, "	money=%f\n", money);
	fprintf(output, "	last_bankrupt=\"%s\"\n", lastBankrupt.toString().c_str());
	for (vector<V2Creditor>::iterator itr = creditors.begin(); itr != creditors.end(); ++itr)
	{
		itr->output(output);
	}
	fprintf(output, "	nationalvalue=\"%s\"\n", nationalValue.c_str());
	if (civilized)
	{
		fprintf(output, "	civilized=yes\n");
	}
	else
	{
		fprintf(output, "	civilized=no\n");
	}
	for(unsigned int i = 0; i < states.size(); i++)
	{
		states[i].output(output);
	}
	fprintf(output, "	badboy=%f\n", badboy);
	fprintf(output, "}\n");
}


int V2Country::getSourceCountryIndex()
{
	return sourceCountryIndex;
}


void V2Country::setPrestige(double newPrestige)
{
	prestige = newPrestige;
}


void V2Country::setArmyTech(double newTechLevel)
{
	if (newTechLevel > 0)
	{
		if (newTechLevel >= 0)
		{
			techs.push_back("flintlock_rifles");
		}
		if (newTechLevel >= 1)
		{
			techs.push_back("military_staff_system");
		}
		if (newTechLevel >= 2)
		{
			techs.push_back("post_napoleonic_thought");
		}
		if (newTechLevel >= 3)
		{
			techs.push_back("bronze_muzzle_loaded_artillery");
		}
		if (newTechLevel >= 4)
		{
			techs.push_back("army_command_principle");
		}
		if (newTechLevel >= 5)
		{
			techs.push_back("muzzle_loaded_rifles");
		}
		if (newTechLevel >= 6)
		{
			techs.push_back("iron_muzzle_loaded_artillery");
		}
		if (newTechLevel >= 7)
		{
			techs.push_back("military_plans");
		}
		if (newTechLevel >= 8)
		{
			techs.push_back("strategic_mobility");
		}
		if (newTechLevel >= 9)
		{
			techs.push_back("army_professionalism");
		}
	}
	
	if (newTechLevel > 7)
	{
		inventions[telegraph_coordination]		= possible;
		inventions[mobilization_time_tables]	= possible;
		inventions[personnel_category_tables]	= possible;
	}

	if (newTechLevel > 7.25)
	{
		inventions[telegraph_coordination]		= active;
		inventions[mobilization_time_tables]	= possible;
		inventions[personnel_category_tables]	= possible;
	}

	if (newTechLevel > 7.5)
	{
		inventions[mobilization_time_tables]	= active;
		inventions[personnel_category_tables]	= possible;
	}

	if (newTechLevel > 7.75)
	{
		inventions[personnel_category_tables]	= active;
	}

	if (newTechLevel > 8)
	{
		inventions[field_fortifications]			= possible;
		inventions[railroad_transport_groups]	= possible;
		inventions[signal_detachments]			= possible;
	}

	if (newTechLevel > 8.25)
	{
		inventions[field_fortifications]			= active;
		inventions[railroad_transport_groups]	= possible;
		inventions[signal_detachments]			= possible;
	}

	if (newTechLevel > 8.5)
	{
		inventions[railroad_transport_groups]	= active;
		inventions[signal_detachments]			= possible;
	}

	if (newTechLevel > 8.75)
	{
		inventions[signal_detachments]			= active;
	}

	if (newTechLevel > 9)
	{
		inventions[army_academic_training]	= possible;
		inventions[field_training]				= possible;
		inventions[army_societal_status]		= possible;
	}
	
	if (newTechLevel > 9.25)
	{
		inventions[army_academic_training]	= active;
		inventions[field_training]				= possible;
		inventions[army_societal_status]		= possible;
	}
	
	if (newTechLevel > 9.5)
	{
		inventions[field_training]				= active;
		inventions[army_societal_status]		= possible;
	}
	
	if (newTechLevel > 9.75)
	{
		inventions[army_societal_status]		= active;
	}
}


void V2Country::setNavyTech(double newTechLevel)
{
	if (newTechLevel > 0)
	{
		if (newTechLevel >= 0)
		{
			techs.push_back("clipper_design");
		}
		if (newTechLevel >= 1)
		{
			techs.push_back("naval_design_bureaus");
		}
		if (newTechLevel >= 2)
		{
			techs.push_back("post_nelsonian_thought");
		}
		if (newTechLevel >= 3)
		{
			techs.push_back("alphabetic_flag_signaling");
		}
		if (newTechLevel >= 4)
		{
			techs.push_back("the_command_principle");
		}
		if (newTechLevel >= 5)
		{
			techs.push_back("steamers");
		}
		if (newTechLevel >= 6)
		{
			techs.push_back("fire_control_systems");
		}
		if (newTechLevel >= 7)
		{
			techs.push_back("naval_plans");
		}
		if (newTechLevel >= 8)
		{
			techs.push_back("naval_professionalism");
		}
		if (newTechLevel >= 9)
		{
			techs.push_back("battleship_column_doctrine");
		}
	}

	if (newTechLevel > 0)
	{
		inventions[building_station_shipyards]	= active;
	}
	
	if (newTechLevel > 5)
	{
		inventions[mechanized_fishing_vessels]					= possible;
		inventions[steamer_automatic_construction_plants]	= possible;
		inventions[steamer_transports]							= possible;
		inventions[commerce_raiders]								= possible;
	}

	if (newTechLevel > 5.2)
	{
		inventions[mechanized_fishing_vessels]					= active;
		inventions[steamer_automatic_construction_plants]	= possible;
		inventions[steamer_transports]							= possible;
		inventions[commerce_raiders]								= possible;
	}
	
	if (newTechLevel > 5.4)
	{
		inventions[steamer_automatic_construction_plants]	= active;
		inventions[steamer_transports]							= possible;
		inventions[commerce_raiders]								= possible;
	}
	
	if (newTechLevel > 5.6)
	{
		inventions[steamer_transports]							= active;
		inventions[commerce_raiders]								= possible;
	}
	
	if (newTechLevel > 5.8)
	{
		inventions[commerce_raiders]								= active;
	}

	if (newTechLevel > 6)
	{
		inventions[optical_rangefinders]							= possible;
		inventions[gyrostabilized_fire_control]				= possible;
		inventions[armour_piercing_projectiles]				= possible;
		inventions[armour_piercing_exploding_projectiles]	= possible;
		inventions[armoured_turrets]								= possible;
	}

	if (newTechLevel > 6 + (1/6))
	{
		inventions[optical_rangefinders]							= active;
		inventions[gyrostabilized_fire_control]				= possible;
		inventions[armour_piercing_projectiles]				= possible;
		inventions[armour_piercing_exploding_projectiles]	= possible;
		inventions[armoured_turrets]								= possible;
	}
	
	if (newTechLevel > 6 + (2/6))
	{
		inventions[gyrostabilized_fire_control]				= active;
		inventions[armour_piercing_projectiles]				= possible;
		inventions[armour_piercing_exploding_projectiles]	= possible;
		inventions[armoured_turrets]								= possible;
	}
	
	if (newTechLevel > 6 + (3/6))
	{
		inventions[armour_piercing_projectiles]				= active;
		inventions[armour_piercing_exploding_projectiles]	= possible;
		inventions[armoured_turrets]								= possible;
	}
	
	if (newTechLevel > 6 + (4/6))
	{
		inventions[armour_piercing_exploding_projectiles]	= active;
		inventions[armoured_turrets]								= possible;
	}
	
	if (newTechLevel > 6 + (5/6))
	{
		inventions[armoured_turrets]								= active;
	}
	
	if (newTechLevel > 7)
	{
		inventions[night_training]						= possible;
		inventions[enemy_plotting_cards]				= possible;
		inventions[target_hit_profile_analysis]	= possible;
	}

	if (newTechLevel > 7.25)
	{
		inventions[night_training]						= active;
		inventions[enemy_plotting_cards]				= possible;
		inventions[target_hit_profile_analysis]	= possible;
	}
	
	if (newTechLevel > 7.5)
	{
		inventions[enemy_plotting_cards]				= active;
		inventions[target_hit_profile_analysis]	= possible;
	}
	
	if (newTechLevel > 7.75)
	{
		inventions[target_hit_profile_analysis]	= active;
	}
	
	if (newTechLevel > 8)
	{
		inventions[academic_training]			= possible;
		inventions[combat_station_training]	= possible;
		inventions[societal_status]			= possible;
	}

	if (newTechLevel > 8.25)
	{
		inventions[academic_training]			= active;
		inventions[combat_station_training]	= possible;
		inventions[societal_status]			= possible;
	}
	
	if (newTechLevel > 8.5)
	{
		inventions[combat_station_training]	= active;
		inventions[societal_status]			= possible;
	}
	
	if (newTechLevel > 8.75)
	{
		inventions[societal_status]			= active;
	}
	
	if (newTechLevel > 9)
	{
		inventions[long_range_fire_tactic]		= possible;
		inventions[speedy_maneuvering_tactic]	= possible;
	}

	if (newTechLevel > 9 + (1/3))
	{
		inventions[long_range_fire_tactic]		= active;
		inventions[speedy_maneuvering_tactic]	= possible;
	}
	
	if (newTechLevel > 9 + (2/3))
	{
		inventions[speedy_maneuvering_tactic]	= active;
	}
	}


void V2Country::setCommerceTech(double newTechLevel)
{
	if (newTechLevel > 0)
	{
		if (newTechLevel >= 0)
		{
			techs.push_back("no_standard");
		}
		if (newTechLevel >= 1)
		{
			techs.push_back("guild_based_production");
		}
		if (newTechLevel >= 2)
		{
			techs.push_back("freedom_of_trade");
		}
		if (newTechLevel >= 3)
		{
			techs.push_back("private_banks");
		}
		if (newTechLevel >= 4)
		{
			techs.push_back("early_classical_theory_and_critique");
		}
		if (newTechLevel >= 5)
		{
			techs.push_back("ad_hoc_money_bill_printing");
		}
		if (newTechLevel >= 6)
		{
			techs.push_back("market_structure");
		}
		if (newTechLevel >= 7)
		{
			techs.push_back("organized_factories");
		}
		if (newTechLevel >= 8)
		{
			techs.push_back("late_classical_theory");
		}
		if (newTechLevel >= 9)
		{
			techs.push_back("stock_exchange");
		}
	}

	if (newTechLevel > 4)
	{
		inventions[john_ramsay_mcculloch]	= possible;
		inventions[nassau_william_sr]			= possible;
		inventions[james_mill]					= possible;
	}
	
	if (newTechLevel > 4.25)
	{
		inventions[john_ramsay_mcculloch]	= active;
		inventions[nassau_william_sr]			= possible;
		inventions[james_mill]					= possible;
	}
	
	if (newTechLevel > 4.5)
	{
		inventions[nassau_william_sr]			= active;
		inventions[james_mill]					= possible;
	}
	
	if (newTechLevel > 4.75)
	{
		inventions[james_mill]					= active;
	}

	if (newTechLevel > 5)
	{
		inventions[silver_standard]			= possible;
		inventions[decimal_monetary_system]	= possible;
	}
	
	if (newTechLevel > 5 + (1/3))
	{
		inventions[silver_standard]			= active;
		inventions[decimal_monetary_system]	= possible;
	}
	
	if (newTechLevel > 5 + (2/3))
	{
		inventions[decimal_monetary_system]	= active;
	}

	if (newTechLevel > 6)
	{
		inventions[polypoly_structure]	= possible;
		inventions[oligopoly_structure]	= possible;
		inventions[monopoly_structure]	= possible;
	}
	
	if (newTechLevel > 6.25)
	{
		inventions[polypoly_structure]	= active;
		inventions[oligopoly_structure]	= possible;
		inventions[monopoly_structure]	= possible;
	}
	
	if (newTechLevel > 6.5)
	{
		inventions[oligopoly_structure]	= active;
		inventions[monopoly_structure]	= possible;
	}
	
	if (newTechLevel > 6.75)
	{
		inventions[monopoly_structure]	= active;
	}

	if (newTechLevel > 7)
	{
		inventions[work_classification]			= possible;
		inventions[work_specialization]			= possible;
		inventions[work_control_and_hierarchy]	= possible;
	}
	
	if (newTechLevel > 7.25)
	{
		inventions[work_classification]			= active;
		inventions[work_specialization]			= possible;
		inventions[work_control_and_hierarchy]	= possible;
	}
	
	if (newTechLevel > 7.5)
	{
		inventions[work_specialization]			= active;
		inventions[work_control_and_hierarchy]	= possible;
	}
	
	if (newTechLevel > 7.75)
	{
		inventions[work_control_and_hierarchy]	= active;
	}

	if (newTechLevel > 8)
	{
		inventions[john_elliot_cairnes]	= possible;
		inventions[robert_torrens]			= possible;
		inventions[john_stuart_mill]		= possible;
	}
	
	if (newTechLevel > 8.25)
	{
		inventions[john_elliot_cairnes]	= active;
		inventions[robert_torrens]			= possible;
		inventions[john_stuart_mill]		= possible;
	}
	
	if (newTechLevel > 8.5)
	{
		inventions[robert_torrens]			= active;
		inventions[john_stuart_mill]		= possible;
	}
	
	if (newTechLevel > 8.75)
	{
		inventions[john_stuart_mill]		= active;
	}

	if (newTechLevel > 9)
	{
		inventions[multitude_of_financial_instruments]		= possible;
		inventions[insurance_companies]							= possible;
		inventions[regulated_buying_and_selling_of_stocks]	= possible;
	}
	
	if (newTechLevel > 9.25)
	{
		inventions[multitude_of_financial_instruments]		= active;
		inventions[insurance_companies]							= possible;
		inventions[regulated_buying_and_selling_of_stocks]	= possible;
	}
	
	if (newTechLevel > 9.5)
	{
		inventions[insurance_companies]							= active;
		inventions[regulated_buying_and_selling_of_stocks]	= possible;
	}
	
	if (newTechLevel > 9.75)
	{
		inventions[regulated_buying_and_selling_of_stocks]	= active;
	}
}


void V2Country::setIndustryTech(double newTechLevel)
{
	if (newTechLevel > 0)
	{
		if (newTechLevel >= 0)
		{
			techs.push_back("water_wheel_power");
		}
		if (newTechLevel >= 1)
		{
			techs.push_back("basic_chemistry");
		}
		if (newTechLevel >= 2)
		{
			techs.push_back("publishing_industry");
		}
		if (newTechLevel >= 3)
		{
			techs.push_back("experimental_railroad");
		}
		if (newTechLevel >= 4)
		{
			techs.push_back("mechanized_mining");
		}
		if (newTechLevel >= 5)
		{
			techs.push_back("clean_coal");
		}
		if (newTechLevel >= 6)
		{
			techs.push_back("medicine");
		}
		if (newTechLevel >= 7)
		{
			techs.push_back("practical_steam_engine");
		}
		if (newTechLevel >= 8)
		{
			techs.push_back("mechanical_production");
		}
	}

	if (newTechLevel > 4)
	{
		inventions[ammunition_production]	= possible;
		inventions[small_arms_production]	= possible;
		inventions[explosives_production]	= possible;
		inventions[artillery_production]		= possible;
	}
	
	if (newTechLevel > 4.2)
	{
		inventions[ammunition_production]	= active;
		inventions[small_arms_production]	= possible;
		inventions[explosives_production]	= possible;
		inventions[artillery_production]		= possible;
	}
	
	if (newTechLevel > 4.4)
	{
		inventions[small_arms_production]	= active;
		inventions[explosives_production]	= possible;
		inventions[artillery_production]		= possible;
	}
	
	if (newTechLevel > 4.6)
	{
		inventions[explosives_production]	= active;
		inventions[artillery_production]		= possible;
	}
	
	if (newTechLevel > 4.8)
	{
		inventions[artillery_production]		= active;
	}

	if (newTechLevel > 5)
	{
		inventions[pit_coal]	= possible;
		inventions[coke]		= possible;
	}
	
	if (newTechLevel > 5 + (1/3))
	{
		inventions[pit_coal]	= active;
		inventions[coke]		= possible;
	}
	
	if (newTechLevel > 5 + (2/3))
	{
		inventions[coke]		= active;
	}

	if (newTechLevel > 6)
	{
		inventions[combat_medicine]											= possible;
		inventions[aerial_bacteria_and_antiseptic_principle]			= possible;
		inventions[vaccination]													= possible;
		inventions[chemotherapy]												= possible;
		inventions[genetics_heredity]											= possible;
		inventions[clinical_thermometers_and_binaural_stethoscopes]	= possible;
		inventions[prophylaxis_against_malaria]							= possible;
		inventions[pressure_chambers_for_thorax_surgery]				= possible;
	}
	
	if (newTechLevel > 6 + (1/9))
	{
		inventions[combat_medicine]											= active;
		inventions[aerial_bacteria_and_antiseptic_principle]			= possible;
		inventions[vaccination]													= possible;
		inventions[chemotherapy]												= possible;
		inventions[genetics_heredity]											= possible;
		inventions[clinical_thermometers_and_binaural_stethoscopes]	= possible;
		inventions[prophylaxis_against_malaria]							= possible;
		inventions[pressure_chambers_for_thorax_surgery]				= possible;
	}
	
	if (newTechLevel > 6 + (2/9))
	{
		inventions[aerial_bacteria_and_antiseptic_principle]			= active;
		inventions[vaccination]													= possible;
		inventions[chemotherapy]												= possible;
		inventions[genetics_heredity]											= possible;
		inventions[clinical_thermometers_and_binaural_stethoscopes]	= possible;
		inventions[prophylaxis_against_malaria]							= possible;
		inventions[pressure_chambers_for_thorax_surgery]				= possible;
	}
	
	if (newTechLevel > 6 + (3/9))
	{
		inventions[vaccination]													= active;
		inventions[chemotherapy]												= possible;
		inventions[genetics_heredity]											= possible;
		inventions[clinical_thermometers_and_binaural_stethoscopes]	= possible;
		inventions[prophylaxis_against_malaria]							= possible;
		inventions[pressure_chambers_for_thorax_surgery]				= possible;
	}
	
	if (newTechLevel > 6 + (4/9))
	{
		inventions[chemotherapy]												= active;
		inventions[genetics_heredity]											= possible;
		inventions[clinical_thermometers_and_binaural_stethoscopes]	= possible;
		inventions[prophylaxis_against_malaria]							= possible;
		inventions[pressure_chambers_for_thorax_surgery]				= possible;
	}
	
	if (newTechLevel > 6 + (5/9))
	{
		inventions[genetics_heredity]											= active;
		inventions[clinical_thermometers_and_binaural_stethoscopes]	= possible;
		inventions[prophylaxis_against_malaria]							= possible;
		inventions[pressure_chambers_for_thorax_surgery]				= possible;
	}
	
	if (newTechLevel > 6 + (6/9))
	{
		inventions[clinical_thermometers_and_binaural_stethoscopes]	= active;
		inventions[prophylaxis_against_malaria]							= possible;
		inventions[pressure_chambers_for_thorax_surgery]				= possible;
	}
	
	if (newTechLevel > 6 + (7/9))
	{
		inventions[prophylaxis_against_malaria]							= active;
		inventions[pressure_chambers_for_thorax_surgery]				= possible;
	}
	
	if (newTechLevel > 6 + (8/9))
	{
		inventions[pressure_chambers_for_thorax_surgery]				= active;
	}

	if (newTechLevel > 8)
	{
		inventions[sharp_n_roberts_power_loom]				= possible;
		inventions[jacquard_power_loom]						= possible;
		inventions[northrop_power_loom]						= possible;
		inventions[mechanical_saw]								= possible;
		inventions[mechanical_precision_saw]				= possible;
		inventions[hussey_n_mccormicks_reaping_machine]	= possible;
		inventions[pitts_threshing_machine]					= possible;
		inventions[mechanized_slaughtering_block]			= possible;
	}
	
	if (newTechLevel > 8 + (1/9))
	{
		inventions[sharp_n_roberts_power_loom]				= active;
		inventions[jacquard_power_loom]						= possible;
		inventions[northrop_power_loom]						= possible;
		inventions[mechanical_saw]								= possible;
		inventions[mechanical_precision_saw]				= possible;
		inventions[hussey_n_mccormicks_reaping_machine]	= possible;
		inventions[pitts_threshing_machine]					= possible;
		inventions[mechanized_slaughtering_block]			= possible;
	}
	
	if (newTechLevel > 8 + (2/9))
	{
		inventions[jacquard_power_loom]						= active;
		inventions[northrop_power_loom]						= possible;
		inventions[mechanical_saw]								= possible;
		inventions[mechanical_precision_saw]				= possible;
		inventions[hussey_n_mccormicks_reaping_machine]	= possible;
		inventions[pitts_threshing_machine]					= possible;
		inventions[mechanized_slaughtering_block]			= possible;
	}
	
	if (newTechLevel > 8 + (3/9))
	{
		inventions[northrop_power_loom]						= active;
		inventions[mechanical_saw]								= possible;
		inventions[mechanical_precision_saw]				= possible;
		inventions[hussey_n_mccormicks_reaping_machine]	= possible;
		inventions[pitts_threshing_machine]					= possible;
		inventions[mechanized_slaughtering_block]			= possible;
	}
	
	if (newTechLevel > 8 + (4/9))
	{
		inventions[mechanical_saw]								= active;
		inventions[mechanical_precision_saw]				= possible;
		inventions[hussey_n_mccormicks_reaping_machine]	= possible;
		inventions[pitts_threshing_machine]					= possible;
		inventions[mechanized_slaughtering_block]			= possible;
	}
	
	if (newTechLevel > 8 + (5/9))
	{
		inventions[mechanical_precision_saw]				= active;
		inventions[hussey_n_mccormicks_reaping_machine]	= possible;
		inventions[pitts_threshing_machine]					= possible;
		inventions[mechanized_slaughtering_block]			= possible;
	}
	
	if (newTechLevel > 8 + (6/9))
	{
		inventions[hussey_n_mccormicks_reaping_machine]	= active;
		inventions[pitts_threshing_machine]					= possible;
		inventions[mechanized_slaughtering_block]			= possible;
	}
	
	if (newTechLevel > 8 + (7/9))
	{
		inventions[pitts_threshing_machine]					= active;
		inventions[mechanized_slaughtering_block]			= possible;
	}
	
	if (newTechLevel > 8 + (8/9))
	{
		inventions[mechanized_slaughtering_block]			= active;
	}
}


void V2Country::setCultureTech(double newTechLevel)
{
	if (newTechLevel > 0)
	{
		if (newTechLevel >= 0)
		{
			techs.push_back("malthusian_thought");
		}
		if (newTechLevel >= 1)
		{
			techs.push_back("late_enlightenment_philosophy");
		}
		if (newTechLevel >= 2)
		{
			techs.push_back("enlightenment_thought");
		}
		if (newTechLevel >= 3)
		{
			techs.push_back("classicism_n_early_romanticism");
		}
		if (newTechLevel >= 4)
		{
			techs.push_back("introspectionism");
		}
		if (newTechLevel >= 5)
		{
			techs.push_back("positivism");
		}
		if (newTechLevel >= 6)
		{
			techs.push_back("associationism");
		}
		if (newTechLevel >= 7)
		{
			techs.push_back("romanticism");
		}
		if (newTechLevel >= 8)
		{
			techs.push_back("ideological_thought");
		}
	}

	if (newTechLevel > 7)
	{
		inventions[romanticist_literature]	= possible;
		inventions[romanticist_art]			= possible;
		inventions[romanticist_music]			= possible;
	}
	
	if (newTechLevel > 7.25)
	{
		inventions[romanticist_literature]	= active;
		inventions[romanticist_art]			= possible;
		inventions[romanticist_music]			= possible;
	}
	
	if (newTechLevel > 7.5)
	{
		inventions[romanticist_art]			= active;
		inventions[romanticist_music]			= possible;
	}
	
	if (newTechLevel > 7.75)
	{
		inventions[romanticist_music]			= active;
	}

	if (newTechLevel > 8)
	{
		inventions[authoritarianism]		= possible;
		inventions[hierarchical_order]	= possible;
		inventions[traditionalism]			= possible;
		inventions[political_religion]	= possible;
		inventions[paternalism]				= possible;
		inventions[constitutionalism]		= possible;
		inventions[individualism]			= possible;
		inventions[rationalism]				= possible;
		inventions[secularization]			= possible;
		inventions[social_conscience]		= possible;
		inventions[egalitarianism]			= possible;
		inventions[collectivism]			= possible;
		inventions[determinism]				= possible;
		inventions[atheism]					= possible;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.0625)
	{
		inventions[authoritarianism]		= active;
		inventions[hierarchical_order]	= possible;
		inventions[traditionalism]			= possible;
		inventions[political_religion]	= possible;
		inventions[paternalism]				= possible;
		inventions[constitutionalism]		= possible;
		inventions[individualism]			= possible;
		inventions[rationalism]				= possible;
		inventions[secularization]			= possible;
		inventions[social_conscience]		= possible;
		inventions[egalitarianism]			= possible;
		inventions[collectivism]			= possible;
		inventions[determinism]				= possible;
		inventions[atheism]					= possible;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.125)
	{
		inventions[hierarchical_order]	= active;
		inventions[traditionalism]			= possible;
		inventions[political_religion]	= possible;
		inventions[paternalism]				= possible;
		inventions[constitutionalism]		= possible;
		inventions[individualism]			= possible;
		inventions[rationalism]				= possible;
		inventions[secularization]			= possible;
		inventions[social_conscience]		= possible;
		inventions[egalitarianism]			= possible;
		inventions[collectivism]			= possible;
		inventions[determinism]				= possible;
		inventions[atheism]					= possible;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.1875)
	{
		inventions[traditionalism]			= active;
		inventions[political_religion]	= possible;
		inventions[paternalism]				= possible;
		inventions[constitutionalism]		= possible;
		inventions[individualism]			= possible;
		inventions[rationalism]				= possible;
		inventions[secularization]			= possible;
		inventions[social_conscience]		= possible;
		inventions[egalitarianism]			= possible;
		inventions[collectivism]			= possible;
		inventions[determinism]				= possible;
		inventions[atheism]					= possible;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.25)
	{
		inventions[political_religion]	= active;
		inventions[paternalism]				= possible;
		inventions[constitutionalism]		= possible;
		inventions[individualism]			= possible;
		inventions[rationalism]				= possible;
		inventions[secularization]			= possible;
		inventions[social_conscience]		= possible;
		inventions[egalitarianism]			= possible;
		inventions[collectivism]			= possible;
		inventions[determinism]				= possible;
		inventions[atheism]					= possible;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.3125)
	{
		inventions[paternalism]				= active;
		inventions[constitutionalism]		= possible;
		inventions[individualism]			= possible;
		inventions[rationalism]				= possible;
		inventions[secularization]			= possible;
		inventions[social_conscience]		= possible;
		inventions[egalitarianism]			= possible;
		inventions[collectivism]			= possible;
		inventions[determinism]				= possible;
		inventions[atheism]					= possible;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.375)
	{
		inventions[constitutionalism]		= active;
		inventions[individualism]			= possible;
		inventions[rationalism]				= possible;
		inventions[secularization]			= possible;
		inventions[social_conscience]		= possible;
		inventions[egalitarianism]			= possible;
		inventions[collectivism]			= possible;
		inventions[determinism]				= possible;
		inventions[atheism]					= possible;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.4375)
	{
		inventions[individualism]			= active;
		inventions[rationalism]				= possible;
		inventions[secularization]			= possible;
		inventions[social_conscience]		= possible;
		inventions[egalitarianism]			= possible;
		inventions[collectivism]			= possible;
		inventions[determinism]				= possible;
		inventions[atheism]					= possible;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.5)
	{
		inventions[rationalism]				= active;
		inventions[secularization]			= possible;
		inventions[social_conscience]		= possible;
		inventions[egalitarianism]			= possible;
		inventions[collectivism]			= possible;
		inventions[determinism]				= possible;
		inventions[atheism]					= possible;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.5625)
	{
		inventions[secularization]			= active;
		inventions[social_conscience]		= possible;
		inventions[egalitarianism]			= possible;
		inventions[collectivism]			= possible;
		inventions[determinism]				= possible;
		inventions[atheism]					= possible;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.625)
	{
		inventions[social_conscience]		= active;
		inventions[egalitarianism]			= possible;
		inventions[collectivism]			= possible;
		inventions[determinism]				= possible;
		inventions[atheism]					= possible;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.6875)
	{
		inventions[egalitarianism]			= active;
		inventions[collectivism]			= possible;
		inventions[determinism]				= possible;
		inventions[atheism]					= possible;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.75)
	{
		inventions[collectivism]			= active;
		inventions[determinism]				= possible;
		inventions[atheism]					= possible;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.8125)
	{
		inventions[determinism]				= active;
		inventions[atheism]					= possible;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.875)
	{
		inventions[atheism]					= active;
		inventions[social_justice]			= possible;
	}
	
	if (newTechLevel > 8.93875)
	{
		inventions[social_justice]			= active;
	}
}


void V2Country::outputTech(FILE* output)
{
	fprintf(output, "	technology=\n");
	fprintf(output, "	{\n");

	for (vector<string>::iterator itr = techs.begin(); itr != techs.end(); ++itr)
	{
		fprintf(output, "\t\t"); fprintf(output, itr->c_str()); fprintf(output, "={1 0.000}\n");
	}

	fprintf(output, "	}\n");
}


void V2Country::outputInventions(FILE* output)
{
	fprintf(output, "	active_inventions=\n");
	fprintf(output, "	{\n");
	fprintf(output, "		");
	for (unsigned int i = 0; i < naval_exercises; i++)
	{
		if (inventions[i] == active)
		{
			fprintf(output, "%d ", i + 1);
		}
	}
	fprintf(output, "\n	}\n");

	fprintf(output, "	possible_inventions=\n");
	fprintf(output, "	{\n");
	fprintf(output, "		");
	for (unsigned int i = 0; i < naval_exercises; i++)
	{
		if (inventions[i] == possible)
		{
			fprintf(output, "%d ", i + 1);
		}
	}
	fprintf(output, "\n	}\n");

	fprintf(output, "	illegal_inventions=\n");
	fprintf(output, "	{\n");
	fprintf(output, "		");
	for (unsigned int i = 0; i < naval_exercises; i++)
	{
		if (inventions[i] == illegal)
		{
			fprintf(output, "%d ", i + 1);
		}
	}
	fprintf(output, "\n	}\n");
}


inventionStatus V2Country::getInventionState(inventionType invention)
{
	return inventions[invention];
}


void V2Country::addPrestige(double additionalPrestige)
{
	prestige += additionalPrestige;
}


void V2Country::setGovernment(string newGovernment)
{
	government = newGovernment;
}


void V2Country::addRelations(V2Relations _rel)
{
	relations.push_back(_rel);
}


V2Relations* V2Country::getRelations(string withWhom)
{
	for (size_t i = 0; i < relations.size(); ++i)
	{
		if (relations[i].getTag() == withWhom)
			return &relations[i];
	}
	return NULL;
}


void V2Country::addArmy(V2Army _army)
{
	armies.push_back(_army);
}


// find the army most in need of a regiment of this category
V2Army*	V2Country::getArmyForRemainder(RegimentCategory rc)
{
	V2Army* retval = NULL;
	double retvalRemainder = -1000.0;
	bool isNavy = (rc >= big_ship);
	for (vector<V2Army>::iterator itr = armies.begin(); itr != armies.end(); ++itr)
	{
		// only add units to armies that originally had units of the same category
		if (itr->getSourceArmy()->getTotalTypeStrength(rc) > 0)
		{
			if (itr->getArmyRemainder(rc) > retvalRemainder)
			{
				retvalRemainder = itr->getArmyRemainder(rc);
				retval = &(*itr);
			}
		}
	}
	return retval;
}


void V2Country::setReforms(EU3Country* srcCountry)
{
	reforms.init(srcCountry);
}


static bool FactoryCandidateSortPredicate(const pair<int, V2State*>& lhs, const pair<int, V2State*>& rhs)
{
	if (lhs.first != rhs.first)
		return lhs.first > rhs.first;
	return lhs.second->getID() < rhs.second->getID();
}


bool V2Country::addFactory(V2Factory factory)
{
	// check factory techs
	string requiredTech = factory.getRequiredTech();
	if (requiredTech != "")
	{
		vector<string>::iterator itr = find(techs.begin(), techs.end(), requiredTech);
		if (itr == techs.end())
		{
			log("%s rejected %s (missing reqd tech: %s)\n", tag.c_str(), factory.getTypeName().c_str(), requiredTech.c_str());
			return false;
		}
	}
	
	// check factory inventions
	inventionType requiredInvention = factory.getRequiredInvention();
	if (requiredInvention >= 0 && inventions[requiredInvention] != active)
	{
		log("%s rejected %s (missing reqd invention: %s)\n", tag.c_str(), factory.getTypeName().c_str(), inventionNames[requiredInvention]);
		return false;
	}

	// find a state to add the factory to, which meets the factory's requirements
	vector<pair<int, V2State*>> candidates;
	for (vector<V2State>::iterator itr = states.begin(); itr != states.end(); ++itr)
	{
		if (itr->isColonial())
			continue;

		if (itr->getFactoryCount() >= 8)
			continue;

		if (factory.requiresCoastal())
		{
			if (!itr->isCoastal())
				continue;
		}

		vector<string> requiredProducts = factory.getRequiredRGO();
		if (requiredProducts.size() > 0)
		{
			bool hasInput = false;
			for (vector<string>::iterator prod = requiredProducts.begin(); prod != requiredProducts.end(); ++prod)
			{
				if (itr->hasLocalSupply(*prod))
				{
					hasInput = true;
					break;
				}
			}
			if (!hasInput)
				continue;
		}

		candidates.push_back(pair<int, V2State*>(itr->getCraftsmenPerFactory(), &(*itr)));
	}

	sort(candidates.begin(), candidates.end(), FactoryCandidateSortPredicate);

	if (candidates.size() == 0)
	{
		log("%s rejected %s (no candidate states)\n", tag.c_str(), factory.getTypeName().c_str());
		return false;
	}

	V2State* target = candidates[0].second;
	target->addFactory(factory);
	log("%s accepted %s (%d candidate states)\n", tag.c_str(), factory.getTypeName().c_str(), candidates.size());
	return true;
}


void V2Country::setNationalIdea(EU3Country* srcCountry, int& libertyLeft, int& equalityLeft)
{
	int orderScore = 0;
	orderScore += srcCountry->getOffensiveDefensive();
	orderScore += srcCountry->getInnovativeNarrowminded();
	orderScore += srcCountry->getQualityQuantity();
	vector<string> nationalIdeas = srcCountry->getNationalIdeas();
	for (unsigned int i = 0; i < nationalIdeas.size(); i++)
	{
		if (nationalIdeas[i] == "deus_vult")
		{
			orderScore += 2;
		}
		if (nationalIdeas[i] == "church_attendance_duty")
		{
			orderScore += 2;
		}
		if (nationalIdeas[i] == "divine_supremacy")
		{
			orderScore += 2;
		}
		if (nationalIdeas[i] == "national_conscripts")
		{
			orderScore += 2;
		}
		if (nationalIdeas[i] == "press_gangs")
		{
			orderScore += 1;
		}
		if (nationalIdeas[i] == "military_drill")
		{
			orderScore += 1;
		}
		if (nationalIdeas[i] == "bureaucracy")
		{
			orderScore += 1;
		}
	}

	int libertyScore = 0;
	libertyScore += srcCountry->getCentralizationDecentralization();
	libertyScore += srcCountry->getSerfdomFreesubjects();
	libertyScore += srcCountry->getMercantilismFreetrade();
	for (unsigned int i = 0; i < nationalIdeas.size(); i++)
	{
		if (nationalIdeas[i] == "liberty_egalite_fraternity")
		{
			libertyScore += 4;
		}
		if (nationalIdeas[i] == "smithian_economics")
		{
			libertyScore += 2;
		}
		if (nationalIdeas[i] == "bill_of_rights")
		{
			libertyScore += 2;
		}
		if (nationalIdeas[i] == "scientific_revolution")
		{
			libertyScore += 1;
		}
		if (nationalIdeas[i] == "ecumenism")
		{
			libertyScore += 1;
		}
	}

	int equalityScore = 0;
	equalityScore += srcCountry->getAristocracyPlutocracy();
	equalityScore += srcCountry->getSerfdomFreesubjects();
	for (unsigned int i = 0; i < nationalIdeas.size(); i++)
	{
		if (nationalIdeas[i] == "liberty_egalite_fraternity")
		{
			equalityScore += 4;
		}
		if (nationalIdeas[i] == "humanist_tolerance")
		{
			equalityScore += 2;
		}
		if (nationalIdeas[i] == "bill_of_rights")
		{
			equalityScore += 2;
		}
		if (nationalIdeas[i] == "ecumenism")
		{
			equalityScore += 1;
		}
	}

	if ( (equalityScore > orderScore) && (equalityScore >= libertyScore) && (equalityLeft > 0) )
	{
		nationalValue = "nv_equality";
		equalityLeft--;
	}
	else if ( (libertyScore > orderScore) && (libertyLeft > 0) )
	{
		nationalValue = "nv_liberty";
		libertyLeft--;
	}
	else
	{
		nationalValue = "nv_order";
	}
}


void V2Country::setMoney(double _cash)
{
	money = _cash;
}


void V2Country::setLastBankrupt(date _lastBankrupt)
{
	lastBankrupt = _lastBankrupt;
}


void V2Country::addLoan(string creditor, double size, double interest)
{
	bool found = false;
	for (vector<V2Creditor>::iterator itr = creditors.begin(); itr != creditors.end(); ++itr)
	{
		if (creditor == itr->getCountry())
		{
			itr->addLoan(size, interest);
			found = true;
			break;
		}
	}
	if (!found)
	{
		V2Creditor cred;
		cred.setCountry(creditor);
		cred.addLoan(size, interest);
		creditors.push_back(cred);
	}
}


void V2Country::setBankReserves(double money)
{
	bankReserves = money;
}


void V2Country::setDiploPoints(double points)
{
	diploPoints = points;
}


void V2Country::setBadboy(double _badboy)
{
	badboy = _badboy;
}


void V2Country::addLeader(V2Leader leader)
{
	leaders.push_back(leader);
}


void V2Country::setupPops(EU3World& sourceWorld)
{
	EU3Country oldCountry = sourceWorld.getCountries()[sourceCountryIndex];

	// calculate NATIONAL con and mil modifiers (province modifiers are done in V2Province::setPopConMil)
	double con = 0.0;
	double mil = 0.0;
	// 0 to 3 points of mil from stability, based on a similar discontinuous function to that used by EU3 to calc revolt risk
	double stability = oldCountry.getStability();
	if (stability > -FLT_EPSILON) // positive stability: 3 stab -> 0 mil ==> 0 stab -> 1 mil
		mil += fabs(stability - 3.0) / 3.0;
	else // negative stability: 0 stab -> 1 mil ==> -3 stab -> 3 mil
		mil += fabs(stability) * 2.0 / 3.0 + 1.0;
	// 0 to 2 points of con from serfdom<->free subjects
	double serf_fs = oldCountry.getSerfdomFreesubjects();
	con += (serf_fs * 2.0 / 5.0) + 1.0;
	// TODO: national decisions, national events, war exhaustion

	// create the pops
	for (vector<V2State>::iterator itr = states.begin(); itr != states.end(); ++itr)
	{
		itr->setupPops(sourceWorld, primaryCulture, acceptedCultures, religion, con, mil);
	}
}
void V2Country::setLiteracy(double newLiteracy)
{
	literacy = newLiteracy;
}


double V2Country::getLiteracy()
{
	return literacy;
}

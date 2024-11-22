#include "UpdateBools.h"
#include "Globals.h"
#include "Constants.h"
#include "Logger.h"
#include "Levels/Overworld.h"

void updateBools(const std::shared_ptr<NPC>& NPC) {

	//CHECK IF FIRST QUEST IS COMPLETED
	if (g_RDF_isInvestigationAssigned == false) 
	{
		//check quest completion parameters if current npc is king
		if (NPC->getName() == STRING_KING_NAME) 
		{
			//check if quest is completed, set parameters for completed quest
			if (NPC->mHappiness >= 0.59 && NPC->mHostility <= 0.31)
			{
				LOG_INFO("<<QUEST>> First quest has been completed.");

				//set quest state to finished
				g_RDF_isInvestigationAssigned = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_2_USER_PROMPT;

				//set required rdf strings
				gKing->mRDFDynamicContext = RDF_INVESTIGATION_ASSIGNED_TRUE;
				gInnkeeper->mRDFDynamicContext = RDF_INNKEEPER_TOLD_ABOUT_WOMAN_FALSE;

				//set required emotion attributes
				gInnkeeper->mHappiness = 0.5f;
				gInnkeeper->mTrust = 0.2f;
				gInnkeeper->mHostility = 0.4f;
			}
		}
	}
	//IF PREVIOUS QUEST IS DONE, CHECK FOR SECOND QUEST
	else if (g_RDF_innkeeperToldAboutWoman == false) 
	{
		//check quest completion parameters if current npc is inkeeper
		if (NPC->getName() == STRING_INNKEEPER_NAME)
		{
			//check if quest is completed, set parameters for completed quest
			if (NPC->mTrust >= 0.49)
			{
				LOG_INFO("<<QUEST>> Second quest has been completed.");

				//set quest state to finished
				g_RDF_innkeeperToldAboutWoman = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_2_ADDITIONAL_USER_PROMPT;
				//Change to QUEST_3 upon entering Maqruis' house

				//set required rdf strings
				gInnkeeper->mRDFDynamicContext = RDF_INNKEEPER_TOLD_ABOUT_WOMAN_TRUE;
				gMarquis->mRDFDynamicContext = RDF_MARQUIS_TOLD_ABOUT_WOMAN_FALSE;

				//set required emotion attributes
				gMarquis->mHappiness = 0.6f;
				gMarquis->mTrust = 0.3f;
				gMarquis->mHostility = 0.3f;
			}
		}
	}
	//IF PREVIOUS QUEST IS DONE, CHECK FOR THIRD QUEST
	else if (g_RDF_marquisToldAboutWoman == false)
	{
		//check quest completion parameters if current npc is inkeeper
		if (NPC->getName() == STRING_MARQUIS_NAME)
		{
			//check if quest is completed, set parameters for completed quest
			if (NPC->mHostility >= 0.69)
			{
				LOG_INFO("<<QUEST>> Third quest has been completed.");

				//set quest state to finished
				g_RDF_marquisToldAboutWoman = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_4_USER_PROMPT;

				//set required rdf strings
				gFather->mRDFDynamicContext = RDF_FATHER_TOLD_ABOUT_WOMAN_FALSE;
				gMarquis->mRDFDynamicContext = RDF_MARQUIS_TOLD_ABOUT_WOMAN_TRUE;

				//set required emotion attributes
				gFather->mHappiness = 0.5f;
				gFather->mTrust = 0.4f;
				gFather->mHostility = 0.3f;

				//RoyalGuard: Castle -> Overworld
				//todo: Ustawic RoyalGuardowi niezmienny kontekst zeby pod zadnym pozorem nie przepuscil gracza do domu Markiza
			}
		}
	}
	//IF PREVIOUS QUEST IS DONE, CHECK FOR FOURTH QUEST
	else if (g_RDF_fatherToldAboutWoman == false)
	{
		//check quest completion parameters if current npc is inkeeper
		if (NPC->getName() == STRING_FATHER_NAME)
		{
			//check if quest is completed, set parameters for completed quest
			if (NPC->mTrust >= 0.69) // not always registering at 0.7
			{
				LOG_INFO("<<QUEST>> Fourth quest has been completed.");

				//set quest state to finished
				g_RDF_fatherToldAboutWoman = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_5_USER_PROMPT;

				//set required rdf strings
				gFather->mRDFDynamicContext = RDF_FATHER_TOLD_ABOUT_WOMAN_TRUE;
				gRoyalGuard->mRDFDynamicContext = RDF_ROYALGUARD_CONVINCED_FALSE;
				// Teraz moze przepuscic gracza, bo quest z ojcem zostal wykonany

				//set required emotion attributes
				gRoyalGuard->mHappiness = 0.4f;
				gRoyalGuard->mTrust = 0.2f;
				gRoyalGuard->mHostility = 0.4f;
			}
		}
	}
	//********************EVERYTHING LOWER IS STILL NOT DONE**************************
	//IF PREVIOUS QUEST IS DONE, CHECK FOR FIFTH QUEST
	else if (g_RDF_isGuardConvinced == false) 
	{
		//check quest completion parameters if current npc is inkeeper
		if (NPC->getName() == STRING_ROYALGUARD_NAME)
		{
			//check if quest is completed, set parameters for completed quest
			if (NPC->mTrust >= 0.59)
			{
				LOG_INFO("<<QUEST>> Fifth quest has been completed.");

				//set quest state to finished
				g_RDF_isGuardConvinced = true;

				//set required rdf strings
				gRoyalGuard->mRDFDynamicContext = RDF_ROYALGUARD_CONVINCED_TRUE;
				gMarquis->mRDFDynamicContext = RDF_MARQUIS_TOLD_ABOUT_INNKEEPER_FALSE;

				//set required emotion attributes
				gMarquis->mHappiness = 0.1f;
				gMarquis->mTrust = 0.3f;
				gMarquis->mHostility = 0.7f;

				//TODO: set current quest string
				//
				// Father sent you to find out if Marquis was hiding something. (quest zostaje ten sam)

				// ********************************* zostaje ten sam?
				// ***** mozna teoretycznie zmienic na ten kolejny ktory ma szerszy opis
				gCurrentQuestPrompt = &STRING_QUEST_6_USER_PROMPT; // jak nie to wywal ta linijke

				//Unlock the house
				if (Overworld::get()->mHouseWallInserted)
				{
					Overworld::get()->mWalls.pop_back();
					Overworld::get()->mHouseWallInserted = false;

					LOG_INFO("<wall> Unlocked Marquis house (UpdateBools)");
				}
			}
		}
	}
	//IF PREVIOUS QUEST IS DONE, CHECK FOR SIXTH QUEST
	else if (g_RDF_marquisToldAboutInnkeeper == false)
	{
		//check quest completion parameters if current npc is inkeeper
		if (NPC->getName() == STRING_MARQUIS_NAME)
		{
			//check if quest is completed, set parameters for completed quest
			if (NPC->mHostility <= 0.41 && NPC->mTrust >= 0.49)
			{
				LOG_INFO("<<QUEST>> Sixth quest has been completed.");

				//set quest state to finished
				g_RDF_marquisToldAboutInnkeeper = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_7_USER_PROMPT;

				//set required rdf strings
				gInnkeeper->mRDFDynamicContext = RDF_INNKEEPER_TOLD_ABOUT_REBELS_FALSE;
				gMarquis->mRDFDynamicContext = RDF_MARQUIS_TOLD_ABOUT_INNKEEPER_TRUE;

				//set required emotion attributes
				gInnkeeper->mHappiness = 0.4f;
				gInnkeeper->mTrust = 0.2f;
				gInnkeeper->mHostility = 0.4f;
			}
		}
	}
	//IF PREVIOUS QUEST IS DONE, CHECK FOR SEVENTH QUEST
	else if (g_RDF_innkeeperToldAboutRebels == false)
	{
		//check quest completion parameters if current npc is inkeeper
		if (NPC->getName() == STRING_INNKEEPER_NAME)
		{
			//check if quest is completed, set parameters for completed quest
			if (NPC->mTrust >= 0.79 && NPC->mHostility <= 0.31)//TODO
			{
				LOG_INFO("<<QUEST>> Seventh quest has been completed.");

				//set quest state to finished
				g_RDF_innkeeperToldAboutRebels = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_7_ADDITIONAL_USER_PROMPT;

				//set required rdf strings
				gInnkeeper->mRDFDynamicContext = RDF_INNKEEPER_TOLD_ABOUT_REBELS_TRUE;
			}
		}
	}
	//IF PREVIOUS QUEST IS DONE AND PLAYER LEFT THE INN
	else if (g_additional_playerLeftInnAfterQuestSeven == true) 
	{
		//check if player talks to king
		if (NPC->getName() == STRING_KING_NAME) 
		{
			if (NPC->mHostility >= 0.69)
			{
				LOG_INFO("<<QUEST>> Eigth quest has been completed. Sided with the king.");

				//set quest state to finished
				g_final_playerSidedWithKing = true;
				g_RDF_joinedTheRebellion = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = nullptr; //TODO jakas nazwa questa

				//set required rdf strings
				gInnkeeper->mRDFDynamicContext = RDF_JOIN_THE_REBELLION_SIDE_WITH_KING_TRUE;
			}
		}
		//check if player talks to innkeeper
		else if (NPC->getName() == STRING_INNKEEPER_NAME)
		{
			if (NPC->mTrust >= 0.89)
			{
				LOG_INFO("<<QUEST>> Eigth quest has been completed. Sided with the innkeeper.");

				//set quest state to finished
				g_final_playerSidedWithInnkeeper = true;
				g_RDF_joinedTheRebellion = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = nullptr; //TODO jakas nazwa questa

				//set required rdf strings
				gInnkeeper->mRDFDynamicContext = RDF_JOIN_THE_REBELLION_SIDE_WITH_INNKEEPER_TRUE;
			}
		}
	}
}
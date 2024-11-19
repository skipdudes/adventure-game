#include "UpdateBools.h"
#include "Globals.h"
#include "Constants.h"
#include "Logger.h"

void updateBools(const std::shared_ptr<NPC>& NPC) {

	//CHECK IF FIRST QUEST IS COMPLETED
	if (g_RDF_isInvestigationAssigned == false) 
	{
		//check quest completion parameters if current npc is king
		if (NPC->getName() == STRING_KING_NAME) 
		{
			//check if quest is completed, set parameters for completed quest
			if (NPC->mHappiness >= 0.6 && NPC->mHostility <= 0.3)
			{
				LOG_INFO("First quest has been completed.");

				//set quest state to finished
				g_RDF_isInvestigationAssigned = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_2_USER_PROMPT;

				//set required rdf strings
				gKing->mRDFDynamicContext = RDF_INVESTIGATION_ASSIGNED_TRUE;
				gFather->mRDFDynamicContext = "Other quest dependent string";
				gRoyalGuard->mRDFDynamicContext = "Other quest dependent string";
				gInnkeeper->mRDFDynamicContext = RDF_INNKEEPER_TOLD_ABOUT_WOMAN_FALSE;
				gMarquis->mRDFDynamicContext = "Other quest dependent string";

				//set required emotion attributes
				gInnkeeper->mHappiness = 0.5f;
				gInnkeeper->mTrust = 0.2f;
				gInnkeeper->mHostility = 0.4f;

				//TODO: set current quest string
				// 
				// Meet with the Innkeeper and ask about Richard. The investigation is kept secret, so don’t tell anyone about it. 
				// Innkeeper won't talk unless you gain his trust (Trust > 50%)
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
			if (NPC->mTrust >= 0.5)
			{
				LOG_INFO("Second quest has been completed.");

				//set quest state to finished
				g_RDF_innkeeperToldAboutWoman = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_3_USER_PROMPT;

				//set required rdf strings
				gKing->mRDFDynamicContext = "Other quest dependent string";
				gFather->mRDFDynamicContext = "Other quest dependent string";
				gRoyalGuard->mRDFDynamicContext = "Other quest dependent string";
				gInnkeeper->mRDFDynamicContext = RDF_INNKEEPER_TOLD_ABOUT_WOMAN_TRUE;
				gMarquis->mRDFDynamicContext = RDF_MARQUIS_TOLD_ABOUT_WOMAN_FALSE;

				//set required emotion attributes
				gMarquis->mHappiness = 0.6f;
				gMarquis->mTrust = 0.3f;
				gMarquis->mHostility = 0.3f;

				//TODO: set current quest string
				// 
				// Find out more about the woman and try to find a next person to interrogate
				//
				// dodatkowo nazwa questa zmieni sie tez po wejsciu do domu marquiza (House.cpp)
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
			if (NPC->mHostility >= 0.7)
			{
				LOG_INFO("Third quest has been completed.");

				//set quest state to finished
				g_RDF_marquisToldAboutWoman = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_4_USER_PROMPT;

				//set required rdf strings
				gKing->mRDFDynamicContext = "Other quest dependent string";
				gFather->mRDFDynamicContext = RDF_FATHER_TOLD_ABOUT_WOMAN_FALSE;
				gRoyalGuard->mRDFDynamicContext = "Other quest dependent string";
				gInnkeeper->mRDFDynamicContext = "Other quest dependent string";
				gMarquis->mRDFDynamicContext = RDF_MARQUIS_TOLD_ABOUT_WOMAN_TRUE;

				//set required emotion attributes
				gFather->mHappiness = 0.5f;
				gFather->mTrust = 0.4f;
				gFather->mHostility = 0.3f;

				//TODO: set current quest string
				//
				// Tell your father what you have found out.



				// dodatkowo, jesli ten quest jest wykonany i gracz wejdzie do zamku, to RoyalGuard powinien zostac teleportowany przed dom Marquiza
				
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
			if (NPC->mTrust >= 0.7)
			{
				LOG_INFO("Fourth quest has been completed.");

				//set quest state to finished
				g_RDF_fatherToldAboutWoman = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_FINAL_USER_PROMPT;

				//set required rdf strings
				gKing->mRDFDynamicContext = "Other quest dependent string";
				gFather->mRDFDynamicContext = RDF_FATHER_TOLD_ABOUT_WOMAN_TRUE;
				gRoyalGuard->mRDFDynamicContext = RDF_ROYALGUARD_CONVINCED_FALSE;
				gInnkeeper->mRDFDynamicContext = "Other quest dependent string";
				gMarquis->mRDFDynamicContext = "Other quest dependent string";

				//set required emotion attributes
				gRoyalGuard->mHappiness = 0.4f;
				gRoyalGuard->mTrust = 0.2f;
				gRoyalGuard->mHostility = 0.4f;

				//TODO: set current quest string
				//
				// Father sent you to find out if Marquis was hiding something. 
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
			if (NPC->mTrust >= 0.6)
			{
				LOG_INFO("Fifth quest has been completed.");

				//set quest state to finished
				g_RDF_isGuardConvinced = true;

				//set required rdf strings
				gKing->mRDFDynamicContext = "Other quest dependent string";
				gFather->mRDFDynamicContext = "Other quest dependent string";
				gRoyalGuard->mRDFDynamicContext = RDF_ROYALGUARD_CONVINCED_TRUE;
				gInnkeeper->mRDFDynamicContext = "Other quest dependent string";
				gMarquis->mRDFDynamicContext = "Other quest dependent string";//TODO

				//set required emotion attributes
				gMarquis->mHappiness = 0.4f;
				gMarquis->mTrust = 0.2f;
				gMarquis->mHostility = 0.4f;

				//TODO: set current quest string
				//
				// Father sent you to find out if Marquis was hiding something. (quest zostaje ten sam)
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
			if (NPC->mTrust >= 0.6)//TODO
			{
				LOG_INFO("Sixth quest has been completed.");

				//set quest state to finished
				g_RDF_marquisToldAboutInnkeeper = true;

				//set required rdf strings
				gKing->mRDFDynamicContext = "Other quest dependent string";
				gFather->mRDFDynamicContext = "Other quest dependent string";
				gFather->mRDFDynamicContext = "Other quest dependent string";
				gInnkeeper->mRDFDynamicContext = "Other quest dependent string";//TODO
				gMarquis->mRDFDynamicContext = "Other quest dependent string";//TODO

				//set required emotion attributes
				gInnkeeper->mHappiness = 0.4f;
				gInnkeeper->mTrust = 0.2f;
				gInnkeeper->mHostility = 0.4f;

				//TODO: set current quest string
				//
				// 
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
			if (NPC->mTrust >= 0.6)//TODO
			{
				LOG_INFO("Seventh quest has been completed.");

				//set quest state to finished
				g_RDF_innkeeperToldAboutRebels = true;

				//set required rdf strings
				gKing->mRDFDynamicContext = "Other quest dependent string";//TODO
				gFather->mRDFDynamicContext = "Other quest dependent string";
				gFather->mRDFDynamicContext = "Other quest dependent string";
				gInnkeeper->mRDFDynamicContext = "Other quest dependent string";//TODO
				gMarquis->mRDFDynamicContext = "Other quest dependent string";

				//set required emotion attributes

				//TODO: set current quest string
				//
				// 
			}
		}
	}
}
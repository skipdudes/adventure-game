#include "UpdateBools.h"
#include "Globals.h"
#include "Constants.h"
#include "Logger.h"

void updateBools(const std::shared_ptr<NPC>& NPC) {

	//check if first quest is completed
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
			}
		}
		
	}
	//if first quest is done, then we cehck for second
	else if (g_RDF_innkeeperToldAboutWoman == false) 
	{
		//check quest completion parameters if current npc is inkeeper
		if (NPC->getName() == STRING_INNKEEPER_NAME)
		{
			//check if quest is completed, set parameters for completed quest
			if (NPC->mTrust >= 0.5)
			{
				//set quest state to finished
				g_RDF_innkeeperToldAboutWoman = true;

				//set required rdf strings
				gKing->mRDFDynamicContext = "Other quest dependent string";
				gFather->mRDFDynamicContext = "Other quest dependent string";
				gRoyalGuard->mRDFDynamicContext = "Other quest dependent string";
				gInnkeeper->mRDFDynamicContext = RDF_INNKEEPER_TOLD_ABOUT_WOMAN_TRUE;
				gMarquis->mRDFDynamicContext = "Other quest dependent string";

				//set required emotion attributes
				gMarquis->mHappiness = 0.6f;
				gMarquis->mTrust = 0.3f;
				gMarquis->mHostility = 0.3f;

				//TODO: set current quest string
			}
		}
	}
}
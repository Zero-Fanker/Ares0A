#ifndef BUILDINGTYPE_EXT_H
#define BUILDINGTYPE_EXT_H

#include <CCINIClass.h>

#include "../_Container.hpp"
#include "../../Ares.h"
#include "../../Utilities/Constructs.h"
#include "../../Utilities/Enums.h"
#include "../../Utilities/Template.h"

#include <vector>

//ifdef DEBUGBUILD -- legit needs to log things, so no debug
#include "../../Misc/Debug.h"
//endif

class BuildingClass;
class BuildingTypeClass;
class InfantryTypeClass;
class VocClass;

class BuildingTypeExt
{
public:
	typedef BuildingTypeClass TT;

	static const eFoundation CustomFoundation = static_cast<eFoundation>(0x7F);

	class cPrismForwarding {
		public:
		//properties
		enum eEnabled {NO, YES, FORWARD, ATTACK} Enabled;	//is this tower a prism tower? FORWARD means can support, but not attack. ATTACK means can attack but not support.
		ValueableVector<BuildingTypeClass *> Targets;	//the types of buiding that this tower can forward to
		Nullable<int> MaxFeeds;					//max number of towers that can feed this tower
		Valueable<signed int> MaxChainLength;				//max length of any given (preceding) branch of the network
		Nullable<int> MaxNetworkSize;				//max number of towers that can be in the network
		Nullable<int> SupportModifier; 				//Per-building PrismSupportModifier
		Valueable<signed int> DamageAdd; 					//amount of flat damage to add to the firing beam (before multiplier)
		Nullable<int> MyHeight;						//Per-building PrismSupportHeight
		Valueable<signed int> Intensity;						//amount to adjust beam thickness by when supported
		Valueable<int> ChargeDelay;					//the amount to delay start of charging per backward chain
		Valueable<bool> ToAllies;						//can this tower support allies' towers or not
		Valueable<bool> BreakSupport;					//can the slave tower become a master tower at the last second
		Valueable<signed int> SupportWeaponIndex;
		Valueable<signed int> EliteSupportWeaponIndex;

		//methods
		signed int GetUnusedWeaponSlot(BuildingTypeClass*, bool);
		void Initialize(BuildingTypeClass* );
		void LoadFromINIFile(BuildingTypeClass *, CCINIClass *);

		int GetMaxFeeds() const {
			return this->MaxFeeds.Get(RulesClass::Instance->PrismSupportMax);
		}

		int GetMaxNetworkSize() const {
			return this->MaxNetworkSize.Get(RulesClass::Instance->PrismSupportMax);
		}

		int GetSupportModifier() const {
			return this->SupportModifier.Get(RulesClass::Instance->PrismSupportModifier);
		}

		int GetMyHeight() const {
			return this->MyHeight.Get(RulesClass::Instance->PrismSupportHeight);
		}

		// constructor
		cPrismForwarding() : Enabled(NO),
			Targets(),
			MaxFeeds(),
			MaxChainLength(1),
			MaxNetworkSize(),
			SupportModifier(),
			DamageAdd(0),
			MyHeight(),
			Intensity(-2),
			ChargeDelay(1),
			ToAllies(false),
			BreakSupport(false),
			SupportWeaponIndex(-1),
			EliteSupportWeaponIndex(-1)
		{};
	};

	class ExtData : public Extension<TT>
	{
	public:
		// solid
		int Solid_Height;

		// foundations
		bool IsCustom;
		int CustomWidth;
		int CustomHeight;
		int OutlineLength;
		std::vector<CellStruct> CustomData;
		std::vector<CellStruct> OutlineData;

		DynamicVectorClass<Point2D> FoundationRadarShape;

		// new secret lab
		DynamicVectorClass<TechnoTypeClass *> Secret_Boons;
		Valueable<bool> Secret_RecalcOnCapture;
		bool Secret_Placed;

		// new firestorm wall
		bool Firewall_Is;

		Valueable<bool> IsPassable;

		// lightning rod
		double LightningRod_Modifier;

		// added on 11.11.09 for #221 and children (Trenches)
		double UCPassThrough; 					//!< How many percent of the shots pass through the building to the occupants? 0.0 = 0%, 1.0 = 100%; Defaults to 0.0.
		double UCFatalRate; 					//!< Chance of an occupant getting killed instantly when a bullet passes through. 0.0 = 0%, 1.0 = 100%; Defaults to 0.0.
		double UCDamageMultiplier; 				//!< How many percent of normal damage are applied if an occupant is hit when a bullet passes through. 0.0 = 0%, 1.0 = 100%; Defaults to 1.0.
		bool BunkerRaidable; 					//!< Can this BuildingType be occupied by hostile forces despite being owned by a player, if empty?
		signed int IsTrench; 					//!< Enables moving between segments - saves ID of a kind of trench. \sa trenchKinds
		Valueable<BuildingTypeClass*> RubbleIntact; 		//!< What BuildingType to turn into when reconstructed. (This is the normal building, set on rubble.)
		Valueable<BuildingTypeClass*> RubbleDestroyed;	//!< What BuildingType to turn into when destroyed. (This is the rubble, set on normal buildings.)
		static std::vector<std::string> trenchKinds; //!< Vector of strings associating known trench names with IsTrench IDs. \sa IsTrench

		Valueable<AnimTypeClass*> RubbleDestroyedAnim;
		Valueable<AnimTypeClass*> RubbleIntactAnim;
		Valueable<OwnerHouseKind> RubbleDestroyedOwner;
		Valueable<OwnerHouseKind> RubbleIntactOwner;
		Valueable<int> RubbleDestroyedStrength;
		Valueable<int> RubbleIntactStrength;
		Valueable<bool> RubbleDestroyedRemove;
		Valueable<bool> RubbleIntactRemove;

		// added 03.03.10 for #696 (switch for spied radar behavior)
//		bool LegacyRadarEffect; //!< Whether to use RA's "reveal radar to spy" in addition to RA2's "disrupt radar for victim" on spying of a radar. Defaults to false, requires DisplayProduction to be true. \sa DisplayProduction

		// added 04.03.10 for 633#c2727
//		bool DisplayProduction; //!< Whether to show a factory/radar's production/view to the house who spied it. (RA spy behavior.) Defaults to false. \sa LegacyRadarEffect

		Valueable<bool> InfiltrateCustom;
		Valueable<bool> RevealProduction;
		Valueable<bool> ResetSW;
		Valueable<bool> ResetRadar;
		Valueable<bool> RevealRadar;
		Valueable<bool> RevealRadarPersist;
		Valueable<bool> GainVeterancy;
		Valueable<bool> UnReverseEngineer;
		Valueable<int> StolenTechIndex;
		Valueable<int> StolenMoneyAmount;
		Valueable<int> StolenMoneyPercentage;
		Valueable<int> PowerOutageDuration;

		// #218 Specific Occupiers
		ValueableVector<InfantryTypeClass *> AllowedOccupiers;

		Nullable<bool> Returnable;
		
		cPrismForwarding PrismForwarding;

		Valueable<bool> ReverseEngineersVictims;

		// clones vehicles
		Valueable<bool> CloningFacility;

		// use this factory only if techno states it is built here
		Valueable<bool> Factory_ExplicitOnly;

		// gates
		NullableIdx<VocClass> GateDownSound;
		NullableIdx<VocClass> GateUpSound;

		// academy
		mutable OptionalStruct<bool> Academy;
		ValueableVector<TechnoTypeClass*> AcademyWhitelist;
		ValueableVector<TechnoTypeClass*> AcademyBlacklist;
		Valueable<double> AcademyInfantry;
		Valueable<double> AcademyAircraft;
		Valueable<double> AcademyVehicle;
		Valueable<double> AcademyBuilding;

		ExtData(TT* const OwnerObject) : Extension<TT>(OwnerObject),
			Solid_Height (0),
			IsCustom (false),
			CustomWidth (0),
			CustomHeight (0),
			OutlineLength (0),
			CustomData (),
			OutlineData (),
			FoundationRadarShape (),
			Secret_RecalcOnCapture (false),
			Secret_Placed (false),
			Firewall_Is (false),
			IsPassable (false),
			UCPassThrough (0.0),
			UCFatalRate (0.0),
			UCDamageMultiplier (1.0),
			BunkerRaidable (false),
			IsTrench (-1),
			RubbleIntact (nullptr),
			RubbleDestroyed (nullptr),
			RubbleDestroyedAnim (nullptr),
			RubbleIntactAnim (nullptr),
			RubbleDestroyedOwner (OwnerHouseKind::Default),
			RubbleIntactOwner (OwnerHouseKind::Default),
			RubbleDestroyedStrength (0),
			RubbleIntactStrength (-1),
			RubbleDestroyedRemove (false),
			RubbleIntactRemove (false),
			LightningRod_Modifier (1.0),
			GateDownSound (),
			GateUpSound (),
			InfiltrateCustom (false),
			RevealProduction (false),
			ResetSW (false),
			ResetRadar (false),
			RevealRadar (false),
			RevealRadarPersist (false),
			GainVeterancy (false),
			UnReverseEngineer (false),
			StolenTechIndex (-1),
			StolenMoneyAmount (0),
			StolenMoneyPercentage (0),
			PowerOutageDuration (0),
			AllowedOccupiers (),
			Returnable (),
			PrismForwarding(),
			ReverseEngineersVictims (false),
			CloningFacility (false),
			Factory_ExplicitOnly (false)
			{ };

		virtual ~ExtData() = default;

		virtual void LoadFromINIFile(CCINIClass* pINI) override;
		virtual void Initialize() override;
		virtual void CompleteInitialization(TT *pThis);

		virtual void InvalidatePointer(void *ptr, bool bRemoved) {
			AnnounceInvalidPointer(RubbleIntact, ptr);
			AnnounceInvalidPointer(RubbleDestroyed, ptr);
		}

		bool IsLinkable();

		bool CanBeOccupiedBy(InfantryClass *whom);

		void UpdateFoundationRadarShape();

		bool IsAcademy() const;
	};

	static Container<BuildingTypeExt> ExtMap;

	static void UpdateSecretLabOptions(BuildingClass *pThis);
	static bool IsFoundationEqual(BuildingTypeClass *pTBldA, BuildingTypeClass *pTBldB);
};

#endif

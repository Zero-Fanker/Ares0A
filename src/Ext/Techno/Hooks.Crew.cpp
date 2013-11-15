#include "Body.h"

#include "../House/Body.h"
#include "../Side/Body.h"

DEFINE_HOOK(6F3950, TechnoClass_GetCrewCount, 8)
{
	GET(TechnoClass*, pThis, ECX);
	auto pType = pThis->GetTechnoType();

	// previous default for crew count was -1
	int count = -1;
	if(auto pExt = TechnoTypeExt::ExtMap.Find(pType)) {
		count = pExt->Survivors_PilotCount;
	}

	// default to original formula
	if(count < 0) {
		count = pType->Crewed ? 1 : 0;
	}

	R->EAX(count);
	return 0x6F3967;
}

DEFINE_HOOK(451330, BuildingClass_GetCrewCount, A)
{
	GET(BuildingClass*, pThis, ECX);

	int count = 0;

	if(!pThis->NoCrew && pThis->Type->Crewed) {
		auto pHouse = pThis->Owner;

		// get the divisor
		auto pExt = HouseExt::ExtMap.Find(pHouse);
		int divisor = pExt->GetSurvivorDivisor();

		if(divisor > 0) {
			// if captured, less survivors
			if(pThis->HasBeenCaptured) {
				divisor *= 2;
			}

			// value divided by "cost per survivor"
			count = pThis->Type->GetRefund(pHouse, 0) / divisor;

			// clamp between 1 and 5
			if(count < 1) {
				count = 1;
			}
			if(count > 5) {
				count = 5;
			}
		}
	}

	R->EAX(count);
	return 0x4513CD;
}

DEFINE_HOOK(707D20, TechnoClass_GetCrew, 5)
{
	GET(TechnoClass*, pThis, ECX);
	auto pType = pThis->GetTechnoType();

	auto pHouse = pThis->Owner;
	auto pHouseExt = HouseExt::ExtMap.Find(pHouse);

	InfantryTypeClass* pCrewType = nullptr;

	// YR defaults to 15 for armed objects,
	int TechnicianChance = pThis->IsArmed() ? 15 : 0;

	// Ares < 0.5 defaulted to 0 for non-buildings.
	if(abstract_cast<FootClass*>(pThis)) {
		TechnicianChance = 0;
	}

	if(pType->Crewed) {
		// for civilian houses always technicians. random for others
		bool isTechnician = false;
		if(pHouse->Type->SideIndex == -1) {
			isTechnician = true;
		} else if(TechnicianChance > 0 && ScenarioClass::Instance->Random.RandomRanged(0, 99) < TechnicianChance) {
			isTechnician = true;
		}

		// chose the appropriate type
		if(!isTechnician) {
			// get the side's crew
			pCrewType = pHouseExt->GetCrew();

			// replace with this techno's pilot type
			if(auto pExt = TechnoTypeExt::ExtMap.Find(pType)) {
				auto& pPilots = pExt->Survivors_Pilots;
				int index = pHouse->SideIndex;

				// only use it if non-null, as documented
				if(auto pPilotType = pPilots.GetItemOrDefault(index)) {
					pCrewType = pPilotType;
				}
			}
		} else {
			// either civilian side or chance
			pCrewType = pHouseExt->GetTechnician();
		}
	}

	R->EAX(pCrewType);
	return 0x707DCF;
}

DEFINE_HOOK(44EB10, BuildingClass_GetCrew, 9)
{
	GET(BuildingClass*, pThis, ECX);
	HouseClass* pHouse = pThis->Owner;

	InfantryTypeClass* pType = nullptr;

	// with some luck, and if building-producing building has not been captured, spawn an engineer
	if(!pThis->HasBeenCaptured
		&& ScenarioClass::Instance->Random.RandomRanged(0, 99) < 25
		&& pThis->Type->Factory == BuildingTypeClass::AbsID)
	{
		auto pExt = HouseExt::ExtMap.Find(pHouse);
		pType = pExt->GetEngineer();
	} else {
		// call base
		SET_REG32(ECX, pThis);
		CALL(0x707D20);
		GET_REG32(pType, EAX);
	}

	R->EAX(pType);
	return 0x44EB5B;
}

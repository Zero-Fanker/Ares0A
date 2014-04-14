#ifndef WEAPONTYPE_EXT_H
#define WEAPONTYPE_EXT_H

#include <xcompile.h>
#include <CCINIClass.h>
#include <WeaponTypeClass.h>

#include "../../Misc/Debug.h"

#include "../_Container.hpp"

#include "../../Utilities/Constructs.h"
#include "../../Utilities/Template.h"

class BombClass;
class BulletClass;
class EBolt;
class RadBeam;
class RadType;
class RadSiteClass;
struct SHPStruct;
class VocClass;
class WarheadTypeClass;
class WaveClass;

class WeaponTypeExt
{
public:
	typedef WeaponTypeClass TT;
	class ExtData : public Extension<TT>
	{
	public:
		// static defaults
		static const ColorStruct DefaultWaveColor;
		static const ColorStruct DefaultWaveColorSonic;
		static const ColorStruct DefaultWaveColorMagBeam;

		// Generic
		bool Weapon_Loaded;

		// Coloured Rad Beams
		Nullable<ColorStruct> Beam_Color;
		int    Beam_Duration;
		double Beam_Amplitude;
		bool   Beam_IsHouseColor;

		// Coloured EBolts
		Nullable<ColorStruct> Bolt_Color1;
		Nullable<ColorStruct> Bolt_Color2;
		Nullable<ColorStruct> Bolt_Color3;

		// TS Lasers
		bool   Wave_IsHouseColor;
		bool   Wave_IsLaser;
		bool   Wave_IsBigLaser;
		Nullable<ColorStruct> Wave_Color;
		bool   Wave_Reverse[5];

		Valueable<int> Laser_Thickness;
/*
		int    Wave_InitialIntensity;
		int    Wave_IntensityStep;
		int    Wave_FinalIntensity;
*/
		// custom Ivan Bombs
		Valueable<bool> Ivan_KillsBridges;
		Valueable<bool> Ivan_Detachable;
		Nullable<int> Ivan_Damage;
		Nullable<int> Ivan_Delay;
		NullableIdx<VocClass> Ivan_TickingSound;
		NullableIdx<VocClass> Ivan_AttachSound;
		Nullable<WarheadTypeClass *> Ivan_WH;
		Nullable<SHPStruct *> Ivan_Image;
		Nullable<int> Ivan_FlickerRate;
		Nullable<bool> Ivan_CanDetonateTimeBomb;
		Nullable<bool> Ivan_CanDetonateDeathBomb;

		RadType * Rad_Type;

//		MouseCursor Cursor_Attack;
//		bool Cursor_Custom;

		// #680 Chrono Prison
		Valueable<bool> Abductor; //!< Will this weapon force eligible targets into the passenger hold of the shooter?
		Valueable<AnimTypeClass *> Abductor_AnimType;
		Valueable <bool> Abductor_ChangeOwner;
		Valueable<double> Abductor_AbductBelowPercent;

		// brought back from TS
		Valueable<Leptons> ProjectileRange;
		
		ExtData(TT* const OwnerObject) : Extension<TT>(OwnerObject),
			Weapon_Loaded (false),
			Beam_Color (),
			Beam_Duration (15),
			Beam_Amplitude (40.0),
			Beam_IsHouseColor (false),
			Bolt_Color1 (),
			Bolt_Color2 (),
			Bolt_Color3 (),
			Wave_IsHouseColor (false),
			Wave_IsLaser (false),
			Wave_IsBigLaser (false),
			Wave_Color (),
			Laser_Thickness (-1),
			Ivan_KillsBridges (true),
			Ivan_Detachable (true),
			Ivan_Damage (),
			Ivan_Delay (),
			Ivan_TickingSound (),
			Ivan_AttachSound (),
			Ivan_WH (),
			Ivan_Image (),
			Ivan_FlickerRate (),
			Ivan_CanDetonateTimeBomb (),
			Ivan_CanDetonateDeathBomb (),
			Rad_Type (nullptr),
			ProjectileRange (Leptons(100000)),
			Abductor(false),
			Abductor_AnimType(nullptr),
			Abductor_ChangeOwner(false),
			Abductor_AbductBelowPercent(1)
			{
				for(int i = 0; i < 5; ++i) {
					this->Wave_Reverse[i] = false;
				}
			};

		virtual void LoadFromINIFile(TT *pThis, CCINIClass *pINI);
		virtual void Initialize(TT* pThis);

		virtual void InvalidatePointer(void *ptr, bool bRemoved) {
			AnnounceInvalidPointer(Rad_Type, ptr);
		}

		bool IsWave(WeaponTypeClass *pThis) {
			return pThis->IsSonic || pThis->IsMagBeam || this->Wave_IsLaser || this->Wave_IsBigLaser;
		}

		ColorStruct GetWaveColor() const;
		ColorStruct GetBeamColor() const;

		bool conductAbduction(BulletClass *);

		void PlantBomb(TechnoClass* pSource, ObjectClass* pTarget) const;

		int GetProjectileRange() const {
			return this->ProjectileRange.Get();
		}
	};

/*
	struct WeaponWeight
	{
		short index;
		bool InRange;
		float DPF;
		bool operator < (const WeaponWeight &RHS) const {
			return (this->InRange < RHS.InRange && this->DPF < RHS.DPF);
		}
	};

	EXT_P_DECLARE(WeaponTypeClass);
	EXT_FUNCS(WeaponTypeClass);
	EXT_INI_FUNCS(WeaponTypeClass);

*/

	static Container<WeaponTypeExt> ExtMap;

	static hash_map<BombClass *, ExtData *> BombExt;
	static hash_map<WaveClass *, ExtData *> WaveExt;
	static hash_map<EBolt *, ExtData *> BoltExt;
	static hash_map<RadSiteClass *, ExtData *> RadSiteExt;

#define idxVehicle 0
#define idxAircraft 1
#define idxBuilding 2
#define idxInfantry 3
#define idxOther 4

	static char AbsIDtoIdx(eAbstractType absId)
		{
			switch(absId)
			{
				case abs_Unit:
					return idxVehicle;
				case abs_Aircraft:
					return idxAircraft;
				case abs_Building:
					return idxBuilding;
				case abs_Infantry:
					return idxInfantry;
				default:
					return idxOther;
			}
		};

	// @return skipNormalHandling?
	static bool ModifyWaveColor(WORD *src, WORD *dst, int Intensity, WaveClass *Wave);
};

typedef hash_map<BombClass *, WeaponTypeExt::ExtData *> hash_bombExt;
typedef hash_map<WaveClass *, WeaponTypeExt::ExtData *> hash_waveExt;
typedef hash_map<EBolt *, WeaponTypeExt::ExtData *> hash_boltExt;
typedef hash_map<RadSiteClass *, WeaponTypeExt::ExtData *> hash_radsiteExt;

#endif

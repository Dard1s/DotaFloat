#pragma once
#include <cstdint>

namespace horizon::offsets {

constexpr uintptr_t kClientBase           = 0x0;
constexpr uintptr_t kEntityList           = 0x1A8F4D0;
constexpr uintptr_t kGlobalVars           = 0x1B42A10;
constexpr uintptr_t kLocalPlayer          = 0x1B7E9C8;
constexpr uintptr_t kGameRules            = 0x1C03B48;
constexpr uintptr_t kViewMatrix           = 0x1C8D110;
constexpr uintptr_t kViewRender           = 0x1C9A2F0;
constexpr uintptr_t kParticleManager      = 0x1CA1108;
constexpr uintptr_t kFogController        = 0x1CB0040;
constexpr uintptr_t kCameraManager        = 0x1CB8A90;
constexpr uintptr_t kHudElement           = 0x1CC3200;
constexpr uintptr_t kMatchSignout         = 0x1CD0B80;
constexpr uintptr_t kInventoryManager     = 0x1CE44D0;
constexpr uintptr_t kAbilityPool          = 0x1CEF9A0;
constexpr uintptr_t kModifierTable        = 0x1CF70E0;
constexpr uintptr_t kConVarBase           = 0x1D0A600;

namespace netvars {
    constexpr uintptr_t m_hHero                  = 0x0B18;
    constexpr uintptr_t m_iHealth                = 0x0E38;
    constexpr uintptr_t m_iMaxHealth             = 0x0E3C;
    constexpr uintptr_t m_iMana                  = 0x0E60;
    constexpr uintptr_t m_flHullRadius           = 0x0594;
    constexpr uintptr_t m_iTeamNum               = 0x03F0;
    constexpr uintptr_t m_bIsDormant             = 0x02E9;
    constexpr uintptr_t m_vecOrigin              = 0x0388;
    constexpr uintptr_t m_vecAbsOrigin           = 0x0310;
    constexpr uintptr_t m_hOwnerEntity           = 0x044C;
    constexpr uintptr_t m_fFlags                 = 0x01A0;
    constexpr uintptr_t m_lifeState              = 0x02FA;
    constexpr uintptr_t m_angRotation            = 0x0394;
    constexpr uintptr_t m_flPlaybackRate         = 0x0A54;
    constexpr uintptr_t m_nSequence              = 0x0A58;
    constexpr uintptr_t m_hAbilities             = 0x0C20;
    constexpr uintptr_t m_hItems                 = 0x0C48;
    constexpr uintptr_t m_bIsHexed               = 0x0E90;
    constexpr uintptr_t m_bIsStunned             = 0x0E94;
    constexpr uintptr_t m_modifierCount          = 0x0F10;
    constexpr uintptr_t m_hModifiers             = 0x0F18;
    constexpr uintptr_t m_iPlayerID              = 0x0D40;
    constexpr uintptr_t m_iGold                  = 0x0D88;
    constexpr uintptr_t m_iReliableGold          = 0x0D8C;
    constexpr uintptr_t m_iUnreliableGold        = 0x0D90;
    constexpr uintptr_t m_flCameraDistance       = 0x0410;
    constexpr uintptr_t m_flFogStart             = 0x0504;
    constexpr uintptr_t m_flFogEnd               = 0x0508;
    constexpr uintptr_t m_bFogEnabled            = 0x0510;
    constexpr uintptr_t m_iAbilityLevel          = 0x05A8;
    constexpr uintptr_t m_flCooldown             = 0x05B4;
    constexpr uintptr_t m_flCooldownLength       = 0x05B8;
    constexpr uintptr_t m_iManaCost              = 0x05D0;
    constexpr uintptr_t m_szAbilityName          = 0x0608;
}

namespace convars {
    constexpr uintptr_t dota_camera_distance      = 0x3A80;
    constexpr uintptr_t fog_enable               = 0x2F10;
    constexpr uintptr_t r_farz                   = 0x2A44;
    constexpr uintptr_t dota_ability_debug       = 0x1E90;
}

namespace patterns {
    constexpr const char* kEntitySystem      = "48 8B 0D ? ? ? ? 48 85 C9 74 0A E8 ? ? ? ? 48 8B D8";
    constexpr const char* kGetLocalPlayer    = "E8 ? ? ? ? 48 8B D8 48 85 C0 74 12 48 8B CB";
    constexpr const char* kWorldToScreen     = "40 53 48 83 EC 20 48 8B D9 48 8B 0D ? ? ? ? E8 ? ? ? ?";
    constexpr const char* kDispatchParticle  = "48 89 5C 24 ? 57 48 83 EC 30 41 8B F8 8B D9";
    constexpr const char* kPrepareUnitOrders = "40 55 53 56 57 41 54 41 56 41 57 48 8D AC 24 ? ? ? ?";
}

}

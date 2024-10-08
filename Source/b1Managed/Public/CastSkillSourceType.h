#pragma once
#include "CoreMinimal.h"
#include "CastSkillSourceType.generated.h"

UENUM(BlueprintType)
enum class CastSkillSourceType : uint8 {
    Default,
    Blueprint,
    PersonalTest,
    Notify,
    QA,
    SkillPreview,
    BT,
    AI_GOAP,
    AI_Combo,
    AI_Score,
    AI_ReturnHome,
    AI_OnFight,
    AI_Born,
    GroupAI,
    OnFightTrigger,
    AIInteract,
    ChargeSkill,
    CloudMove,
    DodgeSkill,
    FaBaoSkill,
    GlideMove,
    GM,
    HardMove,
    Intimidate,
    MagicallyChange,
    PhantomRush,
    PlayerItem,
    PlayerTrans,
    PreciseDodge,
    Rebirth,
    BornSkill,
    PhaseTransition,
    SmartCastSkill,
    SummonBehavior,
    CBG,
    CPG,
    CQG,
    ComboDebug,
    Interact,
    Teleport,
    SummonBorn,
    Trans,
    TransitionPhase,
    TurnSkill,
};


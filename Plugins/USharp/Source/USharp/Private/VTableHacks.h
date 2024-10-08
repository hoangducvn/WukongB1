#pragma once

#include "Engine/EngineBaseTypes.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Components/ActorComponent.h"
#include "ExportedFunctions/ExportedFunctionsConventions.h"
#include "VTableHacks.generated.h"

USTRUCT()
struct USHARP_API FSharpActorTickFunction : public FActorTickFunction
{
	GENERATED_USTRUCT_BODY()
	
	virtual void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override
	{
		if (Target && IsValid(Target))
		{
			if (TickType != LEVELTICK_ViewportsOnly || Target->ShouldTickIfViewportsOnly())
			{
				FScopeCycleCounterUObject ActorScope(Target);
				
				if (!Target->GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
				{
					Target->ReceiveTick(DeltaTime*Target->CustomTimeDilation);
				}
				Target->TickActor(DeltaTime*Target->CustomTimeDilation, TickType, *this);
			}
		}
	}
};

template<>
struct TStructOpsTypeTraits<FSharpActorTickFunction> : public TStructOpsTypeTraitsBase2<FSharpActorTickFunction>
{
	enum
	{
		WithCopy = false
	};
};

USTRUCT()
struct USHARP_API FSharpActorComponentTickFunction : public FActorComponentTickFunction
{
	GENERATED_USTRUCT_BODY()
	
	virtual void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override
	{
		ExecuteTickHelper(Target, Target->bTickInEditor, DeltaTime, TickType, [this, TickType](float DilatedTime)
		{
			if (!Target->GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
			{
				Target->ReceiveTick(DilatedTime);
			}
			Target->TickComponent(DilatedTime, TickType, this);
		});
	}
};

template<>
struct TStructOpsTypeTraits<FSharpActorComponentTickFunction> : public TStructOpsTypeTraitsBase2<FSharpActorComponentTickFunction>
{
	enum
	{
		WithCopy = false
	};
};

// NOTE: We MUST define a new class for each function get ONE vtable difference in each class
// NOTE: Redefinition errors if callbacks are assigned in this file (due to VTableHacks.generated.h includes)

/////////////////////////////////////////////////////////////////////////////
// UObject::GetLifetimeReplicatedProps
/////////////////////////////////////////////////////////////////////////////

typedef void (CSCONV *GetLifetimeReplicatedPropsCallbackSig)(const UObject* Obj, TArray<FLifetimeProperty>& OutLifetimeProps);
extern GetLifetimeReplicatedPropsCallbackSig GetLifetimeReplicatedPropsCallback;

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummyRepProps1 : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		if (GetLifetimeReplicatedPropsCallback != nullptr)
		{
			GetLifetimeReplicatedPropsCallback(this, OutLifetimeProps);
		}
	}
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummyRepProps2 : public UDummyRepProps1
{
	GENERATED_BODY()
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummyRepProps3 : public UDummyRepProps2
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		FMsg::Logf("", 0, FName(TEXT("USharp")), ELogVerbosity::Log, TEXT("UDummyRepProps3-GetLifetimeReplicatedProps"));
	}
};

/////////////////////////////////////////////////////////////////////////////
// APawn::SetupPlayerInputComponent
/////////////////////////////////////////////////////////////////////////////

typedef void (CSCONV *SetupPlayerInputComponentCallbackSig)(APawn* Obj, UInputComponent* PlayerInputComponent);
extern SetupPlayerInputComponentCallbackSig SetupPlayerInputComponentCallback;

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummySetupPlayerInput1 : public APawn
{
	GENERATED_BODY()

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override
	{
		if (SetupPlayerInputComponentCallback != nullptr)
		{
			SetupPlayerInputComponentCallback(this, PlayerInputComponent);
		}
	}
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummySetupPlayerInput2 : public ADummySetupPlayerInput1
{
	GENERATED_BODY()
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummySetupPlayerInput3 : public ADummySetupPlayerInput2
{
	GENERATED_BODY()

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override
	{
		FMsg::Logf("", 0, FName(TEXT("USharp")), ELogVerbosity::Log, TEXT("ADummySetupPlayerInput3-SetupPlayerInputComponent"));
	}
};

/////////////////////////////////////////////////////////////////////////////
// AActor::BeginPlay
/////////////////////////////////////////////////////////////////////////////

typedef void(CSCONV *ActorBeginPlayCallbackSig)(AActor* Obj);
extern ActorBeginPlayCallbackSig ActorBeginPlayCallback;

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyActorBeginPlay1 : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override
	{
		if (ActorBeginPlayCallback != nullptr)
		{
			ActorBeginPlayCallback(this);
		}
	}
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyActorBeginPlay2 : public ADummyActorBeginPlay1
{
	GENERATED_BODY()
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyActorBeginPlay3 : public ADummyActorBeginPlay2
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override
	{
		FMsg::Logf("", 0, FName(TEXT("USharp")), ELogVerbosity::Log, TEXT("ADummyActorBeginPlay3-BeginPlay"));
	}
};

/////////////////////////////////////////////////////////////////////////////
// AActor::EndPlay
/////////////////////////////////////////////////////////////////////////////

typedef void(CSCONV *ActorEndPlayCallbackSig)(AActor* Obj, const EEndPlayReason::Type EndPlayReason);
extern ActorEndPlayCallbackSig ActorEndPlayCallback;

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyActorEndPlay1 : public AActor
{
	GENERATED_BODY()

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override
	{
		if (ActorEndPlayCallback != nullptr)
		{
			ActorEndPlayCallback(this, EndPlayReason);
		}
	}
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyActorEndPlay2 : public ADummyActorEndPlay1
{
	GENERATED_BODY()
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyActorEndPlay3 : public ADummyActorEndPlay2
{
	GENERATED_BODY()

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override
	{
		FMsg::Logf("", 0, FName(TEXT("USharp")), ELogVerbosity::Log, TEXT("ADummyActorEndPlay3-EndPlay"));
	}
};

/////////////////////////////////////////////////////////////////////////////
// AActor::EndPlay
/////////////////////////////////////////////////////////////////////////////

typedef void(CSCONV *ActorGetActorEyesViewPointCallbackSig)(const AActor* Obj, FVector& OutLocation, FRotator& OutRotation);
extern ActorGetActorEyesViewPointCallbackSig ActorGetActorEyesViewPointCallback;

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyActorGetActorEyesViewPoint1 : public AActor
{
	GENERATED_BODY()

public:
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override
	{
        if (ActorGetActorEyesViewPointCallback != nullptr)
		{
			ActorGetActorEyesViewPointCallback(this, OutLocation, OutRotation);
		}
	}
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyActorGetActorEyesViewPoint2 : public ADummyActorGetActorEyesViewPoint1
{
	GENERATED_BODY()
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyActorGetActorEyesViewPoint3 : public ADummyActorGetActorEyesViewPoint2
{
	GENERATED_BODY()

public:
    virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override
	{
		FMsg::Logf("", 0, FName(TEXT("USharp")), ELogVerbosity::Log, TEXT("ADummyActorGetActorEyesViewPoint3-GetActorEyesViewPoint"));
	}
};

/////////////////////////////////////////////////////////////////////////////
// UActorComponent::BeginPlay
/////////////////////////////////////////////////////////////////////////////

typedef void(CSCONV *ActorComponentBeginPlayCallbackSig)(UActorComponent* Obj);
extern ActorComponentBeginPlayCallbackSig ActorComponentBeginPlayCallback;

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummyActorComponentBeginPlay1 : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override
	{
		if (ActorComponentBeginPlayCallback != nullptr)
		{
			ActorComponentBeginPlayCallback(this);
		}
	}
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummyActorComponentBeginPlay2 : public UDummyActorComponentBeginPlay1
{
	GENERATED_BODY()
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummyActorComponentBeginPlay3 : public UDummyActorComponentBeginPlay2
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override
	{
		FMsg::Logf("", 0, FName(TEXT("USharp")), ELogVerbosity::Log, TEXT("ADummyActorComponentBeginPlay3-BeginPlay"));
	}
};

/////////////////////////////////////////////////////////////////////////////
// UActorComponent::EndPlay
/////////////////////////////////////////////////////////////////////////////

typedef void(CSCONV *ActorComponentEndPlayCallbackSig)(UActorComponent* Obj, const EEndPlayReason::Type EndPlayReason);
extern ActorComponentEndPlayCallbackSig ActorComponentEndPlayCallback;

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummyActorComponentEndPlay1 : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override
	{
		if (ActorComponentEndPlayCallback != nullptr)
		{
			ActorComponentEndPlayCallback(this, EndPlayReason);
		}
	}
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummyActorComponentEndPlay2 : public UDummyActorComponentEndPlay1
{
	GENERATED_BODY()
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummyActorComponentEndPlay3 : public UDummyActorComponentEndPlay2
{
	GENERATED_BODY()

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override
	{
		FMsg::Logf("", 0, FName(TEXT("USharp")), ELogVerbosity::Log, TEXT("ADummyActorComponentEndPlay3-EndPlay"));
	}
};

/////////////////////////////////////////////////////////////////////////////
// APlayerController::SetupInputComponent
/////////////////////////////////////////////////////////////////////////////

typedef void(CSCONV *PlayerControllerSetupInputComponentCallbackSig)(APlayerController* Obj);
extern PlayerControllerSetupInputComponentCallbackSig PlayerControllerSetupInputComponentCallback;

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyPlayerControllerSetupInputComponent1 : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override
	{
		if (PlayerControllerSetupInputComponentCallback != nullptr)
		{
            PlayerControllerSetupInputComponentCallback(this);
		}
	}
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyPlayerControllerSetupInputComponent2 : public ADummyPlayerControllerSetupInputComponent1
{
	GENERATED_BODY()
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyPlayerControllerSetupInputComponent3 : public ADummyPlayerControllerSetupInputComponent2
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override
	{
		FMsg::Logf("", 0, FName(TEXT("USharp")), ELogVerbosity::Log, TEXT("ADummyPlayerControllerSetupInputComponent3-SetupInputComponent"));
	}
};

/////////////////////////////////////////////////////////////////////////////
// APlayerController::UpdateRotation
/////////////////////////////////////////////////////////////////////////////

typedef void(CSCONV *PlayerControllerUpdateRotationCallbackSig)(APlayerController* Obj, float DeltaTime);
extern PlayerControllerUpdateRotationCallbackSig PlayerControllerUpdateRotationCallback;

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyPlayerControllerUpdateRotation1 : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void UpdateRotation(float DeltaTime) override
	{
		if (PlayerControllerUpdateRotationCallback != nullptr)
		{
            PlayerControllerUpdateRotationCallback(this, DeltaTime);
		}
	}
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyPlayerControllerUpdateRotation2 : public ADummyPlayerControllerUpdateRotation1
{
	GENERATED_BODY()
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API ADummyPlayerControllerUpdateRotation3 : public ADummyPlayerControllerUpdateRotation2
{
	GENERATED_BODY()

protected:
	virtual void UpdateRotation(float DeltaTime) override
	{
		FMsg::Logf("", 0, FName(TEXT("USharp")), ELogVerbosity::Log, TEXT("ADummyPlayerControllerUpdateRotation3-UpdateRotation"));
	}
};

/////////////////////////////////////////////////////////////////////////////
// UGameInstance::Init
/////////////////////////////////////////////////////////////////////////////

typedef void(CSCONV *GameInstanceInitCallbackSig)(UGameInstance* Obj);
extern GameInstanceInitCallbackSig GameInstanceInitCallback;

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummyGameInstanceInit1 : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override
	{
		if (GameInstanceInitCallback != nullptr)
		{
            GameInstanceInitCallback(this);
		}
	}
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummyGameInstanceInit2 : public UDummyGameInstanceInit1
{
	GENERATED_BODY()
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummyGameInstanceInit3 : public UDummyGameInstanceInit2
{
	GENERATED_BODY()

public:
	virtual void Init() override
	{
		FMsg::Logf("", 0, FName(TEXT("USharp")), ELogVerbosity::Log, TEXT("UDummyGameInstanceInit3-Init"));
	}
};

/////////////////////////////////////////////////////////////////////////////
// USubsystem::Initialize
/////////////////////////////////////////////////////////////////////////////

typedef void(CSCONV *SubsystemInitializeCallbackSig)(USubsystem* Obj, FSubsystemCollectionBase& Collection);
extern SubsystemInitializeCallbackSig SubsystemInitializeCallback;

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummySubsystemInitialize1 : public USubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override
	{
		if (SubsystemInitializeCallback != nullptr)
		{
            SubsystemInitializeCallback(this, Collection);
		}
	}
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummySubsystemInitialize2 : public UDummySubsystemInitialize1
{
	GENERATED_BODY()
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummySubsystemInitialize3 : public UDummySubsystemInitialize2
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override
	{
		FMsg::Logf("", 0, FName(TEXT("USharp")), ELogVerbosity::Log, TEXT("UDummySubsystemInitialize3-Initialize"));
	}
};

/////////////////////////////////////////////////////////////////////////////
// USubsystem::Deinitialize
/////////////////////////////////////////////////////////////////////////////

typedef void(CSCONV *SubsystemDeinitializeCallbackSig)(USubsystem* Obj);
extern SubsystemDeinitializeCallbackSig SubsystemDeinitializeCallback;

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummySubsystemDeinitialize1 : public USubsystem
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override
	{
		if (SubsystemDeinitializeCallback != nullptr)
		{
            SubsystemDeinitializeCallback(this);
		}
	}
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummySubsystemDeinitialize2 : public UDummySubsystemDeinitialize1
{
	GENERATED_BODY()
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummySubsystemDeinitialize3 : public UDummySubsystemDeinitialize2
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override
	{
		FMsg::Logf("", 0, FName(TEXT("USharp")), ELogVerbosity::Log, TEXT("UDummySubsystemDeinitialize3-Deinitialize"));
	}
};

/////////////////////////////////////////////////////////////////////////////
// USubsystem::ShouldCreateSubsystem
/////////////////////////////////////////////////////////////////////////////

typedef bool(CSCONV *SubsystemShouldCreateSubsystemCallbackSig)(const USubsystem* Obj, UObject* Other);
extern SubsystemShouldCreateSubsystemCallbackSig SubsystemShouldCreateSubsystemCallback;

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummySubsystemShouldCreateSubsystem1 : public USubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Other) const override
	{
		if (SubsystemShouldCreateSubsystemCallback != nullptr)
		{
            return SubsystemShouldCreateSubsystemCallback(this, Other);
		}

		return false;
	}
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummySubsystemShouldCreateSubsystem2 : public UDummySubsystemShouldCreateSubsystem1
{
	GENERATED_BODY()
};

UCLASS(NotBlueprintable, NotBlueprintType)
class USHARP_API UDummySubsystemShouldCreateSubsystem3 : public UDummySubsystemShouldCreateSubsystem2
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Other) const override
	{
		FMsg::Logf("", 0, FName(TEXT("USharp")), ELogVerbosity::Log, TEXT("UDummySubsystemShouldCreateSubsystem3-ShouldCreateSubsystem"));

		return false;
	}
};
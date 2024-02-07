// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Character\KiteCharacterHero.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Common/KiteGameplayTags.h"
#include "Common/KiteMacros.h"
#include "Components/CapsuleComponent.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/KiteInputComponent.h"
#include "InputMappingContext.h"
#include "AbilitySystem/KiteAbilitySet.h"
#include "AbilitySystem/KiteAbilitySystemComponent.h"
#include "Player/KitePlayerState.h"


AKiteCharacterHero::AKiteCharacterHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
		
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void AKiteCharacterHero::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKiteCharacterHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKiteCharacterHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(!InputConfig.IsValid()) {
		UE_LOG(LogKite, Log, TEXT("InputConfig is not valid"));
		return;
	}
	
	// 异步加载，练习用
	UAssetManager::GetStreamableManager().RequestAsyncLoad(InputConfig.ToSoftObjectPath(), FStreamableDelegate::CreateLambda(
		[this, PlayerInputComponent]()
		{
			if (UKiteInputComponent* KiteIC = CastChecked<UKiteInputComponent>(PlayerInputComponent))
			{
				// Add the key mappings that may have been set by the player
				// KiteIC->AddInputMappings(InputConfig, Subsystem);

				// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
				// be triggered directly by these input actions Triggered events.
				KiteIC->BindNativeAction(InputConfig.Get(), KiteGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
				KiteIC->BindNativeAction(InputConfig.Get(), KiteGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
				KiteIC->BindNativeAction(InputConfig.Get(), KiteGameplayTags::InputTag_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump, /*bLogIfNotFound=*/ false);
				KiteIC->BindNativeAction(InputConfig.Get(), KiteGameplayTags::InputTag_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping, /*bLogIfNotFound=*/ false);
				
				TArray<uint32> BindHandles;
				KiteIC->BindAbilityActions(InputConfig.Get(), this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
			} 
		})
	);

	// 绑定MappingContext
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// 同步加载，练习用
			UInputMappingContext* MappingContext = LoadObject<UInputMappingContext>(this, *DefaultMappingContext.ToSoftObjectPath().ToString());
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void AKiteCharacterHero::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilitySystem();

	AddAbilitySet();
}

void AKiteCharacterHero::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilitySystem();
}

void AKiteCharacterHero::Input_LookMouse(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AKiteCharacterHero::Input_Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AKiteCharacterHero::Input_AbilityInputTagPressed(FGameplayTag AbilityInputTag)
{
	if (AbilityInputTag.IsValid())
	{
		AbilitySystemComponent->AbilityInputTagPressed(AbilityInputTag);
	}
}

void AKiteCharacterHero::Input_AbilityInputTagReleased(FGameplayTag AbilityInputTag)
{
	if (AbilityInputTag.IsValid())
	{
		AbilitySystemComponent->AbilityInputTagReleased(AbilityInputTag);
	}
}

void AKiteCharacterHero::InitAbilitySystem()
{
	AKitePlayerState* PS = GetPlayerState<AKitePlayerState>();
	if (PS)
	{
		// 以防万一Cast一下
		AbilitySystemComponent = Cast<UKiteAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
}

// 只用在服务器上添加技能即可，对应的GameplayAbilitySpec会被自动复制到Owning Client。其他客户端（Simulated Proxies）将不会收到GameplayAbilitySpec
void AKiteCharacterHero::AddAbilitySet()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent) || AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	if (!IsValid(AbilitySet))
	{
		UE_LOG(LogKite, Warning, TEXT("AbilitySet is not valid"));
		return;
	}
	
	AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, OutGrantedHandles);

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void AKiteCharacterHero::RemoveAbilitySet()
{
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent) || !AbilitySystemComponent->bCharacterAbilitiesGiven || !OutGrantedHandles)
	{
		return;
	}
	
	OutGrantedHandles->TakeFromAbilitySystem(AbilitySystemComponent);

	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
}

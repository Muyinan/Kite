// Fill out your copyright notice in the Description page of Project Settings.


#include "KiteCharacter.h"

#include "KiteDefaultPawnComponent.h"
#include "Camera/CameraComponent.h"
#include "Common/KiteMacros.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/KiteExperienceManagerComponent.h"
#include "GameModes/KiteGameModeBase.h"


// Sets default values
AKiteCharacter::AKiteCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

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

	DefaultPawnComponent = ObjectInitializer.CreateDefaultSubobject<UKiteDefaultPawnComponent>(this, TEXT("DefaultPawnComponent"));
}

// Called when the game starts or when spawned
void AKiteCharacter::BeginPlay()
{
	Super::BeginPlay();

	kwarn("~ AKiteCharacter BeginPlay %s, ", *GetNameSafe(this));
	
	if (InputComponent)
	{
		UKiteExperienceManagerComponent* ExperienceComponent = GetWorld()->GetGameState()->FindComponentByClass<UKiteExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->CallOrRegister_OnExperienceLoaded_LowPriority(FOnKiteExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::Init));
	}

	// //Add Input Mapping Context
	// if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	// {
	// 	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	// 	{
	// 		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	// 	}
	// }

	// //添加技能给ASC
	// if (AbilitySystem)
	// {
	// 	// 本地控制的角色才添加技能
	// 	if(HasAuthority())
	// 	{
	// 		for(const auto& AbilityClass : Abilities)
	// 		{
	// 			AbilitySystem->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, 0));
	// 		}
	// 	}
	// 	// 初始化ASC
	// 	AbilitySystem->InitAbilityActorInfo(this, this);
	// }
}

void AKiteCharacter::Init(const UKiteExperienceDefinition* ExperienceDefinition)
{
	if (AKiteGameModeBase* KiteGameMode = GetWorld()->GetAuthGameMode<AKiteGameModeBase>())
	{
		if (const UKitePawnData* NewPawnData = KiteGameMode->GetPawnDataForController(GetController()))
		{
			DefaultPawnComponent->SetPawnData(NewPawnData);
			DefaultPawnComponent->InitializePlayerInput(InputComponent);
		}
		else
		{
			UE_LOG(LogKite, Error, TEXT("AKiteCharacter::Init(): Unable to find PawnData to initialize player AKiteCharacter [%s]!"), *GetNameSafe(this));
		}
	}
}

// // Called every frame
// void AKiteCharacter::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

// Called to bind functionality to input
// void AKiteCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);

 
	
	// // Set up action bindings
	// if (UEnhancedInputComponent* KiteInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
	// 	//Looking
	// 	lookHandle = KiteInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look).GetHandle();
	// 	//Moving
	// 	KiteInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	// 	//Jumping
	// 	KiteInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	// 	KiteInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
	// 	
	// 	KiteInputComponent->BindAction(MeleeAttackAction, ETriggerEvent::Started, this, &ThisClass::MeleeAttack);
	// }
// }

// void AKiteCharacter::Look(const FInputActionValue& Value)
// {
// 	// input is a Vector2D
// 	FVector2D LookAxisVector = Value.Get<FVector2D>();
//
// 	if (Controller != nullptr)
// 	{
// 		// add yaw and pitch input to controller
// 		AddControllerYawInput(LookAxisVector.X);
// 		AddControllerPitchInput(LookAxisVector.Y);
// 	}
// }
//
// void AKiteCharacter::Move(const FInputActionValue& Value)
// {
// 	// input is a Vector2D
// 	FVector2D MovementVector = Value.Get<FVector2D>();
// 	
// 	if (Controller != nullptr)
// 	{
// 		// find out which way is forward
// 		const FRotator Rotation = Controller->GetControlRotation();
// 		const FRotator YawRotation(0, Rotation.Yaw, 0);
//
// 		// get forward vector
// 		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
// 		
// 		// get right vector 
// 		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
// 		
// 		// add movement 
// 		AddMovementInput(ForwardDirection, MovementVector.Y);
// 		AddMovementInput(RightDirection, MovementVector.X);
// 	}
// }

// void AKiteCharacter::MeleeAttack(const FInputActionValue& Value)
// {
// 	K2_MeleeAttack(Value);
// }

// void AKiteCharacter::OnRep_CurrentHealth()
// {
// 	
// }

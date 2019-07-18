// All rights reserved.

#include "CharacterManager.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Twalien.h"
#include "Weapon.h"
#include "Dustpan.h"
#include "TwalienSkillComponent.h"
#include "GameFramework/HUD.h"
#include "LifeComponent.h"
#include "SharedLifeComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UserWidget.h"
#include "FMODEvent.h"
#include "FMODBlueprintStatics.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Color.h"

// Sets default values
ACharacterManager::ACharacterManager() : Continues(3), SwitchDistance(100), PlayerIsDoingSwitch(false), Player1Color(0, 0.161f, 1, 0), Player2Color(1, .217f, 0, 0)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<APawn> TWalien(TEXT("/Game/Characters/Player/BP_Twalien_0"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshGunFinder(TEXT("/Game/Weapons/Gun/SM_Gun_01_Skeletal_SM_Gun_01"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshDustpanFinder(TEXT("/Game/Weapons/Dustpan/SM_Collector_01_Skeletal_SM_Collector_01"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialGunFinder(TEXT("/Game/Weapons/Materials/M_Gun_01"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialDustpanFinder(TEXT("/Game/Weapons/Materials/M_Collector_01"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialBodyPlayer1Finder(TEXT("/Game/Characters/Materials/M_Character_01"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialHeadPlayer1Finder(TEXT("/Game/Characters/Materials/M_CharacterFace_01"));
	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> MaterialBodyPlayer2Finder(TEXT("/Game/Characters/Materials/MI_Character_02"));
	ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> MaterialHeadPlayer2Finder(TEXT("/Game/Characters/Materials/MI_CharacterFace_02"));
	ConstructorHelpers::FClassFinder<UUserWidget> GameOverClassFinder(TEXT("/Game/Interface/GameOverMenu"));

	//Check if all search are successful
	if (MeshDustpanFinder.Succeeded() && MeshGunFinder.Succeeded() && MaterialGunFinder.Succeeded() && MaterialDustpanFinder.Succeeded() && MaterialBodyPlayer1Finder.Succeeded() && MaterialBodyPlayer2Finder.Succeeded() && MaterialHeadPlayer1Finder.Succeeded() && MaterialHeadPlayer2Finder.Succeeded())
	{
		//Gun
		GunMaterial = MeshGunFinder.Object;
		GunMaterialInterface = MaterialGunFinder.Object;
		
		//Dustpan
		DustpanMaterial = MeshDustpanFinder.Object;
		DustpanMaterialInterface = MaterialDustpanFinder.Object;

		//Material Player 1
		Player1Body = MaterialBodyPlayer1Finder.Object;
		Player1Head = MaterialHeadPlayer1Finder.Object;

		//Material Player 2
		Player2Body = MaterialBodyPlayer2Finder.Object;
		Player2Head = MaterialHeadPlayer2Finder.Object;
	}

	SwitchAudio = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Switch Audio"));
	SwitchAudio->SetupAttachment(RootComponent);

	SourceTwaliensHealth = CreateDefaultSubobject<ULifeComponent>("CommonPlayerHealth");
	AddOwnedComponent(SourceTwaliensHealth);

	Player = TWalien.Class;

	FindRespawnLocations();
	//FindPlayers();

	GameOverWidget = GameOverClassFinder.Class;

	//Set delegate
	PlayerDeathDelegate.BindUFunction(this, FName("StartPlayerRespawnTimer"));
	PlayerSwitchDelegate.BindUFunction(this, FName("SwitchCompleted"));
}

void ACharacterManager::StartPlayerRespawnTimer()
{
	const UWorld * World = GetWorld();
	//Get a non null World
	if (World)
	{
		//If Respawn timer is not active
		if (!World->GetTimerManager().IsTimerActive(RespawnTimerHandle))
		{
			//We set Respawn timer to RespawnTime value
			World->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ACharacterManager::RespawnPlayers, 1.f, false, RespawnTime);
			//We decrement amount of continues
			SetContinues(-1);
			//check if we reach game over, and if true show game over menu
			if (GetContinues() == 0)
			{
				ShowGameOverMenu();
			}
			//Pause switch timer
			GetWorldTimerManager().PauseTimer(SwitchTimerHandle);
			//disable both players
			Players[0]->Disable();
			Players[1]->Disable();
		}
	}
}

void ACharacterManager::RespawnPlayers()
{
	//We do this for twice player
	//we enable player and the set their location to their spawn point	
	Players[0]->SetActorLocation(RespawnLocation[0]->GetActorLocation());
	Players[0]->OnPlayerSpawn();

	Players[1]->SetActorLocation(RespawnLocation[1]->GetActorLocation());
	Players[1]->OnPlayerSpawn();

	//Un pause timer for switch weapons
	GetWorldTimerManager().UnPauseTimer(SwitchTimerHandle);
}

void ACharacterManager::ShowGameOverMenu()
{
	GameOver = CreateWidget<UUserWidget>(GetWorld(), GameOverWidget);

	if (GameOver)
	{
		//Get a reference to player controller from first player (first or second doesnt matter)
		APlayerController* PlayerController = Players[0]->GetPlayerController();
		//set game pause
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		FInputModeGameAndUI inputMode;
		//Do not lock mouse functionlity to viewport
		inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		//Show mouse on game over menu
		PlayerController->bShowMouseCursor = true;
		//set new input mode
		PlayerController->SetInputMode(FInputModeGameAndUI(inputMode));
		//Add widget to wiewport 
		GameOver->AddToViewport();
	}
}

void ACharacterManager::SwitchCompleted()
{
	SwitchGun();
	GetWorldTimerManager().SetTimer(SwitchTimerHandle, SwitchTime, false);
	PlayerIsDoingSwitch = false;
}

// Called when the game starts or when spawned
void ACharacterManager::BeginPlay()
{
	Super::BeginPlay();

	//Create Dynamic Material Instance for gun and dustpan
	MIDGun = UMaterialInstanceDynamic::Create(GunMaterialInterface, this);
	MIDDustpan = UMaterialInstanceDynamic::Create(DustpanMaterialInterface, this);

	//If CreatePlayers flag is set to true
	if(CreatePlayers)
	{
		//Spawn player and do some config stuff
		SpawnPlayers();
	}
	else
	{
		//If not we find player on map and do some config stuff
		FindPlayers();
	}

	//Setup sounds for player weapons
	SetWeaponSounds();

	//set timer for switching weapons
	GetWorldTimerManager().SetTimer(SwitchTimerHandle, SwitchTime, false);
}

// Called every frame
void ACharacterManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Validate if the timer has ended
	//Calculate the distance between players if it is lower than the limit switch de guns
	if(GetWorldTimerManager().GetTimerRemaining(SwitchTimerHandle) <= 0 && !PlayerIsDoingSwitch)
	{
		//start reducing the life until the weapons has switched
		SourceTwaliensHealth->SetHitPoints(-GunCurseDamage);

		//We perform first a soft operation to check if player could be at SwitchDistance and if we get a promising result we do hard operation later
		if(SourceTwaliensHealth->GetHitPoints() > 0 && PlayerAreApproximatelyAtSwitchDistance() && GetPlayersDistance() <= SwitchDistance)
		{
			//send events to player for start switching weapons
			Players[0]->OnWeaponSwitchingStart();
			Players[1]->OnWeaponSwitchingStart();

			PlayerIsDoingSwitch = true;
		}
	}
}

void ACharacterManager::FindRespawnLocations()
{
	class UWorld* const world = GetWorld();
	int32 i = 0;

	if (world)
	{
		for (TActorIterator<APlayerStart> Itr(world); Itr; ++Itr)
		{
			APlayerStart* FoundPoint = *Itr;

			if (FoundPoint != nullptr)
			{
				if (FoundPoint->PlayerStartTag == FName(*FString::FromInt(i)))
				{

					if (AStartPoints != FoundPoint)
					{
						AStartPoints = FoundPoint;

						// Once the location is found, add it to array
						RespawnLocation.Add(AStartPoints);
					}
				}
			}
			++i;
		}
	}
}

void ACharacterManager::SpawnPlayers()
{
	const FName ParamName("EmisColor");
	class UWorld* const world = GetWorld();
	int MaxPlayerCount;

	MaxPlayerCount = RespawnLocation.Num();

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = Instigator;

	for (int i = 0; i < MaxPlayerCount; ++i)
	{
		//Initializing player and playercontroller
		APlayerController* NewPlayerController = Cast<APlayerController>(UGameplayStatics::CreatePlayer(world, i));
		ATwalien* ResultPawn = world->SpawnActor<ATwalien>(Player, RespawnLocation[i]->GetActorLocation(), RespawnLocation[i]->GetActorRotation(), spawnParams);
		
		//Set Twalien internal reference to their APlayerController object
		ResultPawn->SetPlayerController(*NewPlayerController);

		//Possessing player
		ResultPawn->SetControllerID(i);

		UGameplayStatics::GetPlayerController(GetWorld(), i)->Possess(ResultPawn);

		//Saving player in TArray
		Players.Add(ResultPawn);

		//Setting to player their sharedlifecomponent source of life
		ResultPawn->GetLifeComponent()->SetCommonLifeComponent(SourceTwaliensHealth);

		//Saving health value for vibration stuff
		ResultPawn->SetCurrentHealth(SourceTwaliensHealth->GetHitPoints());

		//If Player 1 should be Dustpan player
		if (ShouldSwitch)
		{
			//If this object is first constructed
			if (i == 0)
			{
				ResultPawn->SetBodyHeadMaterial(*Player1Body, *Player1Head, 2);
				MIDDustpan->SetVectorParameterValue(ParamName, Player1Color);
				ResultPawn->SwitchToDustpan(nullptr, *DustpanMaterial, *MIDDustpan);
				ResultPawn->AddDelegateToPlayerDeath(PlayerDeathDelegate);
				ResultPawn->AddDelegateToPlayerExchange(PlayerSwitchDelegate);
			}
			else if (i == 1)
			{
				MIDGun->SetVectorParameterValue(ParamName, Player2Color);
				ResultPawn->SetBodyHeadMaterial(*Player2Body, *Player2Head, 4);
				ResultPawn->SwitchToWeapon(nullptr, *GunMaterial, *MIDGun);
			}
		}
		else
		{
			if (i == 1)
			{
				MIDDustpan->SetVectorParameterValue(ParamName, Player2Color);
				ResultPawn->SetBodyHeadMaterial(*Player2Body, *Player2Head, 4);
				ResultPawn->SwitchToDustpan(nullptr, *DustpanMaterial, *MIDDustpan);
			}
			else if (i == 0)
			{
				MIDGun->SetVectorParameterValue(ParamName, Player1Color);
				ResultPawn->SetBodyHeadMaterial(*Player1Body, *Player1Head, 2);
				ResultPawn->SwitchToWeapon(nullptr, *GunMaterial, *MIDGun);
				ResultPawn->AddDelegateToPlayerDeath(PlayerDeathDelegate);
				ResultPawn->AddDelegateToPlayerExchange(PlayerSwitchDelegate);
			}
		}
	}
}

void ACharacterManager::FindPlayers()
{
	const FName ParamName("EmisColor");
	class UWorld* const world = GetWorld();
	int32 i = 0;

	//If World exists
	if (world)
	{
		//We search all twaliens in world
		for (TActorIterator<ATwalien> Itr(world); Itr; ++Itr)
		{
			ATwalien* FoundPlayer= *Itr;

			//If there is player
			if (FoundPlayer)
			{
				//Create a controller for i player
				APlayerController* NewPlayerController = Cast<APlayerController>(UGameplayStatics::CreatePlayer(world, i));

				//Set Twalien internal reference to their APlayerController object
				FoundPlayer->SetPlayerController(*NewPlayerController);

				//Set Controller id
				FoundPlayer->SetControllerID(i);

				//Do posses from created controller to i player
				UGameplayStatics::GetPlayerController(GetWorld(), i)->Possess(FoundPlayer);

				//Add this player to Player TArray
				Players.Add(FoundPlayer);
				
				//Setting to player their sharedlifecomponent source of life
				FoundPlayer->GetLifeComponent()->SetCommonLifeComponent(SourceTwaliensHealth);
				
				//Saving health value for vibration stuff
				FoundPlayer->SetCurrentHealth(SourceTwaliensHealth->GetHitPoints());
			}
			//If player 1 should be dustpan player
			if (ShouldSwitch)
			{
				//If this player is first searched player
				if (i == 0)
				{
					MIDDustpan->SetVectorParameterValue(ParamName, Player1Color);
					FoundPlayer->SetBodyHeadMaterial(*Player1Body, *Player1Head, 2);
					FoundPlayer->SwitchToDustpan(nullptr, *DustpanMaterial, *MIDDustpan);
					FoundPlayer->AddDelegateToPlayerDeath(PlayerDeathDelegate);
					FoundPlayer->AddDelegateToPlayerExchange(PlayerSwitchDelegate);
				}
				else if (i == 1)
				{
					MIDGun->SetVectorParameterValue(ParamName, Player2Color);
					FoundPlayer->SetBodyHeadMaterial(*Player2Body, *Player2Head, 4);
					FoundPlayer->SwitchToWeapon(nullptr, *GunMaterial, *MIDGun);
				}
			}
			else
			{
				if (i == 1)
				{
					MIDDustpan->SetVectorParameterValue(ParamName, Player2Color);
					FoundPlayer->SetBodyHeadMaterial(*Player2Body, *Player2Head, 4);
					FoundPlayer->SwitchToDustpan(nullptr, *DustpanMaterial, *MIDDustpan);
				}
				else if (i == 0)
				{
					MIDGun->SetVectorParameterValue(ParamName, Player1Color);
					FoundPlayer->SetBodyHeadMaterial(*Player1Body, *Player1Head, 2);
					FoundPlayer->SwitchToWeapon(nullptr, *GunMaterial, *MIDGun);
					FoundPlayer->AddDelegateToPlayerDeath(PlayerDeathDelegate);
					FoundPlayer->AddDelegateToPlayerExchange(PlayerSwitchDelegate);
				}
			}

			++i;
		}
	}
}

void ACharacterManager::SwitchGun()
{
	const FName ParamName("EmisColor");

	ShouldSwitch = !ShouldSwitch;

	if (!ShouldSwitch)
	{
		MIDGun->SetVectorParameterValue(ParamName, Player1Color);
		Players[0]->SwitchToWeapon(Players[1]->GetPlayerWeapon(), *GunMaterial, *MIDGun);

		MIDDustpan->SetVectorParameterValue(ParamName, Player2Color);
		Players[1]->SwitchToDustpan(Players[0]->GetPlayerDustpan(), *DustpanMaterial, *MIDDustpan);
	}
	else
	{
		MIDDustpan->SetVectorParameterValue(ParamName, Player1Color);
		Players[0]->SwitchToDustpan(Players[1]->GetPlayerDustpan(), *DustpanMaterial, *MIDDustpan);

		MIDGun->SetVectorParameterValue(ParamName, Player2Color);
		Players[1]->SwitchToWeapon(Players[0]->GetPlayerWeapon(), *GunMaterial, *MIDGun);
	}

	if(SwitchAudio->Event.Get())
	{
		SwitchAudio->SetWorldLocation(Players[0]->GetActorLocation());
		SwitchAudio->Play();
	}
}

void ACharacterManager::DisablePlayer(ATwalien* APlayer)
{
	APlayer->Disable();
}

void ACharacterManager::EnablePlayer(ATwalien* APlayer)
{
	APlayer->Enable();
}

void ACharacterManager::GiveSomeAmmo(int AmmoAmount)
{
	const ATwalien * Player1 = GetPlayer(0);
	const bool Player1HasWeapon = !Player1->GetIsDustpanPlayer();
	const ATwalien * Player2 = GetPlayer(1);

	if (Player1HasWeapon)
	{
		Player1->GetPlayerWeapon()->SetAmmunition(AmmoAmount);
	}
	else
	{
		Player2->GetPlayerWeapon()->SetAmmunition(AmmoAmount);
	}
}

void ACharacterManager::GiveSomeAlternativeAmmo(int StingAmount)
{
	const ATwalien * Player1 = GetPlayer(0);
	const bool Player1HasWeapon = !Player1->GetIsDustpanPlayer();
	const ATwalien * Player2 = GetPlayer(1);

	if (Player1HasWeapon)
	{
		Player1->GetPlayerWeapon()->SetAltAmmunition(StingAmount);
	}
	else
	{
		Player2->GetPlayerWeapon()->SetAltAmmunition(StingAmount);
	}
}

class ATwalien * ACharacterManager::GetPlayerWithWeapon() const
{
	ATwalien * Player1 = GetPlayer(0);
	const bool Player1HasWeapon = !Player1->GetIsDustpanPlayer();
	ATwalien * Player2 = GetPlayer(1);

	if (Player1HasWeapon)
	{
		return Player1;
	}
	else
	{
		return Player2;
	}
}

class ATwalien * ACharacterManager::GetPlayerWithDustpan() const
{
	ATwalien * Player1 = GetPlayer(0);
	const bool Player1HasDustpan = Player1->GetIsDustpanPlayer();
	ATwalien * Player2 = GetPlayer(1);

	if (Player1HasDustpan)
	{
		return Player1;
	}
	else
	{
		return Player2;
	}
}

float ACharacterManager::GetRemainingSwitchTime() const
{
	if (GetWorld() && SwitchTimerHandle.IsValid())
	{
		return FMath::RoundToZero(GetWorldTimerManager().GetTimerRemaining(SwitchTimerHandle) + 0.5);
	}

	return 0;
}

ATwalien * ACharacterManager::GetPlayer(int Index) const
{ 
	if (Players.Num() > 0)
	{
		return Players[Index];
	}

	return nullptr;
}

float ACharacterManager::GetRemainingRespawnTime() const
{ 
	if (GetWorld() && RespawnTimerHandle.IsValid())
	{
		return GetWorldTimerManager().GetTimerRemaining(RespawnTimerHandle);
	}

	return 0;
}

FTimerHandle ACharacterManager::GetSwitchTimerHandle()
{
	return SwitchTimerHandle;
}

void ACharacterManager::SetSwitchTimerHandle(float Time)
{
	GetWorldTimerManager().SetTimer(SwitchTimerHandle, Time, false);
}

void ACharacterManager::SetRespawnPosition(AActor* Actor, int Index)
{
	RespawnLocation[Index] = Actor;
}

void ACharacterManager::SetWeaponSounds()
{
	UWeapon* Weapon;
	UDustpan* Dustpan;

	if (FireSoundEvent)
	{
		Weapon = Cast<UWeapon>(Players[0]->GetComponentByClass(UWeapon::StaticClass()));
		Weapon->SetFMODEvent(FireSoundEvent);
		Weapon = Cast<UWeapon>(Players[1]->GetComponentByClass(UWeapon::StaticClass()));
		Weapon->SetFMODEvent(FireSoundEvent);
	}

	if(SuckSoundEvent)
	{
		Dustpan = Cast<UDustpan>(Players[0]->GetComponentByClass(UDustpan::StaticClass()));
		Dustpan->SetFMODEvent(SuckSoundEvent);
		Dustpan = Cast<UDustpan>(Players[1]->GetComponentByClass(UDustpan::StaticClass()));
		Dustpan->SetFMODEvent(SuckSoundEvent);
	}
}

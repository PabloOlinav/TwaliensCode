// All rights reserved.

#include "Dustpan.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Engine.h"
#include "BasicWarrior.h"
#include "Ammo.h"
#include "EngineUtils.h"
#include "CharacterManager.h"
#include "Crypteck/Sting.h"
#include "Crypteck/ToxicArea.h"
#include "Interfaces/SuckableObjects.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDustpan::UDustpan() : SuckVelocity(1000), SuckedDistance(50), SuckRadio(150), EnemiesToSuck(3), CanSuckEnemy(false), EnemiesSucked(0), Exclusive(nullptr), ExclusiveIndex(-1), DustpanSkillCD(60.f), Super()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UParticleSystem> SuckerEffect(TEXT("/Game/Effects/Particles/PS_Wind_01"));

	// ...

	//Initialize components
	SuckArea = CreateDefaultSubobject<USphereComponent>("SuckArea");
	SuckArea->SetupAttachment(this);
	SuckArea->SetRelativeLocation(GetForwardVector() * SuckRadio);
	SuckArea->SetSphereRadius(SuckRadio);

	//We assign functions to delegates
	OverlapEnterDelegate.BindUFunction(this, FName("OnObjectEnterToArea"));
	SuckArea->OnComponentBeginOverlap.Add(OverlapEnterDelegate);

	OverlapExitDelegate.BindUFunction(this, FName("OnObjectExitToArea"));
	SuckArea->OnComponentEndOverlap.Add(OverlapExitDelegate);

	SuckAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Suck Audio"));
	SuckAudioComponent->SetupAttachment(this);

	PS_DustpanComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PS_DustpanComponnt"));
	PS_DustpanComponent->SetupAttachment(this);
	PS_DustpanComponent->SetRelativeLocation(SuckArea->GetComponentLocation());

	// Fixed Location
	PS_DustpanComponent->SetRelativeLocation(FVector(110.f, 0, -15.f));
	PS_DustpanComponent->SetRelativeRotation(FRotator(-100, 0, 0));

	if (SuckerEffect.Succeeded())
	{
		PS_DustpanComponent->SetTemplate(SuckerEffect.Object);
	}
	PS_DustpanComponent->bAutoActivate = false;
}


// Called when the game starts
void UDustpan::BeginPlay()
{
	Super::BeginPlay();

	// ...

	TActorIterator<ACharacterManager> Iterator(GetWorld());

	if (Iterator)
	{
		CharacterManager = *Iterator;
	}
	
	//PS_DustpanComponent->ActivateSystem(true);
	//PS_DustpanComponent->bVisible = false;
}

// Called every frame
void UDustpan::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDustpan::Suck()
{
	//There are 3 types of sucked objects
	//Normal objects -> Dustpan only sucks this objects
	//Exclusive objects -> Dustpan sucks exclusively this object
	//Affected by this skill -> Dustpan sucks this type of objects only when skill is activated

	//If exclusive we suck this object for check later if it is still exclusive later

	if(!GetWorld()->GetTimerManager().IsTimerActive(PlayEffectHandle) && SuckAudioComponent->Event.Get())
	{
		GetWorld()->GetTimerManager().SetTimer(PlayEffectHandle, 1.4f, false);
		SuckAudioComponent->Play();
	}

	if (Exclusive)
	{
		Exclusive->OnBeingSucked(*this);

		if (Exclusive && Exclusive->GetIsSucked())
		{
			AActor * Suckable = Cast<AActor>(Exclusive);
			ActorsInArea[ExclusiveIndex] = ActorsInArea[ActorsInArea.Num() - 1];
			ActorsInArea[ActorsInArea.Num() - 1] = Exclusive;
			ActorsInArea.RemoveAt(ActorsInArea.Num() - 1);
			Suckable->Destroy();
			Exclusive = nullptr;
			ExclusiveIndex = -1;
		}
	}

	int VariableLength = ActorsInArea.Num();

	//If there isn't an exclusive object or exclusive is not exclusive at this moment
	if (!Exclusive || !Exclusive->GetIsExclusive())
	{

		//Iterate over all objects
		for (int i = 0; i < VariableLength; ++i)
		{
			ISuckableObjects * Suckable = ActorsInArea[i];
			//If objects isn't skill affected we do normal operations
			if (!Suckable->GetIsSkillAffected())
			{
				Suckable->OnBeingSucked(*this);
			}
			//If objects is skill affected, skill is activated and are enemies to suck left
			else if (Suckable->GetIsSkillAffected() && CanSuckEnemy && EnemiesSucked < EnemiesToSuck)
			{
				Suckable->OnBeingSucked(*this);
				++EnemiesSucked;
			}

			//If object is exclusive
			if (Suckable->GetIsExclusive())
			{
				Exclusive = Suckable;
				ExclusiveIndex = i;
			}
			
			//if object is sucked, we prepare remove operation
			if (Suckable->GetIsSucked())
			{
				//Remove operation:
				//we swap sucked element with last array element for easy erase
				--VariableLength;
				ActorsInArea[i] = ActorsInArea[VariableLength];
				ActorsInArea[VariableLength] = Suckable;
			}
		}

		//iterate from ActorsInArea.Num() to VariableLength
		for (int i = ActorsInArea.Num() - 1; i >= VariableLength; --i)
		{
			//Get a reference to i-element
			//Remove i-element from array
			//Destroy that element
			//Validate if the actor is a pooling actor

			AActor * Suckable = Cast<AActor>(ActorsInArea[i]);

			ActorsInArea.RemoveAt(i, 1, false);

			IIPooledObject* PoolObject = Cast<IIPooledObject>(Suckable);

			if (PoolObject->HasValidPool())
			{
				PoolObject->ReturnToPool();
			}
			else
			{
				Suckable->Destroy();
			}
		}
	}
}

void UDustpan::OnObjectEnterToArea(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ISuckableObjects * Suckable = Cast<ISuckableObjects>(OtherActor);
	if (Suckable && !ActorsInArea.Contains(Suckable))
	{
		ActorsInArea.Add(Suckable);
	}
}

void UDustpan::OnObjectExitToArea(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ISuckableObjects * Suckable = Cast<ISuckableObjects>(OtherActor);
	if (Suckable && ActorsInArea.Contains(Suckable))
	{
		if (Suckable == Exclusive)
		{
			AActor * Suckable = Cast<AActor>(Exclusive);
			ActorsInArea[ExclusiveIndex] = ActorsInArea[ActorsInArea.Num() - 1];
			ActorsInArea[ActorsInArea.Num() - 1] = Exclusive;
			ActorsInArea.RemoveAt(ActorsInArea.Num() - 1);

			Exclusive = nullptr;
			ExclusiveIndex = -1;
		}
		else
		{
			ActorsInArea.Remove(Suckable);
			if (Exclusive)
			{
				ActorsInArea.Find(Exclusive, ExclusiveIndex);
			}
		}
	}
}

//Add a function to delegate that broadcast whenever actor dies.
void UDustpan::AddDelegateToSuck(const FScriptDelegate & Delegate)
{
	OnActorHasBeenSucked.Add(Delegate);
}

//Remove a function from life changes delegate.
void UDustpan::RemoveDelegateFromSuck(const FScriptDelegate & Delegate)
{
	OnActorHasBeenSucked.Remove(Delegate);
}

//Clear all functions from life changes delegate.
void UDustpan::RemoveAllDelegatesFromSuck()
{
	OnActorHasBeenSucked.Clear();
}

void UDustpan::DustpanSkillActivate() 
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(DustpanSkillHandle))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Skill Activated"));
		CanSuckEnemy = true;
		EnemiesSucked = 0;
		GetWorld()->GetTimerManager().SetTimer(DustpanSkillHandle, this, &UDustpan::DustpanSkillDeactivate, 1.f, false, DustpanSkillCD);
	}
}

void UDustpan::DustpanSkillDeactivate() 
{
	CanSuckEnemy = false;
}

void UDustpan::TransferState(const UDustpan & OtherDustpan)
{
	check(SuckVelocity == OtherDustpan.SuckVelocity);
	check(SuckedDistance == OtherDustpan.SuckedDistance);
	check(SuckRadio == OtherDustpan.SuckRadio);
	check(DustpanSkillCD == OtherDustpan.DustpanSkillCD);
	check(EnemiesToSuck == OtherDustpan.EnemiesToSuck);

	//Skill
	if (GetWorld()->GetTimerManager().IsTimerActive(OtherDustpan.DustpanSkillHandle))
	{
		float SkillTimeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(OtherDustpan.DustpanSkillHandle);
		GetWorld()->GetTimerManager().SetTimer(DustpanSkillHandle, this, &UDustpan::DustpanSkillDeactivate, 1.f, false, SkillTimeRemaining);
		CanSuckEnemy = OtherDustpan.CanSuckEnemy;
		EnemiesSucked = OtherDustpan.EnemiesSucked;
	}
}

void UDustpan::ActivateParticules(bool activateParticules)
{
	if (!activateParticules && !PS_DustpanComponent->bWasDeactivated)
		PS_DustpanComponent->DeactivateSystem();
	
	if (activateParticules && PS_DustpanComponent->bWasDeactivated)
		PS_DustpanComponent->ActivateSystem(true);
	
}


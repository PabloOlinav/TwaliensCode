// All rights reserved.

#include "CrypteckArea.h"
#include "ConstructorHelpers.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Twalien.h"
#include "SharedLifeComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACrypteckArea::ACrypteckArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrypteckAreaMesh"));
	Mesh->SetCollisionProfileName(FName("NoCollision"));
	RootComponent = Mesh;

	MeshTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("CrypteckAreaTrigger"));
	MeshTrigger->SetSphereRadius(160.f);
	MeshTrigger->SetGenerateOverlapEvents(true);
	MeshTrigger->SetNotifyRigidBodyCollision(false);
	MeshTrigger->SetCollisionProfileName(FName("Custom..."));
	MeshTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshTrigger->SetCollisionObjectType(ECC_WorldDynamic);
	MeshTrigger->SetCollisionResponseToAllChannels(ECR_Overlap);
	MeshTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACrypteckArea::OnAreaEnter);
	MeshTrigger->OnComponentEndOverlap.AddDynamic(this, &ACrypteckArea::OnAreaExit);
	MeshTrigger->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACrypteckArea::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACrypteckArea::OnAreaEnter(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ATwalien * Player = Cast<ATwalien>(OtherActor);
	if (Player && Cast<UCapsuleComponent>(OtherComp) && !PlayerInArea.Contains(OtherActor))
	{
		PlayerInArea.Add(Player);
	}
}

void ACrypteckArea::OnAreaExit(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ATwalien * Player = Cast<ATwalien>(OtherActor);
	if (Player && Cast<UCapsuleComponent>(OtherComp) && PlayerInArea.Contains(OtherActor))
	{
		PlayerInArea.Remove(Player);
	}
}

void ACrypteckArea::DoDamageToPlayer(float DeltaTime) const
{
	for (ATwalien * Player : PlayerInArea)
	{
		Player->GetLifeComponent()->SetHitPoints(-DPS * DeltaTime);
	}
}

// Called every frame
void ACrypteckArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoDamageToPlayer(DeltaTime);
}

void ACrypteckArea::OnConstruct(float Percentage)
{
	if (Percentage > 1.f)
		Percentage = 1.f;
	else if (Percentage < 0)
		Percentage = 0;

	DPS = MaximumDPS * Percentage;
	OnSpecificConstruct(Percentage);
}


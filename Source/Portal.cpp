// All rights reserved.

#include "Portal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Twalien.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "CharacterManager.h"
#include "TimerManager.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	OnComponentBeginOverlapDelegate.BindUFunction(this, FName("DetectIfPlayerEnter"));

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>("Portal Mesh");
	RootComponent = PortalMesh;

	PortalTrigger = CreateDefaultSubobject<UBoxComponent>("Portal Trigger");
	PortalTrigger->SetupAttachment(RootComponent);
	PortalTrigger->OnComponentBeginOverlap.Add(OnComponentBeginOverlapDelegate);

}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();

	TActorIterator<ACharacterManager> Iterator(GetWorld());
	PlayerManager = *Iterator;

}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::RestoreTeleport()
{
	CanTeleport = true;
}

void APortal::DetectIfPlayerEnter(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ATwalien * Player;
	Player = Cast<ATwalien>(OtherActor);

	if(Player && CanTeleport && OtherPortal && OtherPortal->CanTeleport)
	{
		PlayerManager->GetPlayer(0)->SetActorLocation(OtherPortal->GetActorLocation() + OtherPortalTeleportOffset);
		PlayerManager->GetPlayer(1)->SetActorLocation(OtherPortal->GetActorLocation() + OtherPortalTeleportOffset + OtherPlayersOffset);
		CanTeleport = false;
		if(TwoWayPortal)
		{
			GetWorld()->GetTimerManager().SetTimer(CDTimer, this, &APortal::RestoreTeleport, TeleportCooldown, false, TeleportCooldown);
		}
	}
}
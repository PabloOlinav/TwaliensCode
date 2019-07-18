// All rights reserved.

#include "CryptecksLevel.h"
#include "EngineUtils.h"
#include "Crypteck.h"
#include "GameFramework/Actor.h"
#include "FireBall.h"
#include "ToxicBall.h"
#include "SpawnBall.h"
#include "SmashBall.h"
#include "Engine/Note.h"


ACryptecksLevel::ACryptecksLevel() : CurrentOuterIndex(0), LevelCenter(0, 0, 0)
{

}

void ACryptecksLevel::InitializeCells()
{
	TActorIterator<ANote> Points(GetWorld());
	int ToxicAmount = 0, FireAmount = 0;
	
	//Initialize inner 'toxic' areas
	InnerCells.Reserve(AmountToxicAreas);
	InnerFreeCells.Reserve(AmountToxicAreas);
	InnerCellsPositions.Reserve(AmountToxicAreas);

	//Initialize outer 'fire' areas
	OuterCells.Reserve(AmountFireAreas);
	OuterFreeCells.Reserve(AmountFireAreas);
	OuterCellsPositions.Reserve(AmountFireAreas);

	for (TActorIterator<ANote> It = Points; It; ++It)
	{
		if ((*It)->GetName().Contains("Toxic"))
		{
			InnerCells.Push(false);
			InnerFreeCells.Push(ToxicAmount++);
			InnerCellsPositions.Push((*It)->GetActorLocation());
		}
		else if ((*It)->GetName().Contains("Burning"))
		{
			OuterCells.Push(false);
			OuterFreeCells.Push(FireAmount++);
			OuterCellsPositions.Push((*It)->GetActorLocation());
		}
	}
}

void ACryptecksLevel::InitializeCrypteckReferences()
{
	//We assume that all object are unique.

	TActorIterator<ACrypteck> CrypteckIterator(GetWorld());
	Crypteck = *CrypteckIterator;

	TActorIterator<AFireBall> CrypteckFireBallIterator(GetWorld());
	CryptecksFireBall = *CrypteckFireBallIterator;

	TActorIterator<AToxicBall> CrypteckToxicBallIterator(GetWorld());
	CryptecksToxicBall = *CrypteckToxicBallIterator;

	TActorIterator<ASpawnBall> CrypteckSpawnBallIterator(GetWorld());
	CryptecksSpawnBall = *CrypteckSpawnBallIterator;

	TActorIterator<ASmashBall> CrypteckSmashBallIterator(GetWorld());
	CryptecksSmashBall = *CrypteckSmashBallIterator;

	Crypteck->SetupBallLinks(CryptecksToxicBall, CryptecksSmashBall, CryptecksFireBall, CryptecksSpawnBall);
}

void ACryptecksLevel::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeCrypteckReferences();

	InitializeCells();
}

void ACryptecksLevel::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool ACryptecksLevel::QueryInnerFreeCell(FVector &Position)
{
	if (InnerFreeCells.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, InnerFreeCells.Num() - 1);
		int NewCellIndex = InnerFreeCells[Index];
		InnerFreeCells.RemoveAt(Index);
		if (InnerCells[NewCellIndex])
		{
			UE_LOG(LogTemp, Warning, TEXT("A non free cell is queried."));
		}
		InnerCells[NewCellIndex] = true;
		Position = InnerCellsPositions[NewCellIndex];
		return true;
	}
	else
	{
		return false;
	}
}

bool ACryptecksLevel::QueryOuterFreeCell(FVector &Position)
{
	if (OuterFreeCells.Num() > 0)
	{
		if (OuterCells[CurrentOuterIndex])
		{
			UE_LOG(LogTemp, Warning, TEXT("A non free cell is queried."));
		}
		OuterCells[CurrentOuterIndex] = true;
		Position = OuterCellsPositions[CurrentOuterIndex++];
		return true;
	}
	else
	{
		return false;
	}
}

void ACryptecksLevel::ReturnInnerCell(const FVector & Position)
{
	for(int i = 0; i < InnerCellsPositions.Num(); ++i)
	{
		if (FMath::Abs(Position.X - InnerCellsPositions[i].X) < 0.1f && FMath::Abs(Position.Y - InnerCellsPositions[i].Y) < 0.1f)
		{
			InnerFreeCells.Push(i);
			i = InnerCellsPositions.Num();
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGeneratorActor.h"
#include "MainRoom.h"
#include "Room.h"
#include "Corrider.h"
#include "EngineUtils.h"

// Sets default values
AMapGeneratorActor::AMapGeneratorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	mapScale = 10;
	ChunkDistance = 2000;
}

// Called when the game starts or when spawned
void AMapGeneratorActor::BeginPlay()
{
	Super::BeginPlay();

	GeneratingMap();
}

// Called every frame
void AMapGeneratorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapGeneratorActor::GeneratingMap()
{
	FActorSpawnParameters tParams;
	
	// 메인 룸 스폰
	AMainRoom* Main = GetWorld()->SpawnActor<AMainRoom>(AMainRoom::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, tParams);
	
	ACorrider* upCorrider = GetWorld()->SpawnActor<ACorrider>(ACorrider::StaticClass(), FVector(GetActorLocation().X + ChunkDistance, GetActorLocation().Y, GetActorLocation().Z), FRotator::ZeroRotator, tParams);
	ACorrider* rightCorrider = GetWorld()->SpawnActor<ACorrider>(ACorrider::StaticClass(), FVector(GetActorLocation().X, GetActorLocation().Y + (2 * ChunkDistance), GetActorLocation().Z), FRotator::ZeroRotator, tParams);
	ACorrider* leftCorrider = GetWorld()->SpawnActor<ACorrider>(ACorrider::StaticClass(), FVector(GetActorLocation().X, GetActorLocation().Y - ChunkDistance, GetActorLocation().Z), FRotator::ZeroRotator, tParams);
	
	upCorrider->SetDoor(FString("down"), true);
	rightCorrider->SetDoor(FString("left"), true);
	leftCorrider->SetDoor(FString("right"), true);

	AChunk* root_1 = upCorrider;
	AChunk* root_2 = rightCorrider;
	AChunk* root_3 = leftCorrider;

	//root 1
	for (int i = 0; i < mapScale; i++)
	{
		root_1 = GeneratingChunk(root_1, mapScale);

		if (root_1 == nullptr)
			break;
	}

	//root 2
	for (int i = 0; i < mapScale; i++)
	{
		root_2 = GeneratingChunk(root_2, mapScale);

		if (root_2 == nullptr)
			break;
	}

	//root 3
	for (int i = 0; i < mapScale; i++)
	{
		root_3 = GeneratingChunk(root_3, mapScale);

		if (root_3 == nullptr)
			break;
	}

	for (TActorIterator<AChunk> Chunk(GetWorld()); Chunk; ++Chunk)
	{
		if (Chunk->ActorHasTag("way"))
		{
			Chunk->SpawnTypeActor();
		}
	}
}

AChunk* AMapGeneratorActor::GeneratingChunk(AChunk* CurrentChunk, int count)
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(CurrentChunk);

	AChunk* nextChunk = CurrentChunk;
	TArray<bool> doorList;

	bool upHitSuccess;
	bool rightHitSuccess;
	bool downHitSuccess;
	bool leftHitSuccess;
	FHitResult upHitResult;
	FHitResult rightHitResult;
	FHitResult downHitResult;
	FHitResult leftHitResult;

	upHitSuccess = GetWorld()->LineTraceSingleByChannel(
		upHitResult,
		CurrentChunk->GetActorLocation(),
		FVector(CurrentChunk->GetActorLocation().X + ChunkDistance, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z),
		ECC_Visibility,
		QueryParams
	);

	rightHitSuccess = GetWorld()->LineTraceSingleByChannel(
		rightHitResult,
		CurrentChunk->GetActorLocation(),
		FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + ChunkDistance, CurrentChunk->GetActorLocation().Z),
		ECC_Visibility,
		QueryParams
	);

	downHitSuccess = GetWorld()->LineTraceSingleByChannel(
		downHitResult,
		CurrentChunk->GetActorLocation(),
		FVector(CurrentChunk->GetActorLocation().X - ChunkDistance, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z),
		ECC_Visibility,
		QueryParams
	);

	leftHitSuccess = GetWorld()->LineTraceSingleByChannel(
		leftHitResult,
		CurrentChunk->GetActorLocation(),
		FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - ChunkDistance, CurrentChunk->GetActorLocation().Z),
		ECC_Visibility,
		QueryParams
	);
	
	/*
	* 위가 막혀있는 경우
	*/
	if(upHitSuccess && !rightHitSuccess && !downHitSuccess && !leftHitSuccess)
	{
		// 방을 스폰시킬지 결정
		if (FMath::RandRange(0, 1) == 0)		// 방을 스폰
		{
			/*
			* 방을 스폰 시킨 뒤, 복도를 하나 더 스폰.
			* Door가 있는 부분도 함께 저장해준다.
			*/
			doorList = { false, true, true, true };
			doorList = SpawnRoom(CurrentChunk, doorList);
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
		else									// 복도를 스폰
		{
			doorList = { false, true, true, true };
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
	}

	/*
	* 오른쪽이 막혀있는 경우
	*/
	else if (!upHitSuccess && rightHitSuccess && !downHitSuccess && !leftHitSuccess)
	{
		// 방을 스폰시킬지 결정
		if (FMath::RandRange(0, 1) == 0)		// 방을 스폰
		{
			/*
			* 방을 스폰 시킨 뒤, 복도를 하나 더 스폰.
			* Door가 있는 부분도 함께 저장해준다.
			*/
			doorList = { true, false, true, true };
			doorList = SpawnRoom(CurrentChunk, doorList);
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
		else									// 복도를 스폰
		{
			doorList = { true, false, true, true };
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
	}

	/*
	* 아래가 막혀있는 경우
	*/
	else if (!upHitSuccess && !rightHitSuccess && downHitSuccess && !leftHitSuccess)
	{
		// 방을 스폰시킬지 결정
		if (FMath::RandRange(0, 1) == 0)		// 방을 스폰
		{
			/*
			* 방을 스폰 시킨 뒤, 복도를 하나 더 스폰.
			* Door가 있는 부분도 함께 저장해준다.
			*/
			doorList = { true, true, false, true };
			doorList = SpawnRoom(CurrentChunk, doorList);
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
		else									// 복도를 스폰
		{
			doorList = { true, true, false, true };
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
	}

	/*
	* 왼쪽이 막혀있는 경우
	*/
	else if (!upHitSuccess && !rightHitSuccess && !downHitSuccess && leftHitSuccess)
	{
		// 방을 스폰시킬지 결정
		if (FMath::RandRange(0, 1) == 0)		// 방을 스폰
		{
			/*
			* 방을 스폰 시킨 뒤, 복도를 하나 더 스폰.
			* Door가 있는 부분도 함께 저장해준다.
			*/
			doorList = { true, true, true, false};
			doorList = SpawnRoom(CurrentChunk, doorList);
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
		else									// 복도를 스폰
		{
			doorList = { true, true, true, false };
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
	}
	
	/*
	* 위와 오른쪽이 막혀있는 경우
	*/
	else if (upHitSuccess && rightHitSuccess && !downHitSuccess && !leftHitSuccess)
	{
		// 방을 스폰시킬지 결정
		if (FMath::RandRange(0, 1) == 0)		// 방을 스폰
		{
			/*
			* 방을 스폰 시킨 뒤, 복도를 하나 더 스폰.
			* Door가 있는 부분도 함께 저장해준다.
			*/
			doorList = { false, false, true, true };
			doorList = SpawnRoom(CurrentChunk, doorList);
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
		else									// 복도를 스폰
		{
			doorList = { false, false, true, true };
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}

		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
	}

	/*
	* 오른쪽과 아래가 막혀있는 경우
	*/
	else if (!upHitSuccess && rightHitSuccess && downHitSuccess && !leftHitSuccess)
	{
		// 방을 스폰시킬지 결정
		if (FMath::RandRange(0, 1) == 0)		// 방을 스폰
		{
			/*
			* 방을 스폰 시킨 뒤, 복도를 하나 더 스폰.
			* Door가 있는 부분도 함께 저장해준다.
			*/
			doorList = { true, false, false, true };
			doorList = SpawnRoom(CurrentChunk, doorList);
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
		else									// 복도를 스폰
		{
			doorList = { true, false, false, true };
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}

		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
	}

	/*
	* 아래와 왼쪽이 막혀있는 경우
	*/
	else if (!upHitSuccess && !rightHitSuccess && downHitSuccess && leftHitSuccess)
	{
		// 방을 스폰시킬지 결정
		if (FMath::RandRange(0, 1) == 0)		// 방을 스폰
		{
			/*
			* 방을 스폰 시킨 뒤, 복도를 하나 더 스폰.
			* Door가 있는 부분도 함께 저장해준다.
			*/
			doorList = { true, true, false, false};
			doorList = SpawnRoom(CurrentChunk, doorList);
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
		else									// 복도를 스폰
		{
			doorList = { true, true, false, false };
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}

		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
	}

	/*
	* 위와 왼쪽이 막혀있는 경우
	*/
	else if (upHitSuccess && !rightHitSuccess && !downHitSuccess && leftHitSuccess)
	{
		// 방을 스폰시킬지 결정
		if (FMath::RandRange(0, 1) == 0)		// 방을 스폰
		{
			/*
			* 방을 스폰 시킨 뒤, 복도를 하나 더 스폰.
			* Door가 있는 부분도 함께 저장해준다.
			*/
			doorList = { false, true, true, false };
			doorList = SpawnRoom(CurrentChunk, doorList);
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
		else									// 복도를 스폰
		{
			doorList = { false, true, true, false };
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}

		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
	}

	/*
	* 위와 아래가 막혀있는 경우
	*/
	else if (upHitSuccess && !rightHitSuccess && downHitSuccess && !leftHitSuccess)
	{
		// 방을 스폰시킬지 결정
		if (FMath::RandRange(0, 1) == 0)		// 방을 스폰
		{
			/*
			* 방을 스폰 시킨 뒤, 복도를 하나 더 스폰.
			* Door가 있는 부분도 함께 저장해준다.
			*/
			doorList = { false, true, false, true };
			doorList = SpawnRoom(CurrentChunk, doorList);
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
		else									// 복도를 스폰
		{
			doorList = { false, true, false, true };
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}

		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
	}

	/*
	* 왼쪽과 오른쪽이 막혀있는 경우
	*/
	else if (upHitSuccess && !rightHitSuccess && !downHitSuccess && leftHitSuccess)
	{
		// 방을 스폰시킬지 결정
		if (FMath::RandRange(0, 1) == 0)		// 방을 스폰
		{
			/*
			* 방을 스폰 시킨 뒤, 복도를 하나 더 스폰.
			* Door가 있는 부분도 함께 저장해준다.
			*/
			doorList = {true,  false, true, false };
			doorList = SpawnRoom(CurrentChunk, doorList);
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
		else									// 복도를 스폰
		{
			doorList = { true,  false, true, false };
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}

		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
	}

	/*
	* 위와 오른쪽, 아래가 막혀있는 경우
	*/
	else if (upHitSuccess && rightHitSuccess && downHitSuccess && !leftHitSuccess)
	{
		// 방을 스폰시킬지 결정
		if (FMath::RandRange(0, 1) == 0)		// 방을 스폰
		{
			/*
			* 방을 스폰 시킨 뒤, 복도를 하나 더 스폰.
			* Door가 있는 부분도 함께 저장해준다.
			*/
			doorList = { false, false, false, true };
			doorList = SpawnRoom(CurrentChunk, doorList);
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
		else									// 복도를 스폰
		{
			doorList = { false, false, false, true };
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}

		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
	}

	/*
	* 오른쪽과 아래, 왼쪽이 막혀있는 경우
	*/
	else if (!upHitSuccess && rightHitSuccess && downHitSuccess && leftHitSuccess)
	{
		// 방을 스폰시킬지 결정
		if (FMath::RandRange(0, 1) == 0)		// 방을 스폰
		{
			/*
			* 방을 스폰 시킨 뒤, 복도를 하나 더 스폰.
			* Door가 있는 부분도 함께 저장해준다.
			*/
			doorList = { true,  false, false, false };
			doorList = SpawnRoom(CurrentChunk, doorList);
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
		else									// 복도를 스폰
		{
			doorList = { true,  false, false, false };
			SpawnCorrider(CurrentChunk, doorList);
		}

		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
	}

	/*
	*아래와 왼쪽, 위가 막혀있는 경우
	*/
	else if (!upHitSuccess && rightHitSuccess && downHitSuccess && leftHitSuccess)
	{
		// 방을 스폰시킬지 결정
		if (FMath::RandRange(0, 1) == 0)		// 방을 스폰
		{
			/*
			* 방을 스폰 시킨 뒤, 복도를 하나 더 스폰.
			* Door가 있는 부분도 함께 저장해준다.
			*/
			doorList = { false, true, false, false };
			doorList = SpawnRoom(CurrentChunk, doorList);
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
		else									// 복도를 스폰
		{
			doorList = { false, true, false, false };
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}

		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
	}

	/*
	*왼쪽과 위, 오른쪽이 막혀있는 경우
	*/
	else if (upHitSuccess && rightHitSuccess && !downHitSuccess && leftHitSuccess)
	{
		// 방을 스폰시킬지 결정
		if (FMath::RandRange(0, 1) == 0)		// 방을 스폰
		{
			/*
			* 방을 스폰 시킨 뒤, 복도를 하나 더 스폰.
			* Door가 있는 부분도 함께 저장해준다.
			*/
			doorList = { false,  false, true, false };
			doorList = SpawnRoom(CurrentChunk, doorList);
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}
		else									// 복도를 스폰
		{
			doorList = { false,  false, true, false };
			nextChunk = SpawnCorrider(CurrentChunk, doorList);
		}

		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
	}

	/*
	*모든 곳이 막혀있는 경우
	*/
	else if (upHitSuccess && rightHitSuccess && downHitSuccess && leftHitSuccess)
	{
		return nullptr;
	}

	return nextChunk;
}

void AMapGeneratorActor::MakingDoor(AChunk* currentChunk, FHitResult upHitResult, FHitResult rightHitResult, FHitResult downHitResult, FHitResult leftHitResult)
{
	if (upHitResult.bBlockingHit)
	{
		if (upHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(upHitResult.Actor)->SetDoor(FString("down"), true);
			currentChunk->SetDoor(FString("up"), true);
		}
	}
	if (rightHitResult.bBlockingHit)
	{
		if (rightHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(rightHitResult.Actor)->SetDoor(FString("left"), true);
			currentChunk->SetDoor(FString("right"), true);
		}
	}
	if (downHitResult.bBlockingHit)
	{
		if (downHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(downHitResult.Actor)->SetDoor(FString("up"), true);
			currentChunk->SetDoor(FString("down"), true);
		}
	}
	if (leftHitResult.bBlockingHit)
	{
		if (leftHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(leftHitResult.Actor)->SetDoor(FString("right"), true);
			currentChunk->SetDoor(FString("left"), true);
		}
	}
}

TArray<bool> AMapGeneratorActor::SpawnRoom(AChunk* CurrentChunk, TArray<bool> DoorList)
{
	int8 RandomNum = -1;
	bool UpDoor = DoorList[0];
	bool RightDoor = DoorList[1];
	bool DownDoor = DoorList[2];
	bool LeftDoor = DoorList[3];

	while (((RandomNum == 0 && !UpDoor) || (RandomNum == 1 && !RightDoor) || (RandomNum == 2 && !DownDoor) || (RandomNum == 3 && !LeftDoor)) || RandomNum == -1)
		RandomNum = FMath::RandRange(0, 3);

	switch (RandomNum)
	{
		// UP
	case 0:
		SpawnRoomActor(FVector(CurrentChunk->GetActorLocation().X + ChunkDistance, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
		CurrentChunk->SetDoor(FString("up"), true);
		UpDoor = !UpDoor;
		break;
		// RIGHT
	case 1:
		SpawnRoomActor(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + ChunkDistance, CurrentChunk->GetActorLocation().Z), FString("left"));
		CurrentChunk->SetDoor(FString("right"), true);
		RightDoor = !RightDoor;
		break;
		// DOWN
	case 2:
		SpawnRoomActor(FVector(CurrentChunk->GetActorLocation().X - ChunkDistance, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
		CurrentChunk->SetDoor(FString("down"), true);
		DownDoor = !DownDoor;
		break;
		// LEFT
	case 3:
		SpawnRoomActor(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - ChunkDistance, CurrentChunk->GetActorLocation().Z), FString("right"));
		CurrentChunk->SetDoor(FString("left"), true);
		LeftDoor = !LeftDoor;
		break;
	default:
		break;
	}

	return { UpDoor, RightDoor, DownDoor, LeftDoor };
}

AChunk* AMapGeneratorActor::SpawnCorrider(AChunk* CurrentChunk, TArray<bool> DoorList)
{
	int8 RandomNum = -1;
	AChunk* returnCorrider = CurrentChunk;
	bool UpDoor = DoorList[0];
	bool RightDoor = DoorList[1];
	bool DownDoor = DoorList[2];
	bool LeftDoor = DoorList[3];

	while (((RandomNum == 0 && !UpDoor) || (RandomNum == 1 && !RightDoor) || (RandomNum == 2 && !DownDoor) || (RandomNum == 3 && !LeftDoor)) || RandomNum == -1)
		RandomNum = FMath::RandRange(0, 3);

	UE_LOG(LogTemp, Warning, TEXT("%d"), RandomNum);
	switch (RandomNum)
	{
		// UP
	case 0:
		returnCorrider = SpawnCorriderActor(FVector(CurrentChunk->GetActorLocation().X + ChunkDistance, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
		CurrentChunk->SetDoor(FString("up"), true);
		UpDoor = !UpDoor;
		break;
		// RIGHT
	case 1:
		returnCorrider = SpawnCorriderActor(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + ChunkDistance, CurrentChunk->GetActorLocation().Z), FString("left"));
		CurrentChunk->SetDoor(FString("right"), true);
		RightDoor = !RightDoor;
		break;
		// DOWN
	case 2:
		returnCorrider = SpawnCorriderActor(FVector(CurrentChunk->GetActorLocation().X - ChunkDistance, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
		CurrentChunk->SetDoor(FString("down"), true);
		DownDoor = !DownDoor;
		break;
		// LEFT
	case 3:
		returnCorrider = SpawnCorriderActor(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - ChunkDistance, CurrentChunk->GetActorLocation().Z), FString("right"));
		CurrentChunk->SetDoor(FString("left"), true);
		LeftDoor = !LeftDoor;
		break;
	default:
		returnCorrider = nullptr;
		break;
	}

	return returnCorrider;
}

void AMapGeneratorActor::SpawnRoomActor(FVector location, FString doorLocation)
{
	FActorSpawnParameters tParams;
	ARoom* Room = GetWorld()->SpawnActor<ARoom>(ARoom::StaticClass(), location, FRotator::ZeroRotator, tParams);
	Room->SetDoor(doorLocation, true);
}

AChunk* AMapGeneratorActor::SpawnCorriderActor(FVector location, FString doorLocation)
{
	FActorSpawnParameters tParams;
	ACorrider* Corrider = GetWorld()->SpawnActor<ACorrider>(ACorrider::StaticClass(), location, FRotator::ZeroRotator, tParams);
	Corrider->SetDoor(doorLocation, true);

	return Corrider;
}
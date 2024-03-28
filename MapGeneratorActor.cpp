// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGeneratorActor.h"
#include "MainRoom.h"
#include "Room.h"
#include "Corrider.h"

// Sets default values
AMapGeneratorActor::AMapGeneratorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	mapScale = 3;
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
	//FVector root_1 = FVector(GetActorLocation().X, GetActorLocation().Y + 1000, GetActorLocation().Z);
	//FVector root_2 = FVector(GetActorLocation().X - 1000, GetActorLocation().Y, GetActorLocation().Z);
	//FVector root_3 = FVector(GetActorLocation().X, GetActorLocation().Y - 1000, GetActorLocation().Z);
	
	// ���� �� ����
	AMainRoom* Main = GetWorld()->SpawnActor<AMainRoom>(AMainRoom::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, tParams);
	
	//ACorrider* upCorrider = GetWorld()->SpawnActor<ACorrider>(ACorrider::StaticClass(), FVector(GetActorLocation().X + ((i + 1) * 1000), GetActorLocation().Y, GetActorLocation().Z), FRotator::ZeroRotator, tParams);
	ACorrider* rightCorrider = GetWorld()->SpawnActor<ACorrider>(ACorrider::StaticClass(), FVector(GetActorLocation().X, GetActorLocation().Y + 1000, GetActorLocation().Z), FRotator::ZeroRotator, tParams);
	ACorrider* downCorrider = GetWorld()->SpawnActor<ACorrider>(ACorrider::StaticClass(), FVector(GetActorLocation().X - 1000, GetActorLocation().Y, GetActorLocation().Z), FRotator::ZeroRotator, tParams);
	ACorrider* leftCorrider = GetWorld()->SpawnActor<ACorrider>(ACorrider::StaticClass(), FVector(GetActorLocation().X, GetActorLocation().Y - 1000, GetActorLocation().Z), FRotator::ZeroRotator, tParams);
	
	rightCorrider->SetDoor(FString("left"), true);
	downCorrider->SetDoor(FString("up"), true);
	leftCorrider->SetDoor(FString("right"), true);

	AChunk* root_1 = rightCorrider;
	AChunk* root_2 = downCorrider;
	AChunk* root_3 = leftCorrider;

	//root 1
	for (int i = 0; i < 10; i++)
	{
		root_1 = GeneratingChunk(root_1);

		if (root_1 == nullptr)
			break;
	}

	//root 2
	/*for (int i = 0; i < 10; i++)
	{
		root_2 = GeneratingChunk(root_2);

		if (root_2 == nullptr)
			break;
	}*/

	//root 3
	/*for (int i = 0; i < 10; i++)
	{
		root_3 = GeneratingChunk(root_3);

		if (root_3 == nullptr)
			break;
	}*/
}

AChunk* AMapGeneratorActor::SpawnRoom(FVector location, FString doorLocation)
{
	FActorSpawnParameters tParams;
	ARoom* Room = GetWorld()->SpawnActor<ARoom>(ARoom::StaticClass(), location, FRotator::ZeroRotator, tParams);
	Room->SetDoor(doorLocation, true);

	return Room;
}

AChunk* AMapGeneratorActor::SpawnCorrider(FVector location, FString doorLocation)
{
	FActorSpawnParameters tParams;
	ACorrider* Corrider = GetWorld()->SpawnActor<ACorrider>(ACorrider::StaticClass(), location, FRotator::ZeroRotator, tParams);
	Corrider->SetDoor(doorLocation, true);

	return Corrider;
}

AChunk* AMapGeneratorActor::GeneratingChunk(AChunk* CurrentChunk)
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(CurrentChunk);

	AChunk* returnActor = CurrentChunk;

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
		FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z),
		ECC_Visibility,
		QueryParams
	);

	rightHitSuccess = GetWorld()->LineTraceSingleByChannel(
		rightHitResult,
		CurrentChunk->GetActorLocation(),
		FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z),
		ECC_Visibility,
		QueryParams
	);

	downHitSuccess = GetWorld()->LineTraceSingleByChannel(
		downHitResult,
		CurrentChunk->GetActorLocation(),
		FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z),
		ECC_Visibility,
		QueryParams
	);

	leftHitSuccess = GetWorld()->LineTraceSingleByChannel(
		leftHitResult,
		CurrentChunk->GetActorLocation(),
		FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z),
		ECC_Visibility,
		QueryParams
	);
	
	/*
	* ���� �����ִ� ���
	*/
	if(upHitSuccess && !rightHitSuccess && !downHitSuccess && !leftHitSuccess)
	{
		// ���� ������ų�� ����
		if (FMath::RandRange(0, 1) == 0)		// ���� ����
		{
			/*
			* ���� ���� ��Ų ��, ������ �ϳ� �� ������ ������ ����.
			* Door�� �ִ� �κе� �Բ� �������ش�.
			*/
			int8 roomLocation;

			switch (FMath::RandRange(0, 2))
			{
				// RIGHT
			case 0:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
				roomLocation = 1;
				CurrentChunk->SetDoor(FString("right"), true);
				break;
				// DOWN
			case 1:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
				roomLocation = 2;
				CurrentChunk->SetDoor(FString("down"), true);
				break;
				// LEFT
			case 2:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
				roomLocation = 3;
				CurrentChunk->SetDoor(FString("left"), true);
				break;
			default:
				break;
			}

			// ���� �ձ� or �� �Ѱ��� ��
			// if ���� ���� FMath::RandRange(0, 1) == 0
			if (true)
			{
				int8 checkCanGo = FMath::RandRange(0, 2);
				TArray<int8> roomNumber = {1, 2, 3};

				while (roomNumber[checkCanGo] == roomLocation)
					checkCanGo = FMath::RandRange(0, 2);

				switch (checkCanGo)
				{
					// RIGHT
				case 0:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
					CurrentChunk->SetDoor(FString("right"), true);
					break;
					// DOWN
				case 1:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
					CurrentChunk->SetDoor(FString("down"), true);
					break;
					// LEFT
				case 2:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
					CurrentChunk->SetDoor(FString("left"), true);
					break;
				default:
					break;
				}
			}
			else
				return nullptr;

		}
		else									// ������ ����
		{
			switch (FMath::RandRange(0, 2))
			{
				// RIGHT
			case 0:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
				CurrentChunk->SetDoor(FString("right"), true);
				break;
				// DOWN
			case 1:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
				CurrentChunk->SetDoor(FString("down"), true);
				break;
				// LEFT
			case 2:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
				CurrentChunk->SetDoor(FString("left"), true);
				break;
			default:
				break;
			}
		}
	}

	/*
	* �������� �����ִ� ���
	*/
	else if (!upHitSuccess && rightHitSuccess && !downHitSuccess && !leftHitSuccess)
	{
		// ���� ������ų�� ����
		if (FMath::RandRange(0, 1) == 0)		// ���� ����
		{
			/*
			* ���� ���� ��Ų ��, ������ �ϳ� �� ������ ������ ����.
			* Door�� �ִ� �κе� �Բ� �������ش�.
			*/
			int8 roomLocation;

			switch (FMath::RandRange(0, 2))
			{
				// UP
			case 0:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
				roomLocation = 0;
				CurrentChunk->SetDoor(FString("up"), true);
				break;
				// DOWN
			case 1:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
				roomLocation = 2;
				CurrentChunk->SetDoor(FString("down"), true);
				break;
				// LEFT
			case 2:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
				roomLocation = 3;
				CurrentChunk->SetDoor(FString("left"), true);
				break;
			default:
				break;
			}

			// ���� �ձ� or �� �Ѱ��� ��
			// if ���� ���� FMath::RandRange(0, 1) == 0
			if (true)
			{
				int8 checkCanGo = FMath::RandRange(0, 2);
				TArray<int8> roomNumber = { 0, 2, 3 };

				while (roomNumber[checkCanGo] == roomLocation)
					checkCanGo = FMath::RandRange(0, 2);

				switch (checkCanGo)
				{
					// UP
				case 0:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
					CurrentChunk->SetDoor(FString("up"), true);
					break;
					// DOWN
				case 1:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
					CurrentChunk->SetDoor(FString("down"), true);
					break;
					// LEFT
				case 2:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
					CurrentChunk->SetDoor(FString("left"), true);
					break;
				default:
					break;
				}
			}
			else
				return nullptr;

		}
		else									// ������ ����
		{
			switch (FMath::RandRange(0, 2))
			{
				// UP
			case 0:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
				CurrentChunk->SetDoor(FString("up"), true);
				break;
				// DOWN
			case 1:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
				CurrentChunk->SetDoor(FString("down"), true);
				break;
				// LEFT
			case 2:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
				CurrentChunk->SetDoor(FString("left"), true);
				break;
			default:
				break;
			}
		}
	}

	/*
	* �Ʒ��� �����ִ� ���
	*/
	else if (!upHitSuccess && !rightHitSuccess && downHitSuccess && !leftHitSuccess)
	{
		// ���� ������ų�� ����
		if (FMath::RandRange(0, 1) == 0)		// ���� ����
		{
			/*
			* ���� ���� ��Ų ��, ������ �ϳ� �� ������ ������ ����.
			* Door�� �ִ� �κе� �Բ� �������ش�.
			*/
			int8 roomLocation;

			switch (FMath::RandRange(0, 2))
			{
				// UP
			case 0:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
				roomLocation = 0;
				CurrentChunk->SetDoor(FString("up"), true);
				break;
				// RIGHT
			case 1:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
				roomLocation = 1;
				CurrentChunk->SetDoor(FString("right"), true);
				break;
				// LEFT
			case 2:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
				roomLocation = 3;
				CurrentChunk->SetDoor(FString("left"), true);
				break;
			default:
				break;
			}

			// ���� �ձ� or �� �Ѱ��� ��
			// if ���� ���� FMath::RandRange(0, 1) == 0
			if (true)
			{
				int8 checkCanGo = FMath::RandRange(0, 2);
				TArray<int8> roomNumber = { 0, 1, 3 };

				while (roomNumber[checkCanGo] == roomLocation)
					checkCanGo = FMath::RandRange(0, 2);

				switch (checkCanGo)
				{
					// UP
				case 0:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
					CurrentChunk->SetDoor(FString("up"), true);
					break;
					// RIGHT
				case 1:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
					CurrentChunk->SetDoor(FString("right"), true);
					break;
					// LEFT
				case 2:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
					CurrentChunk->SetDoor(FString("left"), true);
					break;
				default:
					break;
				}
			}
			else
				return nullptr;

		}
		else									// ������ ����
		{
			switch (FMath::RandRange(0, 2))
			{
				// UP
			case 0:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
				CurrentChunk->SetDoor(FString("up"), true);
				break;
				// RIGHT
			case 1:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
				CurrentChunk->SetDoor(FString("right"), true);
				break;
				// LEFT
			case 2:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
				CurrentChunk->SetDoor(FString("left"), true);
				break;
			default:
				break;
			}
		}
	}

	/*
	* ������ �����ִ� ���
	*/
	else if (!upHitSuccess && !rightHitSuccess && !downHitSuccess && leftHitSuccess)
	{
		// ���� ������ų�� ����
		if (FMath::RandRange(0, 1) == 0)		// ���� ����
		{
			/*
			* ���� ���� ��Ų ��, ������ �ϳ� �� ������ ������ ����.
			* Door�� �ִ� �κе� �Բ� �������ش�.
			*/
			int8 roomLocation;

			switch (FMath::RandRange(0, 2))
			{
				// UP
			case 0:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
				roomLocation = 0;
				CurrentChunk->SetDoor(FString("up"), true);
				break;
				// RIGHT
			case 1:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
				roomLocation = 1;
				CurrentChunk->SetDoor(FString("right"), true);
				break;
				// DOWN
			case 2:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
				roomLocation = 2;
				CurrentChunk->SetDoor(FString("down"), true);
				break;
			default:
				break;
			}

			// ���� �ձ� or �� �Ѱ��� ��
			// if ���� ���� FMath::RandRange(0, 1) == 0
			if (true)
			{
				int8 checkCanGo = FMath::RandRange(0, 2);
				TArray<int8> roomNumber = { 0, 1, 2 };

				while (roomNumber[checkCanGo] == roomLocation)
					checkCanGo = FMath::RandRange(0, 2);

				switch (checkCanGo)
				{
					// UP
				case 0:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
					CurrentChunk->SetDoor(FString("up"), true);
					break;
					// RIGHT
				case 1:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
					CurrentChunk->SetDoor(FString("right"), true);
					break;
					// DOWN
				case 2:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
					CurrentChunk->SetDoor(FString("down"), true);
					break;
				default:
					break;
				}
			}
			else
				return nullptr;

		}
		else									// ������ ����
		{
			switch (FMath::RandRange(0, 2))
			{
				// UP
			case 0:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
				CurrentChunk->SetDoor(FString("up"), true);
				break;
				// RIGHT
			case 1:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
				CurrentChunk->SetDoor(FString("right"), true);
				break;
				// DOWN
			case 2:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
				CurrentChunk->SetDoor(FString("down"), true);
				break;
			default:
				break;
			}
		}
	}
	
	/*
	* ���� �������� �����ִ� ���
	*/
	else if (upHitSuccess && rightHitSuccess && !downHitSuccess && !leftHitSuccess)
	{
		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
		/*if (upHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(upHitResult.Actor)->SetDoor(FString("down"), true);
			CurrentChunk->SetDoor(FString("up"), true);
		}
		if (rightHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(rightHitResult.Actor)->SetDoor(FString("left"), true);
			CurrentChunk->SetDoor(FString("right"), true);
		}*/

		// ���� ������ų�� ����
		if (FMath::RandRange(0, 1) == 0)		// ���� ����
		{
			/*
			* ���� ���� ��Ų ��, ������ �ϳ� �� ������ ������ ����.
			* Door�� �ִ� �κе� �Բ� �������ش�.
			*/
			int8 roomLocation;

			switch (FMath::RandRange(0, 1))
			{
				// DOWN
			case 0:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
				roomLocation = 2;
				CurrentChunk->SetDoor(FString("down"), true);
				break;
				// LEFT
			case 1:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
				roomLocation = 3;
				CurrentChunk->SetDoor(FString("left"), true);
				break;
			default:
				break;
			}

			// ���� �ձ� or �� �Ѱ��� ��
			// if ���� ���� FMath::RandRange(0, 1) == 0
			if (true)
			{
				int8 checkCanGo = FMath::RandRange(0, 1);
				TArray<int8> roomNumber = { 2, 3 };

				while (roomNumber[checkCanGo] == roomLocation)
					checkCanGo = FMath::RandRange(0, 1);

				switch (checkCanGo)
				{
					// DOWN
				case 0:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
					CurrentChunk->SetDoor(FString("down"), true);
					break;
					// LEFT
				case 1:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
					CurrentChunk->SetDoor(FString("left"), true);
					break;
				default:
					break;
				}
			}
			else
				return nullptr;

		}
		else									// ������ ����
		{
			switch (FMath::RandRange(0, 1))
			{
				// DOWN
			case 0:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
				CurrentChunk->SetDoor(FString("down"), true);
				break;
				// LEFT
			case 1:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
				CurrentChunk->SetDoor(FString("left"), true);
				break;
			default:
				break;
			}
		}
	}

	/*
	* �����ʰ� �Ʒ��� �����ִ� ���
	*/
	else if (!upHitSuccess && rightHitSuccess && downHitSuccess && !leftHitSuccess)
	{
		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
		/*if (rightHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(rightHitResult.Actor)->SetDoor(FString("left"), true);
			CurrentChunk->SetDoor(FString("right"), true);
		}
		if (downHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(downHitResult.Actor)->SetDoor(FString("up"), true);
			CurrentChunk->SetDoor(FString("down"), true);
		}*/

		// ���� ������ų�� ����
		if (FMath::RandRange(0, 1) == 0)		// ���� ����
		{
			/*
			* ���� ���� ��Ų ��, ������ �ϳ� �� ������ ������ ����.
			* Door�� �ִ� �κе� �Բ� �������ش�.
			*/
			int8 roomLocation;

			switch (FMath::RandRange(0, 1))
			{
				// UP
			case 0:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
				roomLocation = 0;
				CurrentChunk->SetDoor(FString("up"), true);
				break;
				// LEFT
			case 1:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
				roomLocation = 3;
				CurrentChunk->SetDoor(FString("left"), true);
				break;
			default:
				break;
			}

			// ���� �ձ� or �� �Ѱ��� ��
			// if ���� ���� FMath::RandRange(0, 1) == 0
			if (true)
			{
				int8 checkCanGo = FMath::RandRange(0, 1);
				TArray<int8> roomNumber = { 0, 3 };

				while (roomNumber[checkCanGo] == roomLocation)
					checkCanGo = FMath::RandRange(0, 1);

				switch (checkCanGo)
				{
					// UP
				case 0:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
					CurrentChunk->SetDoor(FString("up"), true);
					break;
					// LEFT
				case 1:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
					CurrentChunk->SetDoor(FString("left"), true);
					break;
				default:
					break;
				}
			}
			else
				return nullptr;

		}
		else									// ������ ����
		{
			switch (FMath::RandRange(0, 1))
			{
				// UP
			case 0:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
				CurrentChunk->SetDoor(FString("up"), true);
				break;
				// LEFT
			case 1:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
				CurrentChunk->SetDoor(FString("left"), true);
				break;
			default:
				break;
			}
		}
	}

	/*
	* �Ʒ��� ������ �����ִ� ���
	*/
	else if (!upHitSuccess && !rightHitSuccess && downHitSuccess && leftHitSuccess)
	{
		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
		/*if (downHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(downHitResult.Actor)->SetDoor(FString("up"), true);
			CurrentChunk->SetDoor(FString("down"), true);
		}
		if (leftHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(leftHitResult.Actor)->SetDoor(FString("right"), true);
			CurrentChunk->SetDoor(FString("left"), true);
		}*/

		// ���� ������ų�� ����
		if (FMath::RandRange(0, 1) == 0)		// ���� ����
		{
			/*
			* ���� ���� ��Ų ��, ������ �ϳ� �� ������ ������ ����.
			* Door�� �ִ� �κе� �Բ� �������ش�.
			*/
			int8 roomLocation;

			switch (FMath::RandRange(0, 1))
			{
				// UP
			case 0:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
				roomLocation = 0;
				CurrentChunk->SetDoor(FString("up"), true);
				break;
				// RIGHT
			case 1:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
				roomLocation = 1;
				CurrentChunk->SetDoor(FString("right"), true);
				break;
			default:
				break;
			}

			// ���� �ձ� or �� �Ѱ��� ��
			// if ���� ���� FMath::RandRange(0, 1) == 0
			if (true)
			{
				int8 checkCanGo = FMath::RandRange(0, 1);
				TArray<int8> roomNumber = { 0, 1 };

				while (roomNumber[checkCanGo] == roomLocation)
					checkCanGo = FMath::RandRange(0, 1);

				switch (checkCanGo)
				{
					// UP
				case 0:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
					CurrentChunk->SetDoor(FString("up"), true);
					break;
					// RIGHT
				case 1:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
					CurrentChunk->SetDoor(FString("right"), true);
					break;
				default:
					break;
				}
			}
			else
				return nullptr;

		}
		else									// ������ ����
		{
			switch (FMath::RandRange(0, 1))
			{
				// UP
			case 0:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
				CurrentChunk->SetDoor(FString("up"), true);
				break;
				// RIGHT
			case 1:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
				CurrentChunk->SetDoor(FString("right"), true);
				break;
			default:
				break;
			}
		}
	}

	/*
	* ���� ������ �����ִ� ���
	*/
	else if (upHitSuccess && !rightHitSuccess && !downHitSuccess && leftHitSuccess)
	{
		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
		/*if (leftHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(leftHitResult.Actor)->SetDoor(FString("right"), true);
			CurrentChunk->SetDoor(FString("left"), true);
		}
		if (upHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(upHitResult.Actor)->SetDoor(FString("down"), true);
			CurrentChunk->SetDoor(FString("up"), true);
		}*/

		// ���� ������ų�� ����
		if (FMath::RandRange(0, 1) == 0)		// ���� ����
		{
			/*
			* ���� ���� ��Ų ��, ������ �ϳ� �� ������ ������ ����.
			* Door�� �ִ� �κе� �Բ� �������ش�.
			*/
			int8 roomLocation;

			switch (FMath::RandRange(0, 1))
			{
				break;
				// RIGHT
			case 0:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
				roomLocation = 1;
				CurrentChunk->SetDoor(FString("right"), true);
				break;
				// DOWN
			case 1:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
				roomLocation = 2;
				CurrentChunk->SetDoor(FString("down"), true);
				break;
			default:
				break;
			}

			// ���� �ձ� or �� �Ѱ��� ��
			// if ���� ���� FMath::RandRange(0, 1) == 0
			if (true)
			{
				int8 checkCanGo = FMath::RandRange(0, 1);
				TArray<int8> roomNumber = { 1, 2 };

				while (roomNumber[checkCanGo] == roomLocation)
					checkCanGo = FMath::RandRange(0, 1);

				switch (checkCanGo)
				{
					// RIGHT
				case 0:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
					CurrentChunk->SetDoor(FString("right"), true);
					break;
					// DOWN
				case 1:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
					CurrentChunk->SetDoor(FString("down"), true);
					break;
				default:
					break;
				}
			}
			else
				return nullptr;

		}
		else									// ������ ����
		{
			switch (FMath::RandRange(0, 1))
			{
				// RIGHT
			case 0:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
				CurrentChunk->SetDoor(FString("right"), true);
				break;
				// DOWN
			case 1:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
				CurrentChunk->SetDoor(FString("down"), true);
				break;
			default:
				break;
			}
		}
	}

	/*
	* ���� �Ʒ��� �����ִ� ���
	*/
	else if (upHitSuccess && !rightHitSuccess && downHitSuccess && !leftHitSuccess)
	{
		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
		/*if (upHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(upHitResult.Actor)->SetDoor(FString("down"), true);
			CurrentChunk->SetDoor(FString("up"), true);
		}
		if (downHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(downHitResult.Actor)->SetDoor(FString("up"), true);
			CurrentChunk->SetDoor(FString("down"), true);
		}*/

		// ���� ������ų�� ����
		if (FMath::RandRange(0, 1) == 0)		// ���� ����
		{
			/*
			* ���� ���� ��Ų ��, ������ �ϳ� �� ������ ������ ����.
			* Door�� �ִ� �κе� �Բ� �������ش�.
			*/
			int8 roomLocation;

			switch (FMath::RandRange(0, 1))
			{
				// RIGHT
			case 0:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
				roomLocation = 1;
				CurrentChunk->SetDoor(FString("right"), true);
				break;
				// LEFT
			case 1:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
				roomLocation = 3;
				CurrentChunk->SetDoor(FString("left"), true);
				break;
			default:
				break;
			}

			// ���� �ձ� or �� �Ѱ��� ��
			// if ���� ���� FMath::RandRange(0, 1) == 0
			if (true)
			{
				int8 checkCanGo = FMath::RandRange(0, 1);
				TArray<int8> roomNumber = { 1, 3 };

				while (roomNumber[checkCanGo] == roomLocation)
					checkCanGo = FMath::RandRange(0, 1);

				switch (checkCanGo)
				{
					// RIGHT
				case 0:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
					CurrentChunk->SetDoor(FString("right"), true);
					break;
					// LEFT
				case 1:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
					CurrentChunk->SetDoor(FString("left"), true);
					break;
				default:
					break;
				}
			}
			else
				return nullptr;

		}
		else									// ������ ����
		{
			switch (FMath::RandRange(0, 1))
			{
				// RIGHT
			case 0:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
				CurrentChunk->SetDoor(FString("right"), true);
				break;
				// LEFT
			case 1:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
				CurrentChunk->SetDoor(FString("left"), true);
				break;
			default:
				break;
			}
		}
	}

	/*
	* ���ʰ� �������� �����ִ� ���
	*/
	else if (upHitSuccess && !rightHitSuccess && !downHitSuccess && leftHitSuccess)
	{
		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
		/*if (leftHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(leftHitResult.Actor)->SetDoor(FString("right"), true);
			CurrentChunk->SetDoor(FString("left"), true);
		}
		if (rightHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(rightHitResult.Actor)->SetDoor(FString("left"), true);
			CurrentChunk->SetDoor(FString("right"), true);
		}*/

		// ���� ������ų�� ����
		if (FMath::RandRange(0, 1) == 0)		// ���� ����
		{
			/*
			* ���� ���� ��Ų ��, ������ �ϳ� �� ������ ������ ����.
			* Door�� �ִ� �κе� �Բ� �������ش�.
			*/
			int8 roomLocation;

			switch (FMath::RandRange(0, 1))
			{
				// UP
			case 0:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
				roomLocation = 0;
				CurrentChunk->SetDoor(FString("up"), true);
				break;
				// DOWN
			case 1:
				SpawnRoom(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
				roomLocation = 2;
				CurrentChunk->SetDoor(FString("down"), true);
				break;
			default:
				break;
			}

			// ���� �ձ� or �� �Ѱ��� ��
			// if ���� ���� FMath::RandRange(0, 1) == 0
			if (true)
			{
				int8 checkCanGo = FMath::RandRange(0, 1);
				TArray<int8> roomNumber = { 0, 2 };

				while (roomNumber[checkCanGo] == roomLocation)
					checkCanGo = FMath::RandRange(0, 1);

				switch (checkCanGo)
				{
					// UP
				case 0:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
					CurrentChunk->SetDoor(FString("up"), true);
					break;
					// DOWN
				case 1:
					returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
					CurrentChunk->SetDoor(FString("down"), true);
					break;
				default:
					break;
				}
			}
			else
				return nullptr;

		}
		else									// ������ ����
		{
			switch (FMath::RandRange(0, 1))
			{
				// UP
			case 0:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
				CurrentChunk->SetDoor(FString("up"), true);
				break;
				// DOWN
			case 1:
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
				CurrentChunk->SetDoor(FString("down"), true);
				break;
			default:
				break;
			}
		}
	}

	/*
	* ���� ������, �Ʒ��� �����ִ� ���
	*/
	else if (upHitSuccess && rightHitSuccess && downHitSuccess && !leftHitSuccess)
	{
		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
		/*if (upHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(upHitResult.Actor)->SetDoor(FString("down"), true);
			CurrentChunk->SetDoor(FString("up"), true);
		}
		if (rightHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(rightHitResult.Actor)->SetDoor(FString("left"), true);
			CurrentChunk->SetDoor(FString("right"), true);
		}
		if (downHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(downHitResult.Actor)->SetDoor(FString("up"), true);
			CurrentChunk->SetDoor(FString("down"), true);
		}*/

		// ���� ������ų�� ����
		if (FMath::RandRange(0, 1) == 0)		// ���� ����
		{
			/*
			* ���� ���� ��Ų ��, ������ �ϳ� �� ������ ������ ����.
			* Door�� �ִ� �κе� �Բ� �������ش�.
			*/
			int8 roomLocation;

			// LEFT
			SpawnRoom(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
			roomLocation = 3;
			CurrentChunk->SetDoor(FString("left"), true);

			// ���� �ձ� or �� �Ѱ��� ��
			// if ���� ���� FMath::RandRange(0, 1) == 0
			if (true)
			{
				// LEFT
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
				CurrentChunk->SetDoor(FString("left"), true);
			}
			else
				return nullptr;

		}
		else									// ������ ����
		{
			// LEFT
			returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y - 1000, CurrentChunk->GetActorLocation().Z), FString("right"));
			CurrentChunk->SetDoor(FString("left"), true);
		}
	}

	/*
	* �����ʰ� �Ʒ�, ������ �����ִ� ���
	*/
	else if (!upHitSuccess && rightHitSuccess && downHitSuccess && leftHitSuccess)
	{
		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
		/*if (rightHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(rightHitResult.Actor)->SetDoor(FString("left"), true);
			CurrentChunk->SetDoor(FString("right"), true);
		}
		if (downHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(downHitResult.Actor)->SetDoor(FString("up"), true);
			CurrentChunk->SetDoor(FString("down"), true);
		}
		if (leftHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(leftHitResult.Actor)->SetDoor(FString("right"), true);
			CurrentChunk->SetDoor(FString("left"), true);
		}*/

		// ���� ������ų�� ����
		if (FMath::RandRange(0, 1) == 0)		// ���� ����
		{
			/*
			* ���� ���� ��Ų ��, ������ �ϳ� �� ������ ������ ����.
			* Door�� �ִ� �κе� �Բ� �������ش�.
			*/
			int8 roomLocation;

			// UP
			SpawnRoom(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
			roomLocation = 0;
			CurrentChunk->SetDoor(FString("up"), true);

			// ���� �ձ� or �� �Ѱ��� ��
			// if ���� ���� FMath::RandRange(0, 1) == 0
			if (true)
			{
				// UP
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
				CurrentChunk->SetDoor(FString("up"), true);
			}
			else
				return nullptr;

		}
		else									// ������ ����
		{
			// UP
			returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X + 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("down"));
			CurrentChunk->SetDoor(FString("up"), true);
		}
	}

	/*
	*�Ʒ��� ����, ���� �����ִ� ���
	*/
	else if (!upHitSuccess && rightHitSuccess && downHitSuccess && leftHitSuccess)
	{
		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
		/*if (downHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(downHitResult.Actor)->SetDoor(FString("up"), true);
			CurrentChunk->SetDoor(FString("down"), true);
		}
		if (leftHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(leftHitResult.Actor)->SetDoor(FString("right"), true);
			CurrentChunk->SetDoor(FString("left"), true);
		}
		if (upHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(upHitResult.Actor)->SetDoor(FString("down"), true);
			CurrentChunk->SetDoor(FString("up"), true);
		}*/

		// ���� ������ų�� ����
		if (FMath::RandRange(0, 1) == 0)		// ���� ����
		{
			/*
			* ���� ���� ��Ų ��, ������ �ϳ� �� ������ ������ ����.
			* Door�� �ִ� �κе� �Բ� �������ش�.
			*/
			int8 roomLocation;

			// RIGHT
			SpawnRoom(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
			roomLocation = 1;
			CurrentChunk->SetDoor(FString("right"), true);

			// ���� �ձ� or �� �Ѱ��� ��
			// if ���� ���� FMath::RandRange(0, 1) == 0
			if (true)
			{
				// RIGHT
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
				CurrentChunk->SetDoor(FString("right"), true);
			}
			else
				return nullptr;

		}
		else									// ������ ����
		{
			// RIGHT
			returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X, CurrentChunk->GetActorLocation().Y + 1000, CurrentChunk->GetActorLocation().Z), FString("left"));
			CurrentChunk->SetDoor(FString("right"), true);
		}
	}

	/*
	*���ʰ� ��, �������� �����ִ� ���
	*/
	else if (upHitSuccess && rightHitSuccess && !downHitSuccess && leftHitSuccess)
	{
		MakingDoor(CurrentChunk, upHitResult, rightHitResult, downHitResult, leftHitResult);
		/*if (leftHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(leftHitResult.Actor)->SetDoor(FString("right"), true);
			CurrentChunk->SetDoor(FString("left"), true);
		}
		if (upHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(upHitResult.Actor)->SetDoor(FString("down"), true);
			CurrentChunk->SetDoor(FString("up"), true);
		}
		if (rightHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(rightHitResult.Actor)->SetDoor(FString("left"), true);
			CurrentChunk->SetDoor(FString("right"), true);
		}*/

		// ���� ������ų�� ����
		if (FMath::RandRange(0, 1) == 0)		// ���� ����
		{
			/*
			* ���� ���� ��Ų ��, ������ �ϳ� �� ������ ������ ����.
			* Door�� �ִ� �κе� �Բ� �������ش�.
			*/
			int8 roomLocation;

			// DOWN
			SpawnRoom(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
			roomLocation = 1;
			CurrentChunk->SetDoor(FString("down"), true);

			// ���� �ձ� or �� �Ѱ��� ��
			// if ���� ���� FMath::RandRange(0, 1) == 0
			if (true)
			{
				// DOWN
				returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
				CurrentChunk->SetDoor(FString("down"), true);
			}
			else
				return nullptr;

		}
		else									// ������ ����
		{
			// DOWN
			returnActor = SpawnCorrider(FVector(CurrentChunk->GetActorLocation().X - 1000, CurrentChunk->GetActorLocation().Y, CurrentChunk->GetActorLocation().Z), FString("up"));
			CurrentChunk->SetDoor(FString("down"), true);
		}
	}

	/*
	*��� ���� �����ִ� ���
	*/
	else if (upHitSuccess && rightHitSuccess && downHitSuccess && leftHitSuccess)
	{
		return nullptr;
	}

	return returnActor;
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
		if (rightHitResult.Actor->IsA<ACorrider>())
		{
			Cast<ACorrider>(leftHitResult.Actor)->SetDoor(FString("right"), true);
			currentChunk->SetDoor(FString("left"), true);
		}
	}
}
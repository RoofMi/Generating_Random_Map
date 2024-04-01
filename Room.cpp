// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"

ARoom::ARoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	SetRootComponent(MainMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));

	if (DefaultMesh.Succeeded())
	{
		MainMesh->SetStaticMesh(DefaultMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Room_1(TEXT("/Game/Contents/Meshes/SM_Room1.SM_Room1"));

	if (Room_1.Succeeded())
	{
		Room1 = Room_1.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Room_2(TEXT("/Game/Contents/Meshes/SM_Room2.SM_Room2"));

	if (Room_2.Succeeded())
	{
		Room2 = Room_2.Object;
	}

	/*Mat_Green = CreateDefaultSubobject<UMaterial>(TEXT("MAIN Mat"));

	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultMat(TEXT("/Game/Geometry/Meshes/M_Green.M_Green"));

	if (DefaultMat.Succeeded())
	{
		Mat_Green = DefaultMat.Object;
	}*/


	chunkType = ROOM;
}

void ARoom::BeginPlay()
{
	Super::BeginPlay();

	/*MainMesh->SetMaterial(0, Mat_Green);*/
}

void ARoom::SpawnTypeActor()
{
	Super::SpawnTypeActor();

	if (up)
		SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
	else if (down)
		SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
	else if (left)
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	
	if (FMath::RandRange(0, 1) == 0)
		MainMesh->SetStaticMesh(Room1);
	else
		MainMesh->SetStaticMesh(Room2);
}
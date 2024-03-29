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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RoomMesh(TEXT("/Game/Geometry/Meshes/SM_Test_Room.SM_Test_Room"));

	if (RoomMesh.Succeeded())
	{
		TypeMesh = RoomMesh.Object;
	}

	Mat_Green = CreateDefaultSubobject<UMaterial>(TEXT("MAIN Mat"));

	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultMat(TEXT("/Game/Geometry/Meshes/M_Green.M_Green"));

	if (DefaultMat.Succeeded())
	{
		Mat_Green = DefaultMat.Object;
	}


	chunkType = ROOM;
}

void ARoom::BeginPlay()
{
	Super::BeginPlay();

	MainMesh->SetMaterial(0, Mat_Green);
}

void ARoom::SpawnTypeActor()
{
	Super::SpawnTypeActor();

	if (right)
		SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
	else if (down)
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	else if (left)
		SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
	
	MainMesh->SetStaticMesh(TypeMesh);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "MainRoom.h"

AMainRoom::AMainRoom()
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MainRoomMesh(TEXT("/Game/Contents/Meshes/SM_MainRoom.SM_MainRoom"));

	if (MainRoomMesh.Succeeded())
	{
		TypeMesh = MainRoomMesh.Object;
	}

	/*Mat_Orange = CreateDefaultSubobject<UMaterial>(TEXT("MAIN Mat"));

	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultMat(TEXT("/Game/Geometry/Meshes/M_Orange.M_Orange"));

	if (DefaultMat.Succeeded())
	{
		Mat_Orange = DefaultMat.Object;
	}*/
	

	chunkType = MAIN;

	down = false;
	up = true;
	left = true;
	right = true;
}

void AMainRoom::BeginPlay()
{
	Super::BeginPlay();

	SetActorScale3D(FVector(20.0f, 40.0f, 1.0f));
	/*MainMesh->SetMaterial(0, Mat_Orange);*/
}

void AMainRoom::SpawnTypeActor()
{
	Super::SpawnTypeActor();
	
	SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
	MainMesh->SetStaticMesh(TypeMesh);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "Corrider.h"

ACorrider::ACorrider()
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CorriderMesh_0(TEXT("/Game/Geometry/Meshes/SM_1way_Corrider.SM_1way_Corrider"));

	if (CorriderMesh_0.Succeeded())
	{
		Corrider_1Way = CorriderMesh_0.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CorriderMesh_1(TEXT("/Game/Contents/Meshes/SM_2way_Corrider_V1.SM_2way_Corrider_V1"));

	if (CorriderMesh_1.Succeeded())
	{
		Corrider_2Way_V1 = CorriderMesh_1.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CorriderMesh_2(TEXT("/Game/Contents/Meshes/SM_2way_Corrider_V2.SM_2way_Corrider_V2"));

	if (CorriderMesh_2.Succeeded())
	{
		Corrider_2Way_V2 = CorriderMesh_2.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CorriderMesh_3(TEXT("/Game/Contents/Meshes/SM_2way_Vertical_Corrider.SM_2way_Vertical_Corrider"));

	if (CorriderMesh_3.Succeeded())
	{
		Corrider_Vertical_2Way = CorriderMesh_3.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CorriderMesh_4(TEXT("/Game/Contents/Meshes/SM_3way_Corrider.SM_3way_Corrider"));

	if (CorriderMesh_4.Succeeded())
	{
		Corrider_3Way = CorriderMesh_4.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CorriderMesh_5(TEXT("/Game/Contents/Meshes/SM_4way_Corrider.SM_4way_Corrider"));

	if (CorriderMesh_5.Succeeded())
	{
		Corrider_4Way = CorriderMesh_5.Object;
	}

	/*Mat_Silver = CreateDefaultSubobject<UMaterial>(TEXT("MAIN Mat"));

	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultMat(TEXT("/Game/Geometry/Meshes/M_Silver.M_Silver"));

	if (DefaultMat.Succeeded())
	{
		Mat_Silver = DefaultMat.Object;
	}*/


	chunkType = CORRIDER;
}

void ACorrider::BeginPlay()
{
	Super::BeginPlay();

	/*MainMesh->SetMaterial(0, Mat_Silver);*/
}

void ACorrider::SpawnTypeActor()
{
	Super::SpawnTypeActor();

	if (up && right && down && left)
		MainMesh->SetStaticMesh(Corrider_4Way);
	else if (up && right && down)
	{
		SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
		MainMesh->SetStaticMesh(Corrider_3Way);
	}
	else if (right && down && left)
	{
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
		MainMesh->SetStaticMesh(Corrider_3Way);
	}
	else if (down && left && up)
	{
		SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
		MainMesh->SetStaticMesh(Corrider_3Way);
	}
	else if (left && up && right)
	{
		MainMesh->SetStaticMesh(Corrider_3Way);
	}
	else if (up && right)
	{
		SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
		MainMesh->SetStaticMesh(Corrider_Vertical_2Way);
	}
	else if (right && down)
	{
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
		MainMesh->SetStaticMesh(Corrider_Vertical_2Way);
	}
	else if (down && left)
	{
		SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
		MainMesh->SetStaticMesh(Corrider_Vertical_2Way);
	}
	else if (left && up)
	{
		MainMesh->SetStaticMesh(Corrider_Vertical_2Way);
	}
	else if (up && down)
	{
		SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));

		if (FMath::RandRange(0, 1) == 0)
			MainMesh->SetStaticMesh(Corrider_2Way_V1);
		else
			MainMesh->SetStaticMesh(Corrider_2Way_V2);
	}
	else if (right && left)
	{
		if (FMath::RandRange(0, 1) == 0)
			MainMesh->SetStaticMesh(Corrider_2Way_V1);
		else
			MainMesh->SetStaticMesh(Corrider_2Way_V2);
	}
	else if (up)
	{
		MainMesh->SetStaticMesh(Corrider_1Way);
	}
	else if (right)
	{
		SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
		MainMesh->SetStaticMesh(Corrider_1Way);
	}
	else if (down)
	{
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
		MainMesh->SetStaticMesh(Corrider_1Way);
	}
	else if (left)
	{
		SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
		MainMesh->SetStaticMesh(Corrider_1Way);
	}
}
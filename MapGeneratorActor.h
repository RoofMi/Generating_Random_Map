// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Chunk.h"

#include "GameFramework/Actor.h"
#include "MapGeneratorActor.generated.h"

UCLASS()
class MAPGENERATOR_API AMapGeneratorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGeneratorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "MapSetting")
	int32 mapScale;

	void GeneratingMap();
	void MakingDoor(AChunk* currentChunk, FHitResult upHitResult, FHitResult rightHitResult, FHitResult downHitResult, FHitResult leftHitResult);

	AChunk* GeneratingChunk(AChunk* CurrentChunk, int count);

	TArray<bool> SpawnRoom(AChunk* CurrentChunk, TArray<bool> DoorList);
	AChunk* SpawnCorrider(AChunk* CurrentChunk, TArray<bool> DoorList);

	void SpawnRoomActor(FVector location, FString doorLocation);
	AChunk* SpawnCorriderActor(FVector location, FString doorLocation);
	

private:
	UStaticMeshComponent* MeshComponent;

	int32 ChunkDistance;
	//FVector upVector;
	//FVector rightVector;
	//FVector downVector;
	//FVector leftVector;

};

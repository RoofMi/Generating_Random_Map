// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chunk.generated.h"

UCLASS()
class MAPGENERATOR_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int8 GetChunkType();
	void SetChunkType(int8 value);
	bool GetDoor(FString direction);
	void SetDoor(FString direction, bool state);

	UPROPERTY(VisibleAnywhere, Category = "RoomSetting")
	bool up;

	UPROPERTY(VisibleAnywhere, Category = "RoomSetting")
	bool right;

	UPROPERTY(VisibleAnywhere, Category = "RoomSetting")
	bool down;

	UPROPERTY(VisibleAnywhere, Category = "RoomSetting")
	bool left;

	virtual void SpawnTypeActor();
	

protected:
	enum Type {
		NONE, MAIN, ROOM, CORRIDER
	};

	int8 chunkType;

	UStaticMesh* TypeMesh;

private:
	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComponent;
};

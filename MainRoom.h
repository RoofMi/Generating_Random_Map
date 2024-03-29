// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chunk.h"
#include "MainRoom.generated.h"

/**
 * 
 */
UCLASS()
class MAPGENERATOR_API AMainRoom : public AChunk
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMainRoom();

	virtual void SpawnTypeActor() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* MainMesh;

	UPROPERTY(EditAnywhere)
		UMaterial* Mat_Orange;

};

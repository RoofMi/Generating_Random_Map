// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chunk.h"
#include "Corrider.generated.h"

/**
 * 
 */
UCLASS()
class MAPGENERATOR_API ACorrider : public AChunk
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACorrider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* MainMesh;

	UPROPERTY(EditAnywhere)
	UMaterial* Mat_Silver;
};

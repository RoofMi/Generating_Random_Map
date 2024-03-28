// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"


// Sets default values
AChunk::AChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	chunkType = NONE;

	down = false;
	up = false;
	left = false;
	right = false;
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorScale3D(FVector(10.0f, 10.0f, 1.0f));
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int8 AChunk::GetChunkType()
{
	return chunkType;
}

void AChunk::SetChunkType(int8 value)
{

}

bool AChunk::GetDoor(FString direction)
{
	if (direction == FString("down"))
		return down;
	else if (direction == FString("up"))
		return up;
	else if (direction == FString("left"))
		return left;
	else if (direction == FString("right"))
		return right;
	else
		return false;
}

void AChunk::SetDoor(FString direction, bool state)
{
	if (direction == FString("down"))
		down = state;
	else if (direction == FString("up"))
		up = state;
	else if (direction == FString("left"))
		left = state;
	else if (direction == FString("right"))
		right = state;
}
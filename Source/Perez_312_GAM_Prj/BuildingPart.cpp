// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPart.h"

// Sets default values
ABuildingPart::ABuildingPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PivotArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Pivot Arrow"));

	RootComponent = PivotArrow;
	Mesh->SetupAttachment(PivotArrow);

}

// Called when the game starts or when spawned
void ABuildingPart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


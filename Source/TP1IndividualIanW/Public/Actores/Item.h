// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class TP1INDIVIDUALIANW_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Salud editable en cualquier parte Blueprints, detalles del editor. ToolTip: Texto de ayuda al pasar el mouse.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Estadisticas", meta = (ClampMin = "0", ClampMax = "100", ToolTip = "Vida del Item"))
	int salud;

	// Solo se puede editar los valores por defecto, no en instancias en el mundo solo get, no set (BlueprintReadOnly)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Estadisticas")
	int experiencia;

	// UIMin / UIMax: Límites visuales para sliders en el editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Estadisticas", meta = (UIMin = "1", UIMax = "90"))
	int Nivel;

	// Velocidad visible pero no editable en el editor
	UPROPERTY(VisibleAnywhere, Category = "Estadisticas")
	float Velocidad;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

// Evita que el archivo se incluya más de una vez en la compilación (protección contra includes múltiples)
#pragma once

// Incluye el módulo base de Unreal Engine (tipos básicos, macros, utilidades)
#include "CoreMinimal.h"

// Incluye la clase base ACharacter (nuestro personaje hereda de ella)
#include "GameFramework/Character.h"

// Incluye macros para poder declarar categorías de logs personalizados
#include "Logging/LogMacros.h"

// Incluye el archivo generado automáticamente por Unreal (necesario para reflección y macros de UCLASS)
#include "TP1IndividualIanWCharacter.generated.h"

// Declaración anticipada de clases que se usarán como punteros 
class USpringArmComponent;
class UCameraComponent;
class UInputAction;

// Declaración anticipada de struct para manejar valores de input
struct FInputActionValue;

// Declara una categoría de log externa para este personaje (para imprimir mensajes en la consola/log)
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

// Macro que indica que esta clase es un Actor de Unreal
// La palabra "abstract" indica que esta clase no puede instanciarse directamente (sirve como base)

UCLASS(abstract)
class ATP1IndividualIanWCharacter : public ACharacter
{
	// Macro de Unreal que genera código necesario para reflección, replicación, etc.
	GENERATED_BODY()

	// Componente SpringArm (brazo de cámara) que controla la distancia y rotación de la cámara

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// Componente de cámara que sigue al personaje (pegado al extremo del SpringArm)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	


protected:

	// --- INPUT ACTIONS (CONFIGURABLES DESDE EL EDITOR) ---

	// Acción de salto (se vincula a Jump/StopJumping)
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	// Acción de movimiento (recibe un vector 2D: adelante/atrás, derecha/izquierda)
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	// Acción de mirar (ej. stick derecho en gamepad)
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	// Acción de mirar con el mouse
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;

	// --- MI INPUT ACTIONS ---  


	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MostrarMensajeAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CambiarVelocidadAction;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Speed")
	bool bFastSpeed;

	// Velocidad normal 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float NormalSpeed;

	// Velocidad rapida 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float FastSpeed;

public:

	// Constructor del Character (inicializa componentes y valores por defecto)
	ATP1IndividualIanWCharacter();

protected:

	// Configura las vinculaciones de input (override de la función en ACharacter)
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// Función que recibe valores de input para mover al personaje
	void Move(const FInputActionValue& Value);

	// Función que recibe valores de input para rotar la cámara
	void Look(const FInputActionValue& Value);


public:

	// --- FUNCIONES DISPONIBLES EN BLUEPRINTS ---

	// Función que aplica movimiento (puede ser llamada desde C++ o Blueprints)
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	// Función que aplica rotación de cámara (Yaw = horizontal, Pitch = vertical)
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	// Función que inicia el salto
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	// Función que detiene el salto
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	// Funcion blueprint-callable para mostrar mensajes
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMostrarMensaje();

	// Funcion blueprint-callable para cambiar la velocidad del personaje
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoCambioVelocidad();

public:

	// --- GETTERS INLINE (acceso rápido a los componentes) ---

	// Devuelve el puntero al CameraBoom (brazo de cámara)
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// Devuelve el puntero a la cámara que sigue al personaje
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

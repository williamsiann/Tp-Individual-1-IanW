#include "TP1IndividualIanWCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TP1IndividualIanW.h"

// ===================== CONSTRUCTOR =====================
ATP1IndividualIanWCharacter::ATP1IndividualIanWCharacter()
{
    // Inicializa el tamaño de la cápsula de colisión
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Desactiva la rotación automática del personaje al rotar el controlador
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configura que el personaje se oriente automáticamente hacia la dirección de movimiento
    GetCharacterMovement()->bOrientRotationToMovement = true;

    // Establece la velocidad de rotación del personaje para seguir la dirección de movimiento
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    // Define la fuerza de salto del personaje
    GetCharacterMovement()->JumpZVelocity = 500.f;

    // Define el control en el aire
    GetCharacterMovement()->AirControl = 0.35f;

    // Velocidad máxima al caminar
    GetCharacterMovement()->MaxWalkSpeed = 500.f;

    // Velocidad mínima para movimiento analógico
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;

    // Desaceleración al detenerse mientras camina
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    // Desaceleración al detenerse en el aire
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    // Crea el SpringArm (brazo de camara)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    // Lo conecta al RootComponent
    CameraBoom->SetupAttachment(RootComponent);
    // Distancia de la camara respecto al personaje
    CameraBoom->TargetArmLength = 400.0f;
    // Hace que la cámara siga la rotación del controlador
    CameraBoom->bUsePawnControlRotation = true;

    // Crea la cámara que sigue al personaje
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    // La conecta al extremo del SpringArm
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    // Indica que la cámara NO rota con el pawn
    FollowCamera->bUsePawnControlRotation = false;

    // Setear variables de velocidad
    bFastSpeed = false;
    NormalSpeed = 500.0f;
    FastSpeed = 1000.0f;
}

// ===================== INPUT =====================
void ATP1IndividualIanWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Configura las acciones de input del jugador usando el sistema Enhanced Input
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

        // ----- Vínculos de Acciones Preexistentes -----
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATP1IndividualIanWCharacter::Move);
        EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATP1IndividualIanWCharacter::Look);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATP1IndividualIanWCharacter::Look);

        // ---------- NUEVAS FUNCIONES: Mostrar Mensaje y Correr ----------
        // Vincula la acción de Mostrar Mensaje
        EnhancedInputComponent->BindAction(MostrarMensajeAction, ETriggerEvent::Started, this, &ATP1IndividualIanWCharacter::DoMostrarMensaje);

        // Vincula la acción de Cambiar Velocidad
        EnhancedInputComponent->BindAction(CambiarVelocidadAction, ETriggerEvent::Started, this, &ATP1IndividualIanWCharacter::DoCambioVelocidad
        );
    }
    else
    {
        // Mensaje de error si no se encuentra un EnhancedInputComponent
        UE_LOG(LogTP1IndividualIanW, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
    }
}

// ===================== MOVIMIENTO =====================
void ATP1IndividualIanWCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    DoMove(MovementVector.X, MovementVector.Y);
}

void ATP1IndividualIanWCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ATP1IndividualIanWCharacter::DoMove(float Right, float Forward)
{
    if (GetController() != nullptr)
    {
        const FRotator Rotation = GetController()->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, Forward);
        AddMovementInput(RightDirection, Right);
    }
}

void ATP1IndividualIanWCharacter::DoLook(float Yaw, float Pitch)
{
    if (GetController() != nullptr)
    {
        AddControllerYawInput(Yaw);
        AddControllerPitchInput(Pitch);
    }
}

// ===================== FUNCIONES PERSONALIZADAS =====================
void ATP1IndividualIanWCharacter::DoMostrarMensaje()
{
    // Muestra un mensaje en pantalla
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Mostrar Mensaje"));
}

void ATP1IndividualIanWCharacter::DoCambioVelocidad()
{
    // Setea el valor contrario en el booleano
    bFastSpeed = !bFastSpeed;

    if (bFastSpeed)
    {
        GetCharacterMovement()->MaxWalkSpeed = FastSpeed;
    }
    else
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}
// ===================== SALTO =====================
void ATP1IndividualIanWCharacter::DoJumpStart()
{
    // Llama a la funcion de salto de la clase base ACharacter
    Jump();
}

void ATP1IndividualIanWCharacter::DoJumpEnd()
{
    // Llama a la funcion para detener el salto de la clase base ACharacter
    StopJumping();
}
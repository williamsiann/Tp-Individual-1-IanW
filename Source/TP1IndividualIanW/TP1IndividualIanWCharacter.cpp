// Incluye el header del Character personalizado (donde se definen funciones y variables del personaje)
#include "TP1IndividualIanWCharacter.h"

// Incluye la clase LocalPlayer, que maneja información del jugador local
#include "Engine/LocalPlayer.h"

// Incluye la clase de la cámara
#include "Camera/CameraComponent.h"

// Incluye la clase del componente de cápsula (colisión principal del Character)
#include "Components/CapsuleComponent.h"

// Incluye el componente que maneja el movimiento del Character
#include "GameFramework/CharacterMovementComponent.h"

// Incluye el componente SpringArm, que permite alejar la cámara del personaje y rotarla
#include "GameFramework/SpringArmComponent.h"

// Incluye la clase Controller, que controla la entrada del jugador (movimiento, cámara, etc.)
#include "GameFramework/Controller.h"

// Incluye la clase para usar el sistema de Input mejorado (Enhanced Input)
#include "EnhancedInputComponent.h"

// Incluye subsistemas de Enhanced Input para manejar mapeos de entrada
#include "EnhancedInputSubsystems.h"

// Incluye la clase InputActionValue, que guarda los valores de las acciones de entrada
#include "InputActionValue.h"

// Incluye el archivo principal del proyecto para logs y configuraciones
#include "TP1IndividualIanW.h"


// Constructor de la clase Character (se ejecuta al crear el personaje)
ATP1IndividualIanWCharacter::ATP1IndividualIanWCharacter()
{
    // Inicializa el tamaño de la cápsula de colisión (radio y altura)
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Indica que el personaje NO rota con la cámara en Pitch (arriba/abajo)
    bUseControllerRotationPitch = false;
    // Indica que el personaje NO rota con la cámara en Yaw (izquierda/derecha)
    bUseControllerRotationYaw = false;
    // Indica que el personaje NO rota con la cámara en Roll (inclinación lateral)
    bUseControllerRotationRoll = false;

    // Hace que el personaje rote en la dirección en la que se mueve
    GetCharacterMovement()->bOrientRotationToMovement = true;
    // Velocidad a la que rota el personaje cuando se mueve (500° por segundo en el eje Yaw)
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    // Define la fuerza de salto del personaje
    GetCharacterMovement()->JumpZVelocity = 500.f;
    // Define el control en el aire (qué tanto puede moverse mientras salta)
    GetCharacterMovement()->AirControl = 0.35f;
    // Velocidad máxima al caminar
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    // Velocidad mínima al caminar con control analógico (ej. joystick)
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    // Desaceleración al detenerse mientras camina
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    // Desaceleración al detenerse en el aire
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    // Crea el SpringArm (brazo que aleja la cámara del personaje)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    // Lo conecta al RootComponent (componente base del Character)
    CameraBoom->SetupAttachment(RootComponent);
    // Distancia de la cámara respecto al personaje
    CameraBoom->TargetArmLength = 400.0f;
    // Hace que la cámara siga la rotación del controlador
    CameraBoom->bUsePawnControlRotation = true;

    // Crea la cámara que sigue al personaje
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    // La conecta al extremo del SpringArm
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    // Indica que la cámara NO rota con el pawn, sino con el controlador
    FollowCamera->bUsePawnControlRotation = false;
}

// Función que configura las vinculaciones de entrada (se llama automáticamente al preparar el Actor para input)
void ATP1IndividualIanWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Intento convertir el componente de entrada al tipo EnhancedInputComponent (sistema moderno de entrada)
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

        // Vinculo la acción JumpAction para el evento Started (cuando se presiona) con la función Jump() de ACharacter
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        // Vinculo la acción JumpAction para el evento Completed (cuando se suelta) con StopJumping() de ACharacter
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        // Vinculo la acción MoveAction (vector 2D) con la función Move() de este personaje; se llama cada vez que hay input
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATP1IndividualIanWCharacter::Move);
        // Vinculo la acción MouseLookAction (movimiento del mouse) con la función Look() para controlar cámara
        EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATP1IndividualIanWCharacter::Look);

        // Vinculo la acción LookAction (ej. stick derecho del gamepad) también con Look()
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATP1IndividualIanWCharacter::Look);

       
        // ---------- NUEVAS FUNCIONES: Mostrar Mensaje y Correr ----------

        // Mostrar mensaje (F)
        if (MostrarMensajeAction)
        {
            EnhancedInputComponent->BindAction(MostrarMensajeAction, ETriggerEvent::Triggered, this, &ATP1IndividualIanWCharacter::DoMostrarMensaje);
        }

        // Correr (Shift) → Start y End
        if (RunAction)
        {
            EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ATP1IndividualIanWCharacter::DoRunStart);
            EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ATP1IndividualIanWCharacter::DoRunEnd);
        }
    }
    else
    {
        // Si el casteo falla (no se está usando Enhanced Input), lo reporto en el log para ayudar a depurar
        // *GetNameSafe(this) obtiene un nombre seguro del objeto para mostrar en el log
        UE_LOG(LogTP1IndividualIanW, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
    }
}

// Función llamada por el binding de MoveAction; recibe el valor de entrada envuelto en FInputActionValue
void ATP1IndividualIanWCharacter::Move(const FInputActionValue& Value)
{
    // Extraigo un FVector2D del valor de entrada (normalmente X=horizontal, Y=vertical o viceversa según el mapeo)
    FVector2D MovementVector = Value.Get<FVector2D>();

    // Llamo a DoMove pasando los componentes X (Right) e Y (Forward) para aplicar el movimiento
    DoMove(MovementVector.X, MovementVector.Y);
}

// Función llamada por el binding de LookAction/MouseLookAction; recibe el valor de entrada (ej. delta del mouse o stick)
void ATP1IndividualIanWCharacter::Look(const FInputActionValue& Value)
{
    // Extraigo un FVector2D con la entrada de look (Yaw, Pitch) según cómo esté configurado el InputAction
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    // Llamo a DoLook pasando los ejes X (Yaw) y Y (Pitch) para rotar la cámara/controlador
    DoLook(LookAxisVector.X, LookAxisVector.Y);
}

// Función que aplica movimiento usando vectores relativos al control (Right y Forward)
// Right = movimiento lateral, Forward = movimiento hacia delante/atrás
void ATP1IndividualIanWCharacter::DoMove(float Right, float Forward)
{
    // Verifico que exista un Controller para poder obtener la rotación de cámara/control
    if (GetController() != nullptr)
    {
        // Obtengo la rotación completa del controlador (pitch, yaw, roll)
        const FRotator Rotation = GetController()->GetControlRotation();
        // Construyo una rotación que solo contiene el Yaw (ignoro pitch y roll) para mover en el plano horizontal
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Obtengo el vector forward (eje X) relativo a la rotación en yaw del controlador
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        // Obtengo el vector right (eje Y) relativo a la rotación en yaw del controlador
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // Aplico movimiento hacia adelante/atrás multiplicado por el valor Forward (escala)
        AddMovementInput(ForwardDirection, Forward);
        // Aplico movimiento lateral multiplicado por el valor Right (escala)
        AddMovementInput(RightDirection, Right);
    }
    // Si no hay controlador, no hago nada (sin controlador no hay referencia de rotación para el movimiento)
}

// Función que aplica rotación de cámara/controlador usando los valores de entrada (Yaw = izquierda/derecha, Pitch = arriba/abajo)
void ATP1IndividualIanWCharacter::DoLook(float Yaw, float Pitch)
{
    // Verifico que haya un Controller para aplicar inputs de control
    if (GetController() != nullptr)
    {
        // Añade input de yaw al controlador (gira horizontalmente)
        AddControllerYawInput(Yaw);
        // Añade input de pitch al controlador (mira arriba/abajo)
        AddControllerPitchInput(Pitch);
    }
    // Si no hay controlador, la entrada de look se ignora
}

// Función para iniciar el salto desde UI/Blueprint o bindings; delega a la función provista por ACharacter
void ATP1IndividualIanWCharacter::DoJumpStart()
{
    // Llama a Jump() que maneja la lógica de salto definida por CharacterMovementComponent
    Jump();
}

// Función para finalizar el salto (cuando se suelta la tecla) y detener la lógica de salto
void ATP1IndividualIanWCharacter::DoJumpEnd()
{
    // Llama a StopJumping() para terminar la fase de salto
    StopJumping();
}



// ================= FUNCIONES MIAS =================

void ATP1IndividualIanWCharacter::DoMostrarMensaje()
{
    // Mensaje en pantalla
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("¡Se presionó MostrarMensaje!"));
}

void ATP1IndividualIanWCharacter::DoRunStart()
{
    GetCharacterMovement()->MaxWalkSpeed = 900.f;  // velocidad de correr
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("Corriendo..."));
}

void ATP1IndividualIanWCharacter::DoRunEnd()
{
    GetCharacterMovement()->MaxWalkSpeed = 500.f;  // velocidad normal
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("Volviendo a caminar"));
}


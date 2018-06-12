// Mr.Fox and friends, s.r.o, https://github.com/alexhajdu, https://twitter.com/alexhajdu

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InfinarioBPLibrary.generated.h"

UCLASS( )
class INFINARIOSDK_API UInfinarioBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY( )


	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	static bool StartSession( UObject* WC );

	UFUNCTION( BlueprintCallable, Category = "Infinario" )
	static void StartSessionWith( UObject* WC );
};

// Mr.Fox and friends, s.r.o, https://github.com/alexhajdu, https://twitter.com/alexhajdu

#include "InfinarioBPLibrary.h"
#include "Infinario.h"
#include <Object.h>
#include "InfinarioSDK.h"

bool UInfinarioBPLibrary::StartSession( UObject* WC )
{
	UInfinario* Inf = UInfinario::GetInstance( WC );
	return true;
}

void UInfinarioBPLibrary::StartSessionWith( UObject* WC )
{
	//TSharedPtr< FInfinarioSDKModule > Provider = FInfinarioSDKModule::Get( );
}

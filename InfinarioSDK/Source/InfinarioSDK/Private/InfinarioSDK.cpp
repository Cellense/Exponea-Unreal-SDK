// Mr.Fox and friends, s.r.o, https://github.com/alexhajdu, https://twitter.com/alexhajdu

#include "InfinarioSDK.h"

#define LOCTEXT_NAMESPACE "FInfinarioSDKModule"
DEFINE_LOG_CATEGORY( LogInfinario );


void FInfinarioSDKModule::StartupModule( )
{
	// This code will execute after your module is loaded into memory; the exact
	// timing is specified in the .uplugin file per-module

	UE_LOG( LogInfinario, Log, TEXT( "Infinario module loaded." ) );
}

void FInfinarioSDKModule::ShutdownModule( )
{
	// This function may be called during shutdown to clean up your module.  For
	// modules that support dynamic reloading, we call this function before
	// unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE( FInfinarioSDKModule, InfinarioSDK )
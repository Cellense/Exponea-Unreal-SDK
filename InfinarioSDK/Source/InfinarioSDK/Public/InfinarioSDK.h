// Mr.Fox and friends, s.r.o, https://github.com/alexhajdu, https://twitter.com/alexhajdu

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInfinario, All, All);

#define INF_LOG(_DESC_, _TEXT_) { UE_LOG( LogInfinario, Log, TEXT( "%s : %s" ), _DESC_, *(_TEXT_) ); }
#define INF_LOG_C(_DESC_, _VERBOSITY_, _TEXT_) { UE_LOG( LogInfinario, _VERBOSITY_, TEXT( "%s : %s" ), _DESC_, *(_TEXT_) ); }
#define INF_LOG_L( _DESC_ ) UE_LOG( LogInfinario, Log, TEXT( "%s" ), _DESC_ );
#define INF_LOG_W( _DESC_ ) UE_LOG( LogInfinario, Warning, TEXT( "%s" ), _DESC_ );
#define INF_LOG_E( _DESC_ ) UE_LOG( LogInfinario, Error, TEXT( "%s" ), _DESC_ );

class FInfinarioSDKModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline FInfinarioSDKModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FInfinarioSDKModule >("InfinarioSDK");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("InfinarioSDK");
	}
};
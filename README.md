# Getting started with Exponea  Unreal Engine SDK (in 5 minutes)

 Exponea UE4 SDK is available in this Git repository :https://github.com/Cellense/Exponea-Unreal-SDK. It provides tracking capabilities for your application. All functionality is expoed for C++ and Blueprints.
 
 > Note: The plugin is not compiled at the moment and it's not possible to use it within your pure Blueprint project. Compiled version is coming soon.


## Installation

* Close UE4 editor
* Download or clone this repository from your command line: git clone `https://github.com/Cellense/Exponea-Unreal-SDK.git`
* Copy downloaded **InfinarioSDK** folder into **[*YourProject*]/Plugins/**.
* Open UE4 editor and make sure the plugin is installed. In UE4 editor open **Settings/Plugins** and see if the plugin has been enabled.

### Engine compatibility and supported platforms

**Unreal Enigne** 
- 4.19.x

**Platforms** 
* Windows (32bit, 64bit)
* Mac
* Linux
* iOS
* Android
* HTML5


## Usage

### Workflow

The basic principle is to create one instance of the Infinario object and store the variable in well accessible place - for instance in the GameMode. After initialization you can use that object for all possilble actions like tracking, player identifying, etc...

## C++

### Prepare your project

First of all, you need to modify `[YourProject].Build.cs` file. Add `InfinarioSDK` to `PrivateDependencyModuleNames`

### Plugin initialization

```
// GameMode.h

UPROPERTY( BlueprintReadOnly, Category = "Infinario" )
UInfinario* Inf;
    
UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Infinario" )
FString Token = "e79ea4ac-5cdc-11e8-aaaa-b083fed3a8cb";
```

```
// GameMode.cpp

void AMyGameModeBase::BeginPlay( )
{
	Super::BeginPlay( );
    
    Inf = UInfinario::GetInstance( this );

	if( Inf == nullptr )
    {
		return;
	}
    
    /** Initialize with a given token amd custom target API */
    Inf->Initialize( Token, TEXT( "api.infinario.com" ) );
}
```

### Identifying Players

To control the identity of the current player use the Identify method. By calling

```
/** Identify player. */
Inf->Identify( FString( TEXT( "C++ Player" ) ) );
```
you can register a new player in Infinario. All events you track by the Track method from now on will belong to this player. To switch to an existing player, simply call Identify with his name. You can switch the identity of the current player as many times as you need to.

### Anonymous Players

Up until you call Identify for the first time, all tracked events belong to an anonymous player (internally identified with a randomly generated uuid). /*Once you call Identify, the previously anonymous player is automatically merged with the newly identified player. - In progress*/

### Infinario Data

User tracking utilizes custom data structure `FInfinarioData`. You can create payload for basic data types like this:

```
/** Create sample payload */
TMap< FString, FInfinarioData > TestData;
TestData.Add( "Integer Key", FInfinarioData( 3 ) );
TestData.Add( "String Key", FInfinarioData( FString( TEXT( "String value." ) ) ) );
TestData.Add( "Float Key", FInfinarioData( 1.2f ) );
TestData.Add( "Bool Key", FInfinarioData( true ) );
```       
  
### Every Call Data
```
/** Create data attached to every call. This part is optional.*/
TMap< FString, FInfinarioData > EveryCallData;
EveryCallData.Add( "every_call_data_int", FInfinarioData( 1 ) );
Inf->CreateEveryCallPayload( EveryCallData );
```

### Basic Tracking

At this moment you should have initialized plugin and prepared data for the event. For basic tracking, you call:
```
Inf->Track( "my_event", TestData );
```
to track *my_event* with *TestData*.

### Tracking Session Start and Session End Events

```
Inf->TrackSessionStart( TestData );
Inf->TrackSessionEnd( TestData );

```

### Tracking Virtual Payment
```
/**
 * Track with default virtual payment data structure.
 * Note: Use standard Track scenario in case you need to track additional data to the currency payload.
 */
FVirtualPayment VirtualPaymentData = FVirtualPayment( FInfinarioData( TEXT( "Gold" ) ), FInfinarioData( 10.0f ), FInfinarioData( TEXT( "Axe" ) ), FInfinarioData( TEXT( "Weapon" ) ) );
Inf->TrackVirtualPayment( VirtualPaymentData );
```

## Blueprints

Tracking in Blueprints is as easy as tracking in C++. 

![Alt text](HowTo/bp_howto.png?raw=true "bp")

## Sample project

Please refer to sample project if something is not clear enough [Exponea-Unreal-Demo-Game](https://github.com/Cellense/Exponea-Unreal-Demo-Game)

## Final Remarks

* It's suggested you make at most one instance of `InfinarioSDK` object during your application lifetime. The good place for instance is the GameMode.

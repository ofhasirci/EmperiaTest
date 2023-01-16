// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ARDataStruct.h"
#include "Interfaces/IHttpRequest.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FAckermannsRouletteModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
	
private:

	void RegisterMenus();

	void OnDataTableSelected(FSoftObjectPath SoftObjectPath);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	TSharedPtr<class FUICommandList> PluginCommands;

	int RandomNumber = -1;

	void OnStaticMeshLoaded();

	void OnDataTableLoaded();

	FReply GetRandomNumber();

	TSoftObjectPtr<UDataTable> DTStaticMesh;

	TArray<FARDataStruct*> StaticMeshArray;

	void OnRandomNumberAPIResponceReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void AddStaticMeshToWorld();

	void SpawnStaticMeshActor(TSoftObjectPtr<UStaticMesh> Mesh);
};

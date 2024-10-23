// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Menu.generated.h"

UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="MultiplayerSessions|Utilities")
	void MenuSetup(int PublicConnections = 4, const FString& MatchType = "FreeForAll", const FString& LobbyPath = "/Game/ThirdPerson/Maps/Lobby");

protected:
	virtual bool Initialize() override;

	virtual void NativeDestruct() override;

	void SwitchInputMode(bool bUIOnly);

	UFUNCTION()
	void OnHostButtonClicked();

	UFUNCTION()
	void OnJoinButtonClicked();

	void OnSessionCreateComplete(bool bSuccessful);

	void OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result);

	void OnFindSessionComplete(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bResult);

	void OnDestroySessionComplete(bool bSuccessful);

	void OnStartSessionComplete(bool bSuccessful);

private:
	void OnProcessing(bool bProcessing) const;

private:
	/*
	 *这里变量的名称要和Widget中按钮的名字一样
	 */
	UPROPERTY(Meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(Meta = (BindWidget))
	UButton* JoinButton;

	TObjectPtr<class UMultiplayerSessionsSubsystem> MultiplayerSessionsSubsystem;

	int M_PublicConnections{4};

	FString PathToLobby{""};

	FString M_MatchType{"FreeForAll"};
};

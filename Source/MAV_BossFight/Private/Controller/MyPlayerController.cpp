// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MyPlayerController.h"

AMyPlayerController::AMyPlayerController()
{
	HeroTeamID = FGenericTeamId(0);
}

FGenericTeamId AMyPlayerController::GetGenericTeamId() const
{
	return HeroTeamID;
}
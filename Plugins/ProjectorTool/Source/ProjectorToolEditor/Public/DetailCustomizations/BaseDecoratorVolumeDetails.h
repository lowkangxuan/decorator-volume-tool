﻿#pragma once
#include "IDetailCustomization.h"
#include "Enums/VolumeActionsEnum.h"

#define BUTTON_PADDING_FIRST 0 /*Left*/, 5 /*Top*/, 5 /*Right*/, 5 /*Bottom*/
#define BUTTON_PADDING 0 /*Left*/, 0 /*Top*/, 5 /*Right*/, 5 /*Bottom*/
#define BUTTON_ALIGNMENT HAlign_Fill

class ABaseDecoratorVolume;
class FReply;

class FBaseDecoratorVolumeDetails : public IDetailCustomization
{
public:
	// Makes a new instance of this detail layout class for a specific detail view requesting it
	static TSharedRef<IDetailCustomization> MakeInstance();
	
	// IDetailCustomization interface 
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	virtual void CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder) override;

	FReply EditorFuncs(EVolumeActionsEnum Type);
	bool CheckIfBaked();

private:
	TWeakPtr<IDetailLayoutBuilder> CachedDetailBuilder;
	TArray<TWeakObjectPtr<UObject>> Objects;
};
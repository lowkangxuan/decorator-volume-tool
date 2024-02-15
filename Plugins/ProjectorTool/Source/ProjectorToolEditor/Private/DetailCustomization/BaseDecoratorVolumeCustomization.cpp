﻿#include "DetailCustomization/BaseDecoratorVolumeCustomization.h"

#include "BaseDecoratorVolume.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Components/InstanceBakingComponent.h"

TSharedRef<IDetailCustomization> FBaseDecoratorVolumeCustomization::MakeInstance()
{
	return MakeShareable(new FBaseDecoratorVolumeCustomization);
}

void FBaseDecoratorVolumeCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.GetObjectsBeingCustomized(Objects);
	const bool bShouldDisplay = !Objects[0]->HasAnyFlags(RF_ArchetypeObject | RF_ClassDefaultObject); // Checking if these flags exist

	IDetailCategoryBuilder& InformationCategory = DetailBuilder.EditCategory("Information", FText::GetEmpty(), ECategoryPriority::Variable);
	IDetailCategoryBuilder& EditorCategory = DetailBuilder.EditCategory("Editor", FText::GetEmpty(), ECategoryPriority::Important);
	IDetailCategoryBuilder& PointsGeneratorCategory = DetailBuilder.EditCategory("PointsGeneratorComponent", FText::GetEmpty(), ECategoryPriority::Important);
	IDetailCategoryBuilder& BaseDecoratorVolumeCategory = DetailBuilder.EditCategory("BaseDecoratorVolume", FText::GetEmpty(), ECategoryPriority::Important);
	IDetailCategoryBuilder& DebugCategory = DetailBuilder.EditCategory("Debug", FText::GetEmpty(), ECategoryPriority::Important);

	if (bShouldDisplay)
	{
		EditorCategory.AddCustomRow(FText::FromString("Editor"))
		.WholeRowContent()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(BUTTON_ALIGNMENT)
			.Padding(BUTTON_PADDING_FIRST)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.HAlign(BUTTON_ALIGNMENT)
				.Padding(0, 0, 5, 0)
				[
					SNew(SButton)
					.Text(FText::FromString("Regenerate"))
					.ToolTipText(FText::FromString("Normal Regeneration, does not generate a new seed"))
					.VAlign(VAlign_Center)
					.ContentPadding(FMargin(0, 5))
					.IsEnabled(CheckIfBaked())
					.OnClicked_Raw(this, &FBaseDecoratorVolumeCustomization::EditorFuncs, EBaseBtnType::Regenerate)
				]
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.HAlign(BUTTON_ALIGNMENT)
				[
					SNew(SButton)
					.Text(FText::FromString("Regenerate (New Seed)"))
					.ToolTipText(FText::FromString("Regenerating with a new seed"))
					.VAlign(VAlign_Center)
					.ContentPadding(FMargin(0, 5))
					.IsEnabled(CheckIfBaked())
					.OnClicked_Raw(this, &FBaseDecoratorVolumeCustomization::EditorFuncs, EBaseBtnType::GenerateNew)
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(BUTTON_ALIGNMENT)
			.Padding(BUTTON_PADDING)
			[
				SNew(SButton)
				.VAlign(VAlign_Center)
				.ContentPadding(FMargin(0, 5))
				.IsEnabled(CheckIfBaked())
				.OnClicked_Raw(this, &FBaseDecoratorVolumeCustomization::EditorFuncs, EBaseBtnType::Clear)
				[
					SNew(STextBlock)
					.Justification(ETextJustify::Center)
					.Text(FText::FromString("Clear Instances"))
				]
			]
		];
	}
}

void FBaseDecoratorVolumeCustomization::CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder)
{
	CachedDetailBuilder = DetailBuilder;
	CustomizeDetails(*DetailBuilder);
}

FReply FBaseDecoratorVolumeCustomization::EditorFuncs(EBaseBtnType Type)
{
	for (TWeakObjectPtr<UObject> Object : Objects)
	{
		if (!Object.IsValid()) continue;

		TWeakObjectPtr<ABaseDecoratorVolume> VolumeActor = Cast<ABaseDecoratorVolume>(Object.Get());

		if (!VolumeActor.IsValid()) continue;

		switch (Type)
		{
		default:
			{
				break;	
			}

		case EBaseBtnType::Regenerate:
			{
				VolumeActor->Regenerate();
				break;
			}

		case EBaseBtnType::GenerateNew:
			{
				VolumeActor->GenerateNewSeed();
				break;
			}

		case EBaseBtnType::Clear:
			{
				VolumeActor->Clear();
				break;
			}
		}
	}

	if (IDetailLayoutBuilder* DetailBuilder = CachedDetailBuilder.Pin().Get())
	{
		DetailBuilder->ForceRefreshDetails();
	}
	
	return FReply::Handled();
}

bool FBaseDecoratorVolumeCustomization::CheckIfBaked()
{
	TWeakObjectPtr<UInstanceBakingComponent> BakingComponent = Cast<ABaseDecoratorVolume>(Objects[0])->GetComponentByClass<UInstanceBakingComponent>();
	if (!BakingComponent.IsValid()) return false;

	return BakingComponent->bIsBaked;
}

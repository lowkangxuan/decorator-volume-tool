#include "DetailCustomization/DecoratorVolumeCustomization.h"

#include "DecoratorVolume.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"

TSharedRef<IDetailCustomization> FDecoratorVolumeCustomization::MakeInstance()
{
	return MakeShareable(new FDecoratorVolumeCustomization);
}

void FDecoratorVolumeCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.GetObjectsBeingCustomized(Objects);
	const bool bShouldDisplay = !Objects[0]->HasAnyFlags(RF_ArchetypeObject | RF_ClassDefaultObject); // Checking if these flags exist
	
	IDetailCategoryBuilder& EditorCategory = DetailBuilder.EditCategory("Editor", FText::GetEmpty(), ECategoryPriority::Important);
	IDetailCategoryBuilder& DecoratorVolumeCategory = DetailBuilder.EditCategory("DecoratorVolume", FText::GetEmpty(), ECategoryPriority::Important);
	IDetailCategoryBuilder& PointsGeneratorCategory = DetailBuilder.EditCategory("PointsGeneratorComponent", FText::GetEmpty(), ECategoryPriority::Important);
	IDetailCategoryBuilder& DebugCategory = DetailBuilder.EditCategory("Debug", FText::GetEmpty(), ECategoryPriority::Important);

	// We want to hide the buttons when being viewed in BP Editor
	if (bShouldDisplay)
	{
		EditorCategory.AddCustomRow(FText::FromString("Editor"))
		.WholeRowContent()
		.VAlign(VAlign_Center)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(BUTTON_ALIGNMENT)
			.Padding(BUTTON_PADDING_FIRST)
			[
				SNew(SButton)
				.Text(FText::FromString("Bake"))
				.VAlign(VAlign_Center)
				.ContentPadding(FMargin(0, 5))
				.OnClicked_Raw(this, &FDecoratorVolumeCustomization::EditorFuncs, EBtnType::Bake)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(BUTTON_ALIGNMENT)
			.Padding(BUTTON_PADDING)
			[
				SNew(SButton)
				.Text(FText::FromString("Unbake"))
				.VAlign(VAlign_Center)
				.ContentPadding(FMargin(0, 5))
				.OnClicked_Raw(this, &FDecoratorVolumeCustomization::EditorFuncs, EBtnType::Unbake)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(BUTTON_ALIGNMENT)
			.Padding(BUTTON_PADDING)
			[
				SNew(SButton)
				.Text(FText::FromString("Regenerate"))
				.ToolTipText(FText::FromString("Normal Regeneration, does not generate a new seed"))
				.VAlign(VAlign_Center)
				.ContentPadding(FMargin(0, 5))
				.OnClicked_Raw(this, &FDecoratorVolumeCustomization::EditorFuncs, EBtnType::Regenerate)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(BUTTON_ALIGNMENT)
			.Padding(BUTTON_PADDING)
			[
				SNew(SButton)
				.Text(FText::FromString("Regenerate (New Seed)"))
				.ToolTipText(FText::FromString("Regenerating with a new seed"))
				.VAlign(VAlign_Center)
				.ContentPadding(FMargin(0, 5))
				.OnClicked_Raw(this, &FDecoratorVolumeCustomization::EditorFuncs, EBtnType::GenerateNew)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(BUTTON_ALIGNMENT)
			.Padding(BUTTON_PADDING)
			[
				SNew(SButton)
				.Text(FText::FromString("Clear"))
				.VAlign(VAlign_Center)
				.ContentPadding(FMargin(0, 5))
				.OnClicked_Raw(this, &FDecoratorVolumeCustomization::EditorFuncs, EBtnType::Clear)
			]
		];
	}
}

FReply FDecoratorVolumeCustomization::EditorFuncs(EBtnType Type)
{
	for (TWeakObjectPtr<UObject> Object : Objects)
	{
		if (!Object.IsValid()) continue;

		ADecoratorVolume* VolumeActor = Cast<ADecoratorVolume>(Object.Get());

		if (!VolumeActor) continue;

		switch (Type)
		{
			default:
			{
				break;	
			}
			
			case Bake:
			{
				VolumeActor->BakeInstances();
				break;
			}

			case Unbake:
			{
				VolumeActor->UnbakeInstances();
				break;
			}

			case Regenerate:
			{
				VolumeActor->Regenerate();
				break;
			}

			case GenerateNew:
			{
				VolumeActor->GenerateNewSeed();
				break;
			}

			case Clear:
			{
				VolumeActor->Clear();
				break;
			}
		}
	}
	
	return FReply::Handled();
}


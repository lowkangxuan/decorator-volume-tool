#pragma once
#include "IDetailCustomization.h"

#define BUTTON_PADDING_FIRST 0 /*Left*/, 5 /*Top*/, 5 /*Right*/, 5 /*Bottom*/
#define BUTTON_PADDING 0 /*Left*/, 0 /*Top*/, 5 /*Right*/, 5 /*Bottom*/
#define BUTTON_ALIGNMENT HAlign_Fill

class ADecoratorVolume;
class FReply;

UENUM()
enum EBtnType
{
	Bake,
	Unbake,
	Regenerate,
	GenerateNew,
	Clear
};

class FDecoratorVolumeCustomization : public IDetailCustomization
{
public:
	// Makes a new instance of this detail layout class for a specific detail view requesting it
	static TSharedRef<IDetailCustomization> MakeInstance();
	
	// IDetailCustomization interface 
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	FReply EditorFuncs(EBtnType Type);

private:
	TArray<TWeakObjectPtr<UObject>> Objects;
};

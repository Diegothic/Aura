// Copyright Diegothic


#include "AttributeInfo.h"


TOptional<const FAuraAttributeInfo*> UAttributeInfo::FindAttributeInfo(
	const FGameplayTag& AttributeTag,
	bool bLogNotFound
) const
{
	for (const FAuraAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return &Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("[AttributeInfo] Attribute info not found for Tag \'%s\'"), *AttributeTag.ToString()
		);
	}

	return TOptional<const FAuraAttributeInfo*>();
}

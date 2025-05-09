// Copyright Diegothic


#include "CharacterClassInfo.h"


const FCharacterClassDefaultInfo& UCharacterClassInfo::GetCharacterClassDefaultInfo(
	ECharacterClass CharacterClass
) const
{
	return CharacterClassDefaultInfo.FindChecked(CharacterClass);
}

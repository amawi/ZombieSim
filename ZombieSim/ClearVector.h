#pragma once
#include <vector>

//Template Functions
template<class CClearVector>
long ClearVector(std::vector<CClearVector*>* pVecToClear)
{
	size_t sizeVector = pVecToClear->size();

	CClearVector* pTarget = nullptr;

	for (size_t idxVector = 0; idxVector < sizeVector; idxVector++)
	{
		pTarget = pVecToClear->at(idxVector);
		delete pTarget;
	}

	return ERR_NOERROR;
}

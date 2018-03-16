//
//  UtilityBehavior.h
//  GameUBT
//
//  Created by River Liu on 15/3/2018.
//  Copyright © 2018 River Liu. All rights reserved.
// 

#pragma once

#include "../bt/Behavior.hpp"
#include "../utility/Consideration.h"

using namespace GameBT;
using namespace GameUtility;

class Blackboard;
namespace GameUBT
{
	class UtilityBehavior : public Behavior
	{
	private:
		std::vector<Consideration*> m_Considerations;

	public:
		UtilityBehavior() : Behavior("UtilityBehavior") { }
		UtilityBehavior(const std::string& _name) : Behavior(_name) { }
		UtilityBehavior(const BehaviorParams& _params)
		{
			auto it = _params.find("name");
			if (it != _params.end())
			{
				m_sName = it->second;
			}
		}
		virtual ~UtilityBehavior() { }
		void AddConsideration(Consideration* _consideration)
		{
			m_Considerations.push_back(_consideration);
		}
		inline virtual float getScore(Blackboard* _blackboard)
		{
			float finalScore = 1.f;
			if (m_Considerations.size() > 0)
			{
				for (Consideration* c : m_Considerations)
				{
					float score = c->getScore(_blackboard);
					finalScore *= score;
					std::cout << "    [" << c->getName() << "] score = " << score << std::endl;
				}
			}
			std::cout << "  [" << m_sName << "] finalScore = " << finalScore << std::endl;
			return finalScore;
		}
		inline static Behavior* create(const BehaviorParams& _params) { return new UtilityBehavior(_params); }
	};
}
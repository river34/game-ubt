//
//  UtilityComposite.h
//  GameUBT
//
//  Created by River Liu on 15/3/2018.
//  Copyright © 2018 River Liu. All rights reserved.
// 

#pragma once

#include "../bt/Composite.hpp"
#include "UtilityBehavior.h"

using namespace GameBT;

class Blackboard;
namespace GameUBT
{
	class UtilityComposite : public UtilityBehavior
	{
	protected:
		typedef std::vector<Behavior*> Behaviors;
		Behaviors m_Children;
		Behaviors::iterator m_CurrentChild;

	public:
		UtilityComposite() : UtilityBehavior("UtilityComposite") { }
		UtilityComposite(const std::string& _name) : UtilityBehavior(_name) { }
		UtilityComposite(const BehaviorParams& _params) : UtilityBehavior(_params) { }
		virtual ~UtilityComposite() { }
		inline virtual void addChild(Behavior* _child) { m_Children.push_back(_child); }
		inline void removeChild(Behavior* _child) {
			auto it = std::find(m_Children.begin(), m_Children.end(), _child);
			if (it != m_Children.end()) { std::swap(*it, m_Children.back()); m_Children.pop_back(); }
		}
		void clearChildern() { m_Children.clear(); }
		virtual float getScore(Blackboard* _blackboard)
		{
			float finalScore = 0.f;
			for (auto it = m_Children.begin(); it != m_Children.end(); it++)
			{
				UtilityBehavior* ub = dynamic_cast<UtilityBehavior*>(*it);
				float score = 1.f;
				if (ub != nullptr)
				{
					score = ub->getScore(_blackboard);
				}
				//finalScore *= score;
				if (finalScore < score)
				{
					finalScore = score;
				}
			}
			return finalScore;
		}
		inline static Behavior* create(const BehaviorParams& _params) { return new UtilityComposite(_params); }
	};
}
//
//  UtilitySelector.h
//  GameUBT
//
//  Created by River Liu on 15/3/2018.
//  Copyright © 2018 River Liu. All rights reserved.
// 

#pragma once

#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include "UtilityComposite.h"

using namespace GameBT;

namespace GameUBT
{
    class UtilitySelector : public UtilityComposite
    {
	private:
		std::map<Behaviors::iterator, float> m_Utility;
		std::vector<std::pair<Behaviors::iterator, float>> m_SortedChildren;
		std::vector<std::pair<Behaviors::iterator, float>>::iterator m_CurrentChildInSorted;

    public:
		UtilitySelector() : UtilityComposite("UtilitySelector") { }
		UtilitySelector(const std::string& _name) : UtilityComposite(_name) { }
		UtilitySelector(const BehaviorParams& _params) : UtilityComposite(_params) { }
        virtual ~UtilitySelector() { }
		virtual void onInitialize(Blackboard* _blackboard) { m_CurrentChild = m_Children.begin(); }
        virtual Status onUpdate(Blackboard* _blackboard) override
        {
			if (m_Utility.size() == 0)
			{
				// Query for child utility values
				for (auto it = m_Children.begin(); it != m_Children.end(); it++)
				{
					UtilityBehavior* ub = dynamic_cast<UtilityBehavior*>(*it);
					if (ub != nullptr)
					{
						m_Utility[it] = ub->getScore(_blackboard);
					}
					else
					{
						m_Utility[it] = 0.f;
					}
					//std::cout << "  [" << (*it)->getName() << "] score = " << m_Utility[it] << std::endl;
				}
				// Sort children from highest to lowest
				m_SortedChildren.clear();
				for (auto it = m_Utility.begin(); it != m_Utility.end(); it++)
				{
					m_SortedChildren.push_back(*it);
				}
				sort(m_SortedChildren.begin(), m_SortedChildren.end(), [=](std::pair<Behaviors::iterator, float>& a, std::pair<Behaviors::iterator, float>& b)
					{
						return a.second > b.second;
					}
				);
				// Set current child to the child with highest score
				m_CurrentChildInSorted = m_SortedChildren.begin();
				m_CurrentChild = m_CurrentChildInSorted->first;
				std::cout << "[UBT] choose child = " << (*m_CurrentChild)->getName() << std::endl;
			}

			// Search in utility order until a child behavior says its running
			for (;;)
            {
				m_CurrentChild = m_CurrentChildInSorted->first;
                Status status = (*m_CurrentChild)->tick(_blackboard);

                // if child succeeds or keeps running
                if (status != Status::BH_FAILURE)
				{ 
					if (status == Status::BH_SUCCESS)
					{
						m_Utility.clear();
					}
					return status;
				}

                // if all children have executed
                if (++m_CurrentChildInSorted == m_SortedChildren.end())
				{ 
					m_Utility.clear();
					return Status::BH_FAILURE;
				}
            }
        }
		inline static Behavior* create(const BehaviorParams& _params) { return new UtilitySelector(_params); }
    };
}

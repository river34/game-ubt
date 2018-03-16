//
//  UtilitySequence.h
//  GameUBT
//
//  Created by River Liu on 15/3/2018.
//  Copyright © 2018 River Liu. All rights reserved.
// 

#pragma once

#include "UtilityComposite.h"

using namespace GameBT;

class Blackboard;
namespace GameUBT
{
	class UtilitySequence : public UtilityComposite
	{
	public:
		UtilitySequence() : UtilityComposite("UtilitySequence") { }
		UtilitySequence(const std::string& _name) : UtilityComposite(_name) { }
		UtilitySequence(const BehaviorParams& _params) : UtilityComposite(_params) { }
		virtual ~UtilitySequence() { }
		virtual void onInitialize(Blackboard* _blackboard) { m_CurrentChild = m_Children.begin(); }
		virtual Status onUpdate(Blackboard* _blackboard) override
		{
			for (;;)
			{
				Status status = (*m_CurrentChild)->tick(_blackboard);

				// if one child behavior fails
				if (status != Status::BH_SUCCESS) { return status; }
				// if all children have executed successfully
				if (++m_CurrentChild == m_Children.end()) { return Status::BH_SUCCESS; }
			}
			return Status::BH_FAILURE;
		}
		inline static Behavior* create(const BehaviorParams& _params) { return new UtilitySequence(_params); }
	};
}
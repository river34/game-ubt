//
//  UtilityBehaviorTreeLoader.h
//  GameUBT
//
//  Created by River Liu on 15/3/2018.
//  Copyright © 2018 River Liu. All rights reserved.
// 

#pragma once

#include "../bt/BehaviorTreeLoader.hpp"
#include "../bt/BehaviorFactory.hpp"
#include "../utility/ResponseCurveFactory.h"
#include "../utility/ConsiderationFactory.h"
#include "UtilityBehavior.h"
#include "UtilityComposite.h"
#include "UtilitySequence.h"
#include "UtilitySelector.h"
#include "../rapidxml/rapidxml.hpp"

using namespace GameBT;
namespace GameUBT
{
	class UtilityBehaviorTreeLoader : public BehaviorTreeLoader
	{
	private:
		ResponseCurveFactory * m_CurveFactory;
		ConsiderationFactory * m_ConsideratioNFactory;
		std::vector<Input*> m_InputPool;

	public:
		void setUtility(ResponseCurveFactory* _curveFactory, ConsiderationFactory* _consideratioNFactory, std::vector<Input*>& _inputPool)
		{
			m_CurveFactory = _curveFactory;
			m_ConsideratioNFactory = _consideratioNFactory;
			m_InputPool = _inputPool;
		}
		virtual void createNodes(BehaviorFactory* _behaviorFactory, Behavior* _behavior, xml_node<>* _node)
		{
			BehaviorTreeLoader::createNodes(_behaviorFactory, _behavior, _node);

			if (_node->first_node())
			{
				UtilityComposite* utilityComposite = dynamic_cast<UtilityComposite*>(_behavior);
				if (utilityComposite != nullptr)
				{
					// for each child, add to the composite
					for (xml_node<>* node = _node->first_node(); node; node = node->next_sibling())
					{
						Behavior* child = _behaviorFactory->createInstance(node);
						assert(child != nullptr);
						utilityComposite->addChild(child);
						createNodes(_behaviorFactory, child, node);
					}
				}

				UtilityBehavior* utilityBehavior = dynamic_cast<UtilityBehavior*>(_behavior);
				if (utilityBehavior != nullptr)
				{
					// iterate over considerations
					for (xml_node<>* consideration_node = _node->first_node("Consideration"); consideration_node; consideration_node = consideration_node->next_sibling("Consideration"))
					{
						// create consideration
						assert(m_ConsideratioNFactory != nullptr);
						auto newConsideration = m_ConsideratioNFactory->createInstance(consideration_node);
						assert(newConsideration);
						utilityBehavior->AddConsideration(newConsideration);

						// create curve
						xml_node<>* curve_node = consideration_node->first_node("ResponseCurve");
						assert(m_CurveFactory != nullptr);
						auto newCurve = m_CurveFactory->createInstance(curve_node);
						assert(newCurve);
						newConsideration->setCurve(newCurve);

						// register input
						xml_node<>* input_node = consideration_node->first_node("Input");
						assert(m_InputPool.size() > 0);
						for (Input* input : m_InputPool)
						{
							if (input->getName() == input_node->first_attribute("id")->value())
							{
								newConsideration->setInput(input);
								break;
							}
						}
					}
				}
			}
		}
	};
}
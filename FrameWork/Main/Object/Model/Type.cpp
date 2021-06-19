#include "stdafx.h"
#include "Type.h"

void VertexWeights::Normalize()
{
	float total = 0.0f;

	for (UINT i = 0; i < MAX_WEIGHTS; i++)
	{
		if (weights[i] != 0.0f)
		{
			total += weights[i];
		}
	}

	for (UINT i = 0; i < MAX_WEIGHTS; i++)
	{
		if (weights[i] != 0.0f)
		{
			weights[i] /= total;
		}
	}
}

void VertexWeights::AddData(UINT bone_id, float weight)
{
	for (UINT i = 0; i < MAX_WEIGHTS; i++)
	{
		if (weights[i] == 0.0f)
		{
			bone_ids[i] = bone_id;
			weights[i] = weight;
			return;
		}
	}
}

Vector3 Interpolated::CalcInterpolatedScaling(AnimIter iter, float time, int Duration)
{
	if (iter->second->scale.empty())
		return Vector3(1.0f, 1.0f, 1.0f);

	if (iter->second->scale.size() == 1)
		return iter->second->scale.front().scale;
	
	if (time == Duration - 1)
	{
		return iter->second->scale.back().scale;
	}
	
	int scaling_index = FindScale(iter, time);

	if (scaling_index == -1)
	{
		return iter->second->scale.back().scale;
	}

	UINT next_scaling_index = scaling_index + 1;
	assert(next_scaling_index < iter->second->scale.size());

	float delta_time = (float)(iter->second->scale[next_scaling_index].time
		- iter->second->scale[scaling_index].time);

	float factor = (time - (float)(iter->second->scale[scaling_index].time)) / delta_time;

	if (factor < 0.0f)
	{
		return iter->second->scale.front().scale;
	}

	Vector3 start = iter->second->scale[scaling_index].scale;
	Vector3 end = iter->second->scale[next_scaling_index].scale;

	start = Linear(start, end, factor);
	return start;
}

Quaternion Interpolated::CalcInterpolatedRotation(AnimIter iter, float time, int Duration)
{
	if (iter->second->rotation.empty())
		return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

	if (iter->second->rotation.size() == 1)
		return iter->second->rotation.front().quater;

	if (time == Duration - 1)
	{
		return iter->second->rotation.back().quater;
	}

	int quter_index = FindRot(iter, time);
	if (quter_index == -1)
	{
		return iter->second->rotation.back().quater;
	}
	UINT next_quter_index = quter_index + 1;
	assert(next_quter_index < iter->second->rotation.size());

	float delta_time = static_cast<float>(iter->second->rotation[next_quter_index].time - iter->second->rotation[quter_index].time);
	float factor = (time - static_cast<float>(iter->second->rotation[quter_index].time)) / delta_time;
	if (factor < 0.0f)
	{
		return iter->second->rotation.front().quater;
	}

	auto start = iter->second->rotation[quter_index].quater;
	auto end = iter->second->rotation[next_quter_index].quater;

	D3DXQuaternionSlerp(&start, &start, &end, factor);
	return start;
}

Vector3 Interpolated::CalcInterpolatedPosition(AnimIter iter, float time, int Duration)
{
	if (iter->second->position.empty())
		return Vector3(0.0f, 0.0f, 0.0f);

	if (iter->second->position.size() == 1)
		return iter->second->position.front().pos;

	if (time == Duration - 1)
	{
		return iter->second->position.back().pos;
	}

	int position_index = FindPos(iter, time);

	if (position_index == -1)
	{
		return iter->second->position.back().pos;
	}

	UINT next_position_index = position_index + 1;
	assert(next_position_index < iter->second->position.size());

	float delta_time = static_cast<float>(iter->second->position[next_position_index].time - iter->second->position[position_index].time);
	float factor = (time - static_cast<float>(iter->second->position[position_index].time)) / delta_time;

	if (factor < 0.0f)
	{
		return iter->second->position.front().pos;
	}

	auto start = iter->second->position[position_index].pos;
	auto end = iter->second->position[next_position_index].pos;

	start = Linear(start, end, factor);
	return start;
}

int Interpolated::FindScale(AnimIter iter, float time)
{
	if (iter->second->scale.empty()) return -1;

	for (UINT i = 0; i < iter->second->scale.size() - 1; i++)
	{
		if (time < (float)(iter->second->scale[i + 1].time))
			return i;
	}

	return -1;
}

int Interpolated::FindRot(AnimIter iter, float time)
{
	if (iter->second->rotation.empty()) return -1;

	for (UINT i = 0; i < iter->second->rotation.size() - 1; i++)
	{
		if (time < static_cast<float>(iter->second->rotation[i + 1].time))
			return i;
	}

	return -1;
}

int Interpolated::FindPos(AnimIter iter, float time)
{
	if (iter->second->position.empty())	return -1;

	for (UINT i = 0; i < iter->second->position.size() - 1; i++)
	{
		if (time < static_cast<float>(iter->second->position[i + 1].time))
			return i;
	}

	return -1;
}
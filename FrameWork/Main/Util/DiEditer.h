#pragma once

class DiEditer
{
private:
	Sphere* sp;
	Line* ln;
public:
	DiEditer();
	~DiEditer();
	int Pick;
	vector<DiNode> Nodes;
	void AddNode(Vector3 pos);
	void PopNode();
	void LinkNode(int id1, int id2);
	int PickNode(Vector3 pos);
	void render();
	void Save(string file);
	void Load(string file);
};
#pragma once
struct Move
{
	Move(int o, int t) : originIndex(o), targetIndex(t) {}
	int originIndex;
	int originPiece;
	int targetIndex;
	int targetPiece;
};
#pragma once
#include<KamataEngine.h>
using namespace KamataEngine;

enum class MapChipType {
	kBlank,
	kBlock
};
struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};
class MapChipField {
	static inline const uint32_t kNumBlockHorizontal = 100;
    static inline const uint32_t kNumBlockVirtical = 20;
	static inline const float kBlockWidth = 2.0f;
    static inline const float kBlockHeight = 2.0f;

public:
	// マップチップのデータをリセット
	void ResetMapChipData();
	// CSVファイルからマップチップのデータを読み込む
	void LoadMapChipCsv(const std::string& filePath);
	// マップチップのデータを取得する
	MapChipType GetMapChipTypeIndex(uint32_t xIndex, uint32_t yIndex);
	// マップチップの座標を取得する
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	// マップチップの横のブロック数を取得する
	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }
	// マップチップの縦のブロック数を取得する
	uint32_t GetNumBlockVirtical() {return kNumBlockVirtical;}

private:
    MapChipData mapChipData_;

};

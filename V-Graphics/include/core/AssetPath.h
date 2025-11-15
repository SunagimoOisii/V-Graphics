#pragma once
#include <filesystem>

//テクスチャデータ, シェーダーコード, モデルデータなどが置かれたファイルパス情報を取り扱うための仕組み

//アセットのルートパスを設定
void SetAssetRootPath(const std::filesystem::path& path);

//現在のアセットルートパスを取得
std::filesystem::path GetAssetRootPath();

enum class AssetType
{
    Shader = 0,
    Texture,
    Model,
    AssetTypeMax,
};
std::filesystem::path GetAssetPath(AssetType type, const std::filesystem::path& fileName);